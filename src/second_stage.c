//
// Created by louis on 11/7/23.
//

#include "woody.h"

void calculate_address_n_len(segment_header_t *segment, uint64_t *address, uint64_t *allignAddress, size_t *len, uint64_t *prevSegmentEnd) {
  *address = segment->p_vaddr + base_address;
  *allignAddress = allign_down(*address, segment->p_align);
  *len = allign_up(segment->p_memsz, segment->p_align);
  if (*prevSegmentEnd && *allignAddress < *prevSegmentEnd) {
    *allignAddress = allign_up(*prevSegmentEnd, segment->p_align);
  }
}

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
  uint64_t prevSegmentEnd = 0;
  for (segment_header_t *segment = bin->program_headers; segment != NULL; segment = segment->next) {
    if (segment->p_type != PT_LOAD)
      continue;
    uint64_t address, allignAddress;
    size_t len;
    calculate_address_n_len(segment, &address, &allignAddress, &len, &prevSegmentEnd);
    if (setup_permission(segment, allignAddress, len))
      return 1;
    printf("\n");
    prevSegmentEnd = allignAddress + len;
  }
  return 0;
}

void init_memory(void *address, segment_header_t *segment, t_bin *bin) {
  if (segment->p_filesz > 0) {
    memcpy(address, bin->raw_data + segment->p_offset, segment->p_filesz);
  }
  if (segment->p_memsz > segment->p_filesz) {
    memset(address + segment->p_filesz, 0, segment->p_memsz - segment->p_filesz);
  }
}

void init_segments(t_bin *bin) {
  for (segment_header_t *segment = bin->program_headers; segment; segment = segment->next) {
    if (segment->p_type != PT_LOAD) {
      continue;
    }
    uint64_t address = segment->p_vaddr + base_address;
    printf("address = %p\n", (void *) address);
    // hangup();
    init_memory((void *) address, segment, bin);
  }
}

int mapping_all_segment(t_bin *bin) {
  uint64_t prevSegmentEnd = 0;
  for (segment_header_t *segment = bin->program_headers; segment != NULL; segment = segment->next) {
    if (segment->p_type != PT_LOAD) {
      continue;
    }
    uint64_t address = 0;
    uint64_t allignAddress = 0;
    size_t len = 0;
    calculate_address_n_len(segment, &address, &allignAddress, &len, &prevSegmentEnd);
    void *result = mmap((void *) allignAddress, len, PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
    printf("address = %p\n", (void *) address);
    printf("allignAddress = %p\n", (void *) allignAddress);
    printf("len = %lu\n", len);
    printf("memss = %lu\n", segment->p_memsz);
    printf("\n");
    if (result == MAP_FAILED) {
      perror("mmap");
      return 1;
    }
    prevSegmentEnd = allignAddress + len;
  }
  return 0;
}

int second_stage(t_bin *bin) {
  printf("pid = %d\n", getpid());
  printf("mapping all loadable segments\n\n");
  if (mapping_all_segment(bin)) {
    printf("Error mapping segment\n");
    return 1;
  }
   hangup();
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