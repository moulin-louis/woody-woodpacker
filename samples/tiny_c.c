//
// Created by louis on 11/3/23.
//

#include <stdio.h>
#include <unistd.h>

int main(int ac, char **av) {
  printf("Hello, World!\n");
  printf("ac: %d\n", ac);
  printf("av: %p\n", av);
  int x[10];
  return 0;
}