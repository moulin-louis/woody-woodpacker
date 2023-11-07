//
// Created by louis on 11/6/23.
//

#include "woody.h"

void making_relocations(t_bin *bin) {
  if (search_segment_type(bin, PT_DYNAMIC) == NULL) {
    printf("NO DYNAMIC SEGMENT SO NO RELOCATIONS\n");
    return;
  }
  segment_dyn_t *rela_segment = search_segment_dyn_type(bin, DT_RELA);
  segment_dyn_t *relazs_segment = search_segment_dyn_type(bin, DT_RELASZ);
  segment_dyn_t *relaent_segment = search_segment_dyn_type(bin, DT_RELAENT);
  if (!rela_segment || !relazs_segment || !relaent_segment) {
    printf("NO RELA/RELASZ/RELAENT SEGMENT\n");
    return;
  }
//  print_dynamic_segment(rela_segment);
//  print_dynamic_segment(relazs_segment);
//  print_dynamic_segment(relaent_segment);
  uint64_t rela_size = relazs_segment->d_val;
  uint64_t rela_ent_size = relaent_segment->d_val;
  for (uint idx = 0; idx < rela_ent_size; idx += rela_size) {
    relocation_type_t relocation;
    memcpy(&relocation, bin->raw_data + rela_segment->d_ptr, sizeof(relocation_type_t));
    printf("RELOCATION: r_offset = 0x%lx\n", relocation.r_offset);
    printf("RELOCATION: type = 0x%x\n", relocation.type);
    printf("RELOCATION: sym = 0x%x\n", relocation.sym);
    printf("RELOCATION: r_addend = 0x%lx\n", relocation.r_addend);
    uint64_t data = *(uint64_t *) (bin->raw_data + relocation.r_offset);
    data += base_address;
    *(uint64_t *) (bin->raw_data + relocation.r_offset) = data;
    printf("data at offset after relocation= 0x%lx\n", *(uint64_t *) (bin->raw_data + relocation.r_offset));
  }
}