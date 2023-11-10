//
// Created by loumouli on 11/9/23.
//

#include "woody.h"

void xor_encrypt(void *data, size_t len, const uint8_t *key) {
  for (size_t i = 0; i < len; i++)
    ((char *) data)[i] ^= key[i % KEY_SIZE];
}

int encryption(t_bin *bin) {
  //get text segment
  const Elf64_Phdr *text_segment = NULL;
  text_segment = get_segment(bin->phdrs, is_text_segment_64);
  //get key from urandom
  bin->key = calloc(1, KEY_SIZE);
  bin->len_key = KEY_SIZE;
  if (get_key(bin->key)) {
    dprintf(2, "Failed to get key\n");
    return 1;
  }
  //print key in hexa
  printf("key: ");
  hexdump(bin->key, bin->len_key, 0);
  //encrypt text segment
  void *data = bin->raw_data + text_segment->p_offset;
  memcpy(data, bin->raw_data + text_segment->p_offset, text_segment->p_filesz);
  xor_encrypt(data, text_segment->p_filesz, bin->key);
  return 0;
}