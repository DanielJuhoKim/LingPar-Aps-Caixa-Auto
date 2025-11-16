/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUMBER = 258,                  /* NUMBER  */
    DECIMAL = 259,                 /* DECIMAL  */
    STRING = 260,                  /* STRING  */
    IDENTIFIER = 261,              /* IDENTIFIER  */
    START = 262,                   /* START  */
    END = 263,                     /* END  */
    PRODUCT = 264,                 /* PRODUCT  */
    STOCK = 265,                   /* STOCK  */
    DEPOSIT = 266,                 /* DEPOSIT  */
    CREATE_CART = 267,             /* CREATE_CART  */
    CART = 268,                    /* CART  */
    SELL = 269,                    /* SELL  */
    PAYMENT = 270,                 /* PAYMENT  */
    CREDIT = 271,                  /* CREDIT  */
    DEBIT = 272,                   /* DEBIT  */
    PIX = 273,                     /* PIX  */
    SCAN = 274,                    /* SCAN  */
    INCREASE = 275,                /* INCREASE  */
    CONSULT = 276,                 /* CONSULT  */
    CREATE_DEPOSIT = 277,          /* CREATE_DEPOSIT  */
    GUARD = 278,                   /* GUARD  */
    APROVED = 279,                 /* APROVED  */
    REFUSED = 280,                 /* REFUSED  */
    PRICE = 281,                   /* PRICE  */
    IF = 282,                      /* IF  */
    WHILE = 283,                   /* WHILE  */
    STOP = 284,                    /* STOP  */
    AND = 285,                     /* AND  */
    OR = 286,                      /* OR  */
    PROVIDE = 287,                 /* PROVIDE  */
    PLUS = 288,                    /* PLUS  */
    MULT = 289,                    /* MULT  */
    MINUS = 290,                   /* MINUS  */
    DIV = 291,                     /* DIV  */
    END_LINE = 292,                /* END_LINE  */
    ASSIGN = 293,                  /* ASSIGN  */
    COLCH_OPEN = 294,              /* COLCH_OPEN  */
    COLCH_CLOSE = 295,             /* COLCH_CLOSE  */
    KEY_OPEN = 296,                /* KEY_OPEN  */
    KEY_CLOSE = 297,               /* KEY_CLOSE  */
    LESSER = 298,                  /* LESSER  */
    LESSER_EQUAL = 299,            /* LESSER_EQUAL  */
    GREATER = 300,                 /* GREATER  */
    GREATER_EQUAL = 301,           /* GREATER_EQUAL  */
    EQUAL = 302,                   /* EQUAL  */
    UMINUS = 303                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 275 "parser.y"

    int num;
    double dbl;
    char* str;

#line 118 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
