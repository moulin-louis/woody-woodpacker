//
// Created by loumouli on 11/10/23.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>

__attribute__((weak)) void decrypt(void *key, size_t len_key, void *data, size_t len_data) {
  for (size_t i = 0; i < len_data; i++) {
    ((char *)data)[i] ^= ((char *)key)[i % len_key];
  }

}

void encrypt(void *key, size_t len_key, void *data, size_t len_data) {
  for (size_t i = 0; i < len_data; i++) {
    ((char *)data)[i] ^= ((char *)key)[i % len_key];
  }
}

int main(int ac, char **av) {
  char *str = av[1];

  write(1, str, strlen(str));
  write(1, "\n", 1);
  encrypt("key", 3, str, sizeof(str) - 1);
  write(1, str, strlen(str));
  write(1, "\n", 1);
  decrypt("key", 3, str, sizeof(str) - 1);
  write(1, str, strlen(str));
  write(1, "\n", 1);
  return 0;
}