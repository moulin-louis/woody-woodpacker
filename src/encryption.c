//
// Created by loumouli on 11/9/23.
//

#include "woody.h"

void xor_encrypt(const uint8_t *key, const uint64_t len_key, uint8_t *data, const uint64_t len_data) {
  for (size_t i = 0; i < len_data; i++)
    data[i] ^= key[i % len_key];
}

int32_t encryption(t_bin *bin) {
  //get text segment
  const Elf64_Phdr *text_segment = NULL;
  text_segment = get_segment(bin->phdrs, is_text_segment_64);
  //get key from urandom
  bin->key = calloc(1, KEY_SIZE);
  bin->len_key = 32;
  if (get_key(bin->key)) {
    dprintf(2, "Failed to get key\n");
    return 1;
  }
  printf("writing at offset %lu\n", text_segment->p_offset + (bin->elf_header->e_entry - text_segment->p_vaddr));
  printf("for a len of %lu\n", text_segment->p_filesz - (bin->elf_header->e_entry - text_segment->p_vaddr));
  void *data = bin->raw_data + text_segment->p_offset + (bin->elf_header->e_entry - text_segment->p_vaddr);
  xor_encrypt(bin->key, bin->len_key, data, text_segment->p_filesz - (bin->elf_header->e_entry - text_segment->p_vaddr));
  return 0;
}