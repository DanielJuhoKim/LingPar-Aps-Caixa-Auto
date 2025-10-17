%{
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern FILE* yyin;
%}

DIGIT    [0-9]
CHAR     [a-zA-Z_]
ID       {CHAR}({CHAR}|{DIGIT})*
NUM      {DIGIT}+(\.{DIGIT}+)?
STRING   \"([^\"\\]|\\.)*\"
COMENTARIO \/\/[^\n]*

%option noyywrap nounput noinput

%%

{COMENTARIO}

"INICIAR"    { return START; }
"FINALIZAR"  { return END; }
"PRODUTO"    { return PRODUCT; }
"ESTOQUE"    { return STOCK; }
"VENDER"     { return SELL; }
"PAGAMENTO"  { return PAYMENT; }
"CREDITO"    { return CREDIT; }
"DEBITO"     { return DEBIT; }
"PIX"        { return PIX; }

"PRECO"  { return PRICE; }
"PAGAMENTO_APROVADO" { return PAYMENT_APPROVED; }
"APROVADO"   { return APROVED; }
"RECUSADO"   { return REFUSED; }

"="          { return ASSIGN; }
":"          { return PROVIDE; }
"+"          { return PLUS; }
"-"          { return MINUS; }
";"          { return END_LINE; }

{NUM}        { yylval.num = atof(yytext); return NUMBER; }
{STRING}     { yylval.str = strdup(yytext); return STRING; }
{ID}         { yylval.str = strdup(yytext); return IDENTIFIER; }

[ \t\r\n]+

.            { 
    printf("[Lexer]: Caractere inválido: '%s'\n", yytext); 
}

%%