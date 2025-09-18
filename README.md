# Caixa Eletrônico Automático
```ebnf
CAIXA_AUTO = CARRINHO | "CANCELAR COMPRA" ;

CARRINHO = ( LISTA_PRODUTOS, { LISTA_PRODUTOS }, PRECO_TOTAL ), PAGAMENTO ;

PAGAMENTO = "VOLTAR" | (("CREDITO" | "DEBITO" | "PIX") , CARTAO) ;

CARTAO = "VOLTAR" | BANCO ;

BANCO = "APROVADO" | "RECUSADO" ;

LISTA_PRODUTOS = "(", PRODUTO_QR, { ("+" | "-" ) }, ")";
PRODUTO_QR = DIGIT , { DIGIT } ;
PRECO_TOTAL = DIGIT , { DIGIT } ;
DIGIT  = "0" | "..." | "9" ;
CHARACTER = "a" | "..." | "z" | "A" | "..." | "Z" ;
```

```
// Os valores de cada produto são guardados em um dicionário

Exemplo de entrada
(10++)  (7) (9-) (101+++) 1023 CREDITO

LISTA_PRODUTOS =
1. [(10)]
2. [(10, +)]
3. [(10, +, +)]
4. [(10, +, +), (7)]
5. [(10, +, +), (7), (9)]
6. [(10, +, +), (7), (9-)]
7. [(10, +, +), (7), (9-), (100)]
8. [(10, +, +), (7), (9-), (100+)]
9. [(10, +, +), (7), (9-), (100++)]
10. [(10, +, +), (7), (9-), (100+++)]

preco_total = 8 // Valor dado pela quantidade de produtos(Isso apenas para esse exemplo)

PAGAMENTO = (CREDITO, APROVADO)
// O banco faz a aprovação dependendo do saldo do cartão e o preco total obtido
```

Essa EBNF representa a linguagem de um caixa eletrônico automático de um mercadinho de um prédio, onde a partir dele, os residentes podem comprar os produtos ao passar os QR code dos produtos e realizar o pagamento

O usuário pode CANCELAR ou continuar com a compra do CARRINHO

Quando o programa recebe "+", ele aumenta a quantidade do PRODUTO_QR atribuido 

Quando o programa recebe "-", ele diminui a quantidade do PRODUTO_QR atribuido

Quando o programa recebe "NEW", ele adiciona o PRODUTO_QR na lista como um novo produto

Depois, terminando de adicionar/remover os itens(PRODUTO_QR's) da LISTA_PRODUTOS, ele vai mostrar o PRECO_TOTAL resultante de todos os produtos na lista e ir para o PAGAMENTO

Ao pagar, o usuário pode VOLTAR para fazer alterações no CARRINHO ou continuar escolhendo a forma de pagamento e por fim, seguir para o CARTAO para finalizar a compra

No CARTAO, o usuário pode decidir VOLTAR para mudar a forma de pagamento ou continuar informando a senha e verificando com o BANCO se o cartão e a senha são válidos

O BANCO vai verificar a partir das informações de um cartão e os valores recebidos se o pagamento seria válido ou não
