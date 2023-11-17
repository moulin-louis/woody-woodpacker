//
// Created by loumouli on 11/10/23.
//

#include "woody.h"

//push back a new node representing a segment into a linked list
void push_back_phdrs(phdr_list_t** head, Elf64_Phdr* phdr) {
  phdr_list_t* new = malloc(sizeof(phdr_list_t));
  if (new == NULL)
    return;
  memset(new, 0, sizeof(*new));
  new->program_header = phdr;
  if (*head == NULL) {
    *head = new;
    return;
  }
  phdr_list_t* tmp = *head;
  while (tmp->next != NULL) {
    tmp = tmp->next;
  }
  tmp->next = new;
}

//parse all program headers and push them into a linked list
int32_t parse_program_headers(t_bin* bin) {
  size_t curr_offset = bin->elf_header->e_phoff;

  //check if the size of program header is correct
  if (sizeof(Elf64_Phdr) != bin->elf_header->e_phentsize) {
    printf("ERROR: Wrong size of program header\n");
    return 1;
  }
  //push all program header into a linked list
  for (uint16_t idx = 0; idx != bin->elf_header->e_phnum; idx++) {
    curr_offset += bin->elf_header->e_phentsize;
    push_back_phdrs(&bin->phdrs, (Elf64_Phdr *)(bin->raw_data + curr_offset));
  }
  return 0;
}

//get a segment from a linked list of segment using a callback function
void* get_segment(const phdr_list_t* head, int32_t (*callback)(const void*)) {
  const phdr_list_t* tmp = head;
  while (tmp) {
    if (callback(tmp->program_header))
      return tmp->program_header;
    tmp = tmp->next;
  }
  return NULL;
}

//check if a given segment is a text segment
int32_t is_text_segment_64(const void* segment) {
  if (((Elf64_Phdr *)segment)->p_type == PT_LOAD && ((Elf64_Phdr *)segment)->p_flags & PF_X) {
    return 1;
  }
  return 0;
}

//check if a given segment is a dynamic segment
int32_t is_dyn_segment_64(const void* segment) {
  if (((Elf64_Phdr *)segment)->p_type == PT_DYNAMIC) {
    return 1;
  }
  return 0;
}

//check if the binary has relocation and if it can cause problem
int check_relocations_presence(const t_bin* bin) {
  const Elf64_Phdr* dyn_header = get_segment(bin->phdrs, is_dyn_segment_64);
  if (dyn_header == NULL)
    return 1;
  const Elf64_Phdr* text_segment = get_segment(bin->phdrs, is_text_segment_64);
  const Elf64_Dyn* rela = NULL;
  const Elf64_Dyn* relasz = NULL;
  const Elf64_Dyn* relaent = NULL;
  //copy all dyn_structure into arr till DT_NULL
  for (uint64_t idx = 0; idx < dyn_header->p_filesz / sizeof(Elf64_Dyn); idx++) {
    const Elf64_Dyn* tmp = (Elf64_Dyn *)(bin->raw_data + dyn_header->p_offset + idx * sizeof(Elf64_Dyn));
    if (tmp->d_tag == DT_RELA)
      rela = tmp;
    else if (tmp->d_tag == DT_RELASZ)
      relasz = tmp;
    else if (tmp->d_tag == DT_RELAENT)
      relaent = tmp;
    else if (tmp->d_tag == DT_NULL)
      break;
  }
  if (rela == NULL || relasz == NULL || relaent == NULL) {
    printf(ANSI_GREEN "LOG: No RELOCATIONS found in the binary\n");
    return 0;
  }
  printf(ANSI_YELLOW "WARNING: Found RELOCATIONS in the binary\n");
  //checking if relocation can cause problem
  printf(ANSI_GREEN "LOG: Checking if RELOCATIONS can cause problem....: ");
  if (rela->d_un.d_ptr >= text_segment->p_vaddr && rela->d_un.d_ptr <= text_segment->p_vaddr + text_segment->p_memsz) {
    printf(ANSI_RED ANSI_CROSS "\n");
    printf(ANSI_RED "ERROR: RELOCATIONS in the text segment\n" ANSI_RESET);
    return 1;
  }
  printf(ANSI_GREEN ANSI_CHECK "\n");
  printf(ANSI_GREEN "LOG: No RELOCATIONS in the text segment\n" ANSI_RESET);
  return 0;
}

