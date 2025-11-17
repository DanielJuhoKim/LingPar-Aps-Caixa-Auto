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

// ========== IMPLEMENTAÇÃO DAS FUNÇÕES DO SISTEMA ==========

void adicionar_produto(char* nome, int qr_code, double preco) {
    if (num_produtos < MAX_PRODUTOS) {
        tabela_produtos[num_produtos].nome = strdup(nome);
        tabela_produtos[num_produtos].qr_code = qr_code;
        tabela_produtos[num_produtos].preco = preco;
        tabela_produtos[num_produtos].definido = 1;
        num_produtos++;
    }
}

void criar_deposito(char* nome) {
    if (num_depositos < MAX_DEPOSITO) {
        depositos[num_depositos].nome = strdup(nome);
        depositos[num_depositos].num_itens = 0;
        depositos[num_depositos].criado = 1;
        num_depositos++;
    }
}

void adicionar_ao_deposito(char* nome_deposito, int qr_code, int quantidade) {
    for (int i = 0; i < num_depositos; i++) {
        if (strcmp(depositos[i].nome, nome_deposito) == 0) {
            // Verificar se produto existe
            int produto_existe = 0;
            for (int j = 0; j < num_produtos; j++) {
                if (tabela_produtos[j].qr_code == qr_code) {
                    produto_existe = 1;
                    break;
                }
            }
            
            if (!produto_existe) {
                // Criar produto genérico
                char nome_produto[50];
                sprintf(nome_produto, "Produto_%d", qr_code);
                adicionar_produto(nome_produto, qr_code, 0.0);
            }
            
            // Adicionar ao depósito
            if (depositos[i].num_itens < MAX_DEPOSITO) {
                depositos[i].itens[depositos[i].num_itens].nome = strdup(nome_deposito);
                depositos[i].itens[depositos[i].num_itens].qr_code = qr_code;
                depositos[i].itens[depositos[i].num_itens].quantidade = quantidade;
                depositos[i].num_itens++;
            }
            return;
        }
    }
}

void criar_carrinho(char* nome) {
    if (num_carrinhos < MAX_PRODUTOS) {
        carrinhos[num_carrinhos].nome = strdup(nome);
        carrinhos[num_carrinhos].num_itens = 0;
        carrinhos[num_carrinhos].criado = 1;
        num_carrinhos++;
    }
}

void adicionar_ao_carrinho(char* nome_carrinho, char* nome_deposito, int qr_code, int quantidade) {
    // Encontrar carrinho
    int carrinho_idx = -1;
    for (int i = 0; i < num_carrinhos; i++) {
        if (strcmp(carrinhos[i].nome, nome_carrinho) == 0) {
            carrinho_idx = i;
            break;
        }
    }
    
    if (carrinho_idx == -1) {
        return;
    }
    
    // Encontrar depósito
    int deposito_idx = -1;
    for (int i = 0; i < num_depositos; i++) {
        if (strcmp(depositos[i].nome, nome_deposito) == 0) {
            deposito_idx = i;
            break;
        }
    }
    
    if (deposito_idx == -1) {
        return;
    }
    
    // Verificar estoque no depósito
    int estoque_disponivel = 0;
    for (int i = 0; i < depositos[deposito_idx].num_itens; i++) {
        if (depositos[deposito_idx].itens[i].qr_code == qr_code) {
            estoque_disponivel = depositos[deposito_idx].itens[i].quantidade;
            break;
        }
    }
    
    if (estoque_disponivel < quantidade) {
        return;
    }
    
    // Adicionar ao carrinho
    if (carrinhos[carrinho_idx].num_itens < MAX_PRODUTOS) {
        carrinhos[carrinho_idx].itens[carrinhos[carrinho_idx].num_itens].nome = strdup(nome_carrinho);
        carrinhos[carrinho_idx].itens[carrinhos[carrinho_idx].num_itens].qr_code = qr_code;
        carrinhos[carrinho_idx].itens[carrinhos[carrinho_idx].num_itens].quantidade_carrinho = quantidade;
        carrinhos[carrinho_idx].itens[carrinhos[carrinho_idx].num_itens].quantidade_vendida = 0;
        
        // Encontrar preço do produto
        double preco = 0.0;
        for (int i = 0; i < num_produtos; i++) {
            if (tabela_produtos[i].qr_code == qr_code) {
                preco = tabela_produtos[i].preco;
                break;
            }
        }
        carrinhos[carrinho_idx].itens[carrinhos[carrinho_idx].num_itens].preco_unitario = preco;
        
        carrinhos[carrinho_idx].num_itens++;
        
        // Remover do depósito
        for (int i = 0; i < depositos[deposito_idx].num_itens; i++) {
            if (depositos[deposito_idx].itens[i].qr_code == qr_code) {
                depositos[deposito_idx].itens[i].quantidade -= quantidade;
                break;
            }
        }
        
    }
}

void processar_venda(char* nome_carrinho, int qr_code, int quantidade_venda) {
    int carrinho_idx = -1;
    for (int i = 0; i < num_carrinhos; i++) {
        if (strcmp(carrinhos[i].nome, nome_carrinho) == 0) {
            carrinho_idx = i;
            break;
        }
    }
    
    if (carrinho_idx == -1) {
        return;
    }
    
    // Encontrar item no carrinho
    int item_idx = -1;
    for (int i = 0; i < carrinhos[carrinho_idx].num_itens; i++) {
        if (carrinhos[carrinho_idx].itens[i].qr_code == qr_code) {
            item_idx = i;
            break;
        }
    }
    
    if (item_idx == -1) {
        return;
    }
    
    if (carrinhos[carrinho_idx].itens[item_idx].quantidade_carrinho < quantidade_venda) {
        return;
    }
    
    // Processar venda
    carrinhos[carrinho_idx].itens[item_idx].quantidade_carrinho -= quantidade_venda;
    carrinhos[carrinho_idx].itens[item_idx].quantidade_vendida += quantidade_venda;
    
    double valor_venda = carrinhos[carrinho_idx].itens[item_idx].preco_unitario * quantidade_venda;
    total_compra += valor_venda;
}

void definir_variavel(char* nome, double valor, int eh_declaracao) {
    if (num_variaveis < MAX_VARIAVEIS) {
        // Verificar se variável já existe
        for (int i = 0; i < num_variaveis; i++) {
            if (strcmp(variaveis[i].nome, nome) == 0) {
                variaveis[i].valor = valor;
                variaveis[i].declarada = 1;
                return;
            }
        }
        
        // Criar nova variável
        variaveis[num_variaveis].nome = strdup(nome);
        variaveis[num_variaveis].valor = valor;
        variaveis[num_variaveis].declarada = eh_declaracao;
        num_variaveis++;
        
        if (eh_declaracao) {
        } else {
        }
    }
}

double consultar_variavel_nome(char* nome) {
    for (int i = 0; i < num_variaveis; i++) {
        if (strcmp(variaveis[i].nome, nome) == 0) {
            return variaveis[i].valor;
        }
    }
    return 0.0;
}

double consultar_produto(int qr_code) {
    for (int i = 0; i < num_produtos; i++) {
        if (tabela_produtos[i].qr_code == qr_code) {
            return tabela_produtos[i].preco;
        }
    }
    return 0.0;
}

void consultar_item_carrinho(char* nome_carrinho, int qr_code) {
    int carrinho_idx = -1;
    for (int i = 0; i < num_carrinhos; i++) {
        if (strcmp(carrinhos[i].nome, nome_carrinho) == 0) {
            carrinho_idx = i;
            break;
        }
    }
    
    if (carrinho_idx == -1) {
        return;
    }
    
    for (int i = 0; i < carrinhos[carrinho_idx].num_itens; i++) {
        if (carrinhos[carrinho_idx].itens[i].qr_code == qr_code) {
            return;
        }
    }
    
}

void consultar_carrinho(char* nome_carrinho) {
    int carrinho_idx = -1;
    for (int i = 0; i < num_carrinhos; i++) {
        if (strcmp(carrinhos[i].nome, nome_carrinho) == 0) {
            carrinho_idx = i;
            break;
        }
    }
    
    if (carrinho_idx == -1) {
        return;
    }
    
    double total = 0.0;
    
    for (int i = 0; i < carrinhos[carrinho_idx].num_itens; i++) {
        if (carrinhos[carrinho_idx].itens[i].quantidade_carrinho > 0) {
            double subtotal = carrinhos[carrinho_idx].itens[i].preco_unitario * 
                            carrinhos[carrinho_idx].itens[i].quantidade_carrinho;
            total += subtotal;
            
            // Encontrar nome do produto
            char nome_produto[50] = "Produto Desconhecido";
            for (int j = 0; j < num_produtos; j++) {
                if (tabela_produtos[j].qr_code == carrinhos[carrinho_idx].itens[i].qr_code) {
                    strcpy(nome_produto, tabela_produtos[j].nome);
                    break;
                }
            }
            
        }
    }
    
}

int consultar_deposito(char* nome_deposito, int qr_code) {
    for (int i = 0; i < num_depositos; i++) {
        if (strcmp(depositos[i].nome, nome_deposito) == 0) {
            for (int j = 0; j < depositos[i].num_itens; j++) {
                if (depositos[i].itens[j].qr_code == qr_code) {
                    return depositos[i].itens[j].quantidade;
                }
            }
            return 0;
        }
    }
    return 0;
}

void nota_fiscal() {
    
    for (int i = 0; i < num_carrinhos; i++) {
        int tem_vendas = 0;
        for (int j = 0; j < carrinhos[i].num_itens; j++) {
            if (carrinhos[i].itens[j].quantidade_vendida > 0) {
                if (!tem_vendas) {
                    tem_vendas = 1;
                }
                
                // Encontrar nome do produto
                char nome_produto[50] = "Produto Desconhecido";
                for (int k = 0; k < num_produtos; k++) {
                    if (tabela_produtos[k].qr_code == carrinhos[i].itens[j].qr_code) {
                        strcpy(nome_produto, tabela_produtos[k].nome);
                        break;
                    }
                }
                
                double subtotal = carrinhos[i].itens[j].preco_unitario * 
                                carrinhos[i].itens[j].quantidade_vendida;
            }
        }
    }
}

void limpa_tudo() {
    // Limpar produtos
    for (int i = 0; i < num_produtos; i++) {
        free(tabela_produtos[i].nome);
    }
    num_produtos = 0;
    
    // Limpar depósitos
    for (int i = 0; i < num_depositos; i++) {
        free(depositos[i].nome);
        for (int j = 0; j < depositos[i].num_itens; j++) {
            free(depositos[i].itens[j].nome);
        }
    }
    num_depositos = 0;
    
    // Limpar carrinhos
    for (int i = 0; i < num_carrinhos; i++) {
        free(carrinhos[i].nome);
        for (int j = 0; j < carrinhos[i].num_itens; j++) {
            free(carrinhos[i].itens[j].nome);
        }
    }
    num_carrinhos = 0;
    
    // Limpar variáveis
    for (int i = 0; i < num_variaveis; i++) {
        free(variaveis[i].nome);
    }
    num_variaveis = 0;
    
    total_compra = 0.0;
}

void executar_demo_vm() {
    
    // Demo: criar produtos
    adicionar_produto("Arroz", 1001, 25.90);
    adicionar_produto("Feijão", 1002, 8.50);
    adicionar_produto("Açúcar", 1003, 4.75);
    
    // Demo: criar depósito
    criar_deposito("Estoque Principal");
    
    // Demo: adicionar estoque
    adicionar_ao_deposito("Estoque Principal", 1001, 50);
    adicionar_ao_deposito("Estoque Principal", 1002, 30);
    adicionar_ao_deposito("Estoque Principal", 1003, 20);
    
    // Demo: criar carrinho
    criar_carrinho("Carrinho Cliente");
    
    // Demo: adicionar ao carrinho
    adicionar_ao_carrinho("Carrinho Cliente", "Estoque Principal", 1001, 2);
    adicionar_ao_carrinho("Carrinho Cliente", "Estoque Principal", 1002, 1);
    
    // Demo: consultas
    consultar_carrinho("Carrinho Cliente");
    consultar_deposito("Estoque Principal", 1001);
    
}

// Função auxiliar para converter expressão para string
char* expr_to_string(double value) {
    static char buffer[50];
    if (value == (int)value) {
        sprintf(buffer, "%d", (int)value);
    } else {
        sprintf(buffer, "%.2f", value);
    }
    return buffer;
}

int yylex(void);
void yyerror(const char *s);
extern FILE* yyin;

// Pilha para controle de condições
int condicao_stack[100];
int condicao_top = -1;

void push_condicao(int valor) {
    if (condicao_top < 99) {
        condicao_stack[++condicao_top] = valor;
    }
}

int pop_condicao() {
    if (condicao_top >= 0) {
        return condicao_stack[condicao_top--];
    }
    return 1; // Default: executar
}

int peek_condicao() {
    if (condicao_top >= 0) {
        return condicao_stack[condicao_top];
    }
    return 1;
}

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
%token JMP

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
        if (asm_file && executar_operacoes) {
            emit("    ; Criar depósito: %s", $2);
        }
        criar_deposito($2);
    };

OPERATIONS:
    | OPERATIONS OPERATION;

CONDITIONAL:
    IF EXPRESSAO {
        int condicao = ($2 != 0.0);
        push_condicao(condicao && peek_condicao());
        if (asm_file) {
            emit("    ; INICIAR CONDICIONAL IF: %s (condição: %s)", expr_to_string($2), condicao ? "verdadeira" : "falsa");
            if (!condicao) {
                emit("    ; PULAR BLOCO IF - CONDICAO FALSA");
            }
        }
    }
    OPERATIONS;

STOP_COND:
    STOP {
        pop_condicao();
        if (asm_file) {
            emit("    ; FIM CONDICIONAL IF");
        }
    }
    |
    STOP_WHILE {
        current_while_depth--;
        pop_condicao();
        
        if (asm_file) {
            emit("    jmp label_%d ; VOLTAR AO INICIO DO WHILE", while_start_labels[current_while_depth]);
            emit("label_%d: ; FIM WHILE", while_end_labels[current_while_depth]);
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
    WHILE EXPRESSAO {  // CORREÇÃO: EXPRESSAO declarada aqui
        int start_label = new_label();
        int end_label = new_label();
        while_start_labels[current_while_depth] = start_label;
        while_end_labels[current_while_depth] = end_label;
        current_while_depth++;
        
        int condicao = ($2 != 0.0);
        int execucao_atual = condicao && peek_condicao();
        
        if (asm_file) {
            emit("label_%d: ; INICIO WHILE", start_label);
            emit("    ; VERIFICAR CONDICAO WHILE");
            emit("    ; CONDICAO WHILE: %s = %s", expr_to_string($2), condicao ? "verdadeira" : "falsa");
            if (!execucao_atual) {
                emit("    jmp label_%d ; SAIR DO WHILE - CONDICAO FALSA", while_end_labels[current_while_depth-1]);
            }
        }
        
        push_condicao(execucao_atual);
    }
    WHILE_BODY;

WHILE_BODY:
    OPERATION
    | WHILE_BODY OPERATION
    ;

PRINTER:
    MESSAGE PAR_OPEN STRING PAR_CLOSE END_LINE {
        if (asm_file && executar_operacoes) {
            emit("    ; Mensagem: \"%s\"", $3);
        }
    };

STOQUE:
    PRODUCT nome_produto ASSIGN NUMBER PRICE EXPRESSAO END_LINE {
        if (asm_file && executar_operacoes) {
            emit("    ; Definir produto: %s, QR: %d, Preço: %.2f", $2, (int)$4, $6);
        }
        adicionar_produto($2, $4, $6);
    };

DEPOSITO_OPERATION:
    nome_deposito COLCH_OPEN PRODUCT qr_code COLCH_CLOSE PROVIDE INCREASE EXPRESSAO END_LINE {
        if (asm_file && executar_operacoes) {
            emit("    ; Adicionar ao depósito %s: produto %d, quantidade %d", $1, $4, (int)$8);
        }
        adicionar_ao_deposito($1, $4, (int)$8);
    };

CREATE_CART_OPERATION:
    CREATE_CART nome_carrinho PROVIDE COLCH_OPEN COLCH_CLOSE END_LINE {
        if (asm_file && executar_operacoes) {
            emit("    ; Criar carrinho: %s", $2);
        }
        criar_carrinho($2);
    };

VARIAVEL_OPERATION:
    GUARD nome_variavel ASSIGN EXPRESSAO END_LINE {
        if (asm_file && executar_operacoes) {
            emit("    ; Guardar variável: %s = %.2f", $2, $4);
        }
        definir_variavel($2, $4, 1);
    }
    |
    GUARD nome_variavel END_LINE {
        if (asm_file && executar_operacoes) {
            emit("    ; Declarar variável: %s", $2);
        }
        definir_variavel($2, 0.0, 0);
    }
    |
    nome_variavel ASSIGN EXPRESSAO END_LINE {
        if (asm_file && executar_operacoes) {
            emit("    ; Atribuir variável: %s = %.2f", $1, $3);
        }
        definir_variavel($1, $3, 1);
    }
    |
    CONSULT nome_variavel END_LINE {
        if (asm_file && executar_operacoes) {
            emit("    ; Consultar variável: %s", $2);
        }
        consultar_variavel_nome($2);
    }
    |
    CONSULT qr_code END_LINE {
        if (asm_file && executar_operacoes) {
            emit("    ; Consultar produto: QR %d", $2);
        }
        consultar_produto($2);
    }
    |
    CONSULT CART nome_carrinho COLCH_OPEN qr_code COLCH_CLOSE END_LINE {
        if (asm_file && executar_operacoes) {
            emit("    ; Consultar carrinho %s[%d]", $3, $5);
        }
        consultar_item_carrinho($3, $5);
    }
    |
    CONSULT CART nome_carrinho END_LINE {
        if (asm_file && executar_operacoes) {
            emit("    ; Consultar carrinho completo: %s", $3);
        }
        consultar_carrinho($3);
    }
    |
    CONSULT DEPOSIT nome_deposito COLCH_OPEN qr_code COLCH_CLOSE END_LINE {
        if (asm_file && executar_operacoes) {
            emit("    ; Consultar depósito %s[%d]", $3, $5);
        }
        consultar_deposito($3, $5);
    };

EXPRESSAO: 
    NUMBER { 
        $$ = $1; 
    }
    |
    DECIMAL { $$ = $1; }
    |
    PRODUCT qr_code { 
        $$ = consultar_produto($2);
    }
    |
    nome_variavel { 
        $$ = consultar_variavel_nome($1);
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
        if (asm_file && executar_operacoes) {
            emit("    ; Adicionar ao carrinho %s: produto %d do depósito %s, quantidade %d", $1, $5, $3, (int)$8);
        }
        adicionar_ao_carrinho($1, $3, $5, (int)$8);
    };

VENDA: 
    SELL nome_carrinho COLCH_OPEN qr_code COLCH_CLOSE SCAN EXPRESSAO END_LINE {
        if (asm_file && executar_operacoes) {
            emit("    ; Vender do carrinho %s: produto %d, quantidade %d", $2, $4, (int)$7);
        }
        processar_venda($2, $4, (int)$7);
    }
    |
    PAYMENT PAGAMENTO MAQUININHA END_LINE {
        if (asm_file && executar_operacoes) {
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
    
    // Inicializar pilha de condições
    push_condicao(1); // Condição inicial: executar
    
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