; in samples/hello-pie.asm
global _start

section .text
_start: mov rdi, 1      ; stdout fd
        lea rsi, [rel msg]
        mov rdx, 9      ; 8 chars + newline
        mov rax, 1      ; write syscall
        syscall

        mov rdi, 0
        mov rsi, [rel buf]
        mov rdx, 1
        mov rax, 0
        syscall

        xor rdi, rdi    ; return code 0
        mov rax, 60     ; exit syscall
        syscall

section .data
msg:    db "hi there", 10

section .bss
buf: resb 128