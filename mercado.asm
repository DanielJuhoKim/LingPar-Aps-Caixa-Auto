; ========== MERCADINHO DO SEU PRÉDINHO - CÓDIGO ASSEMBLY ==========
; Arquivo gerado automaticamente pelo parser
; ============================================================

section .text
global _start
_start:
    ; Início da execução do mercado
    ; Mensagem: ""Teste correto""
    ; Definir produto: salgados, QR: 137, Preço: 15.00
    ; Criar depósito: depositoCondominio7
    ; Adicionar ao depósito depositoCondominio7: produto 137, quantidade 40
    ; Consultar depósito depositoCondominio7[137]
    ; Criar carrinho: carrinhoJoao
    ; Adicionar ao carrinho carrinhoJoao: produto 137 do depósito depositoCondominio7, quantidade 7
    ; Consultar carrinho carrinhoJoao[137]
    ; Vender do carrinho carrinhoJoao: produto 137, quantidade 3
    ; Pagamento: CRÉDITO - APROVADO

; ========== FIM DO PROGRAMA ==========
    ; Finalização do sistema
    call mostrar_resumo
    mov eax, 1
    xor ebx, ebx
    int 0x80

; ========== FUNÇÕES DO SISTEMA ==========
mostrar_resumo:
    ; TODO: Implementar resumo do sistema
    ret
