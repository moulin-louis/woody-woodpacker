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

int32_t is_dyn_segment_64(const void *segment) {
  if (((Elf64_Phdr *)segment)->p_type == PT_DYNAMIC) {
    return 1;
  }
  return 0;
}

void print_dyn_tag(const Elf64_Dyn *tag);


int check_relocations_presence(const t_bin *bin) {
  const Elf64_Phdr *dyn_header = get_segment(bin->phdrs, is_dyn_segment_64);
  const Elf64_Phdr *text_segment = get_segment(bin->phdrs, is_text_segment_64);
  const Elf64_Dyn *dyn_data = (Elf64_Dyn *) (bin->raw_data + dyn_header->p_offset);
  bool found_rel_tag = false;
  //printf info dyn segment
  for (uint64_t idx = 0; idx < dyn_header->p_filesz / sizeof(Elf64_Dyn); idx++) {
    if (dyn_data[idx].d_tag == DT_RELA) {
      found_rel_tag = true;
      const Elf64_Rela *rela = (Elf64_Rela *) (bin->raw_data + dyn_data[idx].d_un.d_ptr);
      for (uint64_t idx_rela = 0; idx_rela < dyn_data[idx + 1].d_un.d_val / sizeof(Elf64_Rela); idx_rela++) {
        //check if relocation is inside the executable segment
        printf("Relocation %lu\n", idx_rela);
        printf("r_offset: 0x%016lx\n", rela[idx_rela].r_offset);
        if (rela[idx_rela].r_offset >= text_segment->p_vaddr &&
            rela[idx_rela].r_offset <= text_segment->p_vaddr + text_segment->p_memsz) {
          printf("WARNING: Relocation inside the executable segment\n");
          return 1;
        }
        printf("\n");
      }
      break;
    }
  }
  if (found_rel_tag)
    printf( "WARNING: FOUND SOME RELOCATIONS IN THE BINARY\n");
  else
    printf("WARNING: NO RELOCATIONS FOUND IN THE BINARY\n");
  return 0;
}

void print_dyn_tag(const Elf64_Dyn *tag) {
  printf("tag.d_tag = 0x%lx : ", tag->d_tag);
    switch (tag->d_tag) {
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
      default:
        printf("UNKNOWN\n");
    }
    printf("tag.d_un = 0x%lx\n", tag->d_un.d_ptr);
}