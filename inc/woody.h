//
// Created by louis on 11/1/23.
//

#ifndef WOODY_WOODPACKER_WOODY_H
#define WOODY_WOODPACKER_WOODY_H

#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include <stdbool.h>

#define KEY_SIZE 16
#define OFFSET_ENTRY 0x0F

//64 BITS
#define OFFSET_KEY_64 0x2a
#define OFFSET_KEY_LEN_64 0x2f

#define OFFSET_DATA_64 0x36
#define OFFSET_DATA_LEN_64 0x3b

#define OFFSET_DECRYPT_FN_64 0x47
#define OFFSET_OG_ENTRY_64 0x4c

//32 BITS

#define OFFSET_KEY_32 0x39
#define OFFSET_KEY_LEN_32 0x3B

#define OFFSET_DATA_32 0x47
#define OFFSET_DATA_LEN_32 0x4C

#define OFFSET_DECRYPT_FN_32 0x56
#define OFFSET_OG_ENTRY_32 0x63

typedef struct phdr_64_s {
  Elf64_Phdr* program_header;
  struct phdr_64_s* next;
} phdr_list_64_t;

typedef struct phdr_32_s {
  Elf32_Phdr* program_header;
  struct phdr_32_s* next;
} phdr_list_32_t;

typedef struct {
  uint8_t* raw_data;
  size_t data_len;
  uint8_t* payload;
  size_t len_payload;
  uint8_t* key;
  size_t len_key;
  //64 bits
  Elf64_Ehdr* elf64_header;
  phdr_list_64_t* phdrs_64;
  //32 bits
  Elf32_Ehdr* elf32_header;
  phdr_list_32_t* phdrs_32;
} t_bin;


#define ALIGN_DOWN(x, align) ((x) & ~(align - 1))
#define ALIGN_UP(x, align) ALIGN_DOWN((x) + (align) - 1, (align)) ? ALIGN_DOWN((x) + (align) - 1, (align)) : (align)

//define red/green/yellow and reset ansi color
#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_RESET   "\x1b[0m"
//define emoji
#define ANSI_CHECK "\xE2\x9C\x94"
#define ANSI_CROSS "\xE2\x9D\x8C"

//prototype of all functions
#include "functions.h"

#endif //WOODY_WOODPACKER_WOODY_H
