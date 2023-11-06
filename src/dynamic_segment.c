//
// Created by louis on 11/6/23.
//

#include "woody.h"

#define BASE_ADDRESS 0x400000

void print_dynamic_segment(segment_dyn_t *segment) {
  printf("Tag: 0x%016lx - ", segment->d_tag);
  printf("%s\n", type_dynamic_to_str(segment->d_tag));
  printf("Value: 0x%016lx\n", segment->d_ptr);
}

void print_dynamic_segments(segment_dyn_t *head) {
  printf("\nPARSING AND PRINTING ALL DYNAMIC SEGMENTS\n\n");
  for (segment_dyn_t *tmp = head; tmp != NULL; tmp = tmp->next) {
    print_dynamic_segment(tmp);
    printf("\n");
  }
}

segment_header_t *search_dyn_segment(t_bin *bin) {
  for (segment_header_t *segment = bin->program_headers; segment != NULL; segment = segment->next) {
    if (segment->p_type == PT_DYNAMIC) {
      return segment;
    }
  }
  return NULL;
}

void lst_add_back_segment_dyn(t_bin *bin, segment_dyn_t **head, segment_dyn_t *data) {
  if (*head == NULL) {
    *head = calloc(1, sizeof(segment_dyn_t));
    memcpy(*head, data, SIZE_OF_DYNAMIC_SEGMENT);
    return;
  }
  segment_dyn_t *tmp = bin->dynamic_segment;
  while (tmp->next)
    tmp = tmp->next;
  tmp->next = calloc(1, sizeof(segment_dyn_t));
  memcpy(tmp->next, data, SIZE_OF_DYNAMIC_SEGMENT);
}

int parse_dynamic_segment(t_bin *bin) {
  segment_header_t *segment = search_dyn_segment(bin);
  if (segment == NULL) {
    printf("WARN: No dynamic segment found\n");
    return 0;
  }
  size_t curr_offset = segment->p_offset;
  for (uint idx = 0; idx != segment->p_filesz / sizeof(segment_dyn_t); idx++) {
    segment_dyn_t tmp = {0};
    memcpy(&tmp, bin->raw_data + curr_offset, sizeof(segment_dyn_t));
    if (tmp.d_tag == DT_NULL) {
      break;
    }
    lst_add_back_segment_dyn(bin, &bin->dynamic_segment, &tmp);
    curr_offset += sizeof(segment_dyn_t);
  }
  return 0;
}