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

__attribute__((unused)) void print_elf_header(elf_header_t *header) {
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
      return ("PT_NULL");
    case PT_LOAD:
      return ("PT_LOAD");
    case PT_DYNAMIC:
      return ("PT_DYNAMIC");
    case PT_INTERP:
      return ("PT_INTERP");
    case PT_NOTE:
      return ("PT_NOTE");
    case PT_SHLIB:
      return ("PT_SHLIB");
    case PT_PHDR:
      return ("PT_PHDR");
    case PT_TLS:
      return ("PT_TLS");
    case PT_NUM:
      return ("PT_NUM");
    case PT_LOOS:
      return ("PT_LOOS");
    case PT_GNU_EH_FRAME:
      return ("PT_GNU_EH_FRAME");
    case PT_GNU_STACK:
      return ("PT_GNU_STACK");
    case PT_GNU_RELRO:
      return ("PT_GNU_RELRO");
    case PT_LOSUNW:
      return ("PT_LOSUNW");
    case PT_SUNWSTACK:
      return ("PT_SUNWSTACK");
    case PT_HIOS:
      return ("PT_HIOS");
    case PT_LOPROC:
      return ("PT_LOPROC");
    case PT_HIPROC:
      return ("PT_HIPROC");
    case PT_GNU_PROPERTY:
      return ("PT_GNU_PROPERTY");
    default:
      return ("Unknown");
  }
}

char *type_dynamic_to_str(typeDynamic tag) {
  switch (tag) {
    case DT_NULL:
      return ("DT_NULL");
    case DT_NEEDED:
      return ("DT_NEEDED");
    case DT_PLTRELSZ:
      return ("DT_PLTRELSZ");
    case DT_PLTGOT:
      return ("DT_PLTGOT");
    case DT_HASH:
      return ("DT_HASH");
    case DT_STRTAB:
      return ("DT_STRTAB");
    case DT_SYMTAB:
      return ("DT_SYMTAB");
    case DT_RELA:
      return ("DT_RELA");
    case DT_RELASZ:
      return ("DT_RELASZ");
    case DT_RELAENT:
      return ("DT_RELAENT");
    case DT_STRSZ:
      return ("DT_STRSZ");
    case DT_SYMENT:
      return ("DT_SYMENT");
    case DT_INIT:
      return ("DT_INIT");
    case DT_FINI:
      return ("DT_FINI");
    case DT_SONAME:
      return ("DT_SONAME");
    case DT_RPATH:
      return ("DT_RPATH");
    case DT_SYMBOLIC:
      return ("DT_SYMBOLIC");
    case DT_REL:
      return ("DT_REL");
    case DT_RELSZ:
      return ("DT_RELSZ");
    case DT_RELENT:
      return ("DT_RELENT");
    case DT_PLTREL:
      return ("DT_PLTREL");
    case DT_DEBUG:
      return ("DT_DEBUG");
    case DT_TEXTREL:
      return ("DT_TEXTREL");
    case DT_JMPREL:
      return ("DT_JMPREL");
    case DT_BIND_NOW:
      return ("DT_BIND_NOW");
    case DT_INIT_ARRAY:
      return ("DT_INIT_ARRAY");
    case DT_FINI_ARRAY:
      return ("DT_FINI_ARRAY");
    case DT_INIT_ARRAYSZ:
      return ("DT_INIT_ARRAYSZ");
    case DT_FLAGS:
      return ("DT_FLAGS");
    case DT_GNUHASH:
      return ("DT_GNUHASH");
    case DT_FLAGS1:
      return ("DT_FLAGS1");
    case DT_RELACOUNT:
      return ("DT_RELACOUNT");
  }
  return ("Unknown");
}

__attribute__((unused)) void hangup(void) {
  char buf[1];
  ssize_t retval = read(1, buf, 1);
  (void) retval;
}

segment_header_t *search_segment_type(t_bin *bin, typeProgram type_program) {
  for (segment_header_t *segment = bin->program_headers; segment != NULL; segment = segment->next) {
    if (segment->p_type == type_program) {
      return segment;
    }
  }
  return NULL;
}

segment_dyn_t *search_segment_dyn_type(t_bin *bin, typeDynamic type_dynamic) {
  for (segment_dyn_t *segment = bin->dynamic_segment; segment != NULL; segment = segment->next) {
    if (segment->d_tag == type_dynamic) {
      return segment;
    }
  }
  return NULL;
}

void cleanup2(t_bin *bin) {
  segment_dyn_t *tmp_dyn = bin->dynamic_segment;
  while (tmp_dyn) {
    segment_dyn_t *next = tmp_dyn->next;
    free(tmp_dyn);
    tmp_dyn = next;
  }
}

void cleanup(t_bin *bin) {
  //clean program header list
  segment_header_t *tmp_segment = bin->program_headers;
  while (tmp_segment) {
    segment_header_t *next = tmp_segment->next;
    free(tmp_segment);
    tmp_segment = next;
  }
  //clean dynamic segment list
  cleanup2(bin);
  free(bin->raw_data);
}