//
// Created by loumouli on 11/10/23.
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

__attribute__((unused)) char *type_program_to_str(Elf64_Word type) {
  switch (type) {
    case PT_NULL:
      return "PT_NULL";
    case PT_LOAD:
      return "PT_LOAD";
    case PT_DYNAMIC:
      return "PT_DYNAMIC";
    case PT_INTERP:
      return "PT_INTERP";
    case PT_NOTE:
      return "PT_NOTE";
    case PT_SHLIB:
      return "PT_SHLIB";
    case PT_PHDR:
      return "PT_PHDR";
    case PT_TLS:
      return "PT_TLS";
    case PT_NUM:
      return "PT_NUM";
    case PT_LOOS:
      return "PT_LOOS";
    case PT_GNU_EH_FRAME:
      return "PT_GNU_EH_FRAME";
    case PT_GNU_STACK:
      return "PT_GNU_STACK";
    case PT_GNU_RELRO:
      return "PT_GNU_RELRO";
    case 0x6474e553:
      return "PT_LOSUNW";
    default:
      return "Unknown type";
  }
}

__attribute__((unused)) void print_program_headers(phdr_list_t *head) {
  for (phdr_list_t *node = head; node; node = node->next) {
    if (node->program_header->p_type != PT_LOAD) {
      continue;
    }
    printf("Type: 0x%08x - ", node->program_header->p_type);
    printf("%s\n", type_program_to_str(node->program_header->p_type));
    printf("Flags: 0x%08x - ", node->program_header->p_flags);
    if (node->program_header->p_flags & (1 << 2)) {
      printf("PF_R ");
    }
    if (node->program_header->p_flags & (1 << 1)) {
      printf("PF_W ");
    }
    if (node->program_header->p_flags & (1 << 0)) {
      printf("PF_X ");
    }
    printf("\n");
    printf("Offset: 0x%016lx\n", node->program_header->p_offset);
    printf("Virtual address: 0x%016lx\n", node->program_header->p_vaddr);
    printf("Size in file: 0x%016lx\n", node->program_header->p_filesz);
    printf("Size in memory: 0x%016lx\n", node->program_header->p_memsz);
    printf("Alignment: 0x%016lx\n", node->program_header->p_align);
    printf("\n");
  }
}
