#include "woody.h"

uint64_t base_address;

int init(t_bin *bin, char **av) {
  int file = open(av[1], O_RDONLY);
  if (file == -1) {
    printf("Error opening file\n");
    return 2;
  }
  if (read_file(file, (char **) &bin->raw_data, &bin->data_len) != 0) {
    printf("Error reading file\n");
    return 3;
  }
  close(file);
  return 0;
}

//void print_elf_header(Elf64_Ehdr *header) {
//  printf("e_ident: %02x %02x %02x %02x\n", header->e_ident[0], header->e_ident[1], header->e_ident[2], header->e_ident[3]);
//  printf("e_type: %04x\n", header->e_type);
//  printf("e_machine: %04x\n", header->e_machine);
//  printf("e_version: %08x\n", header->e_version);
//  printf("e_entry: %016lx\n", header->e_entry);
//  printf("e_phoff: %016lx\n", header->e_phoff);
//  printf("e_shoff: %016lx\n", header->e_shoff);
//  printf("e_flags: %08x\n", header->e_flags);
//  printf("e_ehsize: %04x\n", header->e_ehsize);
//  printf("e_phentsize: %04x\n", header->e_phentsize);
//  printf("e_phnum: %04x\n", header->e_phnum);
//  printf("e_shentsize: %04x\n", header->e_shentsize);
//  printf("e_shnum: %04x (%d)\n", header->e_shnum, header->e_shnum);
//}

int main(int ac, char **av) {
  t_bin bin = {};

//  base_address = BASE_ADDRESS;
  if (ac != 2) {
    printf("Wrong usage\n");
    return 1;
  }
  if (init(&bin, av)) {
    printf("Error init\n");
    return 1;
  }
  memcpy(&bin.elf_header, bin.raw_data, sizeof(Elf64_Ehdr));
  if (memcmp((void *)&bin.elf_header, "\x7F" "ELF", 4) != 0) {
    printf("This is not an ELF file\n");
    return 4;
  }
  print_elf_header(&bin.elf_header);
  if (parse_program_headers(&bin)) {
    printf("Error parsing program headers\n");
    return 5;
  }
  print_program_headers(bin.phdrs);
  for (phdr_list_t *node = bin.phdrs; node; node = node->next) {
    printf("HEXDUMP: \n");
    hexdump(bin.raw_data + node->program_header.p_offset, node->program_header.p_filesz, 16);
  }
//  parse_dynamic_segment(&bin);
//  making_relocations(&bin);
//  print_elf_header(&bin.header);
//  print_program_headers(bin.program_header);
//  print_dynamic_segments(bin.dynamic_segment);
//  if (second_stage(&bin)) {
//    printf("Error in second stage\n");
//  }
//  patch_elf(&bin);
  return 0;
}