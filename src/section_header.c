//
// Created by louis on 11/2/23.
//

#include "woody.h"

void parse_section_headers(t_bin *bin) {
  size_t curr_offset = bin  ->header.e_shoff;

  if (sizeof(section_header_t) != bin->header.e_shentsize) {
    printf("ERROR: Wrong size of section header\n");
    return;
  }
  for (uint idx = 0; idx != bin->header.e_shnum; idx++) {
    section_header_t tmp = {};
    memcpy(&tmp, bin->raw_data + curr_offset, sizeof(tmp));
    lst_add_back_section_header(&bin->section_headers, &tmp);
    curr_offset += bin->header.e_shentsize;
  }
}

void print_section_headers(section_header_list_t *head) {
  printf("\nPARSING AND PRINTING ALL SECTION HEADERS\n\n");
  for (section_header_list_t *tmp = head; tmp != NULL; tmp = tmp->next) {
    print_section_header(&tmp->section_header);
    printf("\n");
  }
}

void print_section_header(section_header_t *sectionHeader) {
  printf("Name offset: 0x%08x\n", sectionHeader->sh_name);
  printf("Type: 0x%08x\n", sectionHeader->sh_type);
  printf("Flags: 0x%016lx\n", sectionHeader->sh_flags);
  printf("Virtual address: 0x%016lx\n", sectionHeader->sh_addr);
  printf("Offset: 0x%016lx\n", sectionHeader->sh_offset);
  printf("Size in file: 0x%016lx\n", sectionHeader->sh_size);
  printf("Link: 0x%08x\n", sectionHeader->sh_link);
  printf("Info: 0x%08x\n", sectionHeader->sh_info);
  printf("Alignment: 0x%016lx\n", sectionHeader->sh_addralign);
  printf("Entry size: 0x%016lx\n", sectionHeader->sh_entsize);
}