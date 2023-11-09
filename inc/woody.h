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

#define PAYLOAD "\x48\x83\xEC\x0F\xC6\x04\x24\x2E\xC6\x44\x24\x01\x2E\xC6\x44\x24\x02\x2E\xC6\x44\x0E\x03\x2E\xC6\x44\x24\x04\x57\xC6\x44\x24\x05\x4F\xC6\x44\x24\x06\x4F\xC6\x44\x24\x07\x44\xC6\x44\x24\x08\x59\xC6\x44\x24\x09\x2E\xC6\x44\x24\x0A\x2E\xC6\x44\x24\x0B\x2E\xC6\x44\x24\x0C\x2E\xB8\x01\x00\x00\x00\xBF\x01\x00\x00\x00\x48\x89\xE6\xBA\x0D\x00\x00\x00\x0F\x05"

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
