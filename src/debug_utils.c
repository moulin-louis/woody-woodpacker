//
// Created by loumouli on 11/10/23.
//

#include "woody.h"

__attribute__((unused)) void hexdump(void* data, size_t len, int32_t row) {
  if (row == 0) {
    for (size_t i = 0; i < len; i++) {
      printf("%02x ", ((uint8_t *)data)[i]);
    }
    printf("\n");
    return;
  }
  for (size_t i = 0; i < len; i += row) {
    for (size_t j = i; j < i + row; j++) {
      if (j == len) {
        break;
      }
      printf("%02x ", ((uint8_t *)data)[j]);
    }
    printf("\n");
  }
  printf("\n");
}

__attribute__((unused)) void asciidump(void* data, size_t len, uint32_t row) {
  if (row == 0) {
    for (size_t i = 0; i < len; i++) {
      if (((uint8_t *)data)[i] >= 0x20 && ((uint8_t *)data)[i] <= 0x7e) {
        fprintf(stdout, "%c", ((uint8_t *)data)[i]);
      }
      else {
        fprintf(stdout, ".");
      }
    }
    fprintf(stdout, "\n");
    return;
  }
  for (size_t i = 0; i < len; i += row) {
    for (size_t j = i; j < i + row; j++) {
      if (j == len) {
        break;
      }
      if (((uint8_t *)data)[j] >= 0x20 && ((uint8_t *)data)[j] <= 0x7e) {
        fprintf(stdout, "%c", ((uint8_t *)data)[j]);
      }
      else {
        fprintf(stdout, ".");
      }
    }
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "\n");
}

__attribute__((unused)) void print_elf_header_64(Elf64_Ehdr* header) {
  printf("e_ident: %02x %02x %02x %02x\n", header->e_ident[0], header->e_ident[1], header->e_ident[2],
         header->e_ident[3]);
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

__attribute__((unused)) void print_elf_header_32(Elf32_Ehdr* header) {
  printf("e_ident: %02x %02x %02x %02x\n", header->e_ident[0], header->e_ident[1], header->e_ident[2],
         header->e_ident[3]);
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
  printf("Entry point: 0x%016u\n", header->e_entry);
  printf("Header table offset: 0x%016u\n", header->e_phoff);
  printf("Section header offset: 0x%016u\n", header->e_shoff);
  printf("Flags: 0x%08x\n", header->e_flags);
  printf("Header size: 0x%04x\n", header->e_ehsize);
  printf("Header entry size: 0x%04x\n", header->e_phentsize);
  printf("Header nbr entries: 0x%04x\n", header->e_phnum);
  printf("Section entry size: 0x%04x\n", header->e_shentsize);
  printf("Section nbr entries: 0x%04x\n", header->e_shnum);
  printf("Section index names: 0x%04x\n", header->e_shstrndx);
}

__attribute__((unused)) void hangup(void) {
  uint8_t buf[1];
  const ssize_t retval = read(1, buf, 1);
  (void)retval;
}

__attribute__((unused)) char* type_program_to_str(Elf64_Word type) {
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

__attribute__((unused)) void print_program_headers_64(phdr_list_64_t* head) {
  for (phdr_list_64_t* node = head; node; node = node->next) {
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

__attribute__((unused)) void print_program_headers_32(phdr_list_32_t* head) {
  for (phdr_list_32_t* node = head; node; node = node->next) {
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
    printf("Offset: 0x%016u\n", node->program_header->p_offset);
    printf("Virtual address: 0x%016u\n", node->program_header->p_vaddr);
    printf("Size in file: 0x%016u\n", node->program_header->p_filesz);
    printf("Size in memory: 0x%016ux\n", node->program_header->p_memsz);
    printf("Alignment: 0x%016u\n", node->program_header->p_align);
    printf("\n");
  }
}

__attribute__((unused)) void print_info_payload(t_bin* bin) {
  printf("total hexdump: \n");
  hexdump(bin->payload, bin->len_payload, 0);
}

//print info about a dynamic tag
void print_dyn_tag_64(const Elf64_Dyn* tag) {
  printf("tag->d_tag = 0x%lx : ", tag->d_tag);
  switch (tag->d_tag) {
    case DT_NULL:
      printf("DT_NULL\n");
      break;
    case DT_NEEDED:
      printf("DT_NEEDED\n");
      break;
    case DT_PLTRELSZ:
      printf("DT_PLTRELSZ\n");
      break;
    case DT_PLTGOT:
      printf("DT_PLTGOT\n");
      break;
    case DT_HASH:
      printf("DT_HASH\n");
      break;
    case DT_STRTAB:
      printf("DT_STRTAB\n");
      break;
    case DT_SYMTAB:
      printf("DT_SYMTAB\n");
      break;
    case DT_RELA:
      printf("DT_RELA\n");
      break;
    case DT_RELASZ:
      printf("DT_RELASZ\n");
      break;
    case DT_RELAENT:
      printf("DT_RELAENT\n");
      break;
    case DT_STRSZ:
      printf("DT_STRSZ\n");
      break;
    case DT_SYMENT:
      printf("DT_SYMENT\n");
      break;
    case DT_INIT:
      printf("DT_INIT\n");
      break;
    case DT_FINI:
      printf("DT_FINI\n");
      break;
    case DT_SONAME:
      printf("DT_SONAME\n");
      break;
    case DT_RPATH:
      printf("DT_RPATH\n");
      break;
    case DT_SYMBOLIC:
      printf("DT_SYMBOLIC\n");
      break;
    case DT_REL:
      printf("DT_REL\n");
      break;
    case DT_RELSZ:
      printf("DT_RELSZ\n");
      break;
    case DT_RELENT:
      printf("DT_RELENT\n");
      break;
    case DT_INIT_ARRAY:
      printf("DT_INIT_ARRAY\n");
      break;
    case DT_FINI_ARRAY:
      printf("DT_FINI_ARRAY\n");
      break;
    case DT_INIT_ARRAYSZ:
      printf("DT_INIT_ARRAYSZ\n");
      break;
    case DT_FINI_ARRAYSZ:
      printf("DT_FINI_ARRAYSZ\n");
      break;
    case DT_PLTREL:
      printf("DT_PLTREL\n");
      break;
    case DT_DEBUG:
      printf("DT_DEBUG\n");
      break;
    case DT_JMPREL:
      printf("DT_JMPREL\n");
      break;
    default:
      printf("UNKNOWN\n");
  }
  printf("tag.d_un = 0x%lx\n", tag->d_un.d_ptr);
}

void print_dyn_tag_32(const Elf32_Dyn* tag) {
  printf("tag->d_tag = 0x%x : ", tag->d_tag);
  switch (tag->d_tag) {
    case DT_NULL:
      printf("DT_NULL\n");
      break;
    case DT_NEEDED:
      printf("DT_NEEDED\n");
      break;
    case DT_PLTRELSZ:
      printf("DT_PLTRELSZ\n");
      break;
    case DT_PLTGOT:
      printf("DT_PLTGOT\n");
      break;
    case DT_HASH:
      printf("DT_HASH\n");
      break;
    case DT_STRTAB:
      printf("DT_STRTAB\n");
      break;
    case DT_SYMTAB:
      printf("DT_SYMTAB\n");
      break;
    case DT_RELA:
      printf("DT_RELA\n");
      break;
    case DT_RELASZ:
      printf("DT_RELASZ\n");
      break;
    case DT_RELAENT:
      printf("DT_RELAENT\n");
      break;
    case DT_STRSZ:
      printf("DT_STRSZ\n");
      break;
    case DT_SYMENT:
      printf("DT_SYMENT\n");
      break;
    case DT_INIT:
      printf("DT_INIT\n");
      break;
    case DT_FINI:
      printf("DT_FINI\n");
      break;
    case DT_SONAME:
      printf("DT_SONAME\n");
      break;
    case DT_RPATH:
      printf("DT_RPATH\n");
      break;
    case DT_SYMBOLIC:
      printf("DT_SYMBOLIC\n");
      break;
    case DT_REL:
      printf("DT_REL\n");
      break;
    case DT_RELSZ:
      printf("DT_RELSZ\n");
      break;
    case DT_RELENT:
      printf("DT_RELENT\n");
      break;
    case DT_INIT_ARRAY:
      printf("DT_INIT_ARRAY\n");
      break;
    case DT_FINI_ARRAY:
      printf("DT_FINI_ARRAY\n");
      break;
    case DT_INIT_ARRAYSZ:
      printf("DT_INIT_ARRAYSZ\n");
      break;
    case DT_FINI_ARRAYSZ:
      printf("DT_FINI_ARRAYSZ\n");
      break;
    case DT_PLTREL:
      printf("DT_PLTREL\n");
      break;
    case DT_DEBUG:
      printf("DT_DEBUG\n");
      break;
    case DT_JMPREL:
      printf("DT_JMPREL\n");
      break;
    default:
      printf("UNKNOWN\n");
  }
  printf("tag.d_un = 0x%x\n", tag->d_un.d_ptr);
}