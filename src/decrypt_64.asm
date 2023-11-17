section .text
;    global decrypt_64 ; export decrypt symbol
    global _start
;decrypt_64: ; void decrypt_64(void *key[$rdi], size_t len_key[$rsi], void *data[$rdx], size_t len_data[$rcx])
_start:
    mov r9, 0x0
start_over:
    mov rax, r9
    push rdx
    mov rdx, 0x0
    div rsi
    movzx eax, byte [rdi+rdx]
    pop rdx
    xor byte [rdx + r9], al
    inc r9
    cmp rcx, r9
    jne start_over
    jmp r8