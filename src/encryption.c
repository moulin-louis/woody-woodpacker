//
// Created by loumouli on 11/9/23.
//

#include "woody.h"

void xor_encrypt(void *data, size_t len, const char *key) {
  for (size_t i = 0; i < len; i++)
    ((char *) data)[i] ^= key[i % KEY_SIZE];
}

int encryption(t_bin *bin) {
  //get text segment
  const Elf64_Phdr *text_segment = NULL;
  text_segment = get_segment(bin->phdrs, is_text_segment_64);
  if (!text_segment) {
    dprintf(2, "No text segment found\n");
    return 1;
  }
  //get key from urandom
  char key[KEY_SIZE] = {0};
  if (get_key(key)) {
    dprintf(2, "Failed to get key\n");
    return 1;
  }
  printf("key = 0x%lx\n", *(uint64_t *) key);
  //encrypt text segment
  void *data = bin->raw_data + text_segment->p_offset;
  memcpy(data, bin->raw_data + text_segment->p_offset, text_segment->p_filesz);
  xor_encrypt(data, text_segment->p_filesz, key);
  return 0;
}