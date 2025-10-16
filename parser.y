%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Tabela de símbolos
typedef struct {
    char* nome;
    int qr_code;
    double preco;
    int estoque;
} Produto;

// Estrutura para armazenar vendas realizadas
typedef struct {
    char* nome;
    int quantidade;
    double preco_unitario;
} Venda;

Produto tabela_produtos[100];
int num_produtos = 0;

Venda vendas_realizadas[100];
int num_vendas = 0;
double total_venda = 0.0;

void adicionar_produto(char* nome, int qr_code, double preco, int estoque) {
    if(num_produtos < 100) {
        tabela_produtos[num_produtos].nome = strdup(nome);
        tabela_produtos[num_produtos].qr_code = qr_code;
        tabela_produtos[num_produtos].preco = preco;
        tabela_produtos[num_produtos].estoque = estoque;
        num_produtos++;
    }
}

void adicionar_venda(char* nome, int quantidade, double preco_unitario) {
    if(num_vendas < 100) {
        // Verifica se já existe uma venda para este produto
        for(int i = 0; i < num_vendas; i++) {
            if(strcmp(vendas_realizadas[i].nome, nome) == 0) {
                vendas_realizadas[i].quantidade += quantidade;
                return;
            }
        }
        
        // Se não existe, adiciona nova venda
        vendas_realizadas[num_vendas].nome = strdup(nome);
        vendas_realizadas[num_vendas].quantidade = quantidade;
        vendas_realizadas[num_vendas].preco_unitario = preco_unitario;
        num_vendas++;
    }
}

int encontrar_produto(char* nome) {
    for(int i = 0; i < num_produtos; i++) {
        if(strcmp(tabela_produtos[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

// Função para imprimir apenas o resumo da compra
void imprimir_resumo_compra() {
    // Imprimir produtos vendidos
    printf("Produtos: ");
    for(int i = 0; i < num_vendas; i++) {
        printf("%dx %s", vendas_realizadas[i].quantidade, vendas_realizadas[i].nome);
        if(i < num_vendas - 1) {
            printf(", ");
        }
    }
    printf("\n");
}

void limpar_vendas() {
    for(int i = 0; i < num_vendas; i++) {
        free(vendas_realizadas[i].nome);
    }
    num_vendas = 0;
    total_venda = 0.0;
}

int yylex(void);
void yyerror(const char *s);
extern FILE* yyin;

int pagamento_aprovado = 1;

%}

%union {
    double num;
    char* str;
    int bool_val;
}

%token <num> NUMBER
%token <str> STRING IDENTIFIER
%token <bool_val> BOOLEAN

%token START END PRODUCT STOCK SELL PAYMENT CREDIT DEBIT PIX
%token IF THEN ELSE PRINT WITH_PRICE QUANTITY PAYMENT_APPROVED
%token APROVED REFUSED
%token PROVIDE PLUS MINUS MULT DIV ASSIGN EQ NEQ GT LT GTE LTE END_LINE

%type <num> expressao
%type <bool_val> condicao
%type <str> identificador
%type <num> numero_preco quantidade
%type <str> metodo_pagamento resultado_pagamento

%%

programa: 
    START bloco END { 
        printf("\nCompra finalizada!\n"); 
        return 0; 
    }
    ;

bloco: 
    | bloco declaracao
    | bloco instrucao
    ;

declaracao: 
    PRODUCT identificador ASSIGN NUMBER WITH_PRICE numero_preco END_LINE {
        adicionar_produto($2, (int)$4, $6, 0);
    }
    | STOCK identificador ASSIGN NUMBER END_LINE {
        int idx = encontrar_produto($2);
        if(idx >= 0) {
            tabela_produtos[idx].estoque = (int)$4;
        }
    }
    ;

instrucao: 
    SELL identificador QUANTITY quantidade END_LINE {
        int idx = encontrar_produto($2);
        if(idx >= 0) {
            if(tabela_produtos[idx].estoque >= (int)$4) {
                double valor_venda = tabela_produtos[idx].preco * $4;
                total_venda += valor_venda;
                tabela_produtos[idx].estoque -= (int)$4;
                
                // ADICIONAR VENDA À LISTA DE VENDAS REALIZADAS
                adicionar_venda($2, (int)$4, tabela_produtos[idx].preco);
            } else {
                printf("Erro: Estoque insuficiente para %s\n", $2);
            }
        } else {
            printf("Erro: Produto %s não encontrado\n", $2);
        }
    }
    | PAYMENT metodo_pagamento resultado_pagamento END_LINE {
        // VERIFICAR SE O PAGAMENTO FOI APROVADO ANTES DE IMPRIMIR O RESUMO
        if (strcmp($3, "APROVADO") == 0) {
            imprimir_resumo_compra();
            printf("Método de pagamento: %s\n", $2);
            printf("Total da venda: R$ %.2f\n", total_venda);
            printf("Status: %s\n", $3);
        } else {
            printf("\nPagamento %s\n", $3);
        }
        
        // Reset para próxima compra
        limpar_vendas();
        num_produtos = 0;
    }
    | IF condicao THEN bloco END_LINE
    | IF condicao THEN bloco ELSE bloco END_LINE
    ;

metodo_pagamento: 
    CREDIT { $$ = "CRÉDITO"; }
    | DEBIT { $$ = "DÉBITO"; }
    | PIX { $$ = "PIX"; }
    ;

resultado_pagamento:
    APROVED { $$ = "APROVADO"; }
    | REFUSED { $$ = "RECUSADO"; }
    ;

condicao: 
    PAYMENT_APPROVED { $$ = pagamento_aprovado; }
    | expressao GT expressao { $$ = $1 > $3; }
    | expressao LT expressao { $$ = $1 < $3; }
    | expressao GTE expressao { $$ = $1 >= $3; }
    | expressao LTE expressao { $$ = $1 <= $3; }
    | expressao EQ expressao { $$ = $1 == $3; }
    | expressao NEQ expressao { $$ = $1 != $3; }
    ;

expressao: 
    NUMBER { $$ = $1; }
    | identificador {
        int idx = encontrar_produto($1);
        if(idx >= 0) {
            $$ = tabela_produtos[idx].preco;
        } else {
            $$ = 0;
        }
    }
    | expressao PLUS expressao { $$ = $1 + $3; }
    | expressao MINUS expressao { $$ = $1 - $3; }
    | expressao MULT expressao { $$ = $1 * $3; }
    | expressao DIV expressao { 
        if($3 != 0) {
            $$ = $1 / $3; 
        } else {
            $$ = 0;
        }
    }
    ;

// Regras auxiliares para definir tipos
identificador: IDENTIFIER { $$ = $1; }
    ;

numero_preco: NUMBER { $$ = $1; }
    ;

quantidade: NUMBER { $$ = $1; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "[PARSER] Erro de sintaxe: %s\n", s);
}

int main(int argc, char *argv[]) {
    printf("======= Mercadinho do seu Prédinho ======\n");
    
    if (argc > 1) {
        // Modo arquivo como argumento
        FILE* file = fopen(argv[1], "r");
        if(!file) {
            fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s\n", argv[1]);
            return 1;
        }
        yyin = file;
    } else {
        // Modo redirecionamento ou interativo
        yyin = stdin;
    }
    
    int result = yyparse();
    
    if (argc > 1 && yyin != stdin) {
        fclose(yyin);
    }
    
    return result;
}