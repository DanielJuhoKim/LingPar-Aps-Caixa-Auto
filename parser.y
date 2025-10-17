%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    char* nome;
    int qr_code;
    double preco;
    int estoque;
} Produto;

typedef struct {
    char* nome;
    int quantidade;
    double preco_unitario;
} carrinho;

Produto tabela_produtos[100];
int num_produtos = 0;

carrinho produtos_carrinho[100];
int num_vendas = 0;
double total_compra = 0.0;

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

int encontrar_produto_qr(int qr_code) {
    for(int i = 0; i < num_produtos; i++) {
        if (tabela_produtos[i].qr_code == qr_code) {
            return i;
        }
    }
    return -1;
}

void adicionar_venda(int qr_code, int quantidade, double preco_unitario) {
    if(num_vendas < 100) {
        int indice = encontrar_produto_qr(qr_code);

        char* nome = tabela_produtos[indice].nome;

        for(int i = 0; i < num_vendas; i++) {
            if(strcmp(produtos_carrinho[i].nome, nome) == 0) {
                produtos_carrinho[i].quantidade += quantidade;
                return;
            }
        }
        
        produtos_carrinho[num_vendas].nome = strdup(nome);
        produtos_carrinho[num_vendas].quantidade = quantidade;
        produtos_carrinho[num_vendas].preco_unitario = preco_unitario;
        num_vendas++;
    }
}

void nota_fiscal() {
    printf("Produtos: ");
    for(int i = 0; i < num_vendas; i++) {
        printf("%dx %s", produtos_carrinho[i].quantidade, produtos_carrinho[i].nome);
        if(i < num_vendas - 1) {
            printf(", ");
        }
    }
    printf("\n");
}

void limpar_vendas() {
    for(int i = 0; i < num_vendas; i++) {
        free(produtos_carrinho[i].nome);
    }
    num_vendas = 0;
    total_compra = 0.0;
}

int yylex(void);
void yyerror(const char *s);
extern FILE* yyin;

int pagamento_aprovado = 1;

%}

%union {
    int num;
    char* str;
    int bool_val;
}

%token <num> NUMBER
%token <str> STRING IDENTIFIER
%token <bool_val> BOOLEAN

%token START END PRODUCT STOCK SELL PAYMENT CREDIT DEBIT PIX
%token PRINT PRICE PAYMENT_APPROVED
%token APROVED REFUSED
%token PROVIDE PLUS MINUS END_LINE ASSIGN

%type <str> nome_produto
%type <num> numero_preco quantidade qr_code
%type <str> PAGAMENTO MAQUININHA

%%

CAIXA_AUTO: 
    START OPERATION END { 
        printf("\nCompra finalizada!\n"); 
        return 0; 
    };

OPERATION: 
    |
    OPERATION STOQUE
    |
    OPERATION CARRINHO;

STOQUE:
    PRODUCT nome_produto ASSIGN NUMBER PRICE numero_preco END_LINE {
        adicionar_produto($2, (int)$4, $6, 0);
    } // Vai criar/definir o produto
    |
    STOCK nome_produto ASSIGN NUMBER END_LINE {
        int indice = encontrar_produto($2);

        if(indice >= 0) {
            tabela_produtos[indice].estoque = (int)$4;
        }

    }; // Vai definir a quantidade de um produto no estoque

CARRINHO: 
    SELL qr_code PROVIDE quantidade END_LINE {
        int indice = encontrar_produto_qr($2);

        if(indice >= 0) {
            if(tabela_produtos[indice].estoque >= (int)$4) {
                double valor_venda = tabela_produtos[indice].preco * $4;
                total_compra += valor_venda;
                tabela_produtos[indice].estoque -= (int)$4;
                
                adicionar_venda($2, (int)$4, tabela_produtos[indice].preco);
            }

            else {
                printf("Erro: Estoque insuficiente\n");
            }
        }

        else {
            printf("Erro: Produto %d não encontrado\n", $2);
        }
    }
    |
    PAYMENT PAGAMENTO MAQUININHA END_LINE {
        if (strcmp($3, "APROVADO") == 0) {
            nota_fiscal();
            printf("Método de pagamento: %s\n", $2);
            printf("Total da venda: R$ %.2f\n", total_compra);
            printf("Status: %s\n", $3);
        }
        
        else {
            printf("\nPagamento %s\n", $3);
        }
        
        limpar_vendas();
        num_produtos = 0;
    }

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

numero_preco: NUMBER { $$ = $1; };

quantidade: NUMBER { $$ = $1; };

qr_code: NUMBER { $$ = $1; };

%%

void yyerror(const char *s) {
    fprintf(stderr, "[PARSER] Erro de sintaxe: %s\n", s);
}

int main(int argc, char *argv[]) {
    printf("======= Mercadinho do seu Prédinho ======\n");
    
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