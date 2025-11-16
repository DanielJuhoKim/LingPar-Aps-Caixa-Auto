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

// Função para encontrar produto por QR_CODE
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


#line 345 "parser.tab.c"

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
  YYSYMBOL_APROVED = 24,                   /* APROVED  */
  YYSYMBOL_REFUSED = 25,                   /* REFUSED  */
  YYSYMBOL_PRICE = 26,                     /* PRICE  */
  YYSYMBOL_IF = 27,                        /* IF  */
  YYSYMBOL_WHILE = 28,                     /* WHILE  */
  YYSYMBOL_STOP = 29,                      /* STOP  */
  YYSYMBOL_AND = 30,                       /* AND  */
  YYSYMBOL_OR = 31,                        /* OR  */
  YYSYMBOL_PROVIDE = 32,                   /* PROVIDE  */
  YYSYMBOL_PLUS = 33,                      /* PLUS  */
  YYSYMBOL_MULT = 34,                      /* MULT  */
  YYSYMBOL_MINUS = 35,                     /* MINUS  */
  YYSYMBOL_DIV = 36,                       /* DIV  */
  YYSYMBOL_END_LINE = 37,                  /* END_LINE  */
  YYSYMBOL_ASSIGN = 38,                    /* ASSIGN  */
  YYSYMBOL_COLCH_OPEN = 39,                /* COLCH_OPEN  */
  YYSYMBOL_COLCH_CLOSE = 40,               /* COLCH_CLOSE  */
  YYSYMBOL_KEY_OPEN = 41,                  /* KEY_OPEN  */
  YYSYMBOL_KEY_CLOSE = 42,                 /* KEY_CLOSE  */
  YYSYMBOL_LESSER = 43,                    /* LESSER  */
  YYSYMBOL_LESSER_EQUAL = 44,              /* LESSER_EQUAL  */
  YYSYMBOL_GREATER = 45,                   /* GREATER  */
  YYSYMBOL_GREATER_EQUAL = 46,             /* GREATER_EQUAL  */
  YYSYMBOL_EQUAL = 47,                     /* EQUAL  */
  YYSYMBOL_UMINUS = 48,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 49,                  /* $accept  */
  YYSYMBOL_CAIXA_AUTO = 50,                /* CAIXA_AUTO  */
  YYSYMBOL_CREATE_DEPOSIT_OPERATION = 51,  /* CREATE_DEPOSIT_OPERATION  */
  YYSYMBOL_OPERATIONS = 52,                /* OPERATIONS  */
  YYSYMBOL_CONDITIONAL = 53,               /* CONDITIONAL  */
  YYSYMBOL_STOP_COND = 54,                 /* STOP_COND  */
  YYSYMBOL_OPERATION = 55,                 /* OPERATION  */
  YYSYMBOL_STOQUE = 56,                    /* STOQUE  */
  YYSYMBOL_DEPOSITO_OPERATION = 57,        /* DEPOSITO_OPERATION  */
  YYSYMBOL_CREATE_CART_OPERATION = 58,     /* CREATE_CART_OPERATION  */
  YYSYMBOL_VARIAVEL_OPERATION = 59,        /* VARIAVEL_OPERATION  */
  YYSYMBOL_EXPRESSAO = 60,                 /* EXPRESSAO  */
  YYSYMBOL_CART_OPERATION = 61,            /* CART_OPERATION  */
  YYSYMBOL_VENDA = 62,                     /* VENDA  */
  YYSYMBOL_PAGAMENTO = 63,                 /* PAGAMENTO  */
  YYSYMBOL_MAQUININHA = 64,                /* MAQUININHA  */
  YYSYMBOL_nome_produto = 65,              /* nome_produto  */
  YYSYMBOL_numero_preco = 66,              /* numero_preco  */
  YYSYMBOL_nome_variavel = 67,             /* nome_variavel  */
  YYSYMBOL_qr_code = 68                    /* qr_code  */
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
typedef yytype_int8 yy_state_t;

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
#define YYLAST   196

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  53
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  127

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   303


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
      45,    46,    47,    48
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   302,   302,   308,   312,   313,   316,   323,   333,   335,
     337,   339,   341,   343,   345,   347,   349,   352,   357,   362,
     367,   371,   375,   395,   402,   409,   422,   438,   446,   448,
     450,   452,   454,   464,   466,   468,   470,   472,   474,   476,
     478,   480,   484,   489,   493,   505,   507,   509,   512,   514,
     516,   518,   520,   522
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
  "SCAN", "INCREASE", "CONSULT", "CREATE_DEPOSIT", "GUARD", "APROVED",
  "REFUSED", "PRICE", "IF", "WHILE", "STOP", "AND", "OR", "PROVIDE",
  "PLUS", "MULT", "MINUS", "DIV", "END_LINE", "ASSIGN", "COLCH_OPEN",
  "COLCH_CLOSE", "KEY_OPEN", "KEY_CLOSE", "LESSER", "LESSER_EQUAL",
  "GREATER", "GREATER_EQUAL", "EQUAL", "UMINUS", "$accept", "CAIXA_AUTO",
  "CREATE_DEPOSIT_OPERATION", "OPERATIONS", "CONDITIONAL", "STOP_COND",
  "OPERATION", "STOQUE", "DEPOSITO_OPERATION", "CREATE_CART_OPERATION",
  "VARIAVEL_OPERATION", "EXPRESSAO", "CART_OPERATION", "VENDA",
  "PAGAMENTO", "MAQUININHA", "nome_produto", "numero_preco",
  "nome_variavel", "qr_code", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-51)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       4,   -51,     8,    27,   -51,   -51,   -51,    11,   -10,     2,
      12,    38,    50,    49,    21,    53,     1,   -51,   -51,   -51,
     -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,    23,   -51,
      35,    70,    42,    69,    43,   -51,   -51,   -51,    33,   -51,
      44,   -34,    54,    60,    57,    26,   -51,    96,   131,   -51,
       1,    97,    96,    61,    76,    96,   -51,   -51,    72,    96,
     -51,    96,   -51,   -51,    74,   -51,     1,   -51,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,    41,
      91,    78,    82,    96,    87,   -51,    93,    94,    98,    59,
     131,   131,   149,     0,   149,     0,   131,   131,   131,   131,
     131,   -51,   132,   105,   -51,   111,   126,   115,   116,   -51,
     -51,   -51,   117,   135,   136,     1,   -51,   -51,   -51,     1,
       1,    77,    95,   113,   -51,   -51,   -51
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     4,     0,     0,     1,    52,     2,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     9,    15,
      16,     5,     8,    10,    11,    14,    12,    13,     0,    50,
       0,     0,     0,     0,     0,    45,    46,    47,     0,    53,
       0,     0,     0,     0,     0,     0,    28,     0,     6,    30,
       0,     0,     0,     0,     0,     0,    48,    49,     0,     0,
      26,     0,    23,    24,     0,    21,     0,    29,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    44,     0,     0,     0,     0,
      40,    41,    33,    31,    34,    32,    35,    36,    37,    38,
      39,    22,     0,     0,    19,     0,     0,     0,     0,     3,
      20,    51,     0,     0,     0,     0,    27,    25,    17,     0,
       0,     0,     0,     0,    43,    18,    42
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,
     -51,   -50,   -51,   -51,   -51,   -51,   -51,   -51,    -1,   -46
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,    18,     3,    19,    20,    21,    22,    23,    24,
      25,    48,    26,    27,    38,    58,    30,   112,    49,    43
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      79,    67,    28,    60,    46,    61,    81,     5,     4,    84,
      47,     1,    42,    86,    45,    87,    89,    29,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,    31,
      68,    69,    33,     5,    32,     6,     7,   105,     8,     9,
      10,    11,    12,    74,    75,    76,    77,    78,    13,    14,
      15,    34,    39,    44,    16,     5,    17,    56,    57,     5,
      40,    50,    41,    65,    66,   121,    35,    36,    37,   122,
     123,    68,    69,    51,    70,    71,    72,    73,   101,    52,
      54,    53,    55,    59,    74,    75,    76,    77,    78,    68,
      69,    62,    70,    71,    72,    73,   110,    63,    64,    39,
      80,    82,    74,    75,    76,    77,    78,    68,    69,    85,
      70,    71,    72,    73,   124,    83,    88,   102,   103,   104,
      74,    75,    76,    77,    78,    68,    69,   106,    70,    71,
      72,    73,   125,   107,   108,   109,   111,   113,    74,    75,
      76,    77,    78,    68,    69,   115,    70,    71,    72,    73,
     126,   114,   116,   117,   118,   119,    74,    75,    76,    77,
      78,    68,    69,     0,    70,    71,    72,    73,   120,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,    68,
      69,     0,     0,    71,     0,    73,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78
};

static const yytype_int8 yycheck[] =
{
      50,    47,     3,    37,     3,    39,    52,     6,     0,    55,
       9,     7,    13,    59,    15,    61,    66,     6,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    39,
      30,    31,    20,     6,    32,     8,     9,    83,    11,    12,
      13,    14,    15,    43,    44,    45,    46,    47,    21,    22,
      23,    13,     3,    32,    27,     6,    29,    24,    25,     6,
      11,    38,    13,    37,    38,   115,    16,    17,    18,   119,
     120,    30,    31,    38,    33,    34,    35,    36,    37,     9,
      11,    39,    39,    39,    43,    44,    45,    46,    47,    30,
      31,    37,    33,    34,    35,    36,    37,    37,    41,     3,
       3,    40,    43,    44,    45,    46,    47,    30,    31,    37,
      33,    34,    35,    36,    37,    39,    42,    26,    40,    37,
      43,    44,    45,    46,    47,    30,    31,    40,    33,    34,
      35,    36,    37,    40,    40,    37,     4,    32,    43,    44,
      45,    46,    47,    30,    31,    19,    33,    34,    35,    36,
      37,    40,    37,    37,    37,    20,    43,    44,    45,    46,
      47,    30,    31,    -1,    33,    34,    35,    36,    32,    -1,
      -1,    -1,    -1,    -1,    43,    44,    45,    46,    47,    30,
      31,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    -1,
      -1,    -1,    43,    44,    45,    46,    47
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     7,    50,    52,     0,     6,     8,     9,    11,    12,
      13,    14,    15,    21,    22,    23,    27,    29,    51,    53,
      54,    55,    56,    57,    58,    59,    61,    62,    67,     6,
      65,    39,    32,    20,    13,    16,    17,    18,    63,     3,
      11,    13,    67,    68,    32,    67,     3,     9,    60,    67,
      38,    38,     9,    39,    11,    39,    24,    25,    64,    39,
      37,    39,    37,    37,    41,    37,    38,    68,    30,    31,
      33,    34,    35,    36,    43,    44,    45,    46,    47,    60,
       3,    68,    40,    39,    68,    37,    68,    68,    42,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    37,    26,    40,    37,    68,    40,    40,    40,    37,
      37,     4,    66,    32,    40,    19,    37,    37,    37,    20,
      32,    60,    60,    60,    37,    37,    37
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    49,    50,    51,    52,    52,    53,    54,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    56,    57,    58,
      59,    59,    59,    59,    59,    59,    59,    59,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    61,    62,    62,    63,    63,    63,    64,    64,
      65,    66,    67,    68
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     3,     5,     0,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     7,     9,     5,
       5,     3,     4,     3,     3,     6,     3,     6,     1,     2,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     9,     8,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1
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
#line 302 "parser.y"
                         { 
        printf("\nCompra finalizada!\n"); 
        return 0; 
    }
#line 1496 "parser.tab.c"
    break;

  case 3: /* CREATE_DEPOSIT_OPERATION: CREATE_DEPOSIT PROVIDE KEY_OPEN KEY_CLOSE END_LINE  */
#line 308 "parser.y"
                                                       {
        criar_deposito();
    }
#line 1504 "parser.tab.c"
    break;

  case 6: /* CONDITIONAL: IF EXPRESSAO  */
#line 316 "parser.y"
                 {
        if (!(yyvsp[0].num)) {
            executar_operacoes = 0;
        }
    }
#line 1514 "parser.tab.c"
    break;

  case 7: /* STOP_COND: STOP  */
#line 323 "parser.y"
         {
        nivel_condicional--;
        executar_operacoes = 1;
        // if (nivel_condicional > 0) {
        //     nivel_condicional--;
        //     executar_operacoes = 1;
        // }
    }
#line 1527 "parser.tab.c"
    break;

  case 17: /* STOQUE: PRODUCT nome_produto ASSIGN NUMBER PRICE numero_preco END_LINE  */
#line 352 "parser.y"
                                                                   {
        adicionar_produto((yyvsp[-5].str), (int)(yyvsp[-3].num), (yyvsp[-1].dbl));
    }
#line 1535 "parser.tab.c"
    break;

  case 18: /* DEPOSITO_OPERATION: DEPOSIT COLCH_OPEN PRODUCT qr_code COLCH_CLOSE PROVIDE INCREASE EXPRESSAO END_LINE  */
#line 357 "parser.y"
                                                                                       {
        adicionar_ao_deposito((yyvsp[-5].num), (int)(yyvsp[-1].num));
    }
#line 1543 "parser.tab.c"
    break;

  case 19: /* CREATE_CART_OPERATION: CREATE_CART PROVIDE COLCH_OPEN COLCH_CLOSE END_LINE  */
#line 362 "parser.y"
                                                        {
        criar_carrinho();
    }
#line 1551 "parser.tab.c"
    break;

  case 20: /* VARIAVEL_OPERATION: GUARD nome_variavel ASSIGN EXPRESSAO END_LINE  */
#line 367 "parser.y"
                                                  {
        definir_variavel((yyvsp[-3].str), (yyvsp[-1].num));
    }
#line 1559 "parser.tab.c"
    break;

  case 21: /* VARIAVEL_OPERATION: GUARD nome_variavel END_LINE  */
#line 371 "parser.y"
                                 {
        definir_variavel((yyvsp[-1].str), 0);
    }
#line 1567 "parser.tab.c"
    break;

  case 22: /* VARIAVEL_OPERATION: nome_variavel ASSIGN EXPRESSAO END_LINE  */
#line 375 "parser.y"
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
                definir_variavel((yyvsp[-3].str), (yyvsp[-1].num));
            }
        } else {
            definir_variavel((yyvsp[-3].str), (yyvsp[-1].num));
        }
    }
#line 1591 "parser.tab.c"
    break;

  case 23: /* VARIAVEL_OPERATION: CONSULT nome_variavel END_LINE  */
#line 395 "parser.y"
                                   {
        if (executar_operacoes) {
            int valor = consultar_variavel_nome((yyvsp[-1].str));
            printf("Variavel %s = %d\n", (yyvsp[-1].str), valor);
        }
    }
#line 1602 "parser.tab.c"
    break;

  case 24: /* VARIAVEL_OPERATION: CONSULT qr_code END_LINE  */
#line 402 "parser.y"
                             {
        if (executar_operacoes) {
            double valor = consultar_produto((yyvsp[-1].num));
            printf("Produto %d = R$ %.2f\n", (yyvsp[-1].num), valor);
        }
    }
#line 1613 "parser.tab.c"
    break;

  case 25: /* VARIAVEL_OPERATION: CONSULT CART COLCH_OPEN qr_code COLCH_CLOSE END_LINE  */
#line 409 "parser.y"
                                                         {
        if (executar_operacoes) {
            int indice_carrinho = encontrar_item_carrinho((yyvsp[-2].num));
            if(indice_carrinho >= 0) {
                printf("Carrinho[%d] = %d unidades\n", 
                    (yyvsp[-2].num), 
                    carrinho[indice_carrinho].quantidade_carrinho - carrinho[indice_carrinho].quantidade_vendida);
            } else {
                printf("Carrinho[%d] = 0 unidades\n", (yyvsp[-2].num));
            }
        }
    }
#line 1630 "parser.tab.c"
    break;

  case 26: /* VARIAVEL_OPERATION: CONSULT CART END_LINE  */
#line 422 "parser.y"
                          {
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
#line 1650 "parser.tab.c"
    break;

  case 27: /* VARIAVEL_OPERATION: CONSULT DEPOSIT COLCH_OPEN qr_code COLCH_CLOSE END_LINE  */
#line 438 "parser.y"
                                                            {
        if (executar_operacoes) {
            int qtd = consultar_deposito((yyvsp[-2].num));
            printf("Depósito[%d] = %d quantidades\n", (yyvsp[-2].num), qtd);
        }
    }
#line 1661 "parser.tab.c"
    break;

  case 28: /* EXPRESSAO: NUMBER  */
#line 446 "parser.y"
           { (yyval.num) = (yyvsp[0].num); }
#line 1667 "parser.tab.c"
    break;

  case 29: /* EXPRESSAO: PRODUCT qr_code  */
#line 448 "parser.y"
                    { (yyval.num) = consultar_produto((yyvsp[0].num)); }
#line 1673 "parser.tab.c"
    break;

  case 30: /* EXPRESSAO: nome_variavel  */
#line 450 "parser.y"
                  { (yyval.num) = consultar_variavel_nome((yyvsp[0].str)); }
#line 1679 "parser.tab.c"
    break;

  case 31: /* EXPRESSAO: EXPRESSAO MULT EXPRESSAO  */
#line 452 "parser.y"
                             { (yyval.num) = (yyvsp[-2].num) * (yyvsp[0].num); }
#line 1685 "parser.tab.c"
    break;

  case 32: /* EXPRESSAO: EXPRESSAO DIV EXPRESSAO  */
#line 454 "parser.y"
                            { if((yyvsp[0].num) == 0) {
            printf("Erro: Divisão por zero!\n");
            (yyval.num) = 0;
        }

        else {
            (yyval.num) = (yyvsp[-2].num) / (yyvsp[0].num); 
        }    
    }
#line 1699 "parser.tab.c"
    break;

  case 33: /* EXPRESSAO: EXPRESSAO PLUS EXPRESSAO  */
#line 464 "parser.y"
                             { (yyval.num) = (yyvsp[-2].num) + (yyvsp[0].num); }
#line 1705 "parser.tab.c"
    break;

  case 34: /* EXPRESSAO: EXPRESSAO MINUS EXPRESSAO  */
#line 466 "parser.y"
                              { (yyval.num) = (yyvsp[-2].num) - (yyvsp[0].num); }
#line 1711 "parser.tab.c"
    break;

  case 35: /* EXPRESSAO: EXPRESSAO LESSER EXPRESSAO  */
#line 468 "parser.y"
                               { (yyval.num) = (yyvsp[-2].num) < (yyvsp[0].num); }
#line 1717 "parser.tab.c"
    break;

  case 36: /* EXPRESSAO: EXPRESSAO LESSER_EQUAL EXPRESSAO  */
#line 470 "parser.y"
                                     { (yyval.num) = (yyvsp[-2].num) <= (yyvsp[0].num); }
#line 1723 "parser.tab.c"
    break;

  case 37: /* EXPRESSAO: EXPRESSAO GREATER EXPRESSAO  */
#line 472 "parser.y"
                                { (yyval.num) = (yyvsp[-2].num) > (yyvsp[0].num); }
#line 1729 "parser.tab.c"
    break;

  case 38: /* EXPRESSAO: EXPRESSAO GREATER_EQUAL EXPRESSAO  */
#line 474 "parser.y"
                                      { (yyval.num) = (yyvsp[-2].num) >= (yyvsp[0].num); }
#line 1735 "parser.tab.c"
    break;

  case 39: /* EXPRESSAO: EXPRESSAO EQUAL EXPRESSAO  */
#line 476 "parser.y"
                              { (yyval.num) = (yyvsp[-2].num) == (yyvsp[0].num); }
#line 1741 "parser.tab.c"
    break;

  case 40: /* EXPRESSAO: EXPRESSAO AND EXPRESSAO  */
#line 478 "parser.y"
                            { (yyval.num) = (yyvsp[-2].num) && (yyvsp[0].num); }
#line 1747 "parser.tab.c"
    break;

  case 41: /* EXPRESSAO: EXPRESSAO OR EXPRESSAO  */
#line 480 "parser.y"
                           { (yyval.num) = (yyvsp[-2].num) || (yyvsp[0].num); }
#line 1753 "parser.tab.c"
    break;

  case 42: /* CART_OPERATION: CART INCREASE DEPOSIT COLCH_OPEN qr_code COLCH_CLOSE PROVIDE EXPRESSAO END_LINE  */
#line 484 "parser.y"
                                                                                    {
        adicionar_ao_carrinho((yyvsp[-4].num), (int)(yyvsp[-1].num));
    }
#line 1761 "parser.tab.c"
    break;

  case 43: /* VENDA: SELL CART COLCH_OPEN qr_code COLCH_CLOSE SCAN EXPRESSAO END_LINE  */
#line 489 "parser.y"
                                                                     {
        processar_venda((yyvsp[-4].num), (int)(yyvsp[-1].num));
    }
#line 1769 "parser.tab.c"
    break;

  case 44: /* VENDA: PAYMENT PAGAMENTO MAQUININHA END_LINE  */
#line 493 "parser.y"
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
#line 1784 "parser.tab.c"
    break;

  case 45: /* PAGAMENTO: CREDIT  */
#line 505 "parser.y"
           { (yyval.str) = "CRÉDITO"; }
#line 1790 "parser.tab.c"
    break;

  case 46: /* PAGAMENTO: DEBIT  */
#line 507 "parser.y"
          { (yyval.str) = "DÉBITO"; }
#line 1796 "parser.tab.c"
    break;

  case 47: /* PAGAMENTO: PIX  */
#line 509 "parser.y"
        { (yyval.str) = "PIX"; }
#line 1802 "parser.tab.c"
    break;

  case 48: /* MAQUININHA: APROVED  */
#line 512 "parser.y"
            { (yyval.str) = "APROVADO"; }
#line 1808 "parser.tab.c"
    break;

  case 49: /* MAQUININHA: REFUSED  */
#line 514 "parser.y"
            { (yyval.str) = "RECUSADO"; }
#line 1814 "parser.tab.c"
    break;

  case 50: /* nome_produto: IDENTIFIER  */
#line 516 "parser.y"
                         { (yyval.str) = (yyvsp[0].str); }
#line 1820 "parser.tab.c"
    break;

  case 51: /* numero_preco: DECIMAL  */
#line 518 "parser.y"
                      { (yyval.dbl) = (yyvsp[0].dbl); }
#line 1826 "parser.tab.c"
    break;

  case 52: /* nome_variavel: IDENTIFIER  */
#line 520 "parser.y"
                          { (yyval.str) = (yyvsp[0].str); }
#line 1832 "parser.tab.c"
    break;

  case 53: /* qr_code: NUMBER  */
#line 522 "parser.y"
                { (yyval.num) = (yyvsp[0].num); }
#line 1838 "parser.tab.c"
    break;


#line 1842 "parser.tab.c"

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

#line 524 "parser.y"


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
