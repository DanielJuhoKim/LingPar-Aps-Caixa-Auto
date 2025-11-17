# Caixa Eletrônico Automático

```
CAIXA_AUTO = "INICIAR", {OPERATION}, "FINALIZAR" ;

OPERATION = 
    STOQUE
    |
    DEPOSITO_OPERATION
    |
    CART_OPERATION
    |
    VENDA
    |
    CONDITIONAL
    |
    WHILE_LOOP
    |
    VARIAVEL_OPERATION
    |
    MENSAGEM ;

STOQUE = "PRODUTO", nome_produto, "=", qr_code, "PRECO", EXPRESSAO, ";" ;

DEPOSITO_OPERATION = nome_deposito, "[", "PRODUTO", qr_code, "]", ":", "ADICIONAR", EXPRESSAO, ";" ;

CART_OPERATION = nome_carrinho, "ADICIONAR", nome_deposito, "[", qr_code, "]", ":", EXPRESSAO, ";" ;

VENDA = "VENDER", nome_carrinho, "[", qr_code, "]", "SCANEAR", EXPRESSAO, ";"
    |
    "PAGAMENTO", PAGAMENTO, MAQUININHA, ";" ;

CONDITIONAL = "VERIFICAR", EXPRESSAO, OPERATION, "@@" ;

WHILE_LOOP = "VERIFICAR_CONSTANTEMENTE", EXPRESSAO, OPERATION, "$$" ;

VARIAVEL_OPERATION = "GUARDAR", nome_variavel, "=", EXPRESSAO, ";"
    |
    "GUARDAR", nome_variavel, ";"
    |
    nome_variavel, "=", EXPRESSAO, ";"
    |
    "CONSULTAR", nome_variavel, ";"
    |
    "CONSULTAR", qr_code, ";"
    |
    "CONSULTAR", "CARRINHO", nome_carrinho, "[", qr_code, "]", ";"
    |
    "CONSULTAR", "CARRINHO", nome_carrinho, ";"
    |
    "CONSULTAR", "DEPOSITO", nome_deposito, "[", qr_code, "]", ";" ;

MENSAGEM = "MENSAGEM", "(", STRING, ")", ";" ;

EXPRESSAO = 
    NUMBER
    |
    DECIMAL
    |
    "PRODUTO", qr_code
    |
    nome_variavel
    |
    EXPRESSAO, ("*" | "/" | "+" | "-"), EXPRESSAO
    |
    EXPRESSAO, ("<" | "<=" | ">" | ">=" | "=="), EXPRESSAO
    |
    EXPRESSAO, ("ALEM_DE" | "OU"), EXPRESSAO ;

PAGAMENTO = "CREDITO" | "DEBITO" | "PIX" ;
MAQUININHA = "APROVADO" | "RECUSADO" ;

nome_produto = IDENTIFIER ;
nome_variavel = IDENTIFIER ;
nome_deposito = IDENTIFIER ;
nome_carrinho = IDENTIFIER ;
qr_code = NUMBER ;
quantidade = NUMBER ;

IDENTIFIER = LETTER, { LETTER | DIGIT | "_" } ;
NUMBER = DIGIT, { DIGIT } ;
DECIMAL = DIGIT, { DIGIT }, ".", DIGIT, { DIGIT } ;
STRING = '"', { qualquer_caractere_exceto_aspas }, '"' ;

LETTER = "a" | "b" | "c" | ... | "z" | "A" | "B" | ... | "Z" | "_" ;
DIGIT = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
```

Linguagem de programação que pode ser usada para fazer simulações de um caixa eletrônico automático de um mercadinho de um prédio, onde a partir dele, podemos simular como seria o processo de um residente fazer compras no mercadinho do prédio e como seriam guardados os produtos no depósito/armazem do prédio


## Comandos | Sintática | Exemplo
**INICIAR**
```
START OPERATIONS END

INICIAR

Instrução que inicializa o processo de compras da maquininha
```

**PRODUTO**
```
PRODUCT nome_produto ASSIGN NUMBER PRICE EXPRESSAO END_LINE

PRODUTO cookie = 137 PRECO 7;

Registrador que guarda informações sobre um determinado produto com um QR-Code de preço X
```

**DEPOSITO/CRIAR_DEPOSITO/CONSULTAR DEPOSITO**
```
CREATE_DEPOSIT nome_deposito PROVIDE KEY_OPEN KEY_CLOSE END_LINE

CRIAR_DEPOSITO depositoCondominio7 : {};

Cria depósito onde guardamos produtos e determinamos a quantidade desses produtos que vão estar presentes no depósito/armazém

CONSULT DEPOSIT nome_deposito COLCH_OPEN qr_code COLCH_CLOSE END_LINE

CONSULTAR DEPOSITO depositoCondominio7[137];

Verificamos a quantidade de unidades de um produto QR-Code no depósito

nome_deposito COLCH_OPEN PRODUCT qr_code COLCH_CLOSE PROVIDE INCREASE EXPRESSAO END_LINE

depositoCondominio7 [PRODUTO 137] : ADICIONAR 40;

Adicionamos uma quantidade X unidades de um determinado produto em um depósito já existente
```

**CARRINHO/CRIAR_CARRINHO/CONSULTAR CARRINHO**
```
CRIAR_CARRINHO carrinhoJoao : [];

Cria um carrinho que vamos guardar produtos que existem em um depósito existente

carrinhoJoao ADICIONAR depositoCondominio7 [137] : 7;
Adiciona ao carrinho X unidades de um produto no depósito Y, onde essa quantidade diminui a quantidade guardada no depósito

CONSULTAR CARRINHO carrinhoJoao[137];
CONSULTAR CARRINHO carrinhoJoao;

Podemos consultar ou a quantidade de um produto específico no carrinho ou todos os produtos do carrinho
```

**MENSAGEM**
```
MENSAGEM ("Teste");

Instrução de print de string
```

**VENDER**
```
SELL nome_carrinho COLCH_OPEN qr_code COLCH_CLOSE SCAN EXPRESSAO END_LINE

VENDER carrinhoJoao[137] SCANEAR 3;

Operação que vai executar a compra de um determinado produto presente no carrinho, onde a quantidade X que é scaneada subtrai a quantidade que está na lista
```

**GUARDAR**
```
GUARDAR cond = 7 * 13;

Registrador que usamos como valor auxiliar durante as operações
```

**VERIFICAR/VERIFICAR_CONSTANTEMENTE**
```
VERIFICAR 1 < 2
@@

Verificação de condição que caso seja true, vai executar as operações até chegar em @@

VERIFICAR_CONSTANTEMENTE 1 < 2
$$

Verificação de condição que caso seja true, vai executar as operações até chegar em $$

```

**PAGAMENTO**
```
PAGAMENTO PIX APROVADO;

O pagamento finaliza tudo, se o pagamento foi RECUSADO, cancela as operações, caso seja APROVADO, continua para a etapa de FINALIZAR
```

**FINALIZAR**
```

Finaliza o processo, caso seja usado antes da linha de PAGAMENTO, vai CANCELAR a compra, caso contrário, vai perguntar se vai querer imprimir a nota fiscal
```

## Compilar/Executar
```
make
./mercadinho algum_arquivo.jota

python3 caixa_autoVM.py mercado.asm
```