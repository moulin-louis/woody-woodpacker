#include "woody.h"
#include <sys/mman.h>

int mapping_all_segments(t_bin *bin) {
  for (program_header_list_t *tmp = bin->program_headers; tmp != NULL; tmp = tmp->next) {
    if (tmp->program_header.p_type == 0x1) {
      void *address = (void *) tmp->program_header.p_vaddr;
      size_t len = tmp->program_header.p_memsz;
      void *result = mmap(address, len, PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
      if (result == MAP_FAILED) {
        perror("mmap");
        return 1;
      }
      if (result != address) {
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
      uint64_t address = tmp->program_header.p_vaddr;
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

void calling_exe(t_bin *bin, program_header_list_t *segment) {
  uint64_t address = (uint64_t) (bin->raw_data + segment->program_header.p_offset);
  address = address & ~(segment->program_header.p_align -1);
  size_t len = segment->program_header.p_memsz;
  printf("making range %p - %p executable\n", (void *) address, (void *) (address + len));
  if (mprotect((void *) address, len, PROT_READ | PROT_EXEC) == 1) {
    perror("mprotect");
    return;
  }
  void (*fn)(void) = (void (*)(void)) (bin->raw_data + segment->program_header.p_offset);
  hangup();
  printf("Calling function at %p\n\n", fn);
  hangup();
  fn();
}

int second_stage(t_bin *bin) {
  printf("Making executable segment executable...\n");
  if (mapping_all_segments(bin)) {
    return 1;
  }
  program_header_list_t *segment = search_entry_segment(bin);
  if (!segment) {
    return 1;
  }
  printf("pid = %d\n", getpid());
  setup_permissions_segments(bin);
  calling_exe(bin, segment);
  return 0;
}

void cleanup(t_bin *bin) {
  //clean program header list
  program_header_list_t *tmp = bin->program_headers;
  while (tmp) {
    program_header_list_t *next = tmp->next;
    free(tmp);
    tmp = next;
  }
  //clean section header list
  section_header_list_t *tmp2 = bin->section_headers;
  while (tmp2) {
    section_header_list_t *next = tmp2->next;
    free(tmp2);
    tmp2 = next;
  }
  free(bin->raw_data);
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
  print_elf_header(&bin.header);
  parse_program_headers(&bin);
  parse_section_headers(&bin);
  printf("PRINTING DATA OF ALL PROGRAM HEADERS\n");
  print_program_headers(bin.program_headers);
//  for (program_header_list_t *tmp = bin.program_headers; tmp != NULL; tmp = tmp->next) {
//    void *address = bin.raw_data + tmp->program_header.p_offset;
//    hexdump(address, tmp->program_header.p_filesz, 0);
//  }
  if (second_stage(&bin)) {
    printf("Error in second stage\n");
  }
  cleanup(&bin);
  return 0;
}