%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Evitar problema de memória, q n funciona com mais de 100000
#define MAX_PRODUTOS 170
#define MAX_DEPOSITO 170
#define MAX_VARIAVEIS 10000

// ========== VIRTUAL MACHINE ==========
#define MEM_SIZE 256
#define NUM_REG 4
#define STACK_SIZE 100

typedef struct {
    int registers[NUM_REG];          // 4 registradores
    int memory[MEM_SIZE];            // Memória principal
    int stack[STACK_SIZE];           // Pilha
    int sp;                          // Stack pointer
    int pc;                          // Program counter
    int running;                     // Estado da VM
    
    // Sensores (variáveis readonly)
    const int sensor_tempo;
    const int sensor_transacoes;
    const int sensor_estoque_total;
} VM;

VM vm;

// Instruções da VM
void vm_init() {
    memset(&vm, 0, sizeof(VM));
    vm.sp = -1;
    vm.pc = 0;
    vm.running = 1;
    
    // Inicializar sensores (valores fictícios)
    *(int*)&vm.sensor_tempo = 1000;
    *(int*)&vm.sensor_transacoes = 42;
    *(int*)&vm.sensor_estoque_total = 500;
}

void vm_push(int value) {
    if (vm.sp < STACK_SIZE - 1) {
        vm.stack[++vm.sp] = value;
    }
}

int vm_pop() {
    if (vm.sp >= 0) {
        return vm.stack[vm.sp--];
    }
    return 0;
}

void vm_add() { 
    int b = vm_pop();
    int a = vm_pop();
    vm_push(a + b);
}

void vm_sub() { 
    int b = vm_pop();
    int a = vm_pop();
    vm_push(a - b);
}

void vm_mul() { 
    int b = vm_pop();
    int a = vm_pop();
    vm_push(a * b);
}

void vm_div() { 
    int b = vm_pop();
    int a = vm_pop();
    if (b != 0) vm_push(a / b);
    else vm_push(0);
}

void vm_load(int addr) {
    if (addr >= 0 && addr < MEM_SIZE) {
        vm_push(vm.memory[addr]);
    }
}

void vm_store(int addr) {
    if (addr >= 0 && addr < MEM_SIZE) {
        vm.memory[addr] = vm_pop();
    }
}

void vm_jmp(int addr) {
    vm.pc = addr;
}

void vm_jz(int addr) {
    if (vm_pop() == 0) {
        vm.pc = addr;
    }
}

void vm_halt() {
    vm.running = 0;
}

// Demonstração da VM - programa que calcula fatorial
void executar_demo_vm() {
    vm_init();
    vm_push(5);
    vm_push(3);
    vm_push(2);
    vm_mul();
    vm_add();
}

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

// ========== FUNÇÕES PARA PRODUTOS ==========
void adicionar_produto(char* nome, int qr_code, double preco) {
    if (!executar_operacoes) return;
    
    if(num_produtos < MAX_PRODUTOS) {
        for(int i = 0; i < num_produtos; i++) {
            if(tabela_produtos[i].qr_code == qr_code) {
                printf("Erro: QR_CODE %d já existe para o produto %s\n", qr_code, tabela_produtos[i].nome);
                return;
            }
        }
        
        tabela_produtos[num_produtos].nome = strdup(nome);
        tabela_produtos[num_produtos].qr_code = qr_code;
        tabela_produtos[num_produtos].preco = preco;
        tabela_produtos[num_produtos].definido = 1;
        num_produtos++;
    } else {
        printf("Erro: Limite máximo de produtos atingido(170)\n");
    }
}

int encontrar_produto_qr(int qr_code) {
    for(int i = 0; i < num_produtos; i++) {
        if (tabela_produtos[i].qr_code == qr_code && tabela_produtos[i].definido) {
            return i;
        }
    }
    return -1;
}

double consultar_produto(int qr_code) {
    int indice = encontrar_produto_qr(qr_code);
    if(indice >= 0) {
        return tabela_produtos[indice].preco;
    }
    return 0.0;
}

// ========== FUNÇÕES PARA DEPÓSITOS ==========
int encontrar_deposito(char* nome) {
    for(int i = 0; i < num_depositos; i++) {
        if(strcmp(depositos[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

void criar_deposito(char* nome) {
    if (!executar_operacoes) return;
    
    if(encontrar_deposito(nome) >= 0) {
        printf("Erro: Depósito '%s' já existe\n", nome);
        return;
    }
    
    if(num_depositos < MAX_DEPOSITO) {
        depositos[num_depositos].nome = strdup(nome);
        depositos[num_depositos].num_itens = 0;
        depositos[num_depositos].criado = 1;
        num_depositos++;
        printf("Depósito '%s' criado\n", nome);
    } else {
        printf("Erro: Limite máximo de depósitos atingido\n");
    }
}

void adicionar_ao_deposito(char* nome_deposito, int qr_code, int quantidade) {
    if (!executar_operacoes) return;
    
    int indice_deposito = encontrar_deposito(nome_deposito);
    if(indice_deposito < 0) {
        printf("Erro: Depósito '%s' não foi criado\n", nome_deposito);
        return;
    }
    
    int indice_produto = encontrar_produto_qr(qr_code);
    if(indice_produto < 0) {
        printf("Erro: Produto com QR_CODE %d não definido\n", qr_code);
        return;
    }
    
    Deposito* dep = &depositos[indice_deposito];
    
    // Verificar se o produto já existe no depósito
    for(int i = 0; i < dep->num_itens; i++) {
        if(dep->itens[i].qr_code == qr_code) {
            dep->itens[i].quantidade = quantidade;
            return;
        }
    }
    
    // Adicionar novo item ao depósito
    if(dep->num_itens < MAX_DEPOSITO) {
        dep->itens[dep->num_itens].nome = strdup(tabela_produtos[indice_produto].nome);
        dep->itens[dep->num_itens].qr_code = qr_code;
        dep->itens[dep->num_itens].quantidade = quantidade;
        dep->num_itens++;
    } else {
        printf("Erro: Limite máximo do depósito '%s' atingido\n", nome_deposito);
    }
}

int consultar_deposito(char* nome_deposito, int qr_code) {
    int indice_deposito = encontrar_deposito(nome_deposito);
    if(indice_deposito < 0) {
        return 0;
    }
    
    Deposito* dep = &depositos[indice_deposito];
    for(int i = 0; i < dep->num_itens; i++) {
        if(dep->itens[i].qr_code == qr_code) {
            return dep->itens[i].quantidade;
        }
    }
    return 0;
}

// ========== FUNÇÕES PARA CARRINHOS ==========
int encontrar_carrinho(char* nome) {
    for(int i = 0; i < num_carrinhos; i++) {
        if(strcmp(carrinhos[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

void criar_carrinho(char* nome) {
    if (!executar_operacoes) return;
    
    if(encontrar_carrinho(nome) >= 0) {
        printf("Erro: Carrinho '%s' já existe\n", nome);
        return;
    }
    
    if(num_carrinhos < MAX_PRODUTOS) {
        carrinhos[num_carrinhos].nome = strdup(nome);
        carrinhos[num_carrinhos].num_itens = 0;
        carrinhos[num_carrinhos].criado = 1;
        num_carrinhos++;
        printf("Carrinho '%s' criado\n", nome);
    } else {
        printf("Erro: Limite máximo de carrinhos atingido\n");
    }
}

void adicionar_ao_carrinho(char* nome_carrinho, char* nome_deposito, int qr_code, int quantidade) {
    if (!executar_operacoes) return;
    
    int indice_carrinho = encontrar_carrinho(nome_carrinho);
    if(indice_carrinho < 0) {
        printf("Erro: Carrinho '%s' não foi criado\n", nome_carrinho);
        return;
    }
    
    int estoque_disponivel = consultar_deposito(nome_deposito, qr_code);
    int indice_produto = encontrar_produto_qr(qr_code);
    
    if(indice_produto >= 0) {
        if(estoque_disponivel >= quantidade) {
            Carrinho* car = &carrinhos[indice_carrinho];
            
            // Verificar se o produto já existe no carrinho
            for(int i = 0; i < car->num_itens; i++) {
                if(car->itens[i].qr_code == qr_code) {
                    car->itens[i].quantidade_carrinho += quantidade;
                    return;
                }
            }
            
            // Adicionar novo item ao carrinho
            if(car->num_itens < MAX_PRODUTOS) {
                car->itens[car->num_itens].qr_code = qr_code;
                car->itens[car->num_itens].quantidade_carrinho = quantidade;
                car->itens[car->num_itens].quantidade_vendida = 0;
                car->itens[car->num_itens].preco_unitario = tabela_produtos[indice_produto].preco;
                car->num_itens++;
            }
        } else {
            printf("Erro: Estoque insuficiente no depósito '%s'. Disponível: %d, Solicitado: %d\n", 
                   nome_deposito, estoque_disponivel, quantidade);
        }
    } else {
        printf("Erro: Produto %d não definido\n", qr_code);
    }
}

int encontrar_item_carrinho(char* nome_carrinho, int qr_code) {
    int indice_carrinho = encontrar_carrinho(nome_carrinho);
    if(indice_carrinho < 0) {
        return -1;
    }
    
    Carrinho* car = &carrinhos[indice_carrinho];
    for(int i = 0; i < car->num_itens; i++) {
        if (car->itens[i].qr_code == qr_code) {
            return i;
        }
    }
    return -1;
}

void processar_venda(char* nome_carrinho, int qr_code, int quantidade_venda) {
    if (!executar_operacoes) return;
    
    int indice_carrinho = encontrar_carrinho(nome_carrinho);
    if(indice_carrinho < 0) {
        printf("Erro: Carrinho '%s' não foi criado\n", nome_carrinho);
        return;
    }
    
    int indice_item = encontrar_item_carrinho(nome_carrinho, qr_code);
    int indice_produto = encontrar_produto_qr(qr_code);
    
    if(indice_item >= 0 && indice_produto >= 0) {
        Carrinho* car = &carrinhos[indice_carrinho];
        int quantidade_disponivel = car->itens[indice_item].quantidade_carrinho - 
                                   car->itens[indice_item].quantidade_vendida;
        
        if(quantidade_venda <= quantidade_disponivel) {
            double valor_venda = car->itens[indice_item].preco_unitario * quantidade_venda;
            total_compra += valor_venda;
            car->itens[indice_item].quantidade_vendida += quantidade_venda;
        } else {
            printf("Erro: Quantidade indisponível no carrinho. Disponível: %d, Solicitado: %d\n", 
                   quantidade_disponivel, quantidade_venda);
        }
    } else {
        printf("Erro: Produto %d não encontrado no carrinho '%s'\n", qr_code, nome_carrinho);
    }
}

void consultar_carrinho(char* nome_carrinho) {
    if (!executar_operacoes) return;
    
    int indice_carrinho = encontrar_carrinho(nome_carrinho);
    if(indice_carrinho < 0) {
        printf("Erro: Carrinho '%s' não foi criado\n", nome_carrinho);
        return;
    }
    
    Carrinho* car = &carrinhos[indice_carrinho];
    printf("========== CARRINHO %s =========\n", nome_carrinho);
    for(int i = 0; i < car->num_itens; i++) {
        int indice_produto = encontrar_produto_qr(car->itens[i].qr_code);
        if(indice_produto >= 0) {
            printf("%s: %d no carrinho (%d vendidas)\n",
                tabela_produtos[indice_produto].nome,
                car->itens[i].quantidade_carrinho - car->itens[i].quantidade_vendida,
                car->itens[i].quantidade_vendida);
        }
    }
    printf("===================\n");
}

void consultar_item_carrinho(char* nome_carrinho, int qr_code) {
    if (!executar_operacoes) return;
    
    int indice_carrinho = encontrar_carrinho(nome_carrinho);
    if(indice_carrinho < 0) {
        printf("Erro: Carrinho '%s' não foi criado\n", nome_carrinho);
        return;
    }
    
    int indice_item = encontrar_item_carrinho(nome_carrinho, qr_code);
    if(indice_item >= 0) {
        Carrinho* car = &carrinhos[indice_carrinho];
        printf("Carrinho %s[%d] = %d unidades\n", 
            nome_carrinho, qr_code, 
            car->itens[indice_item].quantidade_carrinho - car->itens[indice_item].quantidade_vendida);
    } else {
        printf("Carrinho %s[%d] = 0 unidades\n", nome_carrinho, qr_code);
    }
}

// ========== FUNÇÕES PARA VARIÁVEIS ==========
void definir_variavel(char* nome, double valor, int eh_declaracao) {
    if (!executar_operacoes) return;
    
    for(int i = 0; i < num_variaveis; i++) {
        if(strcmp(variaveis[i].nome, nome) == 0) {
            if (eh_declaracao && variaveis[i].declarada) {
                printf("Erro: Variável '%s' já foi declarada\n", nome);
                return;
            }
            variaveis[i].valor = valor;
            if (eh_declaracao) {
                variaveis[i].declarada = 1;
            }
            return;
        }
    }
    variaveis[num_variaveis].nome = strdup(nome);
    variaveis[num_variaveis].valor = valor;
    variaveis[num_variaveis].declarada = eh_declaracao ? 1 : 0;
    num_variaveis++;
}

double consultar_variavel_nome(char* nome) {
    for(int i = 0; i < num_variaveis; i++) {
        if(strcmp(variaveis[i].nome, nome) == 0) {
            return variaveis[i].valor;
        }
    }
    printf("Erro: Variável '%s' não declarada\n", nome);
    return 0.0;
}

// ========== FUNÇÕES DE NOTA FISCAL E LIMPEZA ==========
void nota_fiscal() {
    printf("\n=== NOTA FISCAL ===\n");
    printf("Produtos vendidos:\n");
    
    for(int i = 0; i < num_carrinhos; i++) {
        Carrinho* car = &carrinhos[i];
        for(int j = 0; j < car->num_itens; j++) {
            if(car->itens[j].quantidade_vendida > 0) {
                int indice_produto = encontrar_produto_qr(car->itens[j].qr_code);
                if(indice_produto >= 0) {
                    double subtotal = car->itens[j].preco_unitario * car->itens[j].quantidade_vendida;
                    printf("- %dx %s (carrinho %s): R$ %.2f\n", 
                           car->itens[j].quantidade_vendida,
                           tabela_produtos[indice_produto].nome,
                           car->nome,
                           subtotal);
                }
            }
        }
    }
    printf("Total da venda: R$ %.2f\n", total_compra);
    printf("===================\n");
}

void limpa_tudo() {
    num_produtos = 0;
    num_depositos = 0;
    num_carrinhos = 0;
    num_variaveis = 0;
    total_compra = 0.0;
}

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
%token IF WHILE STOP AND OR
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
        printf("\nCompra finalizada!\n");
        // EXECUTAR DEMONSTRAÇÃO DA VM
        executar_demo_vm();
        return 0; 
    };

CREATE_DEPOSIT_OPERATION:
    CREATE_DEPOSIT nome_deposito PROVIDE KEY_OPEN KEY_CLOSE END_LINE {
        criar_deposito($2);
    };

OPERATIONS:
    | OPERATIONS OPERATION;

CONDITIONAL:
    IF EXPRESSAO {
        if (!$2) {
            executar_operacoes = 0;
        }
    };

STOP_COND:
    STOP {
        nivel_condicional--;
        executar_operacoes = 1;
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
    STOP_COND
    |
    PRINTER;

PRINTER:
    MESSAGE PAR_OPEN STRING PAR_CLOSE END_LINE {
        printf("%s\n", $3);
    }

STOQUE:
    PRODUCT nome_produto ASSIGN NUMBER PRICE EXPRESSAO END_LINE {
        adicionar_produto($2, (int)$4, $6);
    };

DEPOSITO_OPERATION:
    nome_deposito COLCH_OPEN PRODUCT qr_code COLCH_CLOSE PROVIDE INCREASE EXPRESSAO END_LINE {
        adicionar_ao_deposito($1, $4, (int)$8);
    };

CREATE_CART_OPERATION:
    CREATE_CART nome_carrinho PROVIDE COLCH_OPEN COLCH_CLOSE END_LINE {
        criar_carrinho($2);
    };

VARIAVEL_OPERATION:
    GUARD nome_variavel ASSIGN EXPRESSAO END_LINE {
        definir_variavel($2, $4, 1);
    }
    |
    GUARD nome_variavel END_LINE {
        definir_variavel($2, 0, 1);
    }
    |
    nome_variavel ASSIGN EXPRESSAO END_LINE {
        int valor_atual = consultar_variavel_nome($1);
        if(valor_atual == 0) {
            int encontrada = 0;
            for(int i = 0; i < num_variaveis; i++) {
                if(strcmp(variaveis[i].nome, $1) == 0) {
                    encontrada = 1;
                    break;
                }
            }
            if(!encontrada) {
                printf("Erro: Variável %s não declarada. Use GUARD primeiro.\n", $1);
            } else {
                definir_variavel($1, $3, 0);
            }
        } else {
            definir_variavel($1, $3, 0);
        }
    }
    |
    CONSULT nome_variavel END_LINE {
        if (executar_operacoes) {
            double valor = consultar_variavel_nome($2);
            printf("Variavel %s = %.2f\n", $2, valor);
        }
    }
    |
    CONSULT qr_code END_LINE {
        if (executar_operacoes) {
            double valor = consultar_produto($2);
            printf("Produto %d = R$ %.2f\n", $2, valor);
        }
    }
    |
    CONSULT CART nome_carrinho COLCH_OPEN qr_code COLCH_CLOSE END_LINE {
        if (executar_operacoes) {
            consultar_item_carrinho($3, $5);
        }
    }
    |
    CONSULT CART nome_carrinho END_LINE {
        if (executar_operacoes) {
            consultar_carrinho($3);
        }
    }
    |
    CONSULT DEPOSIT nome_deposito COLCH_OPEN qr_code COLCH_CLOSE END_LINE {
        if (executar_operacoes) {
            int qtd = consultar_deposito($3, $5);
            printf("Depósito %s[%d] = %d quantidades\n", $3, $5, qtd);
        }
    };

EXPRESSAO: 
    NUMBER { $$ = $1; }
    |
    DECIMAL { $$ = $1; }
    |
    PRODUCT qr_code { $$ = consultar_produto($2); }
    |
    nome_variavel { $$ = consultar_variavel_nome($1); }
    |
    EXPRESSAO MULT EXPRESSAO { $$ = $1 * $3; }
    |
    EXPRESSAO DIV EXPRESSAO { if($3 == 0) {
            printf("Erro: Divisão por zero!\n");
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
        adicionar_ao_carrinho($1, $3, $5, (int)$8);
    };

VENDA: 
    SELL nome_carrinho COLCH_OPEN qr_code COLCH_CLOSE SCAN EXPRESSAO END_LINE {
        processar_venda($2, $4, (int)$7);
    }
    |
    PAYMENT PAGAMENTO MAQUININHA END_LINE {
        if (strcmp($3, "APROVADO") == 0) {
            nota_fiscal();
            printf("Método de pagamento: %s\n", $2);
            printf("Status: %s\n", $3);
        } else {
            printf("\nPagamento %s\n", $3);
        }
        limpa_tudo();
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
    printf("======= Mercadinho do seu Prédinho ======\n");
    vm_init();
    
    if (argc > 1) {
        FILE* file = fopen(argv[1], "r");
        if(!file) {
            fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s\n", argv[1]);
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
// Para debug

// flex lexer.l
//bison -d -t parser.y
//gcc -DYYDEBUG=1 lex.yy.c parser.tab.c -o mercadinho
//./mercadinho exemplo.jota