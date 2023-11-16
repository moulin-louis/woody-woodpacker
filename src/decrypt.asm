section .text
    global decrypt ; export decrypt symbol
;    global _start
;decrypt: ; void decrypt(void *key[rdi], size_t len_key[rsi], void *data[rdx], size_t len_data[rcx])
decrypt:
    push r8
    mov r9, rdi ; put key in r9
    mov rdi, rdx ; put data in rdi
    mov r8, rcx ; put len_data in r8
    mov ecx, 0x0 ; put 0 in ecx
start_over:
    mov rax, rcx ; move rcx inside rax
    mov edx, 0x0 ; put 0 in rdx
    div rsi ; div rsi by rax
    movzx eax,rcx ; i % len_key
    xor byte [rdi + rcx], al ; data[i] ^= al
    add rcx, 0x1 ; increment rcx
    cmp r8, rcx  ; check if we decrypt all data
    jne start_over ; start over if not
;    ret
    pop r8 ; pop back value of r8
    jmp r8 ; jump to r8
