//
// Created by loumouli on 11/17/23.
//

#include "woody.h"

//check if the binary has relocation and if it can cause problem
int32_t check_relocations_presence_64(const t_bin* bin) {
  const Elf64_Phdr* dyn_header = get_segment_64(bin->phdrs_64, is_dyn_segment_64);
  if (dyn_header == NULL)
    return 1;
  const Elf64_Phdr* text_segment = get_segment_64(bin->phdrs_64, is_text_segment_64);
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
  for (uint32_t idx = 0; idx < relasz->d_un.d_val / relaent->d_un.d_val; idx++) {
    if (rela->d_un.d_val + idx * relaent->d_un.d_val >= bin->data_len)
      continue;
    const Elf64_Rela* tmp = (Elf64_Rela *)(bin->raw_data + rela->d_un.d_val + idx * relaent->d_un.d_val);
    const uint64_t addr_rela = tmp->r_offset;
    if (addr_rela >= text_segment->p_vaddr && addr_rela <= text_segment->p_vaddr + text_segment->p_memsz) {
      printf(ANSI_RED ANSI_CROSS "\n");
      fprintf(stderr, ANSI_RED "ERROR: RELOCATIONS in the text segment\n" ANSI_RESET);
      return 1;
    }
  }
  if (rela->d_un.d_val >= bin->data_len) {
    printf(ANSI_GREEN ANSI_CHECK "\n");
    return 0;
  }
  const uint64_t addr_rela = *(uint64_t *)(bin->raw_data + rela->d_un.d_val);
  if (addr_rela >= text_segment->p_vaddr && addr_rela <= text_segment->p_vaddr + text_segment->p_memsz) {
    printf(ANSI_RED ANSI_CROSS "\n");
    fprintf(stderr, ANSI_RED "ERROR: RELOCATIONS in the text segment\n" ANSI_RESET);
    return 1;
  }
  printf(ANSI_GREEN ANSI_CHECK "\n");
  printf(ANSI_GREEN "LOG: No RELOCATIONS in the text segment\n" ANSI_RESET);
  return 0;
}

int32_t check_relocations_presence_32(const t_bin* bin) {
  const Elf32_Phdr* dyn_header = get_segment_32(bin->phdrs_32, is_dyn_segment_32);
  if (dyn_header == NULL)
    return 1;
  const Elf32_Phdr* text_segment = get_segment_32(bin->phdrs_32, is_text_segment_32);
  const Elf32_Dyn* rela = NULL;
  const Elf32_Dyn* relasz = NULL;
  const Elf32_Dyn* relaent = NULL;
  //copy all dyn_structure into arr till DT_NULL
  for (uint32_t idx = 0; idx < dyn_header->p_filesz / sizeof(Elf32_Dyn); idx++) {
    const Elf32_Dyn* tmp = (Elf32_Dyn *)(bin->raw_data + dyn_header->p_offset + idx * sizeof(Elf32_Dyn));
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
    printf(ANSI_GREEN "LOG: No RELOCATIONS found or missing RELOCATIONS dyn tag in the binary\n");
    return 0;
  }
  //check if all relocation cause problem
  for (uint32_t idx = 0; idx < relasz->d_un.d_val / relaent->d_un.d_val; idx++) {
    const Elf32_Rela* tmp = (Elf32_Rela *)(bin->raw_data + rela->d_un.d_val + idx * relaent->d_un.d_val);
    const uint64_t addr_rela = tmp->r_offset;
    if (addr_rela >= text_segment->p_vaddr && addr_rela <= text_segment->p_vaddr + text_segment->p_memsz) {
      printf(ANSI_RED ANSI_CROSS "\n");
      fprintf(stderr, ANSI_RED "ERROR: RELOCATIONS in the text segment\n" ANSI_RESET);
      return 1;
    }
  }
  printf(ANSI_GREEN ANSI_CHECK "\n");
  printf(ANSI_GREEN "LOG: No RELOCATIONS in the text segment\n" ANSI_RESET);
  return 0;
}