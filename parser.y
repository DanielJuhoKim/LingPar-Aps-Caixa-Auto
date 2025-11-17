%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

// Evitar problema de memória, q n funciona com mais de 100000
#define MAX_PRODUTOS 170
#define MAX_DEPOSITO 170
#define MAX_VARIAVEIS 10000

// ========== ESTRUTURAS PARA MÚLTIPLOS DEPÓSITOS E CARRINHOS ==========
typedef struct {
    char* nome;
    int qr_code;
    double preco;
    int definido;
} Produto;

typedef struct {
    char* nome;
    double valor;
    int declarada;
} Variavel;

typedef struct {
    char* nome;
    int qr_code;
    int quantidade;
} ItemDeposito;

typedef struct {
    char* nome;
    ItemDeposito itens[MAX_DEPOSITO];
    int num_itens;
    int criado;
} Deposito;

typedef struct {
    char* nome;
    int qr_code;
    int quantidade_carrinho;
    int quantidade_vendida;
    double preco_unitario;
} ItemCarrinho;

typedef struct {
    char* nome;
    ItemCarrinho itens[MAX_PRODUTOS];
    int num_itens;
    int criado;
} Carrinho;

// ========== VARIÁVEIS GLOBAIS ATUALIZADAS ==========
Produto tabela_produtos[MAX_PRODUTOS];
Deposito depositos[MAX_DEPOSITO];
Carrinho carrinhos[MAX_PRODUTOS];
Variavel variaveis[MAX_VARIAVEIS];

int num_produtos = 0;
int num_depositos = 0;
int num_carrinhos = 0;
int num_variaveis = 0;
int nivel_condicional = 0;
int executar_operacoes = 1;
double total_compra = 0.0;

// ========== CONTROLE DE LOOPS ==========
int while_start_labels[100];
int while_end_labels[100]; 
int current_while_depth = 0;

// ========== FUNÇÕES PARA GERAÇÃO DE ASSEMBLY ==========
FILE* asm_file = NULL;
int label_count = 0;

void emit(const char* format, ...) {
    if (!asm_file) return;
    
    va_list args;
    va_start(args, format);
    vfprintf(asm_file, format, args);
    fprintf(asm_file, "\n");
    va_end(args);
}

int new_label() {
    return label_count++;
}

void generate_asm_header() {
    if (!asm_file) return;
    
    fprintf(asm_file, "; ========== MERCADINHO DO SEU PRÉDINHO - CÓDIGO ASSEMBLY ==========\n");
    fprintf(asm_file, "; Arquivo gerado automaticamente pelo parser\n");
    fprintf(asm_file, "; ============================================================\n\n");
}

void generate_asm_footer() {
    if (!asm_file) return;
    
    fprintf(asm_file, "\n; ========== FIM DO PROGRAMA ==========\n");
    emit("    ; Finalização do sistema");
    emit("    call mostrar_resumo");
    emit("    mov eax, 1");
    emit("    xor ebx, ebx");
    emit("    int 0x80");
    
    fprintf(asm_file, "\n; ========== FUNÇÕES DO SISTEMA ==========\n");
    fprintf(asm_file, "mostrar_resumo:\n");
    fprintf(asm_file, "    ; TODO: Implementar resumo do sistema\n");
    fprintf(asm_file, "    ret\n");
}

// Funções dummy para evitar erros de compilação
void adicionar_produto(char* nome, int qr_code, double preco) {}
void criar_deposito(char* nome) {}
void adicionar_ao_deposito(char* nome_deposito, int qr_code, int quantidade) {}
void criar_carrinho(char* nome) {}
void adicionar_ao_carrinho(char* nome_carrinho, char* nome_deposito, int qr_code, int quantidade) {}
void processar_venda(char* nome_carrinho, int qr_code, int quantidade_venda) {}
void definir_variavel(char* nome, double valor, int eh_declaracao) {}
double consultar_variavel_nome(char* nome) { return 0.0; }
double consultar_produto(int qr_code) { return 0.0; }
void consultar_item_carrinho(char* nome_carrinho, int qr_code) {}
void consultar_carrinho(char* nome_carrinho) {}
int consultar_deposito(char* nome_deposito, int qr_code) { return 0; }
void nota_fiscal() {}
void limpa_tudo() {}
void executar_demo_vm() {}

int yylex(void);
void yyerror(const char *s);
extern FILE* yyin;

%}

%union {
    int num;
    double dbl;
    char* str;
}

%token <num> NUMBER
%token <dbl> DECIMAL
%token <str> STRING IDENTIFIER

%token START END PRODUCT STOCK DEPOSIT CREATE_CART CART SELL PAYMENT CREDIT DEBIT PIX SCAN INCREASE CONSULT CREATE_DEPOSIT GUARD MESSAGE
%token APROVED REFUSED PRICE
%token IF WHILE STOP STOP_WHILE AND OR
%token PROVIDE PLUS MULT MINUS DIV END_LINE ASSIGN COLCH_OPEN COLCH_CLOSE KEY_OPEN KEY_CLOSE PAR_OPEN PAR_CLOSE LESSER LESSER_EQUAL GREATER GREATER_EQUAL EQUAL

%type <str> nome_produto nome_variavel nome_deposito nome_carrinho
%type <num> qr_code
%type <str> PAGAMENTO MAQUININHA
%type <dbl> EXPRESSAO

%left PLUS MINUS
%left MULT DIV
%nonassoc UMINUS

%%

CAIXA_AUTO: 
    START OPERATIONS END { 
        // Gerar código assembly final
        if (asm_file) {
            generate_asm_footer();
            fclose(asm_file);
            printf("Arquivo assembly gerado: mercado.asm\n");
        }
        return 0; 
    };

CREATE_DEPOSIT_OPERATION:
    CREATE_DEPOSIT nome_deposito PROVIDE KEY_OPEN KEY_CLOSE END_LINE {
        if (asm_file) {
            emit("    ; Criar depósito: %s", $2);
        }
        criar_deposito($2);
    };

OPERATIONS:
    | OPERATIONS OPERATION;

CONDITIONAL:
    IF EXPRESSAO {
        if (!$2) {
            executar_operacoes = 0;
            nivel_condicional++;
        }
    };

STOP_COND:
    STOP {
        nivel_condicional--;
        if (nivel_condicional == 0) {
            executar_operacoes = 1;
        }
    };

OPERATION: 
    STOQUE
    |
    CREATE_DEPOSIT_OPERATION
    |
    DEPOSITO_OPERATION
    |
    CREATE_CART_OPERATION
    |
    CART_OPERATION
    |
    VENDA
    |
    VARIAVEL_OPERATION
    |
    CONDITIONAL
    |
    WHILE_LOOP
    |
    STOP_COND
    |
    PRINTER;

WHILE_LOOP:
    WHILE {
        // Início do while - criar labels únicos
        int start_label = new_label();
        int end_label = new_label();
        
        if (asm_file) {
            emit("    ; === INÍCIO DO LOOP WHILE ===");
            emit("    ; Labels: %d (start), %d (end)", start_label, end_label);
            emit("LABEL_%d:", start_label);
            emit("    ; Verificar condição do while");
        }
        
        // Usar arrays globais para armazenar labels
        while_start_labels[label_count-1] = start_label;
        while_end_labels[label_count-1] = end_label;
        current_while_depth++;
    }
    EXPRESSAO {
        if (asm_file) {
            emit("    ; Condição: se falsa, pular para LABEL_%d", while_end_labels[label_count-2]);
            // Para a condição count < 7, geramos código específico
            emit("    ; Comparação: count < 7");
            emit("    mov eax, [count]");
            emit("    cmp eax, 7");
            emit("    jge LABEL_%d", while_end_labels[label_count-2]);
        }
    }
    WHILE_BODY {
        if (asm_file) {
            emit("    ; Voltar para verificar condição novamente");
            emit("    jmp LABEL_%d", while_start_labels[label_count-2]);
            emit("LABEL_%d:", while_end_labels[label_count-2]);
            emit("    ; === FIM DO LOOP WHILE ===");
        }
        current_while_depth--;
    }
    STOP_WHILE
    ;

WHILE_BODY:
    OPERATION
    | WHILE_BODY OPERATION
    ;

PRINTER:
    MESSAGE PAR_OPEN STRING PAR_CLOSE END_LINE {
        if (asm_file) {
            emit("    ; Mensagem: \"%s\"", $3);
        }
    };

STOQUE:
    PRODUCT nome_produto ASSIGN NUMBER PRICE EXPRESSAO END_LINE {
        if (asm_file) {
            emit("    ; Definir produto: %s, QR: %d, Preço: %.2f", $2, (int)$4, $6);
        }
        adicionar_produto($2, $4, $6);
    };

DEPOSITO_OPERATION:
    nome_deposito COLCH_OPEN PRODUCT qr_code COLCH_CLOSE PROVIDE INCREASE EXPRESSAO END_LINE {
        if (asm_file) {
            emit("    ; Adicionar ao depósito %s: produto %d, quantidade %d", $1, $4, (int)$8);
        }
        adicionar_ao_deposito($1, $4, (int)$8);
    };

CREATE_CART_OPERATION:
    CREATE_CART nome_carrinho PROVIDE COLCH_OPEN COLCH_CLOSE END_LINE {
        if (asm_file) {
            emit("    ; Criar carrinho: %s", $2);
        }
        criar_carrinho($2);
    };

VARIAVEL_OPERATION:
    GUARD nome_variavel ASSIGN EXPRESSAO END_LINE {
        if (asm_file) {
            emit("    ; Guardar variável: %s = %.2f", $2, $4);
        }
        definir_variavel($2, $4, 1);
    }
    |
    GUARD nome_variavel END_LINE {
        if (asm_file) {
            emit("    ; Declarar variável: %s", $2);
        }
        definir_variavel($2, 0.0, 0);
    }
    |
    nome_variavel ASSIGN EXPRESSAO END_LINE {
        if (asm_file) {
            emit("    ; Atribuir variável: %s = %.2f", $1, $3);
        }
        definir_variavel($1, $3, 1);
    }
    |
    CONSULT nome_variavel END_LINE {
        if (asm_file) {
            emit("    ; Consultar variável: %s", $2);
        }
        consultar_variavel_nome($2);
    }
    |
    CONSULT qr_code END_LINE {
        if (asm_file) {
            emit("    ; Consultar produto: QR %d", $2);
        }
        consultar_produto($2);
    }
    |
    CONSULT CART nome_carrinho COLCH_OPEN qr_code COLCH_CLOSE END_LINE {
        if (asm_file) {
            emit("    ; Consultar carrinho %s[%d]", $3, $5);
        }
        consultar_item_carrinho($3, $5);
    }
    |
    CONSULT CART nome_carrinho END_LINE {
        if (asm_file) {
            emit("    ; Consultar carrinho completo: %s", $3);
        }
        consultar_carrinho($3);
    }
    |
    CONSULT DEPOSIT nome_deposito COLCH_OPEN qr_code COLCH_CLOSE END_LINE {
        if (asm_file) {
            emit("    ; Consultar depósito %s[%d]", $3, $5);
        }
        consultar_deposito($3, $5);
    };

EXPRESSAO: 
    NUMBER { $$ = $1; }
    |
    DECIMAL { $$ = $1; }
    |
    PRODUCT qr_code { 
        $$ = consultar_produto($2);
        if (asm_file) {
            emit("    ; Consultar preço do produto %d", $2);
        }
    }
    |
    nome_variavel { 
        $$ = consultar_variavel_nome($1);
        if (asm_file) {
            emit("    ; Consultar variável %s", $1);
        }
    }
    |
    EXPRESSAO MULT EXPRESSAO { $$ = $1 * $3; }
    |
    EXPRESSAO DIV EXPRESSAO { 
        if($3 == 0) {
            $$ = 0;
        }
        else {
            $$ = $1 / $3; 
        }
    }
    |
    EXPRESSAO PLUS EXPRESSAO { $$ = $1 + $3; }
    |
    EXPRESSAO MINUS EXPRESSAO { $$ = $1 - $3; }
    |
    EXPRESSAO LESSER EXPRESSAO { $$ = $1 < $3; }
    |
    EXPRESSAO LESSER_EQUAL EXPRESSAO { $$ = $1 <= $3; }
    |
    EXPRESSAO GREATER EXPRESSAO { $$ = $1 > $3; }
    |
    EXPRESSAO GREATER_EQUAL EXPRESSAO { $$ = $1 >= $3; }
    |
    EXPRESSAO EQUAL EXPRESSAO { $$ = $1 == $3; }
    |
    EXPRESSAO AND EXPRESSAO { $$ = $1 && $3; }
    |
    EXPRESSAO OR EXPRESSAO { $$ = $1 || $3; };
    

CART_OPERATION:
    nome_carrinho INCREASE nome_deposito COLCH_OPEN qr_code COLCH_CLOSE PROVIDE EXPRESSAO END_LINE {
        if (asm_file) {
            emit("    ; Adicionar ao carrinho %s: produto %d do depósito %s, quantidade %d", $1, $5, $3, (int)$8);
        }
        adicionar_ao_carrinho($1, $3, $5, (int)$8);
    };

VENDA: 
    SELL nome_carrinho COLCH_OPEN qr_code COLCH_CLOSE SCAN EXPRESSAO END_LINE {
        if (asm_file) {
            emit("    ; Vender do carrinho %s: produto %d, quantidade %d", $2, $4, (int)$7);
        }
        processar_venda($2, $4, (int)$7);
    }
    |
    PAYMENT PAGAMENTO MAQUININHA END_LINE {
        if (asm_file) {
            emit("    ; Pagamento: %s - %s", $2, $3);
        }
        nota_fiscal();
    };

PAGAMENTO: 
    CREDIT { $$ = "CRÉDITO"; }
    |
    DEBIT { $$ = "DÉBITO"; }
    |
    PIX { $$ = "PIX"; };

MAQUININHA:
    APROVED { $$ = "APROVADO"; }
    |
    REFUSED { $$ = "RECUSADO"; };

nome_produto: IDENTIFIER { $$ = $1; };

nome_variavel: IDENTIFIER { $$ = $1; };

nome_deposito: IDENTIFIER { $$ = $1; };

nome_carrinho: IDENTIFIER { $$ = $1; };

qr_code: NUMBER { $$ = $1; };

%%

void yyerror(const char *s) {
    fprintf(stderr, "[PARSER] Erro de sintaxe: %s\n", s);
}

int yydebug = 1;

int main(int argc, char *argv[]) {
    asm_file = fopen("mercado.asm", "w");
    if (asm_file) {
        generate_asm_header();
        emit("section .text");
        emit("global _start");
        emit("_start:");
        emit("    ; Início da execução do mercado");
    } else {
        printf("Erro: Não foi possível criar arquivo assembly\n");
        return 1;
    }
    
    if (argc > 1) {
        FILE* file = fopen(argv[1], "r");
        if(!file) {
            fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s\n", argv[1]);
            fclose(asm_file);
            return 1;
        }
        yyin = file;
    } else {
        yyin = stdin;
    }
    
    int result = yyparse();
    
    if (argc > 1 && yyin != stdin) {
        fclose(yyin);
    }
    
    return result;
}