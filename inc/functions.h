//
// Created by louis on 11/2/23.
//

#ifndef WOODY_WOODPACKER_FUNCTIONS_H
#define WOODY_WOODPACKER_FUNCTIONS_H

#include "structure.h"

__attribute__((unused)) void hexdump(void *data, size_t len, int32_t row);
int read_file(int file, char **result, size_t *len);
__attribute__((unused)) void print_elf_header(elf_header_t *header);
__attribute__((unused)) void hangup(void);
void cleanup(t_bin *bin);
__attribute__((unused)) void asciidump(void *data, size_t len, uint32_t row);
int parse_dynamic_segment(t_bin *bin);
char *type_dynamic_to_str(typeDynamic tag);
void print_dynamic_segments(segment_dyn_t *head);
void print_dynamic_segment(segment_dyn_t *segment);
segment_header_t *search_segment_type(t_bin *bin, typeProgram type_program);
segment_dyn_t *search_segment_dyn_type(t_bin *bin, typeDynamic type_dynamic);

int making_relocations(t_bin *bin);

/*PROGRAM HEADER STUFF*/
void lst_add_back_program_header(t_bin* bin, segment_header_t **head, segment_header_t *data);
int parse_program_headers(t_bin *bin);
void print_program_headers(segment_header_t *head);
void print_program_header(segment_header_t *programHeader);
char *type_program_to_str(typeProgram type_program);

#endif //WOODY_WOODPACKER_FUNCTIONS_H
