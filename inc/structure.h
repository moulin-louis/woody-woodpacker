//
// Created by louis on 11/2/23.
//

#ifndef WOODY_WOODPACKER_STRUCTURE_H
#define WOODY_WOODPACKER_STRUCTURE_H

#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct elf_header_s {
  uint8_t magic[4]; //magic number, 0x7F + "ELF"
  uint8_t class; // class: 1(32 bits) or 2(64 bits)
  uint8_t endian; //endianness: 1(little endian) or 2(big endian)
  uint8_t version; // version : always 1
  uint8_t os_abi; // system ABI
  uint8_t _padding[8]; //unused padding
  uint16_t e_type; // object file type: 0x02(executable), 0x03(shared library), etc
  uint16_t e_machine; //target instruction set
  uint32_t e_version; //redundant of version
  uint64_t e_entry; //address of the entry point, 0 if none
  uint64_t e_phoff; // offset to the program header table(very likely to be 0x34 or 0x40)
  uint64_t e_shoff; // offset to the section header table
  uint32_t e_flags; //NA
  uint16_t e_ehsize; //size of the elf header (very likely to be 52 or 64)
  uint16_t e_phentsize; // size of the program header
  uint16_t e_phnum; // number of program header table entry
  uint16_t e_shentsize; // size of the section header
  uint16_t e_shnum; // number of program section table entry
  uint16_t e_shstrndx; // index of the name section in the section header table
} elf_header_t;

typedef enum type_program {
  PT_NULL = 0x0,
  PT_LOAD = 0x1,
  PT_DYNAMIC = 0x2,
  PT_INTERP = 0x3,
  PT_NOTE = 0x4,
  PT_SHLIB = 0x5,
  PT_PHDR = 0x6,
  PT_TLS = 0x7,
  PT_NUM = 0x8,
  PT_LOOS = 0x60000000,
  PT_GNU_EH_FRAME = 0x6474e550,
  PT_GNU_STACK = 0x6474e551,
  PT_GNU_RELRO = 0x6474e552,
  PT_LOSUNW = 0x6ffffffa,
  PT_SUNWSTACK = 0x6ffffffb,
  PT_HIOS = 0x6fffffff,
  PT_LOPROC = 0x70000000,
  PT_HIPROC = 0x7fffffff,
  PT_GNU_PROPERTY = 0x6474e553,
} typeProgram;

typedef struct program_header_s {
  typeProgram p_type; //type segment: 0x1(loadable segment), 0x2(dynamic linking segment), etc
  uint32_t p_flags; // segment dependent flags: 0x1(executable segment), 0x2(writeable segment), 0x3(readable segment)
  uint64_t p_offset; // offset to the segment in the file
  uint64_t p_vaddr; // virtual address of the segment
  uint64_t p_addr; //physical address to map it (VERY UNLIKELY TO BE RELEVANT)
  uint64_t p_filesz; // size of the segment in the file, may be 0
  uint64_t p_memsz; // size of the segment in memory, may be 0
  uint64_t p_align; //alignment of the segment (?), 0 and 1 means no alignment
} program_header_t;

typedef struct section_header_s {
  uint32_t sh_name; //offset in the name section that represent the name of this segment
  uint32_t sh_type; // type of header: 0x1(program raw_data), 0x3(string table), etc
  uint64_t sh_flags; //attributes of the section: 0x1(writable), 0x20(null-terminated string), etc
  uint64_t sh_addr; //virtual addres of the section if loaded
  uint64_t sh_offset; // offset of the section in the file
  uint64_t sh_size; //size of the section in the file, may be 0
  uint32_t sh_link; //may contain section index of an associated section based on the current section type
  uint32_t sh_info; //may contain section info based on the current section type
  uint64_t sh_addralign; //required alignement of the section, must be a power of 2
  uint64_t sh_entsize; // may contain the size of each entry based on the section type
} section_header_t;

typedef struct program_header_list_s {
  program_header_t program_header;
  struct program_header_list_s *next;
} program_header_list_t;

typedef struct section_header_list_s {
  section_header_t section_header;
  struct section_header_list_s *next;
} section_header_list_t;

typedef struct {
  uint8_t *raw_data;
  size_t data_len;
  elf_header_t header;
  program_header_list_t *program_headers;
  section_header_list_t *section_headers;
} t_bin;

#endif //WOODY_WOODPACKER_STRUCTURE_H