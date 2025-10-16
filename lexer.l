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

{COMENTARIO}  { /* Ignorar comentários */ }

"INICIAR"    { return START; }
"FINALIZAR"  { return END; }
"PRODUTO"    { return PRODUCT; }
"ESTOQUE"    { return STOCK; }
"VENDER"     { return SELL; }
"PAGAMENTO"  { return PAYMENT; }
"CREDITO"    { return CREDIT; }
"DEBITO"     { return DEBIT; }
"PIX"        { return PIX; }
"SE"         { return IF; }
"ENTAO"      { return THEN; }
"SENAO"      { return ELSE; }
"IMPRIMIR"   { return PRINT; }

"COM_PRECO"  { return WITH_PRICE; }
"QUANTIDADE:" { return QUANTITY; }
"PAGAMENTO_APROVADO" { return PAYMENT_APPROVED; }
"APROVADO"   { return APROVED; }
"RECUSADO"   { return REFUSED; }

"+"          { return PLUS; }
"-"          { return MINUS; }
"*"          { return MULT; }
"/"          { return DIV; }
"="          { return ASSIGN; }
"=="         { return EQ; }
"!="         { return NEQ; }
">"          { return GT; }
"<"          { return LT; }
">="         { return GTE; }
"<="         { return LTE; }
":"          { return ':'; }
";"          { return ';'; }

{NUM}        { yylval.num = atof(yytext); return NUMBER; }
{STRING}     { yylval.str = strdup(yytext); return STRING; }
{ID}         { yylval.str = strdup(yytext); return IDENTIFIER; }

[ \t\r\n]+   { /* ignorar espaços, tabs e CR/LF */ }

.            { 
    printf("[Lexer]: Caractere não reconhecido: '%s' (ASCII: %d)\n", 
           yytext, (unsigned char)yytext[0]); 
}

%%