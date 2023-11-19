//
// Created by loumouli on 11/10/23.
//

#include "woody.h"

//push back a new node representing a segment into a linked list
void push_back_phdrs_64(phdr_list_64_t** head, Elf64_Phdr* phdr) {
  phdr_list_64_t* new = malloc(sizeof(phdr_list_64_t));
  if (new == NULL)
    return;
  memset(new, 0, sizeof(*new));
  new->program_header = phdr;
  if (*head == NULL) {
    *head = new;
    return;
  }
  phdr_list_64_t* tmp = *head;
  while (tmp->next != NULL) {
    tmp = tmp->next;
  }
  tmp->next = new;
}

void push_back_phdrs_32(phdr_list_32_t** head, Elf32_Phdr* phdr) {
  phdr_list_32_t* new = malloc(sizeof(phdr_list_32_t));
  if (new == NULL)
    return;
  memset(new, 0, sizeof(*new));
  new->program_header = phdr;
  if (*head == NULL) {
    *head = new;
    return;
  }
  phdr_list_32_t* tmp = *head;
  while (tmp->next != NULL) {
    tmp = tmp->next;
  }
  tmp->next = new;
}

//parse all program headers and push them into a linked list
int32_t parse_program_headers_64(t_bin* bin) {
  size_t curr_offset = bin->elf64_header->e_phoff;

  //check if the size of program header is correct
  if (sizeof(Elf64_Phdr) != bin->elf64_header->e_phentsize) {
    fprintf(stderr, "ERROR: Wrong size of program header\n");
    return 1;
  }
  //push all program header into a linked list
  for (uint16_t idx = 0; idx != bin->elf64_header->e_phnum; idx++) {
    curr_offset += bin->elf64_header->e_phentsize;
    push_back_phdrs_64(&bin->phdrs_64, (Elf64_Phdr *)(bin->raw_data + curr_offset));
  }
  return 0;
}

int32_t parse_program_headers_32(t_bin* bin) {
  size_t curr_offset = bin->elf32_header->e_phoff;

  //check if the size of program header is correct
  if (sizeof(Elf32_Phdr) != bin->elf32_header->e_phentsize) {
    fprintf(stderr, "ERROR: Wrong size of program header\n");
    return 1;
  }
  //push all program header into a linked list
  for (uint16_t idx = 0; idx != bin->elf32_header->e_phnum; idx++) {
    curr_offset += bin->elf32_header->e_phentsize;
    push_back_phdrs_32(&bin->phdrs_32, (Elf32_Phdr *)(bin->raw_data + curr_offset));
  }
  return 0;
}

//get a segment from a linked list of segment using a callback function
void* get_segment_64(const phdr_list_64_t* head, int32_t (*callback)(const Elf64_Phdr*)) {
  const phdr_list_64_t* tmp = head;
  while (tmp) {
    if (callback(tmp->program_header))
      return tmp->program_header;
    tmp = tmp->next;
  }
  return NULL;
}

void* get_segment_32(const phdr_list_32_t* head, int32_t (*callback)(const Elf32_Phdr*)) {
  const phdr_list_32_t* tmp = head;
  while (tmp) {
    if (callback(tmp->program_header))
      return tmp->program_header;
    tmp = tmp->next;
  }
  return NULL;
}

//check if a given segment is a text segment
int32_t is_text_segment_64(const Elf64_Phdr* segment) {
  if (segment->p_type == PT_LOAD && segment->p_flags & PF_X) {
    return 1;
  }
  return 0;
}

int32_t is_text_segment_32(const Elf32_Phdr* segment) {
  if (segment->p_type == PT_LOAD && segment->p_flags & PF_X) {
    return 1;
  }
  return 0;
}

//check if a given segment is a dynamic segment
int32_t is_dyn_segment_64(const Elf64_Phdr* segment) {
  if (segment->p_type == PT_DYNAMIC) {
    return 1;
  }
  return 0;
}

int32_t is_dyn_segment_32(const Elf32_Phdr* segment) {
  if (segment->p_type == PT_DYNAMIC) {
    return 1;
  }
  return 0;
}
