//
// Created by loumouli on 11/9/23.
//

#include "woody.h"

int get_key(char *key) {
  int fd = open("/dev/urandom", O_RDONLY);
  if (fd == -1) {
    dprintf(2, "Failed to open /dev/urandom\n");
    return 1;
  }
  if (read(fd, key, KEY_SIZE) == -1) {
    dprintf(2, "Failed to read /dev/urandom\n");
    return 1;
  }
  close(fd);
  return 0;
}

const void *get_segment(const phdr_list_t *head, int (*callback)(const Elf64_Phdr *)) {
  const phdr_list_t *tmp = head;
  while (tmp) {
    if (callback(&tmp->program_header))
      return &(tmp->program_header);
    tmp = tmp->next;
  }
  return NULL;
}

int is_text_segment(const Elf64_Phdr *segment) {
  if (segment->p_type == PT_LOAD && segment->p_flags & PF_X) {
    return 1;
  }
  return 0;
}

void xor_encrypt(void *data, size_t len, const char *key) {
  for (size_t i = 0; i < len; i++) {
    ((char *) data)[i] ^= key[i % KEY_SIZE];
  }
}

int encryption(t_bin *bin) {
  //get text segment
  const Elf64_Phdr *text_segment = NULL;
  text_segment = get_segment(bin->phdrs, is_text_segment);
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