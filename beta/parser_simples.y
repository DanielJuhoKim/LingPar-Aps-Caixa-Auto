%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
void yyerror(const char *s);

int produtos[100];
int num_produtos = 0;

typedef struct {
    int qr_code;
    int preco;
} Stoque;

Stoque produtos_stoque[] = {
    {732,  10},
    {123,  8},
    {456, 5},
    {789, 25},
    {111, 4},
    {0, 0}
};

void adicionar_produto(int qr_code, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        if (num_produtos < 100) {
            produtos[num_produtos++] = qr_code;
        }
    }
}

void mostrar_produtos() {
    printf("Produtos: ");
    int total = 0;
    for (int i = 0; i < num_produtos; i++) {
        printf("%d", produtos[i]);
        for (int j = 0; produtos_stoque[j].qr_code != 0; j++) {
            if (produtos_stoque[j].qr_code == produtos[i]) {
                total += produtos_stoque[j].preco;
            }
        }
        if (i < num_produtos - 1) {
            printf(", ");
        }
    }
    printf("\n");

    printf("Total = %d\n", total);
}

void limpar_produtos() {
    num_produtos = 0;
}
%}

%union {
    int num;
    int op_count;  // Agora é int para contar operações
}

%token <num> NUMBER
%token CANCEL BACK CREDIT DEBIT PIX APROVED REFUSED
%token PLUS MINUS PROVIDE

%type <num> OPERACAO OPERACOES  // Mudamos para int

%%

CAIXA:
    COMPRA {
        mostrar_produtos();
        limpar_produtos();
    }
    |
    CANCEL {
        printf("Compra cancelada\n");
        limpar_produtos();
    };

COMPRA:
    LISTA_PRODUTOS PAGAMENTO;

LISTA_PRODUTOS:
    ITEM_PRODUTO
    |
    LISTA_PRODUTOS ITEM_PRODUTO;

ITEM_PRODUTO:
    NUMBER {
        adicionar_produto($1, 1);  // Quantidade padrão: 1
    }
    |
    NUMBER PROVIDE OPERACOES {
        int quantidade = 1 + $3;  // 1 (produto base) + operações
        adicionar_produto($1, quantidade);
    };

OPERACOES:
    OPERACAO {
        $$ = $1;  // Retorna a contagem de operações
    }
    |
    OPERACOES OPERACAO {
        $$ = $1 + $2;  // Soma as contagens
    };

OPERACAO:
    PLUS   { 
        $$ = 1;  // Cada + adiciona 1 unidade
    }
    |
    MINUS  { 
        $$ = -1;  // Cada - remove 1 unidade (mínimo 1)
    };

PAGAMENTO:
    METODO_CARTAO APROVED {
        printf("Aprovado\n\n");
    }
    |
    METODO_CARTAO REFUSED {
        printf("Recusado\n\n");
    };

METODO_CARTAO:
    CREDIT {
        printf("Tipo de pagamento: CREDITO\n");
    }
    |
    DEBIT  {
        printf("Tipo de pagamento: DEBITO\n");
    }
    |
    PIX    {
        printf("Tipo de pagamento: PIX\n");
    };

%%

void yyerror(const char *s) {
    fprintf(stderr, "[Parser]: Erro Sintático\n");
}

int main(void) {
    printf("======= Caixa Automático da Jota Br ======\n");
    printf("Produtos no estoque:\n");
    printf("Arroz(732): R$ 10.00\n");
    printf("Cubos de Frango(123): R$ 8.00\n");
    printf("Brócolis cortado(456): R$ 5.00\n");
    printf("Salmão(789): R$ 25.00\n");
    printf("Entrada: ");
    yyparse();
    return 0;
}
// 732 253: -- CREDITO APROVADO