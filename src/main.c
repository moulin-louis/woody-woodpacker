#include "woody.h"

#define BASE_ADDRESS 0x400000

uint64_t align_address(uint64_t address, uint64_t align) {
  if (align == 0) {
    return address; // No alignment necessary
  }
  uint64_t aligned_address = (address + align - 1) / align * align;
  return aligned_address;
}

int setup_permission(segment_header_t *program_header, uint64_t address, size_t len) {
  int prot = PROT_NONE;
  printf("setup permission: ");
  if (program_header->p_flags & 0x4) {
    printf("Read ");
    prot |= PROT_READ;
  }
  if (program_header->p_flags & 0x2) {
    printf("Write ");
    prot |= PROT_WRITE;
  }
  if (program_header->p_flags & 0x1) {
    printf("Execute ");
    prot |= PROT_EXEC;
  }
  printf("\n");
  printf("SETUP_PERMISSION: address = %p\n", (void *)address);
  printf("SETUP_PERMISSION: len = 0x%lx\n", len);
  printf("SETUP_PERMISSION: prot = %d\n", prot);
  if (mprotect((void *)address, len, prot) == -1) {
    perror("mprotect");
    return 1;
  }
  return 0;
}

void init_memory(void *result, segment_header_t *segment, t_bin *bin) {
  if (segment->p_memsz == segment->p_filesz) {
    memcpy(result, bin->raw_data + segment->p_offset, segment->p_filesz);
    return;
  } else if (segment->p_memsz > segment->p_filesz) {
    memcpy(result, bin->raw_data + segment->p_offset, segment->p_filesz);
    memset(result + segment->p_filesz, 0, segment->p_memsz - segment->p_filesz);
    return;
  } else if (segment->p_memsz < segment->p_filesz) {
    memcpy(result, bin->raw_data + segment->p_offset, segment->p_memsz);
    return;
  }
  memset(result, 0, segment->p_memsz);
}

int mapping_all_loadable_segments(t_bin *bin) {
  for (segment_header_t *segment = bin->program_headers; segment != NULL; segment = segment->next) {
    if (segment->p_type != PT_LOAD) {
      continue;
    }
    uint64_t address = segment->p_vaddr + BASE_ADDRESS;
    uint64_t allign_address = align_address(address, segment->p_align);
    uint64_t padding = address - allign_address;
    size_t len = segment->p_memsz + padding;
    if (len < segment->p_align) {
      len = segment->p_align;
    }
    void *result = mmap((void *)allign_address, len, PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
    if (result == MAP_FAILED) {
      perror("mmap");
      return 1;
    }
    init_memory((void *)address, segment, bin);
  }
  for (segment_header_t *segment = bin->program_headers; segment != NULL; segment = segment->next) {
    if (segment->p_type != PT_LOAD)
      continue;
    uint64_t address = segment->p_vaddr + BASE_ADDRESS;
    uint64_t allign_address = align_address(address, segment->p_align);
    uint64_t padding = address - allign_address;
    size_t len = segment->p_memsz + padding;
    if (len < segment->p_align)
      len = segment->p_align;
    setup_permission(segment, allign_address, len);
    printf("\n");
  }
  return 0;
}

int second_stage(t_bin *bin) {
  printf("pid = %d\n", getpid());
  printf("mapping all loadable segments\n");
  if (mapping_all_loadable_segments(bin)) {
    printf("Error mapping all loadable segments\n");
    return 1;
  }
  printf("setting up permissions for loadable segments\n");
  uint64_t addres_entry = bin->header.e_entry + BASE_ADDRESS;
  printf("Jumping to entry point\n");
  fflush(stdout);
  cleanup(bin);
  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    return 1;
  }
  if (pid == 0) {
    ((void (*)(void)) addres_entry)();
    return 0;
  } else {
    int status;
    waitpid(pid, &status, 0);
    return status;
  }
}

int main(int ac, char **av) {
  t_bin bin = {};

  if (ac != 2) {
    printf("Wrong usage\n");
    return 1;
  }
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
  if (memcmp(bin.header.magic, "\x7F" "ELF", 4) != 0) {
    printf("This is not an ELF file\n");
    return 4;
  }
  print_elf_header(&bin.header);
  if (parse_program_headers(&bin)) {
    printf("Error parsing program headers\n");
    return 5;
  }
  if (parse_dynamic_segment(&bin)) {
    printf("Error parsing dynamic segment\n");
    return 6;
  }
//  printf("PRINTING DATA OF ALL PROGRAM HEADERS\n");
//  print_program_headers(bin.program_headers);
  printf("PRINTING DATA OF ALL DYNAMIC SEGMENTS\n");
  print_dynamic_segments(bin.dynamic_segment);
//  if (second_stage(&bin)) {
//    printf("Error in second stage\n");
//  }
  return 0;
}