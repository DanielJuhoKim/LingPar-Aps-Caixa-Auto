; ========== MERCADINHO DO SEU PRÉDINHO - CÓDIGO ASSEMBLY ==========
; Arquivo gerado automaticamente pelo parser
; ============================================================

section .text
global _start
_start:
    ; Início da execução do mercado
    ; Mensagem: ""Teste Matemático""
    ; Guardar variável: x = 7.00
    ; Guardar variável: y = 3.00
    ; Guardar variável: z = 10.00
    ; Consultar variável: z

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
