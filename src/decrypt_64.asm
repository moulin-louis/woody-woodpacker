section .text
;    global decrypt_64 ; export decrypt symbol
;    global decrypt_32
    global _start
;decrypt_64: ; void decrypt_64(void *key[$rdi], size_t len_key[$rsi], void *data[$rdx], size_t len_data[$rcx])
;    push r8
;    mov r9, rdi ; put key in r9
;    mov rdi, rdx ; put data in rdi
;    mov r8, rcx ; put len_data in r8
;    mov ecx, 0x0 ; put 0 in ecx
;start_over:
;    mov rax, rcx ; move rcx inside rax
;    mov edx, 0x0 ; put 0 in rdx
;    div rsi ; div rsi by rax
;    movzx eax,rcx ; i % len_key
;    xor byte [rdi + rcx], al ; data[i] ^= al
;    add rcx, 0x1 ; increment rcx
;    cmp r8, rcx  ; check if we decrypt all data
;    jne start_over ; start over if not
;    pop r8 ; pop back value of r8
;    jmp r8 ; jump to r8

;decrypt_32: ; void decrypt_32(void *key, size_t len_key, void *data, size_t len_data)
_start:
    push esi ; push esi into the stack
    push edi ; push edi into the stack
    push ecx ; push ecx into the stack
    mov esi, [esp + 12]  ; esi = key
    mov ecx, [esp + 20]  ; ecx = len_data
    mov edi, [esp + 16]  ; edi = data
    xor eax, eax         ; clear eax
start_over:
    mov edx, eax ; mov eax into eax
    div dword [esp + 20] ; divide eax by len_key, result in eax, remainder in edx
    xor byte [edi + eax], dl ; data[i] ^= dl
    inc eax ; increment eax
    cmp ecx, eax ; check if we decrypt all data
    jne start_over ; start over if not
    pop ecx ; pop back old value of ecx
    pop edi ; pop back old value of edi
    pop esi ; pop back old value of esi
    jmp ebx