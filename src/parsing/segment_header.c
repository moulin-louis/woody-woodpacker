//
// Created by louis on 11/2/23.
//

#include "woody.h"

char *type_program_to_str(Elf64_Word type) {
  switch (type) {
    case PT_NULL:
      return "PT_NULL";
    case PT_LOAD:
      return "PT_LOAD";
    case PT_DYNAMIC:
      return "PT_DYNAMIC";
    case PT_INTERP:
      return "PT_INTERP";
    case PT_NOTE:
      return "PT_NOTE";
    case PT_SHLIB:
      return "PT_SHLIB";
    case PT_PHDR:
      return "PT_PHDR";
    case PT_TLS:
      return "PT_TLS";
    case PT_NUM:
      return "PT_NUM";
    case PT_LOOS:
      return "PT_LOOS";
    case PT_GNU_EH_FRAME:
      return "PT_GNU_EH_FRAME";
    case PT_GNU_STACK:
      return "PT_GNU_STACK";
    case PT_GNU_RELRO:
      return "PT_GNU_RELRO";
    case 0x6474e553:
      return "PT_LOSUNW";
    default:
      return "Unknown type";
  }
}

void push_back_phdrs(phdr_list_t **head, Elf64_Phdr *phdr) {
  phdr_list_t *new = malloc(sizeof(phdr_list_t));
  memcpy(&new->program_header, phdr, sizeof(Elf64_Phdr));
  new->next = NULL;
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

int parse_program_headers(t_bin *bin) {
  size_t curr_offset = bin->elf_header.e_phoff;

  if (sizeof(Elf64_Phdr) != bin->elf_header.e_phentsize) {
    printf("ERROR: Wrong size of program header\n");
    return 1;
  }
  for (uint idx = 0; idx != bin->elf_header.e_phnum; idx++) {
    Elf64_Phdr tmp = {0};
    memcpy(&tmp, bin->raw_data + curr_offset,  sizeof(tmp));
    curr_offset += bin->elf_header.e_phentsize;
    push_back_phdrs(&bin->phdrs, &tmp);
  }
  return 0;
}

__attribute__((unused)) void print_program_headers(phdr_list_t *head) {
  for (phdr_list_t *node = head; node; node = node->next) {

    printf("Type: 0x%08x - ", node->program_header.p_type);
    printf("%s\n", type_program_to_str(node->program_header.p_type));
    printf("Flags: 0x%08x - ", node->program_header.p_flags);
    if (node->program_header.p_flags & (1 << 2)) {
      printf("PF_R ");
    }
    if (node->program_header.p_flags & (1 << 1)) {
      printf("PF_W ");
    }
    if (node->program_header.p_flags & (1 << 0)) {
      printf("PF_X ");
    }
    printf("\n");
    printf("Offset: 0x%016lx\n", node->program_header.p_offset);
    printf("Virtual address: 0x%016lx\n", node->program_header.p_vaddr);
    printf("Size in file: 0x%016lx\n", node->program_header.p_filesz);
    printf("Size in memory: 0x%016lx\n", node->program_header.p_memsz);
    printf("Alignment: 0x%016lx\n", node->program_header.p_align);
    printf("\n");
  }
}