/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

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


#line 595 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUMBER = 3,                     /* NUMBER  */
  YYSYMBOL_DECIMAL = 4,                    /* DECIMAL  */
  YYSYMBOL_STRING = 5,                     /* STRING  */
  YYSYMBOL_IDENTIFIER = 6,                 /* IDENTIFIER  */
  YYSYMBOL_START = 7,                      /* START  */
  YYSYMBOL_END = 8,                        /* END  */
  YYSYMBOL_PRODUCT = 9,                    /* PRODUCT  */
  YYSYMBOL_STOCK = 10,                     /* STOCK  */
  YYSYMBOL_DEPOSIT = 11,                   /* DEPOSIT  */
  YYSYMBOL_CREATE_CART = 12,               /* CREATE_CART  */
  YYSYMBOL_CART = 13,                      /* CART  */
  YYSYMBOL_SELL = 14,                      /* SELL  */
  YYSYMBOL_PAYMENT = 15,                   /* PAYMENT  */
  YYSYMBOL_CREDIT = 16,                    /* CREDIT  */
  YYSYMBOL_DEBIT = 17,                     /* DEBIT  */
  YYSYMBOL_PIX = 18,                       /* PIX  */
  YYSYMBOL_SCAN = 19,                      /* SCAN  */
  YYSYMBOL_INCREASE = 20,                  /* INCREASE  */
  YYSYMBOL_CONSULT = 21,                   /* CONSULT  */
  YYSYMBOL_CREATE_DEPOSIT = 22,            /* CREATE_DEPOSIT  */
  YYSYMBOL_GUARD = 23,                     /* GUARD  */
  YYSYMBOL_MESSAGE = 24,                   /* MESSAGE  */
  YYSYMBOL_APROVED = 25,                   /* APROVED  */
  YYSYMBOL_REFUSED = 26,                   /* REFUSED  */
  YYSYMBOL_PRICE = 27,                     /* PRICE  */
  YYSYMBOL_IF = 28,                        /* IF  */
  YYSYMBOL_WHILE = 29,                     /* WHILE  */
  YYSYMBOL_STOP = 30,                      /* STOP  */
  YYSYMBOL_STOP_WHILE = 31,                /* STOP_WHILE  */
  YYSYMBOL_AND = 32,                       /* AND  */
  YYSYMBOL_OR = 33,                        /* OR  */
  YYSYMBOL_PROVIDE = 34,                   /* PROVIDE  */
  YYSYMBOL_PLUS = 35,                      /* PLUS  */
  YYSYMBOL_MULT = 36,                      /* MULT  */
  YYSYMBOL_MINUS = 37,                     /* MINUS  */
  YYSYMBOL_DIV = 38,                       /* DIV  */
  YYSYMBOL_END_LINE = 39,                  /* END_LINE  */
  YYSYMBOL_ASSIGN = 40,                    /* ASSIGN  */
  YYSYMBOL_COLCH_OPEN = 41,                /* COLCH_OPEN  */
  YYSYMBOL_COLCH_CLOSE = 42,               /* COLCH_CLOSE  */
  YYSYMBOL_KEY_OPEN = 43,                  /* KEY_OPEN  */
  YYSYMBOL_KEY_CLOSE = 44,                 /* KEY_CLOSE  */
  YYSYMBOL_PAR_OPEN = 45,                  /* PAR_OPEN  */
  YYSYMBOL_PAR_CLOSE = 46,                 /* PAR_CLOSE  */
  YYSYMBOL_LESSER = 47,                    /* LESSER  */
  YYSYMBOL_LESSER_EQUAL = 48,              /* LESSER_EQUAL  */
  YYSYMBOL_GREATER = 49,                   /* GREATER  */
  YYSYMBOL_GREATER_EQUAL = 50,             /* GREATER_EQUAL  */
  YYSYMBOL_EQUAL = 51,                     /* EQUAL  */
  YYSYMBOL_JMP = 52,                       /* JMP  */
  YYSYMBOL_UMINUS = 53,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 54,                  /* $accept  */
  YYSYMBOL_CAIXA_AUTO = 55,                /* CAIXA_AUTO  */
  YYSYMBOL_CREATE_DEPOSIT_OPERATION = 56,  /* CREATE_DEPOSIT_OPERATION  */
  YYSYMBOL_OPERATIONS = 57,                /* OPERATIONS  */
  YYSYMBOL_CONDITIONAL = 58,               /* CONDITIONAL  */
  YYSYMBOL_59_1 = 59,                      /* $@1  */
  YYSYMBOL_STOP_COND = 60,                 /* STOP_COND  */
  YYSYMBOL_OPERATION = 61,                 /* OPERATION  */
  YYSYMBOL_WHILE_LOOP = 62,                /* WHILE_LOOP  */
  YYSYMBOL_63_2 = 63,                      /* $@2  */
  YYSYMBOL_WHILE_BODY = 64,                /* WHILE_BODY  */
  YYSYMBOL_PRINTER = 65,                   /* PRINTER  */
  YYSYMBOL_STOQUE = 66,                    /* STOQUE  */
  YYSYMBOL_DEPOSITO_OPERATION = 67,        /* DEPOSITO_OPERATION  */
  YYSYMBOL_CREATE_CART_OPERATION = 68,     /* CREATE_CART_OPERATION  */
  YYSYMBOL_VARIAVEL_OPERATION = 69,        /* VARIAVEL_OPERATION  */
  YYSYMBOL_EXPRESSAO = 70,                 /* EXPRESSAO  */
  YYSYMBOL_CART_OPERATION = 71,            /* CART_OPERATION  */
  YYSYMBOL_VENDA = 72,                     /* VENDA  */
  YYSYMBOL_PAGAMENTO = 73,                 /* PAGAMENTO  */
  YYSYMBOL_MAQUININHA = 74,                /* MAQUININHA  */
  YYSYMBOL_nome_produto = 75,              /* nome_produto  */
  YYSYMBOL_nome_variavel = 76,             /* nome_variavel  */
  YYSYMBOL_nome_deposito = 77,             /* nome_deposito  */
  YYSYMBOL_nome_carrinho = 78,             /* nome_carrinho  */
  YYSYMBOL_qr_code = 79                    /* qr_code  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   262

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  26
/* YYNRULES -- Number of rules.  */
#define YYNRULES  64
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  150

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   308


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   553,   553,   564,   571,   572,   575,   575,   588,   595,
     606,   608,   610,   612,   614,   616,   618,   620,   622,   624,
     626,   629,   629,   653,   654,   658,   665,   673,   681,   689,
     696,   703,   710,   717,   724,   731,   738,   746,   750,   752,
     756,   760,   762,   771,   773,   775,   777,   779,   781,   783,
     785,   787,   791,   799,   806,   814,   816,   818,   821,   823,
     825,   827,   829,   831,   833
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NUMBER", "DECIMAL",
  "STRING", "IDENTIFIER", "START", "END", "PRODUCT", "STOCK", "DEPOSIT",
  "CREATE_CART", "CART", "SELL", "PAYMENT", "CREDIT", "DEBIT", "PIX",
  "SCAN", "INCREASE", "CONSULT", "CREATE_DEPOSIT", "GUARD", "MESSAGE",
  "APROVED", "REFUSED", "PRICE", "IF", "WHILE", "STOP", "STOP_WHILE",
  "AND", "OR", "PROVIDE", "PLUS", "MULT", "MINUS", "DIV", "END_LINE",
  "ASSIGN", "COLCH_OPEN", "COLCH_CLOSE", "KEY_OPEN", "KEY_CLOSE",
  "PAR_OPEN", "PAR_CLOSE", "LESSER", "LESSER_EQUAL", "GREATER",
  "GREATER_EQUAL", "EQUAL", "JMP", "UMINUS", "$accept", "CAIXA_AUTO",
  "CREATE_DEPOSIT_OPERATION", "OPERATIONS", "CONDITIONAL", "$@1",
  "STOP_COND", "OPERATION", "WHILE_LOOP", "$@2", "WHILE_BODY", "PRINTER",
  "STOQUE", "DEPOSITO_OPERATION", "CREATE_CART_OPERATION",
  "VARIAVEL_OPERATION", "EXPRESSAO", "CART_OPERATION", "VENDA",
  "PAGAMENTO", "MAQUININHA", "nome_produto", "nome_variavel",
  "nome_deposito", "nome_carrinho", "qr_code", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-85)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-64)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       0,   -85,    21,    25,   -85,   -14,   -85,    13,    18,    18,
      76,     7,    20,    30,    -7,    19,    19,   -85,   -85,   -85,
     -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,
     -85,     2,    16,    39,   -85,     5,   -85,    26,    33,   -85,
     -85,   -85,     4,   -85,   -85,    20,    18,    36,    38,   -85,
      44,   -24,    75,   -85,   -85,    80,   191,   -85,   191,    19,
      77,    20,   102,    65,    80,   -85,   -85,    68,    69,   -27,
     -85,   -85,    66,   -85,    19,    62,   -85,    19,    19,    19,
      19,    19,    19,    19,    19,    19,    19,    19,   -85,    67,
      83,    80,    70,    86,    81,    82,   -85,    80,   -85,    80,
      85,   103,    87,   191,   191,   211,    52,   211,    52,   191,
     191,   191,   191,   191,    67,   -85,    67,   -85,    95,    80,
      19,   104,   106,   105,   107,   109,   -85,   -85,   -85,   110,
     115,   123,   -85,    19,   124,   125,   -85,   126,   111,   -85,
     143,   -85,   -85,    19,    19,   -85,   163,   183,   -85,   -85
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     4,     0,     0,     1,    61,     2,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     8,     9,    11,
      17,    19,     5,    18,    20,    10,    12,    13,    16,    14,
      15,     0,     0,     0,    60,     0,    63,     0,     0,    55,
      56,    57,     0,    64,    61,     0,     0,     0,     0,    62,
       0,     0,     0,    37,    38,     0,     6,    40,    21,     0,
       0,     0,     0,     0,     0,    58,    59,     0,     0,     0,
      32,    33,     0,    30,     0,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     0,
       0,     0,     0,     0,     0,     0,    54,     0,    35,     0,
       0,     0,     0,    50,    51,    43,    41,    44,    42,    45,
      46,    47,    48,    49,     7,    23,    22,    31,     0,     0,
       0,     0,     0,     0,     0,     0,    29,    25,    24,     0,
       0,     0,    28,     0,     0,     0,     3,     0,     0,    26,
       0,    36,    34,     0,     0,    53,     0,     0,    27,    52
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -85,   -85,   -85,    78,   -85,   -85,   -85,   -84,   -85,   -85,
     -85,   -85,   -85,   -85,   -85,   -85,   -16,   -85,   -85,   -85,
     -85,   -85,    -2,   -10,    -5,   -47
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,    19,     3,    20,    88,    21,    22,    23,    89,
     116,    24,    25,    26,    27,    28,    56,    29,    30,    42,
      67,    35,    57,    32,    33,    48
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      58,    31,    50,    37,    38,   115,   -63,     1,    76,    47,
      43,    51,    98,    44,    99,    73,    74,    95,    45,    34,
      46,     4,    53,    54,    36,    44,    49,   -62,    55,    65,
      66,     5,   128,     6,     7,    68,    44,     8,    52,     9,
      10,    69,    59,    90,   118,    62,    11,    12,    13,    14,
     123,    92,   124,    15,    16,    17,    18,    60,   101,    61,
      63,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   130,     5,    64,    70,     7,    71,    72,     8,
      75,     9,    10,    43,    77,    78,    91,    31,    11,    12,
      13,    14,    39,    40,    41,    15,    16,    17,    18,    83,
      84,    85,    86,    87,   131,    93,    94,    96,   102,   100,
      97,   119,    31,   120,    31,    77,    78,   140,    79,    80,
      81,    82,   117,   121,   122,   133,   127,   146,   147,   125,
      83,    84,    85,    86,    87,    77,    78,   129,    79,    80,
      81,    82,   126,   132,   137,   144,   143,   134,   136,   135,
      83,    84,    85,    86,    87,    77,    78,   138,    79,    80,
      81,    82,   139,   141,   142,     0,   114,     0,     0,     0,
      83,    84,    85,    86,    87,    77,    78,     0,    79,    80,
      81,    82,   145,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,    87,    77,    78,     0,    79,    80,
      81,    82,   148,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,    87,    77,    78,     0,    79,    80,
      81,    82,   149,    77,    78,     0,    79,    80,    81,    82,
      83,    84,    85,    86,    87,     0,     0,     0,    83,    84,
      85,    86,    87,    77,    78,     0,     0,    80,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,    87
};

static const yytype_int16 yycheck[] =
{
      16,     3,    12,     8,     9,    89,    20,     7,    55,    11,
       3,    13,    39,     6,    41,    39,    40,    64,    11,     6,
      13,     0,     3,     4,     6,     6,     6,    41,     9,    25,
      26,     6,   116,     8,     9,    45,     6,    12,    45,    14,
      15,    46,    40,    59,    91,    40,    21,    22,    23,    24,
      97,    61,    99,    28,    29,    30,    31,    41,    74,    20,
      34,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,   119,     6,    41,    39,     9,    39,    34,    12,
       5,    14,    15,     3,    32,    33,     9,    89,    21,    22,
      23,    24,    16,    17,    18,    28,    29,    30,    31,    47,
      48,    49,    50,    51,   120,     3,    41,    39,    46,    43,
      41,    41,   114,    27,   116,    32,    33,   133,    35,    36,
      37,    38,    39,    42,    42,    19,    39,   143,   144,    44,
      47,    48,    49,    50,    51,    32,    33,    42,    35,    36,
      37,    38,    39,    39,    34,    34,    20,    42,    39,    42,
      47,    48,    49,    50,    51,    32,    33,    42,    35,    36,
      37,    38,    39,    39,    39,    -1,    88,    -1,    -1,    -1,
      47,    48,    49,    50,    51,    32,    33,    -1,    35,    36,
      37,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    48,    49,    50,    51,    32,    33,    -1,    35,    36,
      37,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    48,    49,    50,    51,    32,    33,    -1,    35,    36,
      37,    38,    39,    32,    33,    -1,    35,    36,    37,    38,
      47,    48,    49,    50,    51,    -1,    -1,    -1,    47,    48,
      49,    50,    51,    32,    33,    -1,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    48,
      49,    50,    51
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     7,    55,    57,     0,     6,     8,     9,    12,    14,
      15,    21,    22,    23,    24,    28,    29,    30,    31,    56,
      58,    60,    61,    62,    65,    66,    67,    68,    69,    71,
      72,    76,    77,    78,     6,    75,     6,    78,    78,    16,
      17,    18,    73,     3,     6,    11,    13,    76,    79,     6,
      77,    76,    45,     3,     4,     9,    70,    76,    70,    40,
      41,    20,    40,    34,    41,    25,    26,    74,    77,    78,
      39,    39,    34,    39,    40,     5,    79,    32,    33,    35,
      36,    37,    38,    47,    48,    49,    50,    51,    59,    63,
      70,     9,    77,     3,    41,    79,    39,    41,    39,    41,
      43,    70,    46,    70,    70,    70,    70,    70,    70,    70,
      70,    70,    70,    70,    57,    61,    64,    39,    79,    41,
      27,    42,    42,    79,    79,    44,    39,    39,    61,    42,
      79,    70,    39,    19,    42,    42,    39,    34,    42,    39,
      70,    39,    39,    20,    34,    39,    70,    70,    39,    39
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    54,    55,    56,    57,    57,    59,    58,    60,    60,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    63,    62,    64,    64,    65,    66,    67,    68,    69,
      69,    69,    69,    69,    69,    69,    69,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    70,    71,    72,    72,    73,    73,    73,    74,    74,
      75,    76,    77,    78,    79
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     3,     6,     0,     2,     0,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     4,     1,     2,     5,     7,     9,     6,     5,
       3,     4,     3,     3,     7,     4,     7,     1,     1,     2,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     9,     8,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* CAIXA_AUTO: START OPERATIONS END  */
#line 553 "parser.y"
                         { 
        // Gerar código assembly final
        if (asm_file) {
            generate_asm_footer();
            fclose(asm_file);
            printf("Arquivo assembly gerado: mercado.asm\n");
        }
        return 0; 
    }
#line 1789 "parser.tab.c"
    break;

  case 3: /* CREATE_DEPOSIT_OPERATION: CREATE_DEPOSIT nome_deposito PROVIDE KEY_OPEN KEY_CLOSE END_LINE  */
#line 564 "parser.y"
                                                                     {
        if (asm_file && executar_operacoes) {
            emit("    ; Criar depósito: %s", (yyvsp[-4].str));
        }
        criar_deposito((yyvsp[-4].str));
    }
#line 1800 "parser.tab.c"
    break;

  case 6: /* $@1: %empty  */
#line 575 "parser.y"
                 {
        int condicao = ((yyvsp[0].dbl) != 0.0);
        push_condicao(condicao && peek_condicao());
        if (asm_file) {
            emit("    ; INICIAR CONDICIONAL IF: %s (condição: %s)", expr_to_string((yyvsp[0].dbl)), condicao ? "verdadeira" : "falsa");
            if (!condicao) {
                emit("    ; PULAR BLOCO IF - CONDICAO FALSA");
            }
        }
    }
#line 1815 "parser.tab.c"
    break;

  case 8: /* STOP_COND: STOP  */
#line 588 "parser.y"
         {
        pop_condicao();
        if (asm_file) {
            emit("    ; FIM CONDICIONAL IF");
        }
    }
#line 1826 "parser.tab.c"
    break;

  case 9: /* STOP_COND: STOP_WHILE  */
#line 595 "parser.y"
               {
        current_while_depth--;
        pop_condicao();
        
        if (asm_file) {
            emit("    jmp label_%d ; VOLTAR AO INICIO DO WHILE", while_start_labels[current_while_depth]);
            emit("label_%d: ; FIM WHILE", while_end_labels[current_while_depth]);
        }
    }
#line 1840 "parser.tab.c"
    break;

  case 21: /* $@2: %empty  */
#line 629 "parser.y"
                    {  // CORREÇÃO: EXPRESSAO declarada aqui
        int start_label = new_label();
        int end_label = new_label();
        while_start_labels[current_while_depth] = start_label;
        while_end_labels[current_while_depth] = end_label;
        current_while_depth++;
        
        int condicao = ((yyvsp[0].dbl) != 0.0);
        int execucao_atual = condicao && peek_condicao();
        
        if (asm_file) {
            emit("label_%d: ; INICIO WHILE", start_label);
            emit("    ; VERIFICAR CONDICAO WHILE");
            emit("    ; CONDICAO WHILE: %s = %s", expr_to_string((yyvsp[0].dbl)), condicao ? "verdadeira" : "falsa");
            if (!execucao_atual) {
                emit("    jmp label_%d ; SAIR DO WHILE - CONDICAO FALSA", while_end_labels[current_while_depth-1]);
            }
        }
        
        push_condicao(execucao_atual);
    }
#line 1866 "parser.tab.c"
    break;

  case 25: /* PRINTER: MESSAGE PAR_OPEN STRING PAR_CLOSE END_LINE  */
#line 658 "parser.y"
                                               {
        if (asm_file && executar_operacoes) {
            emit("    ; Mensagem: \"%s\"", (yyvsp[-2].str));
        }
    }
#line 1876 "parser.tab.c"
    break;

  case 26: /* STOQUE: PRODUCT nome_produto ASSIGN NUMBER PRICE EXPRESSAO END_LINE  */
#line 665 "parser.y"
                                                                {
        if (asm_file && executar_operacoes) {
            emit("    ; Definir produto: %s, QR: %d, Preço: %.2f", (yyvsp[-5].str), (int)(yyvsp[-3].num), (yyvsp[-1].dbl));
        }
        adicionar_produto((yyvsp[-5].str), (yyvsp[-3].num), (yyvsp[-1].dbl));
    }
#line 1887 "parser.tab.c"
    break;

  case 27: /* DEPOSITO_OPERATION: nome_deposito COLCH_OPEN PRODUCT qr_code COLCH_CLOSE PROVIDE INCREASE EXPRESSAO END_LINE  */
#line 673 "parser.y"
                                                                                             {
        if (asm_file && executar_operacoes) {
            emit("    ; Adicionar ao depósito %s: produto %d, quantidade %d", (yyvsp[-8].str), (yyvsp[-5].num), (int)(yyvsp[-1].dbl));
        }
        adicionar_ao_deposito((yyvsp[-8].str), (yyvsp[-5].num), (int)(yyvsp[-1].dbl));
    }
#line 1898 "parser.tab.c"
    break;

  case 28: /* CREATE_CART_OPERATION: CREATE_CART nome_carrinho PROVIDE COLCH_OPEN COLCH_CLOSE END_LINE  */
#line 681 "parser.y"
                                                                      {
        if (asm_file && executar_operacoes) {
            emit("    ; Criar carrinho: %s", (yyvsp[-4].str));
        }
        criar_carrinho((yyvsp[-4].str));
    }
#line 1909 "parser.tab.c"
    break;

  case 29: /* VARIAVEL_OPERATION: GUARD nome_variavel ASSIGN EXPRESSAO END_LINE  */
#line 689 "parser.y"
                                                  {
        if (asm_file && executar_operacoes) {
            emit("    ; Guardar variável: %s = %.2f", (yyvsp[-3].str), (yyvsp[-1].dbl));
        }
        definir_variavel((yyvsp[-3].str), (yyvsp[-1].dbl), 1);
    }
#line 1920 "parser.tab.c"
    break;

  case 30: /* VARIAVEL_OPERATION: GUARD nome_variavel END_LINE  */
#line 696 "parser.y"
                                 {
        if (asm_file && executar_operacoes) {
            emit("    ; Declarar variável: %s", (yyvsp[-1].str));
        }
        definir_variavel((yyvsp[-1].str), 0.0, 0);
    }
#line 1931 "parser.tab.c"
    break;

  case 31: /* VARIAVEL_OPERATION: nome_variavel ASSIGN EXPRESSAO END_LINE  */
#line 703 "parser.y"
                                            {
        if (asm_file && executar_operacoes) {
            emit("    ; Atribuir variável: %s = %.2f", (yyvsp[-3].str), (yyvsp[-1].dbl));
        }
        definir_variavel((yyvsp[-3].str), (yyvsp[-1].dbl), 1);
    }
#line 1942 "parser.tab.c"
    break;

  case 32: /* VARIAVEL_OPERATION: CONSULT nome_variavel END_LINE  */
#line 710 "parser.y"
                                   {
        if (asm_file && executar_operacoes) {
            emit("    ; Consultar variável: %s", (yyvsp[-1].str));
        }
        consultar_variavel_nome((yyvsp[-1].str));
    }
#line 1953 "parser.tab.c"
    break;

  case 33: /* VARIAVEL_OPERATION: CONSULT qr_code END_LINE  */
#line 717 "parser.y"
                             {
        if (asm_file && executar_operacoes) {
            emit("    ; Consultar produto: QR %d", (yyvsp[-1].num));
        }
        consultar_produto((yyvsp[-1].num));
    }
#line 1964 "parser.tab.c"
    break;

  case 34: /* VARIAVEL_OPERATION: CONSULT CART nome_carrinho COLCH_OPEN qr_code COLCH_CLOSE END_LINE  */
#line 724 "parser.y"
                                                                       {
        if (asm_file && executar_operacoes) {
            emit("    ; Consultar carrinho %s[%d]", (yyvsp[-4].str), (yyvsp[-2].num));
        }
        consultar_item_carrinho((yyvsp[-4].str), (yyvsp[-2].num));
    }
#line 1975 "parser.tab.c"
    break;

  case 35: /* VARIAVEL_OPERATION: CONSULT CART nome_carrinho END_LINE  */
#line 731 "parser.y"
                                        {
        if (asm_file && executar_operacoes) {
            emit("    ; Consultar carrinho completo: %s", (yyvsp[-1].str));
        }
        consultar_carrinho((yyvsp[-1].str));
    }
#line 1986 "parser.tab.c"
    break;

  case 36: /* VARIAVEL_OPERATION: CONSULT DEPOSIT nome_deposito COLCH_OPEN qr_code COLCH_CLOSE END_LINE  */
#line 738 "parser.y"
                                                                          {
        if (asm_file && executar_operacoes) {
            emit("    ; Consultar depósito %s[%d]", (yyvsp[-4].str), (yyvsp[-2].num));
        }
        consultar_deposito((yyvsp[-4].str), (yyvsp[-2].num));
    }
#line 1997 "parser.tab.c"
    break;

  case 37: /* EXPRESSAO: NUMBER  */
#line 746 "parser.y"
           { 
        (yyval.dbl) = (yyvsp[0].num); 
    }
#line 2005 "parser.tab.c"
    break;

  case 38: /* EXPRESSAO: DECIMAL  */
#line 750 "parser.y"
            { (yyval.dbl) = (yyvsp[0].dbl); }
#line 2011 "parser.tab.c"
    break;

  case 39: /* EXPRESSAO: PRODUCT qr_code  */
#line 752 "parser.y"
                    { 
        (yyval.dbl) = consultar_produto((yyvsp[0].num));
    }
#line 2019 "parser.tab.c"
    break;

  case 40: /* EXPRESSAO: nome_variavel  */
#line 756 "parser.y"
                  { 
        (yyval.dbl) = consultar_variavel_nome((yyvsp[0].str));
    }
#line 2027 "parser.tab.c"
    break;

  case 41: /* EXPRESSAO: EXPRESSAO MULT EXPRESSAO  */
#line 760 "parser.y"
                             { (yyval.dbl) = (yyvsp[-2].dbl) * (yyvsp[0].dbl); }
#line 2033 "parser.tab.c"
    break;

  case 42: /* EXPRESSAO: EXPRESSAO DIV EXPRESSAO  */
#line 762 "parser.y"
                            { 
        if((yyvsp[0].dbl) == 0) {
            (yyval.dbl) = 0;
        }
        else {
            (yyval.dbl) = (yyvsp[-2].dbl) / (yyvsp[0].dbl); 
        }
    }
#line 2046 "parser.tab.c"
    break;

  case 43: /* EXPRESSAO: EXPRESSAO PLUS EXPRESSAO  */
#line 771 "parser.y"
                             { (yyval.dbl) = (yyvsp[-2].dbl) + (yyvsp[0].dbl); }
#line 2052 "parser.tab.c"
    break;

  case 44: /* EXPRESSAO: EXPRESSAO MINUS EXPRESSAO  */
#line 773 "parser.y"
                              { (yyval.dbl) = (yyvsp[-2].dbl) - (yyvsp[0].dbl); }
#line 2058 "parser.tab.c"
    break;

  case 45: /* EXPRESSAO: EXPRESSAO LESSER EXPRESSAO  */
#line 775 "parser.y"
                               { (yyval.dbl) = (yyvsp[-2].dbl) < (yyvsp[0].dbl); }
#line 2064 "parser.tab.c"
    break;

  case 46: /* EXPRESSAO: EXPRESSAO LESSER_EQUAL EXPRESSAO  */
#line 777 "parser.y"
                                     { (yyval.dbl) = (yyvsp[-2].dbl) <= (yyvsp[0].dbl); }
#line 2070 "parser.tab.c"
    break;

  case 47: /* EXPRESSAO: EXPRESSAO GREATER EXPRESSAO  */
#line 779 "parser.y"
                                { (yyval.dbl) = (yyvsp[-2].dbl) > (yyvsp[0].dbl); }
#line 2076 "parser.tab.c"
    break;

  case 48: /* EXPRESSAO: EXPRESSAO GREATER_EQUAL EXPRESSAO  */
#line 781 "parser.y"
                                      { (yyval.dbl) = (yyvsp[-2].dbl) >= (yyvsp[0].dbl); }
#line 2082 "parser.tab.c"
    break;

  case 49: /* EXPRESSAO: EXPRESSAO EQUAL EXPRESSAO  */
#line 783 "parser.y"
                              { (yyval.dbl) = (yyvsp[-2].dbl) == (yyvsp[0].dbl); }
#line 2088 "parser.tab.c"
    break;

  case 50: /* EXPRESSAO: EXPRESSAO AND EXPRESSAO  */
#line 785 "parser.y"
                            { (yyval.dbl) = (yyvsp[-2].dbl) && (yyvsp[0].dbl); }
#line 2094 "parser.tab.c"
    break;

  case 51: /* EXPRESSAO: EXPRESSAO OR EXPRESSAO  */
#line 787 "parser.y"
                           { (yyval.dbl) = (yyvsp[-2].dbl) || (yyvsp[0].dbl); }
#line 2100 "parser.tab.c"
    break;

  case 52: /* CART_OPERATION: nome_carrinho INCREASE nome_deposito COLCH_OPEN qr_code COLCH_CLOSE PROVIDE EXPRESSAO END_LINE  */
#line 791 "parser.y"
                                                                                                   {
        if (asm_file && executar_operacoes) {
            emit("    ; Adicionar ao carrinho %s: produto %d do depósito %s, quantidade %d", (yyvsp[-8].str), (yyvsp[-4].num), (yyvsp[-6].str), (int)(yyvsp[-1].dbl));
        }
        adicionar_ao_carrinho((yyvsp[-8].str), (yyvsp[-6].str), (yyvsp[-4].num), (int)(yyvsp[-1].dbl));
    }
#line 2111 "parser.tab.c"
    break;

  case 53: /* VENDA: SELL nome_carrinho COLCH_OPEN qr_code COLCH_CLOSE SCAN EXPRESSAO END_LINE  */
#line 799 "parser.y"
                                                                              {
        if (asm_file && executar_operacoes) {
            emit("    ; Vender do carrinho %s: produto %d, quantidade %d", (yyvsp[-6].str), (yyvsp[-4].num), (int)(yyvsp[-1].dbl));
        }
        processar_venda((yyvsp[-6].str), (yyvsp[-4].num), (int)(yyvsp[-1].dbl));
    }
#line 2122 "parser.tab.c"
    break;

  case 54: /* VENDA: PAYMENT PAGAMENTO MAQUININHA END_LINE  */
#line 806 "parser.y"
                                          {
        if (asm_file && executar_operacoes) {
            emit("    ; Pagamento: %s - %s", (yyvsp[-2].str), (yyvsp[-1].str));
        }
        nota_fiscal();
    }
#line 2133 "parser.tab.c"
    break;

  case 55: /* PAGAMENTO: CREDIT  */
#line 814 "parser.y"
           { (yyval.str) = "CRÉDITO"; }
#line 2139 "parser.tab.c"
    break;

  case 56: /* PAGAMENTO: DEBIT  */
#line 816 "parser.y"
          { (yyval.str) = "DÉBITO"; }
#line 2145 "parser.tab.c"
    break;

  case 57: /* PAGAMENTO: PIX  */
#line 818 "parser.y"
        { (yyval.str) = "PIX"; }
#line 2151 "parser.tab.c"
    break;

  case 58: /* MAQUININHA: APROVED  */
#line 821 "parser.y"
            { (yyval.str) = "APROVADO"; }
#line 2157 "parser.tab.c"
    break;

  case 59: /* MAQUININHA: REFUSED  */
#line 823 "parser.y"
            { (yyval.str) = "RECUSADO"; }
#line 2163 "parser.tab.c"
    break;

  case 60: /* nome_produto: IDENTIFIER  */
#line 825 "parser.y"
                         { (yyval.str) = (yyvsp[0].str); }
#line 2169 "parser.tab.c"
    break;

  case 61: /* nome_variavel: IDENTIFIER  */
#line 827 "parser.y"
                          { (yyval.str) = (yyvsp[0].str); }
#line 2175 "parser.tab.c"
    break;

  case 62: /* nome_deposito: IDENTIFIER  */
#line 829 "parser.y"
                          { (yyval.str) = (yyvsp[0].str); }
#line 2181 "parser.tab.c"
    break;

  case 63: /* nome_carrinho: IDENTIFIER  */
#line 831 "parser.y"
                          { (yyval.str) = (yyvsp[0].str); }
#line 2187 "parser.tab.c"
    break;

  case 64: /* qr_code: NUMBER  */
#line 833 "parser.y"
                { (yyval.num) = (yyvsp[0].num); }
#line 2193 "parser.tab.c"
    break;


#line 2197 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 835 "parser.y"


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
