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

Produto tabela_produtos[100];
int num_produtos = 0;
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
    printf("\n=== RESUMO DA COMPRA ===\n");
    
    // Contar produtos únicos e suas quantidades
    typedef struct {
        char* nome;
        int quantidade;
        double preco_unitario;
    } ItemResumo;
    
    ItemResumo itens[100];
    int num_itens = 0;
    
    for(int i = 0; i < num_produtos; i++) {
        int encontrado = 0;
        for(int j = 0; j < num_itens; j++) {
            if(strcmp(itens[j].nome, tabela_produtos[i].nome) == 0) {
                itens[j].quantidade++;
                encontrado = 1;
                break;
            }
        }
        if(!encontrado && num_itens < 100) {
            itens[num_itens].nome = tabela_produtos[i].nome;
            itens[num_itens].quantidade = 1;
            itens[num_itens].preco_unitario = tabela_produtos[i].preco;
            num_itens++;
        }
    }
    
    // Imprimir produtos
    printf("Produtos: ");
    for(int i = 0; i < num_itens; i++) {
        printf("%dx %s", itens[i].quantidade, itens[i].nome);
        if(i < num_itens - 1) {
            printf(", ");
        }
    }
    printf("\n");
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
%token PLUS MINUS MULT DIV ASSIGN EQ NEQ GT LT GTE LTE

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
    PRODUCT identificador '=' NUMBER WITH_PRICE numero_preco ';' {
        adicionar_produto($2, (int)$4, $6, 0);
    }
    | STOCK identificador '=' NUMBER ';' {
        int idx = encontrar_produto($2);
        if(idx >= 0) {
            tabela_produtos[idx].estoque = (int)$4;
        }
    }
    ;

instrucao: 
    SELL identificador ':' QUANTITY quantidade ';' {
        int idx = encontrar_produto($2);
        if(idx >= 0) {
            if(tabela_produtos[idx].estoque >= (int)$5) {
                double valor_venda = tabela_produtos[idx].preco * $5;
                total_venda += valor_venda;
                tabela_produtos[idx].estoque -= (int)$5;
            } else {
                printf("Erro: Estoque insuficiente para %s\n", $2);
            }
        } else {
            printf("Erro: Produto %s não encontrado\n", $2);
        }
    }
    | PAYMENT metodo_pagamento resultado_pagamento ';' {
        imprimir_resumo_compra();
        printf("Método de pagamento: %s\n", $2);
        printf("Total da venda: R$ %.2f\n", total_venda);
        printf("Status: %s\n", $3);
        
        // Reset para próxima compra
        num_produtos = 0;
        total_venda = 0.0;
    }
    | IF condicao THEN bloco ';'
    | IF condicao THEN bloco ELSE bloco ';'
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
    fprintf(stderr, "Erro de sintaxe: %s\n", s);
}

int main(int argc, char *argv[]) {
    printf("======= Sistema de Vendas ======\n");
    
    if (argc > 1) {
        // Modo arquivo (./programa arquivo.jota)
        FILE* file = fopen(argv[1], "r");
        if(!file) {
            fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s\n", argv[1]);
            return 1;
        }
        yyin = file;
        printf("Processando arquivo: %s\n", argv[1]);
    } else {
        // Verifica se a entrada padrão é um terminal (não redirecionada)
        int is_terminal = isatty(fileno(stdin));
        
        if (!is_terminal) {
            // Modo redirecionamento (< arquivo)
            printf("Processando entrada redirecionada...\n");
        } else {
            // Modo interativo
            printf("Digite os comandos (INICIAR para começar, Ctrl+D para finalizar):\n");
        }
        yyin = stdin;
    }
    
    int result = yyparse();
    
    if (argc > 1 && yyin != stdin) {
        fclose(yyin);
    }
    
    return result;
}