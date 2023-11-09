section .text
    push rdi
    push rsi
    sub rsp, 15
    mov byte [rsp+0], 46
    mov byte [rsp+1], 46
    mov byte [rsp+2], 46
    mov byte [rsp+3], 46
    mov byte [rsp+4], 87
    mov byte [rsp+5], 79
    mov byte [rsp+6], 79
    mov byte [rsp+7], 68
    mov byte [rsp+8], 89
    mov byte [rsp+9], 46
    mov byte [rsp+10], 46
    mov byte [rsp+11], 46
    mov byte [rsp+12], 46
    mov rax, 1 ; load write syscall
    mov rdi, 1 ; load stdout fd
    mov rdx, 13 ; load len of 13
    syscall
    add rsp, 15
    mov rsi, 0x1 ; load key (replace 0x1 during execution of woody)
    mov rdi, 0x1 ; load key_len (replace 0x1 during execution of woody)
    mov rdx, 0x1 ; load data (replace 0x1 during execution of woody)
    mov rcx, 0x1 ; load data_len (replace 0x1 during execution of woody)
    jmp 0x1 ; jump to decrypt function (replace 0x1 during execution of woody)
    pop rdi
    pop rsi
    jmp 0x1 ; jump to OG entry point (replace 0x1 during execution of woody)
    ret