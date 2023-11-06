#include "woody.h"

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

int setup_permission_segments(t_bin *bin) {
  for (segment_header_t *segment = bin->program_headers; segment != NULL; segment = segment->next) {
    if (segment->p_type != PT_LOAD)
      continue;
    uint64_t address = segment->p_vaddr + BASE_ADDRESS;
    uint64_t allign_address = ALIGN(address, segment->p_align);
    uint64_t padding = address - allign_address;
    size_t len = segment->p_memsz + padding;
    if (len < segment->p_align)
      len = segment->p_align;
    if (setup_permission(segment, allign_address, len))
      return 1;
    printf("\n");
  }
  return 0;
}

void init_memory(void *result, segment_header_t *segment, t_bin *bin) {
  if (segment->p_memsz == segment->p_filesz) {
    printf("memsz == filesz\n");
    memcpy(result, bin->raw_data + segment->p_offset, segment->p_filesz);
    return;
  } else if (segment->p_memsz > segment->p_filesz) {
    printf("memsz > filesz\n");
    memcpy(result, bin->raw_data + segment->p_offset, segment->p_filesz);
    memset(result + segment->p_filesz, 0, segment->p_memsz - segment->p_filesz);
    return;
  } else if (segment->p_memsz < segment->p_filesz) {
    printf("memsz < filesz\n");
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
    uint64_t allign_address = ALIGN(address, segment->p_align);
    uint64_t padding = allign_address - address ;
    size_t len = segment->p_memsz + padding;
    if (len < segment->p_align) {
      len = segment->p_align;
    }
    void *result = mmap((void *)allign_address, len, PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
    if (result == MAP_FAILED) {
      perror("mmap");
      return 1;
    }
    printf("address = %p\n", (void *)address);
    printf("allign_address = %p\n", (void *)allign_address);
    printf("len = %lu\n", len);
    printf("memsz = %lu\n", segment->p_memsz);
    printf("filesz = %lu\n", segment->p_filesz);
//    hangup();
    init_memory((void *)address, segment, bin);
    printf("\n");
  }
  if (setup_permission_segments(bin)) {
    printf("Error setting up permission\n");
    return 1;
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
  uint64_t addres_entry = bin->header.e_entry + BASE_ADDRESS;
  printf("Jumping to entry point\n");
  fflush(stdout);
  cleanup(bin);
//  pid_t pid = fork();
//  if (pid == -1) {
//    perror("fork");
//    return 1;
//  }
//  if (pid == 0) {
  ((void (*)(void)) addres_entry)();
//    return 0;
//  } else {
//    int status;
//    waitpid(pid, &status, 0);
//    return WEXITSTATUS(status);
//  }
  return 0;
}

int init(t_bin *bin, char **av) {
  int file = open(av[1], O_RDONLY);
  if (file == -1) {
    printf("Error opening file\n");
    return 2;
  }
  if (read_file(file, (char **)&bin->raw_data, &bin->data_len) != 0) {
    printf("Error reading file\n");
    return 3;
  }
  close(file);
  if (bin->data_len < sizeof(elf_header_t)) {
    printf("File too small\n");
    return 4;
  }
  return 0;
}

int main(int ac, char **av) {
  t_bin bin = {};

  if (ac != 2) {
    printf("Wrong usage\n");
    return 1;
  }
  if (init(&bin, av)) {
    printf("Error init\n");
    return 1;
  }

  memcpy(&bin.header, bin.raw_data, sizeof(elf_header_t));
  if (memcmp(bin.header.magic, "\x7F" "ELF", 4) != 0) {
    printf("This is not an ELF file\n");
    return 4;
  }
  if (parse_program_headers(&bin)) {
    printf("Error parsing program headers\n");
    return 5;
  }
  if (parse_dynamic_segment(&bin)) {
    printf("Error parsing dynamic segment\n");
    return 6;
  }
  if (making_relocations(&bin)) {
    printf("Error making relocations\n");
    return 7;
  }
//  if (second_stage(&bin)) {
//    printf("Error in second stage\n");
//  }
  return 0;
}