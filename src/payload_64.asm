section .text
    global _start
woody:     db `....WOODY....\n`, 15
_start:
	push rax
	push rcx
	push rdx
	push rbx
	push rsp
	push rbp
	push rsi
	push rdi
	mov rax, 1 ; load write syscall
    mov rdi, 1 ; load stdout fd
    lea rsi, [rel woody] ; load buff
    mov rdx, 14 ; load len of 13
	syscall ; call syscall loaded in rax
    lea rdi, [rel after_decrypt] ; load key (replace 0x1 during execution of woody)
    mov rcx, 0x10 ; load key_len (replace 0x1 during execution of woody)
    lea rsi, [rel after_decrypt] ; load data (replace 0x1 during execution of woody)
    mov rdx, 0x1 ; load data_len (replace 0x1 during execution of woody)
    lea r8, [rel after_decrypt]
    jmp 0x1 ; jump to decrypt function (replace 0x1 during execution of woody)
after_decrypt:
	pop rdi
	pop rsi
	pop rbp
	pop rsp
	pop rbx
	pop rdx
	pop rcx
	pop rax
    jmp 0x1 ; jump to OG entry point (replace 0x1 during execution of woody)