//
// Created by louis on 11/6/23.
//

void exit(int exit_code) {
  asm(
      "movq $60, %rax\n\t"
      "movq %rdi, %rdi\n\t"
      "syscall\n\t"
      );
  __builtin_unreachable();
}

void write(char *str, unsigned int len) {
  asm("movq $1, %%rax\n\t"          // The system call for sys_write
      "movq $1, %%rdi\n\t"          // File descriptor 1 is stdout
      "movq %0, %%rsi\n\t"          // Address of the string to output
      "movl %1, %%edx\n\t"          // The number of bytes to write
      "syscall\n\t"                 // Invoke the system call
      :
      : "r"(str), "r"(len)
      : "rax", "rdi", "rsi", "rdx");
}

__attribute__((force_align_arg_pointer))
void _start() {
  char str[] = "Hello World!\n";
  unsigned int len = sizeof(str) - 1;
  write(str, len);
  exit(0);
}
