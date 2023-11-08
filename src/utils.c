//
// Created by louis on 11/1/23.
//

#include "woody.h"

__attribute__((unused)) void hexdump(void *data, size_t len, int32_t row) {
  if (row == 0) {
    for (size_t i = 0; i < len; i++) {
      dprintf(1, "%02x ", ((uint8_t *) data)[i]);
    }
    dprintf(1, "\n");
    return;
  }
  for (size_t i = 0; i < len; i += row) {
    for (size_t j = i; j < i + row; j++) {
      if (j == len) {
        break;
      }
      dprintf(1, "%02x ", ((uint8_t *) data)[j]);
    }
    dprintf(1, "\n");
  }
  dprintf(1, "\n");
}

__attribute__((unused)) void asciidump(void *data, size_t len, uint32_t row) {
  if (row == 0) {
    for (size_t i = 0; i < len; i++) {
      if (((uint8_t *) data)[i] >= 0x20 && ((uint8_t *) data)[i] <= 0x7e) {
        dprintf(1, "%c", ((uint8_t *) data)[i]);
      } else {
        dprintf(1, ".");
      }
    }
    dprintf(1, "\n");
    return;
  }
  for (size_t i = 0; i < len; i += row) {
    for (size_t j = i; j < i + row; j++) {
      if (j == len) {
        break;
      }
      if (((uint8_t *) data)[j] >= 0x20 && ((uint8_t *) data)[j] <= 0x7e) {
        dprintf(1, "%c", ((uint8_t *) data)[j]);
      } else {
        dprintf(1, ".");
      }
    }
    dprintf(1, "\n");
  }
  dprintf(1, "\n");
}

int read_file(int file, char **result, size_t *len) {
  while (1) {
    char buff[8192];
    ssize_t retval = read(file, buff, sizeof(buff));
    if (retval == -1) {
      free(*result);  // Free allocated memory before returning
      return 3;
    }
    if (retval == 0) {
      break;
    }
    char *new_result = realloc(*result, *len + retval); // Check realloc return value
    if (new_result == NULL) {
      free((void *) result); // Free previously allocated memory
      return 4; // Return a different error code
    }
    *result = new_result;
    memcpy(*result + *len, buff, retval);
    *len += retval;
  }
  return 0;
}

__attribute__((unused)) void print_elf_header(Elf64_Ehdr *header) {
  printf("e_ident: %02x %02x %02x %02x\n", header->e_ident[0], header->e_ident[1], header->e_ident[2], header->e_ident[3]);
  printf("Class: 0x%02x - ", header->e_ident[EI_CLASS]);
      switch (header->e_ident[EI_CLASS]) {
    case 0x1:
      printf("32-bits\n");
      break;
    case 0x2:
      printf("64-bits\n");
      break;
    default:
      printf("Invalid\n");
      break;
  }
  printf("Endian: 0x%02x - ", header->e_ident[EI_DATA]);
  switch (header->e_ident[EI_DATA]) {
    case 0x1:
      printf("little endianness\n");
      break;
    case 0x2:
      printf("big endianness\n");
      break;
    default:
      printf("Invalid\n");
      break;
  }
  printf("Version: 0x%02x\n", header->e_version);
  printf("OS ABI: 0x%02x\n", header->e_ident[EI_OSABI]);
  printf("Type: 0x%04x\n", header->e_type);
  printf("Machine: 0x%04x\n", header->e_machine);
  printf("Version: 0x%08x\n", header->e_version);
  printf("Entry point: 0x%016lx\n", header->e_entry);
  printf("Header table offset: 0x%016lx\n", header->e_phoff);
  printf("Section header offset: 0x%016lx\n", header->e_shoff);
  printf("Flags: 0x%08x\n", header->e_flags);
  printf("Header size: 0x%04x\n", header->e_ehsize);
  printf("Header entry size: 0x%04x\n", header->e_phentsize);
  printf("Header nbr entries: 0x%04x\n", header->e_phnum);
  printf("Section entry size: 0x%04x\n", header->e_shentsize);
  printf("Section nbr entries: 0x%04x\n", header->e_shnum);
  printf("Section index names: 0x%04x\n", header->e_shstrndx);
}

__attribute__((unused)) void hangup(void) {
  char buf[1];
  ssize_t retval = read(1, buf, 1);
  (void) retval;
}

uint64_t allign_down(uint64_t x, uint64_t align) {
  return (x) & ~(align - 1);
}

uint64_t allign_up(uint64_t x, uint64_t align) {
  uint64_t result = allign_down(x + align -1, align);
  return result ? result : align;
}