%{
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
%}

DIGIT         [0-9]
CHAR          [a-zA-Z]
PALAVRA       {CHAR}+
QR            {DIGIT}+
NUM           {DIGIT}+

%option noyywrap nounput noinput

%%

{NUM}         { yylval.num = atoi(yytext); return NUMBER; }

"CANCELAR"    { return CANCEL; }
"VOLTAR"      { return BACK; }
"CREDITO"     { return CREDIT; }
"DEBITO"      { return DEBIT; }
"PIX"         { return PIX; }
"APROVADO"    { return APROVED; }
"RECUSADO"    { return REFUSED; }
"+"           { return PLUS; }
"-"           { return MINUS; }
":"           { return PROVIDE; }

[ \t\r\n]+

.             { printf("[Lexer]: Caractere não reconhecido: %s\n", yytext); }

%%