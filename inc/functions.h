//
// Created by louis on 11/2/23.
//

#ifndef WOODY_WOODPACKER_FUNCTIONS_H
#define WOODY_WOODPACKER_FUNCTIONS_H

#include "woody.h"


int read_file(int file, char **result, size_t *len);

int parse_program_headers(t_bin *bin);

int encryption(t_bin *bin);

__attribute__((unused)) void print_program_headers(phdr_list_t *head);

__attribute__((unused)) void print_elf_header(Elf64_Ehdr *header);

__attribute__((unused)) void hexdump(void *data, size_t len, int32_t row);

uint64_t allign_down(uint64_t x, uint64_t align);

uint64_t allign_up(uint64_t x, uint64_t align);

int is_text_segment_64(const void *segment);

void *get_segment(const phdr_list_t *head, int (*callback)(const void *));

void *find_code_cave(t_bin *bin);

int get_key(uint8_t *key);

int save_new_file(t_bin *ptr);

int craft_payload(t_bin *bin);

uint8_t get_uint8(uint8_t *data, uint8_t endian);

uint16_t get_uint16(uint8_t *data, uint8_t endian);

uint32_t get_uint32(uint8_t *data, uint8_t endian);

uint64_t get_uint64(uint8_t *data, uint8_t endian);

#endif //WOODY_WOODPACKER_FUNCTIONS_H
