# Caixa Eletrônico Automático

```
CAIXA_AUTO = "INICIAR", OPERATION, "FINALIZAR" ;

OPERATION =
    |
    OPERATION, STOQUE
    |
    OPERATION, CARRINHO ;

STOQUE =
    "PRODUTO", nome_produto, "=", NUMBER, "PRECO", numero_preco, ";"
    |
    "ESTOQUE", nome_produto, "=", NUMBER, ";"

CARRINHO =
    "VENDER", qr_code, ":", quantidade, ";"
    |
    "PAGAMENTO", PAGAMENTO, MAQUININHA, ";"

PAGAMENTO = "CREDITO" | "DEBITO" | "PIX" ;

MAQUININHA = "APROVADO" | "RECUSADO" ;

nome_produto = IDENTIFIER ;
numero_preco = NUMBER ;
quantidade = NUMBER ;
qr_code = NUMBER ;

IDENTIFIER = LETTER, { LETTER | DIGIT | "_" } ;
NUMBER = DIGIT, { DIGIT }, [ ".", DIGIT, { DIGIT } ] ;

LETTER = "a" | "b" | "c" | ... | "z" | "A" | "B" | ... | "Z" | "_" ;
DIGIT = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;

START = "INICIAR" ;
END = "FINALIZAR" ;
PRODUCT = "PRODUTO" ;
STOCK = "ESTOQUE" ;
SELL = "VENDER" ;
PAYMENT = "PAGAMENTO" ;
CREDIT = "CREDITO" ;
DEBIT = "DEBITO" ;
PIX = "PIX" ;
PRICE = "PRECO" ;
APROVED = "APROVADO" ;
REFUSED = "RECUSADO" ;
ASSIGN = "=" ;
PROVIDE = ":" ;
END_LINE = ";" ;

WHITESPACE = " " | "\t" | "\r" | "\n" ;
COMMENT = "//", { qualquer_caractere_exceto_"\n" } ;
```

Essa EBNF representa a linguagem de um caixa eletrônico automático de um mercadinho de um prédio, onde a partir dele, os residentes podem comprar os produtos ao passar os QR code dos produtos e realizar o pagamento

## Exemplo 1
**Arquivo.jota**
```
INICIAR

PRODUTO arroz = 732 PRECO 10.00;
PRODUTO feijao = 123 PRECO 8.00;
PRODUTO carne = 456 PRECO 25.00;

ESTOQUE arroz = 50;
ESTOQUE feijao = 30;
ESTOQUE carne = 20;

VENDER 732 : 2;
VENDER 123 : 1;
VENDER 456 : 1;

PAGAMENTO PIX APROVADO;
FINALIZAR
```

**Saída**
```
======= Mercadinho do seu Prédinho ======
Produtos: 2x arroz, 1x feijao, 1x carne
Método de pagamento: PIX
Total da venda: R$ 53.00
Status: APROVADO

Compra finalizada!
```

## Exemplo 2
**Arquivo.jota**
```
INICIAR

PRODUTO frangostoso = 737 PRECO 10.00;

ESTOQUE frangostoso = 20;

VENDER 737 : 2;

PAGAMENTO PIX APROVADO;
FINALIZAR
```

**Saída**
```
======= Mercadinho do seu Prédinho ======
Produtos: 2x frangostoso
Método de pagamento: PIX
Total da venda: R$ 20.00
Status: APROVADO

Compra finalizada!
```

## Exemplo sem estoque
**Arquivo.jota**
```
INICIAR

PRODUTO frangostoso = 737 PRECO 10.00;

ESTOQUE frangostoso = 20;

VENDER 737 : 30;

PAGAMENTO PIX APROVADO;
FINALIZAR
```

**Saída**
```
======= Mercadinho do seu Prédinho ======
Erro: Estoque do produto 737 insuficiente

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

PRODUTO frangostoso = 737 PRECO 10.00;

ESTOQUE frangostoso = 20;

VENDER 737 : 30;

PAGAMENTO PIX RECUSADO;
FINALIZAR
```

**Saída**
```
======= Mercadinho do seu Prédinho ======
Erro: Estoque do produto 737 insuficiente


Pagamento RECUSADO

Compra finalizada!
```

## Comandos | Sintática | Exemplo
**INICIAR**
```
START OPERATION END

INICIAR

Inicia o processo de compras da maquininha
```

**PRODUTO**
```
PRODUCT nome_produto ASSIGN NUMBER PRICE numero_preco END_LINE

PRODUTO carne = 456 PRECO 25.00;

PRODUTO define um produto x com o qr code y e valor z
```

**ESTOQUE**
```
STOCK nome_produto ASSIGN NUMBER END_LINE

ESTOQUE arroz = 50;

ESTOQUE define a quantidade de unidades de um produto x
```

**VENDER**
```
SELL qr_code PROVIDE quantidade END_LINE

VENDER 737 : 30;

VENDER é adicionar o produto no carrinho e receber uma determinada quantidade para esse produto, também diminui o valor definido em ESTOQUE
```

**PAGAMENTO**
```
PAYMENT PAGAMENTO MAQUININHA END_LINE

PAGAMENTO PIX APROVADO;

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