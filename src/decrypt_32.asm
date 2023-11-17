section .text
global decrypt_32
global _start
;decrypt_32: ; void decrypt_32(void *key[$edi], size_t len_key[$esi], void *data[$edx], size_t len_data[$ecx)
_start:
    mov ebx, 0x0
start_over:
    mov eax, ebx
    push edx
    mov edx, 0x0
    div esi
    movzx eax, byte [edi + edx]
    pop edx
    xor byte [edx + ebx], al
    inc ebx
    cmp ecx, ebx
    jne start_over
    pop eax
    jmp eax