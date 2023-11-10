section .text
    global _start
woody:     db `....WOODY....\n`, 14
_start:
	mov rax, 1 ; load write syscall
    mov rdi, 1 ; load stdout fd
    lea rsi, [rel woody] ; load buff
    mov rdx, 14 ; load len of 13
	syscall ; call syscall loaded in rax
    lea rsi, [rel woody]     ; load key (replace 0x1 during execution of woody)
    mov rdi, 0x1 ; load key_len (replace 0x1 during execution of woody)
    lea rdx, [rel woody] ; load data (replace 0x1 during execution of woody)
    mov rcx, 0x1 ; load data_len (replace 0x1 during execution of woody)
    jmp 0x1 ; jump to decrypt function (replace 0x1 during execution of woody)
    jmp 0x1 ; jump to OG entry point (replace 0x1 during execution of woody)