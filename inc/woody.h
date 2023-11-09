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

#define BASE_ADDRESS 0x400000
#define KEY_SIZE 32
#define SHELLCODE_SIZE 256

typedef struct phdr_s {
  Elf64_Phdr program_header;
  struct phdr_s *next;
} phdr_list_t;

typedef struct {
  uint8_t *raw_data;
  size_t data_len;
  Elf64_Ehdr elf_header;
  phdr_list_t *phdrs;
} t_bin;

#include "functions.h"

extern uint64_t base_address;

#endif //WOODY_WOODPACKER_WOODY_H
