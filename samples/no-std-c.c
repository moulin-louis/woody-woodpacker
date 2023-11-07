//
// Created by louis on 11/6/23.
//

__attribute__((force_align_arg_pointer))
void _start(  ) {
  __asm__("movq $60, %rax\n\t" "movq $42, %rdi\n\t" "syscall\n\t");
  __builtin_unreachable();
}
