//
// Created by louis on 11/2/23.
//

#include "woody.h"

void parse_program_headers(t_bin *bin) {
  size_t curr_offset = bin  ->header.e_phoff;

  if (sizeof(program_header_t) != bin->header.e_phentsize) {
    printf("ERROR: Wrong size of program header\n");
    return;
  }
  for (uint idx = 0; idx != bin->header.e_phnum; idx++) {
    program_header_t tmp = {0};
    memcpy(&tmp, bin->raw_data + curr_offset, sizeof(tmp));
    lst_add_back_program_header(&bin->program_headers, &tmp);
    curr_offset += bin->header.e_phentsize;
  }
}

void print_program_headers(program_header_list_t *head) {
  printf("\nPARSING AND PRINTING ALL PROGRAM HEADERS\n\n");
  size_t nbr_node = 0;
  for (program_header_list_t *tmp = head; tmp != NULL; tmp = tmp->next) {
    print_program_header(&tmp->program_header);
    printf("\n");
    nbr_node +=1;
  }
  printf("There is %ld segment\n", nbr_node);
}

void print_program_header(program_header_t *programHeader) {
  printf("Type: 0x%08x - ", programHeader->p_type);
  printf("%s\n", type_program_to_str(programHeader->p_type));
  printf("Flags: 0x%08x - ", programHeader->p_flags);
  if (programHeader->p_flags & (1 << 2)) {
    printf("PF_R ");
  } if (programHeader->p_flags & (1 << 1)) {
    printf("PF_W ");
  } if (programHeader->p_flags & (1 << 0)) {
    printf("PF_X ");
  }
  printf("\n");
  printf("Offset: 0x%016lx\n", programHeader->p_offset);
  printf("Virtual address: 0x%016lx\n", programHeader->p_vaddr);
  printf("Size in file: 0x%016lx\n", programHeader->p_filesz);
  printf("Size in memory: 0x%016lx\n", programHeader->p_memsz);
  printf("Alignment: 0x%016lx\n", programHeader->p_align);
}