# Caixa Eletrônico Automático

```
programa       = "INICIAR", bloco, "FINALIZAR";

(* Bloco de declarações e instruções *)
bloco          = { declaracao | instrucao };

(* Declarações *)
declaracao     = ( produto_decl | estoque_decl ), "FIM_LINHA";
produto_decl   = "PRODUTO", identificador, "ATRIBUIR", NUMBER, "COM_PRECO", numero_preco;
estoque_decl   = "ESTOQUE", identificador, "ATRIBUIR", NUMBER;

(* Instruções *)
instrucao      = ( venda | pagamento | condicional ), "FIM_LINHA";
venda          = "VENDER", identificador, "QUANTIDADE", quantidade;
pagamento      = "PAGAMENTO", metodo_pagamento, resultado_pagamento;
condicional    = "SE", condicao, "ENTAO", bloco, [ "SENAO", bloco ];

(* Métodos de pagamento *)
metodo_pagamento = "CREDITO" | "DEBITO" | "PIX";
resultado_pagamento = "APROVADO" | "RECUSADO";

(* Expressões e condições *)
condicao       = "PAGAMENTO_APROVADO"
               | expressao, operador_comparacao, expressao;
operador_comparacao = ">" | "<" | ">=" | "<=" | "==" | "!=";

expressao      = termo, { operador_aditivo, termo };
termo          = fator, { operador_multiplicativo, fator };
fator          = NUMBER 
               | identificador
               | "(", expressao, ")";
operador_aditivo = "+" | "-";
operador_multiplicativo = "*" | "/";

(* Elementos básicos *)
identificador  = IDENTIFIER;
numero_preco   = NUMBER;
quantidade     = NUMBER;

(* Tokens terminais *)
NUMBER         = digit, { digit }, [ ".", digit, { digit } ];
IDENTIFIER     = letter, { letter | digit | "_" };
digit          = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
letter         = "a" | "b" | ... | "z" | "A" | "B" | ... | "Z";

(* Palavras reservadas *)
"INICIAR"      = "INICIAR";
"FINALIZAR"    = "FINALIZAR";
"PRODUTO"      = "PRODUTO";
"ESTOQUE"      = "ESTOQUE";
"VENDER"       = "VENDER";
"PAGAMENTO"    = "PAGAMENTO";
"SE"           = "SE";
"ENTAO"        = "ENTAO";
"SENAO"        = "SENAO";
"CREDITO"      = "CREDITO";
"DEBITO"       = "DEBITO";
"PIX"          = "PIX";
"APROVADO"     = "APROVADO";
"RECUSADO"     = "RECUSADO";
"COM_PRECO"    = "COM_PRECO";
"QUANTIDADE"   = "QUANTIDADE";
"PAGAMENTO_APROVADO" = "PAGAMENTO_APROVADO";

(* Operadores e símbolos *)
"ATRIBUIR"     = "=";
"FIM_LINHA"    = ";";
"+"            = "+";
"-"            = "-";
"*"            = "*";
"/"            = "/";
">"            = ">";
"<"            = "<";
">="           = ">=";
"<="           = "<=";
"=="           = "==";
"!="           = "!=";
```

Essa EBNF representa a linguagem de um caixa eletrônico automático de um mercadinho de um prédio, onde a partir dele, os residentes podem comprar os produtos ao passar os QR code dos produtos e realizar o pagamento

## Exemplo 1
**Arquivo.jota**
```
INICIAR
PRODUTO arroz = 732 COM_PRECO 10.00;
PRODUTO frango = 123 COM_PRECO 8.00;
PRODUTO salmao = 789 COM_PRECO 25.00;

ESTOQUE arroz = 100;
ESTOQUE frango = 50;
ESTOQUE salmao = 20;

VENDER arroz QUANTIDADE: 7;
VENDER frango QUANTIDADE: 37;
VENDER salmao QUANTIDADE: 1;

PAGAMENTO PIX APROVADO;
FINALIZAR
```

**Saída**
```
======= Mercadinho do seu Prédinho ======
Produtos: 7x arroz, 37x frango, 1x salmao
Método de pagamento: PIX
Total da venda: R$ 391.00
Status: APROVADO

Compra finalizada!
```

## Exemplo 2
**Arquivo.jota**
```
INICIAR
PRODUTO frangostoso = 111 COM_PRECO 5.00;
ESTOQUE frangostoso = 20;
VENDER frangostoso QUANTIDADE: 1;
PAGAMENTO PIX APROVADO;
FINALIZAR
```

**Saída**
```
======= Mercadinho do seu Prédinho ======
Produtos: 1x frangostoso
Método de pagamento: PIX
Total da venda: R$ 5.00
Status: APROVADO

Compra finalizada!
```

## Exemplo sem estoque
**Arquivo.jota**
```
INICIAR
PRODUTO frangostoso = 111 COM_PRECO 5.00;

VENDER frangostoso QUANTIDADE: 1;
PAGAMENTO PIX APROVADO;
FINALIZAR
```

**Saída**
```
======= Mercadinho do seu Prédinho ======
Erro: Estoque insuficiente para frangostoso
Produtos:
Método de pagamento: PIX
Total da venda: R$ 0.00
Status: APROVADO

Compra finalizada!
```

## Exemplo status RECUSADO
**Arquivo.jota**
```
INICIAR
PRODUTO frangostoso = 111 COM_PRECO 5.00;

VENDER frangostoso QUANTIDADE: 1;
PAGAMENTO PIX RECUSADO;
FINALIZAR
```

**Saída**
```
======= Mercadinho do seu Prédinho ======

Pagamento RECUSADO

Compra finalizada!
```

## Comandos e sintática
**INICIAR**
```
Inicia o processo de compras da maquininha
```

**PRODUTO**
```
PRODUCT identificador ASSIGN NUMBER WITH_PRICE numero_preco END_LINE

PRODUTO define um produto x com o qr code y e valor z
```

**ESTOQUE**
```
STOCK identificador ASSIGN NUMBER END_LINE

ESTOQUE define a quantidade de unidades de um produto x
```

**VENDER**
```
SELL identificador QUANTITY quantidade END_LINE

VENDER é adicionar o produto no carrinho e receber uma determinada quantidade para esse produto, também diminui o valor definido em ESTOQUE
```

**PAGAMENTO**
```
PAYMENT metodo_pagamento resultado_pagamento END_LINE

O pagamento finaliza tudo se o pagamento foi RECUSADO, caso seja APROVADO, continua para a etapa de FINALIZAR
```

**FINALIZAR**
```
END

Finaliza o processo, caso seja usado antes da linha de PAGAMENTO, vai CANCELAR a compra
```

## Compilar/Executar
```
make
./caixa_auto < arquivo.jota

O arquivo.jota você pode criar usando algum dos exemplos acima
```