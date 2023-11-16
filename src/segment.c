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

  //check if the size of program header is correct
  if (sizeof(Elf64_Phdr) != bin->elf_header->e_phentsize) {
    printf("ERROR: Wrong size of program header\n");
    return 1;
  }
  //push all program header into a linked list
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

int32_t is_dyn_segment_64(const void *segment) {
  if (((Elf64_Phdr *)segment)->p_type == PT_DYNAMIC) {
    return 1;
  }
  return 0;
}

void print_dyn_tag(const Elf64_Dyn *tag);


int check_relocations_presence(const t_bin *bin) {
  const Elf64_Phdr *dyn_header = get_segment(bin->phdrs, is_dyn_segment_64);
  if (dyn_header == NULL)
    return 1;
  const Elf64_Phdr *text_segment = get_segment(bin->phdrs, is_text_segment_64);
  const Elf64_Dyn *rela = NULL;
  const Elf64_Dyn *relasz = NULL;
  const Elf64_Dyn *relaent = NULL;
  //copy all dyn_structure into arr till DT_NULL
  for (uint64_t idx = 0; idx < dyn_header->p_filesz / sizeof(Elf64_Dyn); idx++) {
    const Elf64_Dyn *tmp = (Elf64_Dyn *) (bin->raw_data + dyn_header->p_offset + idx * sizeof(Elf64_Dyn));
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


void print_dyn_tag(const Elf64_Dyn *tag) {
  printf("tag->d_tag = 0x%lx : ", tag->d_tag);
    switch (tag->d_tag) {
      case DT_NULL:
        printf("DT_NULL\n");
      break;
      case DT_NEEDED:
        printf("DT_NEEDED\n");
      break;
      case DT_PLTRELSZ:
        printf("DT_PLTRELSZ\n");
      break;
      case DT_PLTGOT:
        printf("DT_PLTGOT\n");
      break;
      case DT_HASH:
        printf("DT_HASH\n");
      break;
      case DT_STRTAB:
        printf("DT_STRTAB\n");
      break;
      case DT_SYMTAB:
        printf("DT_SYMTAB\n");
      break;
      case DT_RELA:
        printf("DT_RELA\n");
      break;
      case DT_RELASZ:
        printf("DT_RELASZ\n");
      break;
      case DT_RELAENT:
        printf("DT_RELAENT\n");
      break;
      case DT_STRSZ:
        printf("DT_STRSZ\n");
      break;
      case DT_SYMENT:
        printf("DT_SYMENT\n");
      break;
      case DT_INIT:
        printf("DT_INIT\n");
      break;
      case DT_FINI:
        printf("DT_FINI\n");
      break;
      case DT_SONAME:
        printf("DT_SONAME\n");
      break;
      case DT_RPATH:
        printf("DT_RPATH\n");
      break;
      case DT_SYMBOLIC:
        printf("DT_SYMBOLIC\n");
      break;
      case DT_REL:
        printf("DT_REL\n");
      break;
      case DT_RELSZ:
        printf("DT_RELSZ\n");
      break;
      case DT_RELENT:
        printf("DT_RELENT\n");
      break;
      case DT_INIT_ARRAY:
        printf("DT_INIT_ARRAY\n");
      break;
      case DT_FINI_ARRAY:
        printf("DT_FINI_ARRAY\n");
      break;
      case DT_INIT_ARRAYSZ:
        printf("DT_INIT_ARRAYSZ\n");
      break;
      case DT_FINI_ARRAYSZ:
        printf("DT_FINI_ARRAYSZ\n");
      break;
      case DT_PLTREL:
        printf("DT_PLTREL\n");
      break;
      case DT_DEBUG:
        printf("DT_DEBUG\n");
      break;
      case DT_JMPREL:
        printf("DT_JMPREL\n");
      break;
      default:
        printf("UNKNOWN\n");
    }
    printf("tag.d_un = 0x%lx\n", tag->d_un.d_ptr);
}