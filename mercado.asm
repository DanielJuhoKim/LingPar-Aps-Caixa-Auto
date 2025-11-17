; ========== MERCADINHO DO SEU PRÉDINHO - CÓDIGO ASSEMBLY ==========
; Arquivo gerado automaticamente pelo parser
; ============================================================

section .text
global _start
_start:
    ; Início da execução do mercado
    ; Mensagem: ""Teste simples""
    ; Definir produto: presuntoSadia, QR: 137, Preço: 1.00
    ; Criar depósito: depositoCondominio7
    ; Adicionar ao depósito depositoCondominio7: produto 137, quantidade 300
    ; Consultar depósito depositoCondominio7[137]
    ; Criar carrinho: carrinhoJoao
    ; Guardar variável: count = 0.00
    ; Mensagem: ""Começo do WHile""
    ; === INÍCIO DO LOOP WHILE ===
    ; Labels: 0 (start), 1 (end)
LABEL_0:
    ; Verificar condição do while
    ; Consultar variável count
    ; Condição: se falsa, pular para LABEL_0
    ; Comparação: count < 7
    mov eax, [count]
    cmp eax, 7
    jge LABEL_0
    ; Adicionar ao carrinho carrinhoJoao: produto 137 do depósito depositoCondominio7, quantidade 1
    ; Consultar variável count
    ; Atribuir variável: count = 1.00
    ; Consultar variável: count
    ; Voltar para verificar condição novamente
    jmp LABEL_0
LABEL_0:
    ; === FIM DO LOOP WHILE ===
    ; Mensagem: ""Fim do While""
    ; Consultar carrinho carrinhoJoao[137]
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
