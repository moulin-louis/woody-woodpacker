//
// Created by louis on 11/1/23.
//

#ifndef WOODY_WOODPACKER_WOODY_H
#define WOODY_WOODPACKER_WOODY_H

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <elf.h>

#define KEY_SIZE 32

// #define PAYLOAD "\x55\x48\x89\xEC\x48\x83\xEC\x0D\xC6\x04\x24\x2E\xC6\x44\x24\x01\x2E\xC6\x44\x24\x02\x2E\xC6\x44\x24\x03\x2E\xC6\x44\x24\x04\x57\xC6\x44\x24\x05\x4F\xC6\x44\x24\x06\x4F\xC6\x44\x24\x07\x44\xC6\x44\x24\x08\x59\xC6\x44\x24\x09\x2E\xC6\x44\x24\x0A\x2E\xC6\x44\x24\x0B\x2E\xC6\x44\x24\x0C\x2E\x48\xC7\xC0\x01\x00\x00\x00\x48\xC7\xC7\x01\x00\x00\x00\x48\x89\xE6\x48\xC7\xC2\x0D\x00\x00\x00\x0F\x05"
// #define PAYLOAD "\x55\x48\x89\xE5\x48\xC7\xC0\x3C\x00\x00\x00\x48\xC7\xC7\x2A\x00\x00\x00\x0F\x05"
// #define PAYLOAD "\x77\x6F\x"

typedef struct phdr_s {
  Elf64_Phdr *program_header;
  struct phdr_s *next;
} phdr_list_t;

typedef struct {
  uint8_t *raw_data;
  size_t data_len;
  Elf64_Ehdr *elf_header;
  phdr_list_t *phdrs;
} t_bin;

#define ALIGN_DOWN(x, align) ((x) & ~(align - 1))
#define ALIGN_UP(x, align) ALIGN_DOWN((x) + (align) - 1, (align)) ? ALIGN_DOWN((x) + (align) - 1, (align)) : (align)

#include "functions.h"

#endif //WOODY_WOODPACKER_WOODY_H
