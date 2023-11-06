//
// Created by louis on 11/2/23.
//

#include "woody.h"

void lst_add_back_program_header(t_bin *bin, segment_header_t **head, segment_header_t *data) {
  if (*head == NULL) {
    *head = calloc(1, sizeof(segment_header_t));
    memcpy(*head, data, bin->header.e_phentsize);
    return;
  }
  segment_header_t *tmp = *head;
  while (tmp->next) {
    tmp = tmp->next;
  }
  tmp->next = calloc(1, sizeof(segment_header_t));
  memcpy(tmp->next, data, bin->header.e_phentsize);
}

int parse_program_headers(t_bin *bin) {
  size_t curr_offset = bin  ->header.e_phoff;

  if (SIZE_OF_PROGRAM_HEADER != bin->header.e_phentsize) {
    printf("ERROR: Wrong size of program header\n");
    return 1;
  }
  for (uint idx = 0; idx != bin->header.e_phnum; idx++) {
    segment_header_t tmp = {0};
    memcpy(&tmp, bin->raw_data + curr_offset, SIZE_OF_PROGRAM_HEADER);
    lst_add_back_program_header(bin, &bin->program_headers, &tmp);
    curr_offset += bin->header.e_phentsize;
  }
  return 0;
}

void print_program_headers(segment_header_t *head) {
  printf("\nPARSING AND PRINTING ALL PROGRAM HEADERS\n\n");
  for (segment_header_t *tmp = head; tmp != NULL; tmp = tmp->next) {
    if (tmp->p_type != PT_DYNAMIC)
      continue;
    print_program_header(tmp);
    printf("\n");
  }
}
void print_program_header(segment_header_t *programHeader) {
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