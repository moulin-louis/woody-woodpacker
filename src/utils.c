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
      free((void *)result); // Free previously allocated memory
      return 4; // Return a different error code
    }
    *result = new_result;
    memcpy(*result + *len, buff, retval);
    *len += retval;
  }
  return 0;
}

program_header_list_t *lst_new_program_header(program_header_t *data) {
  program_header_list_t *result = calloc(1, sizeof(program_header_list_t));
  memcpy(&result->program_header, data, sizeof(*data));
  return result;
}

void lst_add_back_program_header(program_header_list_t **head, program_header_t *data) {
  if (*head == NULL) {
    *head = lst_new_program_header(data);
    return;
  }
  program_header_list_t *tmp = *head;
  while (tmp->next) {
    tmp = tmp->next;
  }
  tmp->next = lst_new_program_header(data);
}

section_header_list_t *lst_new_section_header(section_header_t *data) {
  section_header_list_t *result = calloc(1, sizeof(section_header_list_t));
  memcpy(&result->section_header, data, sizeof(*data));
  return result;
}

void lst_add_back_section_header(section_header_list_t **head, section_header_t *data) {
  if (*head == NULL) {
    *head = lst_new_section_header(data);
    return;
  }
  section_header_list_t *tmp = *head;
  while (tmp->next) {
    tmp = tmp->next;
  }
  tmp->next = lst_new_section_header(data);
}

void print_elf_header(elf_header_t *header) {
  printf("Magic: %02x %02x %02x %02x\n", header->magic[0], header->magic[1], header->magic[2], header->magic[3]);
  printf("Class: 0x%02x - ", header->class);
  switch (header->class) {
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
  printf("Endian: 0x%02x - ", header->endian);
  switch (header->endian) {
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
  printf("Version: 0x%02x\n", header->version);
  printf("OS ABI: 0x%02x\n", header->os_abi);
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

char *type_program_to_str(typeProgram type_program) {
  switch (type_program) {
    case PT_NULL:
      return("PT_NULL");
    case PT_LOAD:
      return("PT_LOAD");
    case PT_DYNAMIC:
      return("PT_DYNAMIC");
    case PT_INTERP:
      return("PT_INTERP");
    case PT_NOTE:
      return("PT_NOTE");
    case PT_SHLIB:
      return("PT_SHLIB");
    case PT_PHDR:
      return("PT_PHDR");
    case PT_TLS:
      return("PT_TLS");
    case PT_NUM:
      return("PT_NUM");
    case PT_LOOS:
      return("PT_LOOS");
    case PT_GNU_EH_FRAME:
      return("PT_GNU_EH_FRAME");
    case PT_GNU_STACK:
      return("PT_GNU_STACK");
    case PT_GNU_RELRO:
      return("PT_GNU_RELRO");
    case PT_LOSUNW:
      return("PT_LOSUNW");
    case PT_SUNWSTACK:
      return("PT_SUNWSTACK");
    case PT_HIOS:
      return("PT_HIOS");
    case PT_LOPROC:
      return("PT_LOPROC");
    case PT_HIPROC:
      return("PT_HIPROC");
    case PT_GNU_PROPERTY:
      return("PT_GNU_PROPERTY");
    default:
      return("Unknown");
  }
}

void hangup(void) {
  char buf[1];
  read(1, buf, 1);
}