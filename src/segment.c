//
// Created by loumouli on 11/10/23.
//

#include "woody.h"

void push_back_phdrs(phdr_list_t **head, Elf64_Phdr *phdr) {
  phdr_list_t *new = calloc(sizeof(phdr_list_t), 1);
  new->program_header = phdr;
  if (*head == NULL) {
    *head = new;
    return;
  }
  phdr_list_t *tmp = *head;
  while (tmp->next != NULL) {
    tmp = tmp->next;
  }
  tmp->next = new;
}

int32_t parse_program_headers(t_bin *bin) {
  size_t curr_offset = bin->elf_header->e_phoff;

  if (sizeof(Elf64_Phdr) != bin->elf_header->e_phentsize) {
    printf("ERROR: Wrong size of program header\n");
    return 1;
  }
  for (uint16_t idx = 0; idx != bin->elf_header->e_phnum; idx++) {
    curr_offset += bin->elf_header->e_phentsize;
    push_back_phdrs(&bin->phdrs, (Elf64_Phdr *) (bin->raw_data + curr_offset));
  }
  return 0;
}

void *get_segment(const phdr_list_t *head, int32_t (*callback)(const void *)) {
  const phdr_list_t *tmp = head;
  while (tmp) {
    if (callback(tmp->program_header))
      return tmp->program_header;
    tmp = tmp->next;
  }
  return NULL;
}

int32_t is_text_segment_64(const void *segment) {
  if (((Elf64_Phdr *) segment)->p_type == PT_LOAD && ((Elf64_Phdr *) segment)->p_flags & PF_X) {
    return 1;
  }
  return 0;
}