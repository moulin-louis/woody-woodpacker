//
// Created by louis on 11/3/23.
//

#include <stdio.h>
#include <unistd.h>

int main(int ac, char **av) {
  printf("Hello, World!\n");
  printf("ac: %d\n", ac);
  printf("av: %p\n", av);
  char buf[1];
  ssize_t test = read(0, buf, 1);
  (void)test;
  return 0;
}