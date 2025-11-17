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
DECIMAL  {DIGIT}+\.{DIGIT}+
STRING   \"([^\"\\]|\\.)*\"
COMENTARIO \/\/[^\n]*

%option noyywrap nounput noinput

%%

{COMENTARIO}

"INICIAR"    { return START; }
"FINALIZAR"  { return END; }
"PRODUTO"    { return PRODUCT; }
"ESTOQUE"    { return STOCK; }
"DEPOSITO"   { return DEPOSIT; }
"CRIAR_CARRINHO" {return CREATE_CART; }
"CRIAR_DEPOSITO" { return CREATE_DEPOSIT; }
"GUARDAR"    { return GUARD; }
"CARRINHO"   { return CART; }
"VENDER"     { return SELL; }
"PAGAMENTO"  { return PAYMENT; }
"ADICIONAR"  { return INCREASE; }
"CREDITO"    { return CREDIT; }
"DEBITO"     { return DEBIT; }
"PIX"        { return PIX; }
"SCANEAR"    { return SCAN; }
"CONSULTAR"  { return CONSULT; }
"MENSAGEM"   { return MESSAGE; }

"PRECO"      { return PRICE; }
"APROVADO"   { return APROVED; }
"RECUSADO"   { return REFUSED; }

"VERIFICAR_CONSTANTEMENTE"   {return WHILE; }   
"VERIFICAR"   {return IF; }
"@@"  { return STOP; }
"$$"  { return STOP_WHILE; }
"ALEM_DE"   { return AND; }
"OU"    {return OR; }

"="          { return ASSIGN; }
":"          { return PROVIDE; }
"+"          { return PLUS; }
"*"          { return MULT; }
"/"          { return DIV; }
"-"          { return MINUS; }
";"          { return END_LINE; }
"["          { return COLCH_OPEN; }
">"         { return GREATER; }
"<"         { return LESSER; }
">="         { return GREATER_EQUAL; }
"<="         { return LESSER_EQUAL; }
"=="         { return EQUAL; }
"]"          { return COLCH_CLOSE; }
"{"          { return KEY_OPEN; }
"}"          { return KEY_CLOSE; }
"("          { return PAR_OPEN; }
")"          { return PAR_CLOSE; }

{DECIMAL}    { yylval.dbl = atof(yytext); return DECIMAL; }
{NUM}        { yylval.num = atoi(yytext); return NUMBER; }
{STRING}     { yylval.str = strdup(yytext); return STRING; }
{ID}         { yylval.str = strdup(yytext); return IDENTIFIER; }

[ \t\r\n]+

.            { 
    printf("[Lexer]: Caractere inválido: '%s'\n", yytext); 
}

%%