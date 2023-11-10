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

#define PAYLOAD_OFFSET_ENTRY 15

#define PAYLOAD_OFFSET_KEY 0x2a
#define PAYLOAD_OFFSET_KEY_LEN 0x2f

#define PAYLOAD_OFFSET_DATA 0x36
#define PAYLOAD_OFFSET_DATA_LEN 0x3b

#define PAYLOAD_OFFSET_DECRYPT_FN 0x40
#define PAYLOAD_OFFSET_OG_ENTRY 0x45

typedef struct phdr_s {
  Elf64_Phdr *program_header;
  struct phdr_s *next;
} phdr_list_t;

typedef struct {
  uint8_t *raw_data;
  size_t data_len;
  uint8_t *payload;
  size_t len_payload;
  uint8_t *key;
  size_t len_key;
  Elf64_Ehdr *elf_header;
  phdr_list_t *phdrs;
} t_bin;

#define ALIGN_DOWN(x, align) ((x) & ~(align - 1))
#define ALIGN_UP(x, align) ALIGN_DOWN((x) + (align) - 1, (align)) ? ALIGN_DOWN((x) + (align) - 1, (align)) : (align)

#include "functions.h"



#endif //WOODY_WOODPACKER_WOODY_H
