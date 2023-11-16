//
// Created by loumouli on 11/9/23.
//

#include "woody.h"

void xor_encrypt(const uint8_t *key, const uint64_t len_key, uint8_t *data, const uint64_t len_data) {
  for (size_t i = 0; i < len_data; i++) {
    // printf("i = %zu and i = len_key = %zu ", i, i % len_key);
    // printf("key char = 0x%02x ", key[i % len_key]);
    data[i] ^= key[i % len_key];
    // printf("data char = 0x%02x\n",data[i]);
  }
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
  // printf("key: ");
  // hexdump(bin->key, bin->len_key, 0);
  //encrypt text segment
  printf("writing at offset %lu\n", text_segment->p_offset + (bin->elf_header->e_entry - text_segment->p_vaddr));
  printf("for a len of %lu\n", text_segment->p_filesz - (bin->elf_header->e_entry - text_segment->p_vaddr));
  void *data = bin->raw_data + text_segment->p_offset + (bin->elf_header->e_entry - text_segment->p_vaddr);
  xor_encrypt(bin->key, bin->len_key, data, text_segment->p_filesz - (bin->elf_header->e_entry - text_segment->p_vaddr));
  return 0;
}