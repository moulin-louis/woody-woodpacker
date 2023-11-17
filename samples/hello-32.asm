; in `elk/samples/hello-pie.asm`
    global _start

section .text

_start:
    mov ebx, 1          ; stdout fd
    mov ecx, msg        ;load msg
    mov edx, 0x08       ; 8 chars + newline
    mov eax, 0x04       ; write syscall
    int 0x80            ; syscall interupt

    xor ebx, ebx        ; return code 0
    mov eax, 0x01       ; exit syscall
    int 0x80            ; syscall interput

section .data

msg:    db "hi there", 9
