#include "woody.h"
#include <sys/mman.h>

#define BASE_ADDRESS 0x400000

int mapping_all_segments(t_bin *bin) {
  for (program_header_list_t *tmp = bin->program_headers; tmp != NULL; tmp = tmp->next) {
    if (tmp->program_header.p_type == 0x1) {
      uint64_t address = tmp->program_header.p_vaddr + BASE_ADDRESS;
      address = address & ~(tmp->program_header.p_align -1);
      uint64_t padding = (tmp->program_header.p_vaddr + BASE_ADDRESS) - address;
      size_t len = tmp->program_header.p_memsz + padding;
      printf("mapping memory range from %p to %p\n", (void *) address, (void *) (address + len));
      if (len == 0) {
        continue;
      }
      void *result = mmap((void *)address, len, PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
      if (result == MAP_FAILED) {
        perror("mmap");
        return 1;
      }
      if ((uint64_t)result != address) {
        printf("Error mapping memory at the right address\n");
        return 1;
      }
      memcpy(result, bin->raw_data + tmp->program_header.p_offset, tmp->program_header.p_filesz);
    }
  }
  return 0;
}

program_header_list_t *search_entry_segment(t_bin *bin) {
  for (program_header_list_t *tmp = bin->program_headers; tmp != NULL; tmp = tmp->next) {
    if (tmp->program_header.p_flags & (1 << 2)) {
      if (tmp->program_header.p_vaddr == bin->header.e_entry) {
        return tmp;
      }
    }
  }
  printf("No segment executable found\n");
  return NULL;
}

int setup_permissions_segments(t_bin *bin) {
  for (program_header_list_t *tmp = bin->program_headers; tmp != NULL; tmp = tmp->next) {
    if (tmp->program_header.p_type == 0x1) {
      uint64_t address = tmp->program_header.p_vaddr + BASE_ADDRESS;
      address = address & ~(tmp->program_header.p_align -1);
      size_t len = tmp->program_header.p_memsz;
      int prot = 0;
      printf("giving this perm to segment: ");
      if (tmp->program_header.p_flags & (1 << 0)) {
        printf("EXEC ");
        prot |= PROT_EXEC;
      }
      if (tmp->program_header.p_flags & (1 << 1)) {
        printf("WRITE ");
        prot |= PROT_WRITE;
      }
      if (tmp->program_header.p_flags & (1 << 2)) {
        printf("READ ");
        prot |= PROT_READ;
      }
      printf("\n");
      printf("protection: %d\n", prot);
      if (mprotect((void *) address, len, prot) == -1) {
        perror("mprotect");
        return 1;
      }
    }
  }
  return 0;
}

int second_stage(t_bin *bin) {
  printf("pid = %d\n", getpid());
  printf("mapping all segments\n");
  if (mapping_all_segments(bin)) {
    return 1;
  }
  setup_permissions_segments(bin);
  printf("searching entry segment\n");
//  hangup();
//  program_header_list_t *segment = search_entry_segment(bin);
//  if (!segment) {
//    printf("Error searching entry segment\n");
//    return 1;
//  }
  printf("setting up permissions\n");
  printf("calling exe\n");
  uint64_t addres_entry = bin->header.e_entry + BASE_ADDRESS;
  printf("address entry = %p\n", (void *) addres_entry);
//  hangup();
  printf("Jumping to entry point\n");
  fflush(stdout);
  ((void (*)(void)) addres_entry)();
  return 0;
}

int main(int ac, char **av) {
  t_bin bin;

  if (ac != 2) {
    printf("Wrong usage\n");
    return 1;
  }
  memset(&bin, 0, sizeof(bin));
int file = open(av[1], O_RDONLY);
  if (file == -1) {
    printf("Error opening file\n");
    return 2;
  }
  if (read_file(file, (char **)&bin.raw_data, &bin.data_len) != 0) {
    printf("Error reading file\n");
    return 3;
  }
  close(file);
  if (bin.data_len < sizeof(elf_header_t)) {
    printf("File too small\n");
    return 4;
  }
  memcpy(&bin.header, bin.raw_data, sizeof(elf_header_t));
  if (!(bin.header.magic[0] == 0x7F && bin.header.magic[1] == 'E' && bin.header.magic[2] == 'L' && bin.header.magic[3] == 'F')) {
    printf("This is not an ELF file\n");
    return 4;
  }
//  print_elf_header(&bin.header);
  parse_program_headers(&bin);
  parse_section_headers(&bin);
//  printf("PRINTING DATA OF ALL PROGRAM HEADERS\n");
//  print_program_headers(bin.program_headers);
  for (program_header_list_t *tmp = bin.program_headers; tmp != NULL; tmp = tmp->next) {
    if (tmp->program_header.p_type != PT_LOAD) {
    continue;
    }
    void *address = bin.raw_data + tmp->program_header.p_offset;
    printf("ASCII DUMP:\n");
    asciidump(address, tmp->program_header.p_filesz, 0);
    printf("HEX DUMP:\n");
    hexdump(address, tmp->program_header.p_filesz, 0);
    printf("\n");
  }
//  if (second_stage(&bin)) {
//    printf("Error in second stage\n");
//  }
  cleanup(&bin);
  return 0;
}