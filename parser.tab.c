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
  YYSYMBOL_AND = 31,                       /* AND  */
  YYSYMBOL_OR = 32,                        /* OR  */
  YYSYMBOL_PROVIDE = 33,                   /* PROVIDE  */
  YYSYMBOL_PLUS = 34,                      /* PLUS  */
  YYSYMBOL_MULT = 35,                      /* MULT  */
  YYSYMBOL_MINUS = 36,                     /* MINUS  */
  YYSYMBOL_DIV = 37,                       /* DIV  */
  YYSYMBOL_END_LINE = 38,                  /* END_LINE  */
  YYSYMBOL_ASSIGN = 39,                    /* ASSIGN  */
  YYSYMBOL_COLCH_OPEN = 40,                /* COLCH_OPEN  */
  YYSYMBOL_COLCH_CLOSE = 41,               /* COLCH_CLOSE  */
  YYSYMBOL_KEY_OPEN = 42,                  /* KEY_OPEN  */
  YYSYMBOL_KEY_CLOSE = 43,                 /* KEY_CLOSE  */
  YYSYMBOL_PAR_OPEN = 44,                  /* PAR_OPEN  */
  YYSYMBOL_PAR_CLOSE = 45,                 /* PAR_CLOSE  */
  YYSYMBOL_LESSER = 46,                    /* LESSER  */
  YYSYMBOL_LESSER_EQUAL = 47,              /* LESSER_EQUAL  */
  YYSYMBOL_GREATER = 48,                   /* GREATER  */
  YYSYMBOL_GREATER_EQUAL = 49,             /* GREATER_EQUAL  */
  YYSYMBOL_EQUAL = 50,                     /* EQUAL  */
  YYSYMBOL_UMINUS = 51,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 52,                  /* $accept  */
  YYSYMBOL_CAIXA_AUTO = 53,                /* CAIXA_AUTO  */
  YYSYMBOL_CREATE_DEPOSIT_OPERATION = 54,  /* CREATE_DEPOSIT_OPERATION  */
  YYSYMBOL_OPERATIONS = 55,                /* OPERATIONS  */
  YYSYMBOL_CONDITIONAL = 56,               /* CONDITIONAL  */
  YYSYMBOL_STOP_COND = 57,                 /* STOP_COND  */
  YYSYMBOL_OPERATION = 58,                 /* OPERATION  */
  YYSYMBOL_PRINTER = 59,                   /* PRINTER  */
  YYSYMBOL_STOQUE = 60,                    /* STOQUE  */
  YYSYMBOL_DEPOSITO_OPERATION = 61,        /* DEPOSITO_OPERATION  */
  YYSYMBOL_CREATE_CART_OPERATION = 62,     /* CREATE_CART_OPERATION  */
  YYSYMBOL_VARIAVEL_OPERATION = 63,        /* VARIAVEL_OPERATION  */
  YYSYMBOL_EXPRESSAO = 64,                 /* EXPRESSAO  */
  YYSYMBOL_CART_OPERATION = 65,            /* CART_OPERATION  */
  YYSYMBOL_VENDA = 66,                     /* VENDA  */
  YYSYMBOL_PAGAMENTO = 67,                 /* PAGAMENTO  */
  YYSYMBOL_MAQUININHA = 68,                /* MAQUININHA  */
  YYSYMBOL_nome_produto = 69,              /* nome_produto  */
  YYSYMBOL_nome_variavel = 70,             /* nome_variavel  */
  YYSYMBOL_nome_deposito = 71,             /* nome_deposito  */
  YYSYMBOL_nome_carrinho = 72,             /* nome_carrinho  */
  YYSYMBOL_qr_code = 73                    /* qr_code  */
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
#define YYLAST   227

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  57
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  140

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   306


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
      45,    46,    47,    48,    49,    50,    51
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   552,   552,   560,   564,   565,   568,   575,   581,   583,
     585,   587,   589,   591,   593,   595,   597,   599,   602,   607,
     612,   617,   622,   626,   630,   650,   657,   664,   670,   676,
     684,   686,   688,   690,   692,   694,   703,   705,   707,   709,
     711,   713,   715,   717,   719,   723,   728,   732,   744,   746,
     748,   751,   753,   755,   757,   759,   761,   763
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
  "APROVED", "REFUSED", "PRICE", "IF", "WHILE", "STOP", "AND", "OR",
  "PROVIDE", "PLUS", "MULT", "MINUS", "DIV", "END_LINE", "ASSIGN",
  "COLCH_OPEN", "COLCH_CLOSE", "KEY_OPEN", "KEY_CLOSE", "PAR_OPEN",
  "PAR_CLOSE", "LESSER", "LESSER_EQUAL", "GREATER", "GREATER_EQUAL",
  "EQUAL", "UMINUS", "$accept", "CAIXA_AUTO", "CREATE_DEPOSIT_OPERATION",
  "OPERATIONS", "CONDITIONAL", "STOP_COND", "OPERATION", "PRINTER",
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

#define YYPACT_NINF (-56)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-57)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -2,   -56,    12,    43,   -56,    -9,   -56,    11,    23,    23,
      28,    30,    24,    33,     6,     4,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,     9,    22,
      50,   -56,    38,   -56,    36,    32,   -56,   -56,   -56,    35,
     -56,   -56,    24,    23,    25,    51,   -56,    41,    15,    77,
     -56,   -56,    85,   157,   -56,     4,    93,    24,   105,    70,
      85,   -56,   -56,    71,    72,    -3,   -56,   -56,    80,   -56,
       4,    66,   -56,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,    49,    85,    73,    87,    88,    90,
     -56,    85,   -56,    85,    91,    69,    92,   157,   157,   177,
      44,   177,    44,   157,   157,   157,   157,   157,   -56,   101,
      85,     4,    94,   114,   107,   108,   112,   -56,   -56,    95,
     110,    89,   -56,     4,   115,   116,   -56,   132,   135,   -56,
     109,   -56,   -56,     4,     4,   -56,   129,   149,   -56,   -56
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     4,     0,     0,     1,    54,     2,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     9,    15,    16,
       5,    17,     8,    10,    11,    14,    12,    13,     0,     0,
       0,    53,     0,    56,     0,     0,    48,    49,    50,     0,
      57,    54,     0,     0,     0,     0,    55,     0,     0,     0,
      30,    31,     0,     6,    33,     0,     0,     0,     0,     0,
       0,    51,    52,     0,     0,     0,    25,    26,     0,    23,
       0,     0,    32,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      47,     0,    28,     0,     0,     0,     0,    43,    44,    36,
      34,    37,    35,    38,    39,    40,    41,    42,    24,     0,
       0,     0,     0,     0,     0,     0,     0,    22,    18,     0,
       0,     0,    21,     0,     0,     0,     3,     0,     0,    19,
       0,    29,    27,     0,     0,    46,     0,     0,    20,    45
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -55,   -56,   -56,   -56,   -56,   -56,     3,   -10,
      -5,   -51
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,    17,     3,    18,    19,    20,    21,    22,    23,
      24,    25,    53,    26,    27,    39,    63,    32,    54,    29,
      30,    45
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      84,    72,    47,    34,    35,     1,    28,    50,    51,    89,
      41,   -56,     4,    52,    44,    95,    48,    31,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,    33,
      46,   -55,    64,    40,   109,    92,    41,    93,    65,    41,
     114,    42,   115,    43,    36,    37,    38,    86,    55,     5,
      49,     6,     7,    69,    70,     8,   121,     9,    10,   120,
      61,    62,    56,    66,    11,    12,    13,    14,   130,    59,
      57,    15,    60,    16,    68,    73,    74,    58,   136,   137,
      73,    74,    71,    75,    76,    77,    78,   108,    40,    67,
      79,    80,    81,    82,    83,    79,    80,    81,    82,    83,
      73,    74,    85,    75,    76,    77,    78,   117,    87,    90,
      88,    96,    91,   110,   111,    79,    80,    81,    82,    83,
      73,    74,    94,    75,    76,    77,    78,   129,   127,   112,
     118,   113,   122,   123,   116,    79,    80,    81,    82,    83,
      73,    74,   119,    75,    76,    77,    78,   135,   124,   125,
     126,   128,   133,   131,   132,    79,    80,    81,    82,    83,
      73,    74,     0,    75,    76,    77,    78,   138,   134,     0,
       0,     0,     0,     0,     0,    79,    80,    81,    82,    83,
      73,    74,     0,    75,    76,    77,    78,   139,    73,    74,
       0,    75,    76,    77,    78,    79,    80,    81,    82,    83,
       0,     0,     0,    79,    80,    81,    82,    83,    73,    74,
       0,     0,    76,     0,    78,     0,     0,     0,     0,     0,
       0,     0,     0,    79,    80,    81,    82,    83
};

static const yytype_int16 yycheck[] =
{
      55,    52,    12,     8,     9,     7,     3,     3,     4,    60,
       6,    20,     0,     9,    11,    70,    13,     6,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,     6,
       6,    40,    42,     3,    85,    38,     6,    40,    43,     6,
      91,    11,    93,    13,    16,    17,    18,    57,    39,     6,
      44,     8,     9,    38,    39,    12,   111,    14,    15,   110,
      25,    26,    40,    38,    21,    22,    23,    24,   123,    33,
      20,    28,    40,    30,    33,    31,    32,    39,   133,   134,
      31,    32,     5,    34,    35,    36,    37,    38,     3,    38,
      46,    47,    48,    49,    50,    46,    47,    48,    49,    50,
      31,    32,     9,    34,    35,    36,    37,    38,     3,    38,
      40,    45,    40,    40,    27,    46,    47,    48,    49,    50,
      31,    32,    42,    34,    35,    36,    37,    38,    33,    41,
      38,    41,    38,    19,    43,    46,    47,    48,    49,    50,
      31,    32,    41,    34,    35,    36,    37,    38,    41,    41,
      38,    41,    20,    38,    38,    46,    47,    48,    49,    50,
      31,    32,    -1,    34,    35,    36,    37,    38,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,
      31,    32,    -1,    34,    35,    36,    37,    38,    31,    32,
      -1,    34,    35,    36,    37,    46,    47,    48,    49,    50,
      -1,    -1,    -1,    46,    47,    48,    49,    50,    31,    32,
      -1,    -1,    35,    -1,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    46,    47,    48,    49,    50
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     7,    53,    55,     0,     6,     8,     9,    12,    14,
      15,    21,    22,    23,    24,    28,    30,    54,    56,    57,
      58,    59,    60,    61,    62,    63,    65,    66,    70,    71,
      72,     6,    69,     6,    72,    72,    16,    17,    18,    67,
       3,     6,    11,    13,    70,    73,     6,    71,    70,    44,
       3,     4,     9,    64,    70,    39,    40,    20,    39,    33,
      40,    25,    26,    68,    71,    72,    38,    38,    33,    38,
      39,     5,    73,    31,    32,    34,    35,    36,    37,    46,
      47,    48,    49,    50,    64,     9,    71,     3,    40,    73,
      38,    40,    38,    40,    42,    64,    45,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    38,    73,
      40,    27,    41,    41,    73,    73,    43,    38,    38,    41,
      73,    64,    38,    19,    41,    41,    38,    33,    41,    38,
      64,    38,    38,    20,    33,    38,    64,    64,    38,    38
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    52,    53,    54,    55,    55,    56,    57,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    59,    60,
      61,    62,    63,    63,    63,    63,    63,    63,    63,    63,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    65,    66,    66,    67,    67,
      67,    68,    68,    69,    70,    71,    72,    73
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     3,     6,     0,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     5,     7,
       9,     6,     5,     3,     4,     3,     3,     7,     4,     7,
       1,     1,     2,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     9,     8,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1
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
#line 552 "parser.y"
                         { 
        printf("\nCompra finalizada!\n");
        // EXECUTAR DEMONSTRAÇÃO DA VM
        executar_demo_vm();
        return 0; 
    }
#line 1765 "parser.tab.c"
    break;

  case 3: /* CREATE_DEPOSIT_OPERATION: CREATE_DEPOSIT nome_deposito PROVIDE KEY_OPEN KEY_CLOSE END_LINE  */
#line 560 "parser.y"
                                                                     {
        criar_deposito((yyvsp[-4].str));
    }
#line 1773 "parser.tab.c"
    break;

  case 6: /* CONDITIONAL: IF EXPRESSAO  */
#line 568 "parser.y"
                 {
        if (!(yyvsp[0].dbl)) {
            executar_operacoes = 0;
        }
    }
#line 1783 "parser.tab.c"
    break;

  case 7: /* STOP_COND: STOP  */
#line 575 "parser.y"
         {
        nivel_condicional--;
        executar_operacoes = 1;
    }
#line 1792 "parser.tab.c"
    break;

  case 18: /* PRINTER: MESSAGE PAR_OPEN STRING PAR_CLOSE END_LINE  */
#line 602 "parser.y"
                                               {
        printf("%s\n", (yyvsp[-2].str));
    }
#line 1800 "parser.tab.c"
    break;

  case 19: /* STOQUE: PRODUCT nome_produto ASSIGN NUMBER PRICE EXPRESSAO END_LINE  */
#line 607 "parser.y"
                                                                {
        adicionar_produto((yyvsp[-5].str), (int)(yyvsp[-3].num), (yyvsp[-1].dbl));
    }
#line 1808 "parser.tab.c"
    break;

  case 20: /* DEPOSITO_OPERATION: nome_deposito COLCH_OPEN PRODUCT qr_code COLCH_CLOSE PROVIDE INCREASE EXPRESSAO END_LINE  */
#line 612 "parser.y"
                                                                                             {
        adicionar_ao_deposito((yyvsp[-8].str), (yyvsp[-5].num), (int)(yyvsp[-1].dbl));
    }
#line 1816 "parser.tab.c"
    break;

  case 21: /* CREATE_CART_OPERATION: CREATE_CART nome_carrinho PROVIDE COLCH_OPEN COLCH_CLOSE END_LINE  */
#line 617 "parser.y"
                                                                      {
        criar_carrinho((yyvsp[-4].str));
    }
#line 1824 "parser.tab.c"
    break;

  case 22: /* VARIAVEL_OPERATION: GUARD nome_variavel ASSIGN EXPRESSAO END_LINE  */
#line 622 "parser.y"
                                                  {
        definir_variavel((yyvsp[-3].str), (yyvsp[-1].dbl), 1);
    }
#line 1832 "parser.tab.c"
    break;

  case 23: /* VARIAVEL_OPERATION: GUARD nome_variavel END_LINE  */
#line 626 "parser.y"
                                 {
        definir_variavel((yyvsp[-1].str), 0, 1);
    }
#line 1840 "parser.tab.c"
    break;

  case 24: /* VARIAVEL_OPERATION: nome_variavel ASSIGN EXPRESSAO END_LINE  */
#line 630 "parser.y"
                                            {
        int valor_atual = consultar_variavel_nome((yyvsp[-3].str));
        if(valor_atual == 0) {
            int encontrada = 0;
            for(int i = 0; i < num_variaveis; i++) {
                if(strcmp(variaveis[i].nome, (yyvsp[-3].str)) == 0) {
                    encontrada = 1;
                    break;
                }
            }
            if(!encontrada) {
                printf("Erro: Variável %s não declarada. Use GUARD primeiro.\n", (yyvsp[-3].str));
            } else {
                definir_variavel((yyvsp[-3].str), (yyvsp[-1].dbl), 0);
            }
        } else {
            definir_variavel((yyvsp[-3].str), (yyvsp[-1].dbl), 0);
        }
    }
#line 1864 "parser.tab.c"
    break;

  case 25: /* VARIAVEL_OPERATION: CONSULT nome_variavel END_LINE  */
#line 650 "parser.y"
                                   {
        if (executar_operacoes) {
            double valor = consultar_variavel_nome((yyvsp[-1].str));
            printf("Variavel %s = %.2f\n", (yyvsp[-1].str), valor);
        }
    }
#line 1875 "parser.tab.c"
    break;

  case 26: /* VARIAVEL_OPERATION: CONSULT qr_code END_LINE  */
#line 657 "parser.y"
                             {
        if (executar_operacoes) {
            double valor = consultar_produto((yyvsp[-1].num));
            printf("Produto %d = R$ %.2f\n", (yyvsp[-1].num), valor);
        }
    }
#line 1886 "parser.tab.c"
    break;

  case 27: /* VARIAVEL_OPERATION: CONSULT CART nome_carrinho COLCH_OPEN qr_code COLCH_CLOSE END_LINE  */
#line 664 "parser.y"
                                                                       {
        if (executar_operacoes) {
            consultar_item_carrinho((yyvsp[-4].str), (yyvsp[-2].num));
        }
    }
#line 1896 "parser.tab.c"
    break;

  case 28: /* VARIAVEL_OPERATION: CONSULT CART nome_carrinho END_LINE  */
#line 670 "parser.y"
                                        {
        if (executar_operacoes) {
            consultar_carrinho((yyvsp[-1].str));
        }
    }
#line 1906 "parser.tab.c"
    break;

  case 29: /* VARIAVEL_OPERATION: CONSULT DEPOSIT nome_deposito COLCH_OPEN qr_code COLCH_CLOSE END_LINE  */
#line 676 "parser.y"
                                                                          {
        if (executar_operacoes) {
            int qtd = consultar_deposito((yyvsp[-4].str), (yyvsp[-2].num));
            printf("Depósito %s[%d] = %d quantidades\n", (yyvsp[-4].str), (yyvsp[-2].num), qtd);
        }
    }
#line 1917 "parser.tab.c"
    break;

  case 30: /* EXPRESSAO: NUMBER  */
#line 684 "parser.y"
           { (yyval.dbl) = (yyvsp[0].num); }
#line 1923 "parser.tab.c"
    break;

  case 31: /* EXPRESSAO: DECIMAL  */
#line 686 "parser.y"
            { (yyval.dbl) = (yyvsp[0].dbl); }
#line 1929 "parser.tab.c"
    break;

  case 32: /* EXPRESSAO: PRODUCT qr_code  */
#line 688 "parser.y"
                    { (yyval.dbl) = consultar_produto((yyvsp[0].num)); }
#line 1935 "parser.tab.c"
    break;

  case 33: /* EXPRESSAO: nome_variavel  */
#line 690 "parser.y"
                  { (yyval.dbl) = consultar_variavel_nome((yyvsp[0].str)); }
#line 1941 "parser.tab.c"
    break;

  case 34: /* EXPRESSAO: EXPRESSAO MULT EXPRESSAO  */
#line 692 "parser.y"
                             { (yyval.dbl) = (yyvsp[-2].dbl) * (yyvsp[0].dbl); }
#line 1947 "parser.tab.c"
    break;

  case 35: /* EXPRESSAO: EXPRESSAO DIV EXPRESSAO  */
#line 694 "parser.y"
                            { if((yyvsp[0].dbl) == 0) {
            printf("Erro: Divisão por zero!\n");
            (yyval.dbl) = 0;
        }
        else {
            (yyval.dbl) = (yyvsp[-2].dbl) / (yyvsp[0].dbl); 
        }    
    }
#line 1960 "parser.tab.c"
    break;

  case 36: /* EXPRESSAO: EXPRESSAO PLUS EXPRESSAO  */
#line 703 "parser.y"
                             { (yyval.dbl) = (yyvsp[-2].dbl) + (yyvsp[0].dbl); }
#line 1966 "parser.tab.c"
    break;

  case 37: /* EXPRESSAO: EXPRESSAO MINUS EXPRESSAO  */
#line 705 "parser.y"
                              { (yyval.dbl) = (yyvsp[-2].dbl) - (yyvsp[0].dbl); }
#line 1972 "parser.tab.c"
    break;

  case 38: /* EXPRESSAO: EXPRESSAO LESSER EXPRESSAO  */
#line 707 "parser.y"
                               { (yyval.dbl) = (yyvsp[-2].dbl) < (yyvsp[0].dbl); }
#line 1978 "parser.tab.c"
    break;

  case 39: /* EXPRESSAO: EXPRESSAO LESSER_EQUAL EXPRESSAO  */
#line 709 "parser.y"
                                     { (yyval.dbl) = (yyvsp[-2].dbl) <= (yyvsp[0].dbl); }
#line 1984 "parser.tab.c"
    break;

  case 40: /* EXPRESSAO: EXPRESSAO GREATER EXPRESSAO  */
#line 711 "parser.y"
                                { (yyval.dbl) = (yyvsp[-2].dbl) > (yyvsp[0].dbl); }
#line 1990 "parser.tab.c"
    break;

  case 41: /* EXPRESSAO: EXPRESSAO GREATER_EQUAL EXPRESSAO  */
#line 713 "parser.y"
                                      { (yyval.dbl) = (yyvsp[-2].dbl) >= (yyvsp[0].dbl); }
#line 1996 "parser.tab.c"
    break;

  case 42: /* EXPRESSAO: EXPRESSAO EQUAL EXPRESSAO  */
#line 715 "parser.y"
                              { (yyval.dbl) = (yyvsp[-2].dbl) == (yyvsp[0].dbl); }
#line 2002 "parser.tab.c"
    break;

  case 43: /* EXPRESSAO: EXPRESSAO AND EXPRESSAO  */
#line 717 "parser.y"
                            { (yyval.dbl) = (yyvsp[-2].dbl) && (yyvsp[0].dbl); }
#line 2008 "parser.tab.c"
    break;

  case 44: /* EXPRESSAO: EXPRESSAO OR EXPRESSAO  */
#line 719 "parser.y"
                           { (yyval.dbl) = (yyvsp[-2].dbl) || (yyvsp[0].dbl); }
#line 2014 "parser.tab.c"
    break;

  case 45: /* CART_OPERATION: nome_carrinho INCREASE nome_deposito COLCH_OPEN qr_code COLCH_CLOSE PROVIDE EXPRESSAO END_LINE  */
#line 723 "parser.y"
                                                                                                   {
        adicionar_ao_carrinho((yyvsp[-8].str), (yyvsp[-6].str), (yyvsp[-4].num), (int)(yyvsp[-1].dbl));
    }
#line 2022 "parser.tab.c"
    break;

  case 46: /* VENDA: SELL nome_carrinho COLCH_OPEN qr_code COLCH_CLOSE SCAN EXPRESSAO END_LINE  */
#line 728 "parser.y"
                                                                              {
        processar_venda((yyvsp[-6].str), (yyvsp[-4].num), (int)(yyvsp[-1].dbl));
    }
#line 2030 "parser.tab.c"
    break;

  case 47: /* VENDA: PAYMENT PAGAMENTO MAQUININHA END_LINE  */
#line 732 "parser.y"
                                          {
        if (strcmp((yyvsp[-1].str), "APROVADO") == 0) {
            nota_fiscal();
            printf("Método de pagamento: %s\n", (yyvsp[-2].str));
            printf("Status: %s\n", (yyvsp[-1].str));
        } else {
            printf("\nPagamento %s\n", (yyvsp[-1].str));
        }
        limpa_tudo();
    }
#line 2045 "parser.tab.c"
    break;

  case 48: /* PAGAMENTO: CREDIT  */
#line 744 "parser.y"
           { (yyval.str) = "CRÉDITO"; }
#line 2051 "parser.tab.c"
    break;

  case 49: /* PAGAMENTO: DEBIT  */
#line 746 "parser.y"
          { (yyval.str) = "DÉBITO"; }
#line 2057 "parser.tab.c"
    break;

  case 50: /* PAGAMENTO: PIX  */
#line 748 "parser.y"
        { (yyval.str) = "PIX"; }
#line 2063 "parser.tab.c"
    break;

  case 51: /* MAQUININHA: APROVED  */
#line 751 "parser.y"
            { (yyval.str) = "APROVADO"; }
#line 2069 "parser.tab.c"
    break;

  case 52: /* MAQUININHA: REFUSED  */
#line 753 "parser.y"
            { (yyval.str) = "RECUSADO"; }
#line 2075 "parser.tab.c"
    break;

  case 53: /* nome_produto: IDENTIFIER  */
#line 755 "parser.y"
                         { (yyval.str) = (yyvsp[0].str); }
#line 2081 "parser.tab.c"
    break;

  case 54: /* nome_variavel: IDENTIFIER  */
#line 757 "parser.y"
                          { (yyval.str) = (yyvsp[0].str); }
#line 2087 "parser.tab.c"
    break;

  case 55: /* nome_deposito: IDENTIFIER  */
#line 759 "parser.y"
                          { (yyval.str) = (yyvsp[0].str); }
#line 2093 "parser.tab.c"
    break;

  case 56: /* nome_carrinho: IDENTIFIER  */
#line 761 "parser.y"
                          { (yyval.str) = (yyvsp[0].str); }
#line 2099 "parser.tab.c"
    break;

  case 57: /* qr_code: NUMBER  */
#line 763 "parser.y"
                { (yyval.num) = (yyvsp[0].num); }
#line 2105 "parser.tab.c"
    break;


#line 2109 "parser.tab.c"

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

#line 765 "parser.y"


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
