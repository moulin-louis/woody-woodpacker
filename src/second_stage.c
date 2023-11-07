//
// Created by louis on 11/7/23.
//

#include "woody.h"

int setup_permission(segment_header_t *program_header, uint64_t address, size_t len) {
  int protection = PROT_NONE;
  printf("setup permission: ");
  if (program_header->p_flags & 0x4) {
    printf("Read ");
    protection |= PROT_READ;
  }
  if (program_header->p_flags & 0x2) {
    printf("Write ");
    protection |= PROT_WRITE;
  }
  if (program_header->p_flags & 0x1) {
    printf("Execute ");
    protection |= PROT_EXEC;
  }
  printf("\n");
  printf("SETUP_PERMISSION: address = %p\n", (void *) address);
  printf("SETUP_PERMISSION: len = 0x%lx\n", len);
  printf("SETUP_PERMISSION: protection = %d\n", protection);
  if (mprotect((void *) address, len, protection) == -1) {
    perror("mprotect");
    return 1;
  }
  return 0;
}

int setup_permission_segments(t_bin *bin) {
  for (segment_header_t *segment = bin->program_headers; segment != NULL; segment = segment->next) {
    if (segment->p_type != PT_LOAD)
      continue;
    uint64_t address = segment->p_vaddr + base_address;
    uint64_t allignAddress = ALIGN(address, segment->p_align);
    uint64_t padding = address - allignAddress;
    size_t len = segment->p_memsz + padding;
    if (allignAddress > address) {
      allignAddress -= segment->p_align;
    }
    if (len < segment->p_align)
      len = segment->p_align;
    if (setup_permission(segment, allignAddress, len))
      return 1;
    printf("\n");
  }
  return 0;
}

void init_memory(void *address, segment_header_t *segment, t_bin *bin) {
  if (segment->p_memsz == segment->p_filesz) {
    printf("memsz == filesz\n");
    memcpy(address, bin->raw_data + segment->p_offset, segment->p_filesz);
    return;
  } else if (segment->p_memsz > segment->p_filesz) {
    printf("memsz > filesz\n");
    memcpy(address, bin->raw_data + segment->p_offset, segment->p_filesz);
    memset(address + segment->p_filesz, 0, segment->p_memsz - segment->p_filesz);
    return;
  } else if (segment->p_memsz < segment->p_filesz) {
    printf("memsz < filesz\n");
    memcpy(address, bin->raw_data + segment->p_offset, segment->p_memsz);
    return;
  }
  memset(address, 0, segment->p_memsz);
}

void init_segments(t_bin *bin) {
  for (segment_header_t *segment = bin->program_headers; segment; segment = segment->next) {
    if (segment->p_type != PT_LOAD) {
      continue;
    }
    uint64_t address = segment->p_vaddr + base_address;
    printf("address = %p\n", (void *) address);
    hangup();
    init_memory((void *) address, segment, bin);
  }
}

int mapping_all_segment(t_bin *bin) {
  for (segment_header_t *segment = bin->program_headers; segment != NULL; segment = segment->next) {
    if (segment->p_type != PT_LOAD) {
      continue;
    }
    uint64_t address = segment->p_vaddr + base_address;
    uint64_t allignAddress = ALIGN(address, segment->p_align);
    uint64_t padding = allignAddress - address;
    size_t len = segment->p_memsz + padding;
    if (len < segment->p_align)
      len = segment->p_align;
    void *result = mmap((void *) allignAddress, len, PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
    if (result == MAP_FAILED) {
      perror("mmap");
      printf("address = %p\n", (void *) address);
      printf("allignAddress = %p\n", (void *) allignAddress);
      printf("len = %lu\n", len);
      return 1;
    }
  }
  return 0;
}

int second_stage(t_bin *bin) {
  printf("pid = %d\n", getpid());
  printf("mapping all loadable segments\n");
  if (mapping_all_segment(bin)) {
    printf("Error mapping segment\n");
    return 1;
  }
  init_segments(bin);
  if (setup_permission_segments(bin)) {
    printf("Error setting up permission\n");
    return 1;
  }
  uint64_t addressEntry = bin->header.e_entry + base_address;
  printf("Jumping to entry point\n");
  fflush(stdout);
  cleanup(bin);
  ((void (*)(void)) addressEntry)();
  return 0;
}