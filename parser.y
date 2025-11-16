%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PRODUTOS 170
#define MAX_DEPOSITO 170
#define MAX_VARIAVEIS 100

typedef struct {
    char* nome;
    int qr_code;
    double preco;
    int definido;
} Produto;

typedef struct {
    char* nome;
    int valor;
} Variavel;

typedef struct {
    int qr_code;
    int quantidade;
} ItemDeposito;

typedef struct {
    int qr_code;
    int quantidade_carrinho;
    int quantidade_vendida;
    double preco_unitario;
} ItemCarrinho;

Produto tabela_produtos[MAX_PRODUTOS];
ItemDeposito deposito[MAX_DEPOSITO];
ItemCarrinho carrinho[MAX_PRODUTOS];

int num_produtos = 0;
int num_itens_deposito = 0;
int num_itens_carrinho = 0;
int nivel_condicional = 0;
int executar_operacoes = 1;
int deposito_criado = 0;
double total_compra = 0.0;
int carrinho_criado = 0;

Variavel variaveis[MAX_VARIAVEIS];
int num_variaveis = 0;

void criar_deposito() {
    if (!executar_operacoes) return;
    deposito_criado = 1;
}

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

void adicionar_ao_deposito(int qr_code, int quantidade) {
    if (!executar_operacoes) return;
    if (!deposito_criado) {
        printf("Erro: Depósito não foi criado. Use CRIAR_DEPOSITO : {} primeiro.\n");
        return;
    }
    int indice_produto = encontrar_produto_qr(qr_code);
    
    if(indice_produto >= 0) {
        for(int i = 0; i < num_itens_deposito; i++) {
            if(deposito[i].qr_code == qr_code) {
                deposito[i].quantidade = quantidade;
                return;
            }
        }
        
        if(num_itens_deposito < MAX_DEPOSITO) {
            deposito[num_itens_deposito].qr_code = qr_code;
            deposito[num_itens_deposito].quantidade = quantidade;
            num_itens_deposito++;
        } else {
            printf("Erro: Limite máximo do depósito atingido(170)\n");
        }
    } else {
        printf("Erro: Produto com QR_CODE %d não definido. Use PRODUTO primeiro.\n", qr_code);
    }
}

int consultar_deposito(int qr_code) {
    for(int i = 0; i < num_itens_deposito; i++) {
        if(deposito[i].qr_code == qr_code) {
            return deposito[i].quantidade;
        }
    }
    return 0;
}

void criar_carrinho() {
    if (!executar_operacoes) return;
    carrinho_criado = 1;
    printf("Iniciando compra de carrinho\n");
}

void adicionar_ao_carrinho(int qr_code, int quantidade) {
    if (!executar_operacoes) return;
    if (!carrinho_criado) {
        printf("Erro: Carrinho não foi criado. Use CRIAR_CARRINHO : [] primeiro.\n");
        return;
    }
    
    int indice_produto = encontrar_produto_qr(qr_code);
    int estoque_disponivel = consultar_deposito(qr_code);
    
    if(indice_produto >= 0) {
        if(estoque_disponivel >= quantidade) {
            for(int i = 0; i < num_itens_carrinho; i++) {
                if(carrinho[i].qr_code == qr_code) {
                    carrinho[i].quantidade_carrinho += quantidade;
                    return;
                }
            }
            
            if(num_itens_carrinho < MAX_PRODUTOS) {
                carrinho[num_itens_carrinho].qr_code = qr_code;
                carrinho[num_itens_carrinho].quantidade_carrinho = quantidade;
                carrinho[num_itens_carrinho].quantidade_vendida = 0;
                carrinho[num_itens_carrinho].preco_unitario = tabela_produtos[indice_produto].preco;
                num_itens_carrinho++;
            }
        } else {
            printf("Erro: Estoque insuficiente. Disponível: %d, Solicitado: %d\n", 
                   estoque_disponivel, quantidade);
        }
    } else {
        printf("Erro: Produto %d não definido\n", qr_code);
    }
}

int encontrar_item_carrinho(int qr_code) {
    for(int i = 0; i < num_itens_carrinho; i++) {
        if (carrinho[i].qr_code == qr_code) {
            return i;
        }
    }
    return -1;
}

void processar_venda(int qr_code, int quantidade_venda) {
    if (!executar_operacoes) return;
    if (!carrinho_criado) {
        printf("Erro: Carrinho não foi criado. Use CRIAR_CARRINHO : [] primeiro.\n");
        return;
    }
    
    int indice_carrinho = encontrar_item_carrinho(qr_code);
    int indice_produto = encontrar_produto_qr(qr_code);
    int estoque_deposito = consultar_deposito(qr_code);
    
    if(indice_carrinho >= 0 && indice_produto >= 0) {
        int quantidade_disponivel = carrinho[indice_carrinho].quantidade_carrinho - 
                                   carrinho[indice_carrinho].quantidade_vendida;
        
        if(quantidade_venda <= quantidade_disponivel && quantidade_venda <= estoque_deposito) {
            for(int i = 0; i < num_itens_deposito; i++) {
                if(deposito[i].qr_code == qr_code) {
                    deposito[i].quantidade -= quantidade_venda;
                    break;
                }
            }
            
            double valor_venda = carrinho[indice_carrinho].preco_unitario * quantidade_venda;
            total_compra += valor_venda;
            carrinho[indice_carrinho].quantidade_vendida += quantidade_venda;
        } else {
            printf("Erro: Quantidade indisponível. Carrinho: %d, Depósito: %d, Solicitado: %d\n", 
                   quantidade_disponivel, estoque_deposito, quantidade_venda);
        }
    } else {
        printf("Erro: Produto %d não encontrado no carrinho\n", qr_code);
    }
}

void nota_fiscal() {
    printf("\n=== NOTA FISCAL ===\n");
    printf("Produtos vendidos:\n");
    
    for(int i = 0; i < num_itens_carrinho; i++) {
        if(carrinho[i].quantidade_vendida > 0) {
            int indice_produto = encontrar_produto_qr(carrinho[i].qr_code);
            if(indice_produto >= 0) {
                double subtotal = carrinho[i].preco_unitario * carrinho[i].quantidade_vendida;
                printf("- %dx %s: R$ %.2f\n", 
                       carrinho[i].quantidade_vendida,
                       tabela_produtos[indice_produto].nome,
                       subtotal);
            }
        }
    }
    printf("Total da venda: R$ %.2f\n", total_compra);
    printf("===================\n");
}

void definir_variavel(char* nome, int valor) {
    if (!executar_operacoes) return;
    for(int i = 0; i < num_variaveis; i++) {
        if(strcmp(variaveis[i].nome, nome) == 0) {
            variaveis[i].valor = valor;
            return;
        }
    }
    
    if(num_variaveis < MAX_VARIAVEIS) {
        variaveis[num_variaveis].nome = strdup(nome);
        variaveis[num_variaveis].valor = valor;
        num_variaveis++;
    }
}

double consultar_produto(int qr_code) {
    int indice = encontrar_produto_qr(qr_code);
    if(indice >= 0) {
        return tabela_produtos[indice].preco;
    }
    return 0.0;
}

int consultar_variavel_nome(char* nome) {
    for(int i = 0; i < num_variaveis; i++) {
        if(strcmp(variaveis[i].nome, nome) == 0) {
            return variaveis[i].valor;
        }
    }
    return 0;
}
void limpa_tudo() {
    num_produtos = 0;
    num_itens_deposito = 0;
    num_itens_carrinho = 0;
    total_compra = 0.0;
    carrinho_criado = 0;
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

%token START END PRODUCT STOCK DEPOSIT CREATE_CART CART SELL PAYMENT CREDIT DEBIT PIX SCAN INCREASE CONSULT CREATE_DEPOSIT GUARD
%token APROVED REFUSED PRICE
%token IF WHILE STOP AND OR
%token PROVIDE PLUS MULT MINUS DIV END_LINE ASSIGN COLCH_OPEN COLCH_CLOSE KEY_OPEN KEY_CLOSE LESSER LESSER_EQUAL GREATER GREATER_EQUAL EQUAL

%type <str> nome_produto nome_variavel  
%type <num> qr_code EXPRESSAO
%type <str> PAGAMENTO MAQUININHA
%type <dbl> numero_preco

%left PLUS MINUS
%left MULT DIV
%nonassoc UMINUS

%%

CAIXA_AUTO: 
    START OPERATIONS END { 
        printf("\nCompra finalizada!\n"); 
        return 0; 
    };

CREATE_DEPOSIT_OPERATION:
    CREATE_DEPOSIT PROVIDE KEY_OPEN KEY_CLOSE END_LINE {
        criar_deposito();
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
        // if (nivel_condicional > 0) {
        //     nivel_condicional--;
        //     executar_operacoes = 1;
        // }
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
    STOP_COND;

STOQUE:
    PRODUCT nome_produto ASSIGN NUMBER PRICE numero_preco END_LINE {
        adicionar_produto($2, (int)$4, $6);
    };

DEPOSITO_OPERATION:
    DEPOSIT COLCH_OPEN PRODUCT qr_code COLCH_CLOSE PROVIDE INCREASE EXPRESSAO END_LINE {
        adicionar_ao_deposito($4, (int)$8);
    };

CREATE_CART_OPERATION:
    CREATE_CART PROVIDE COLCH_OPEN COLCH_CLOSE END_LINE {
        criar_carrinho();
    };

VARIAVEL_OPERATION:
    GUARD nome_variavel ASSIGN EXPRESSAO END_LINE {
        definir_variavel($2, $4);
    }
    |
    GUARD nome_variavel END_LINE {
        definir_variavel($2, 0);
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
                definir_variavel($1, $3);
            }
        } else {
            definir_variavel($1, $3);
        }
    }
    |
    CONSULT nome_variavel END_LINE {
        if (executar_operacoes) {
            int valor = consultar_variavel_nome($2);
            printf("Variavel %s = %d\n", $2, valor);
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
    CONSULT CART COLCH_OPEN qr_code COLCH_CLOSE END_LINE {
        if (executar_operacoes) {
            int indice_carrinho = encontrar_item_carrinho($4);
            if(indice_carrinho >= 0) {
                printf("Carrinho[%d] = %d unidades\n", 
                    $4, 
                    carrinho[indice_carrinho].quantidade_carrinho - carrinho[indice_carrinho].quantidade_vendida);
            } else {
                printf("Carrinho[%d] = 0 unidades\n", $4);
            }
        }
    }
    |
    CONSULT CART END_LINE {
        if (executar_operacoes) {
            printf("========== CARRINHO =========\n");
            for(int i = 0; i < num_itens_carrinho; i++) {
                int indice_produto = encontrar_produto_qr(carrinho[i].qr_code);
                if(indice_produto >= 0) {
                    printf("- %s: %d no carrinho (%d vendidas)\n",
                        tabela_produtos[indice_produto].nome,
                        carrinho[i].quantidade_carrinho,
                        carrinho[i].quantidade_vendida);
                }
            }
            printf("===================\n");
        }
    }
    |
    CONSULT DEPOSIT COLCH_OPEN qr_code COLCH_CLOSE END_LINE {
        if (executar_operacoes) {
            int qtd = consultar_deposito($4);
            printf("Depósito[%d] = %d quantidades\n", $4, qtd);
        }
    };

EXPRESSAO: 
    NUMBER { $$ = $1; }
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
    CART INCREASE DEPOSIT COLCH_OPEN qr_code COLCH_CLOSE PROVIDE EXPRESSAO END_LINE {
        adicionar_ao_carrinho($5, (int)$8);
    };

VENDA: 
    SELL CART COLCH_OPEN qr_code COLCH_CLOSE SCAN EXPRESSAO END_LINE {
        processar_venda($4, (int)$7);
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

numero_preco: DECIMAL { $$ = $1; };

nome_variavel: IDENTIFIER { $$ = $1; };

qr_code: NUMBER { $$ = $1; };

%%

void yyerror(const char *s) {
    fprintf(stderr, "[PARSER] Erro de sintaxe: %s\n", s);
}

int yydebug = 1;

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

// Para debug

// flex lexer.l
//bison -d -t parser.y
//gcc -DYYDEBUG=1 lex.yy.c parser.tab.c -o mercadinho
//./mercadinho exemplo.jota