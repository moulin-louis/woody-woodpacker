section .text
;    global decrypt ; export decrypt symbol
    global _start
;decrypt: ; void decrypt(void *key[rdi], size_t len_key[rsi], void *data[rdx], size_t len_data[rcx])
_start:
    push r8
    mov r9, rdi
    mov rdi, rdx
    mov r8, rcx
    mov ecx, 0x0
start_over:
    mov rax, rcx
    mov edx, 0x0
    div rsi
    movzx eax, byte [r9 + rdx]
    xor byte [rdi + rcx], al
    add rcx, 0x1
    cmp r8, rcx
    jne start_over
    ret
    pop r8
    jmp r8
