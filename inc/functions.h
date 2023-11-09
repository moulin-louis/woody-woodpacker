//
// Created by louis on 11/2/23.
//

#ifndef WOODY_WOODPACKER_FUNCTIONS_H
#define WOODY_WOODPACKER_FUNCTIONS_H

#include "structure.h"
#include "woody.h"


int read_file(int file, char **result, size_t *len);

int parse_program_headers(t_bin *bin);

int encryption(t_bin *bin);

__attribute__((unused)) void print_program_headers(phdr_list_t *head);

__attribute__((unused)) void print_elf_header(Elf64_Ehdr *header);

__attribute__((unused)) void hexdump(void *data, size_t len, int32_t row);

uint64_t allign_down(uint64_t x, uint64_t align);

uint64_t allign_up(uint64_t x, uint64_t align);

#endif //WOODY_WOODPACKER_FUNCTIONS_H
