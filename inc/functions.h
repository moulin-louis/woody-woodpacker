//
// Created by louis on 11/2/23.
//

#ifndef WOODY_WOODPACKER_FUNCTIONS_H
#define WOODY_WOODPACKER_FUNCTIONS_H

#include <sys/types.h>
#include <stdint.h>
#include "structure.h"

__attribute__((unused)) void hexdump(void *data, size_t len, int32_t row);
int read_file(int file, char **result, size_t *len);
void print_elf_header(elf_header_t *header);
void hangup(void);
void cleanup(t_bin *bin);
void asciidump(void *data, size_t len, uint32_t row);

/*PROGRAM HEADER STUFF*/
void lst_add_back_program_header(program_header_list_t **head, program_header_t *data);
program_header_list_t *lst_new_program_header(program_header_t *data);
void parse_program_headers(t_bin *bin);
void print_program_headers(program_header_list_t *head);
void print_program_header(program_header_t *programHeader);
char *type_program_to_str(typeProgram type_program);

/*SECTION HEADER STUFF*/
void lst_add_back_section_header(section_header_list_t **head, section_header_t *data);
section_header_list_t *lst_new_section_header(section_header_t *data);
void parse_section_headers(t_bin *bin);
void print_section_headers(section_header_list_t *head);
void print_section_header(section_header_t *sectionHeader);

#endif //WOODY_WOODPACKER_FUNCTIONS_H
