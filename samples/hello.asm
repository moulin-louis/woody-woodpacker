; in samples/hello-pie.asm
global _start

section .text
_start: mov rdi, 1      ; stdout fd
        lea rsi, [rel msg]
        mov rdx, 9      ; 8 chars + newline
        mov rax, 1      ; write syscall
        syscall

        mov rdi, 0      ; stdin fd
        lea rsi, [rel buf] ; loading buf into rsi
        mov rdx, 1 ; reading only 1 bytes
        mov rax, 0 ; read syscall
        syscall

        xor rdi, rdi    ; return code 0
        mov rax, 60     ; exit syscall
        syscall

section .data
    msg:    db "hi there", 10

section .bss
    buf: resb 128