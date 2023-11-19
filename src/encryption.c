//
// Created by loumouli on 11/9/23.
//

#include "woody.h"

//expose symbol for asm code
void encrypt(const uint8_t* key, const uint64_t len_key, uint8_t* data, const uint64_t len_data);

//temporary xor encrypt
void xor_encrypt(const uint8_t* key, const uint64_t len_key, uint8_t* data, const uint64_t len_data) {
  for (size_t i = 0; i < len_data; i++)
    data[i] ^= key[i % len_key];
}

//Encrypt the text segment base on a random key provided by urandom or the user in the ENV
int32_t encryption_64(t_bin* bin) {
  //get text segment
  const Elf64_Phdr* text_segment = NULL;
  text_segment = get_segment_64(bin->phdrs_64, is_text_segment_64);
  //get key from urandom
  bin->key = calloc(1, KEY_SIZE);
  bin->len_key = 32;
  const char* key = getenv("KEY");
  if (key != NULL) {
    printf("Found key in ENV\n");
    bin->key = (uint8_t *)strdup(key);
    if (bin->key == NULL) {
      return 1;
    }
    bin->len_key = strlen((char *)bin->key);
  }
  else if (get_key(bin->key)) {
    return 1;
  }
  void* data = bin->raw_data + text_segment->p_offset;
  xor_encrypt(bin->key, bin->len_key, data, text_segment->p_filesz);
  return 0;
}

int32_t encryption_32(t_bin *bin) {
  //get text segment
  const Elf32_Phdr* text_segment = NULL;
  text_segment = get_segment_32(bin->phdrs_32, is_text_segment_32);
  //get key from urandom
  bin->key = calloc(1, KEY_SIZE);
  bin->len_key = 32;
  const char* key = getenv("KEY");
  if (key != NULL) {
    printf("Found key in ENV\n");
    bin->key = (uint8_t *)strdup(key);
    if (bin->key == NULL) {
      return 1;
    }
    bin->len_key = strlen((char *)bin->key);
  }
  else if (get_key(bin->key)) {
    return 1;
  }
  void* data = bin->raw_data + text_segment->p_offset;
  xor_encrypt(bin->key, bin->len_key, data, text_segment->p_filesz);
  return 0;
}
