//
// Created by louis on 11/2/23.
//

#ifndef WOODY_WOODPACKER_FUNCTIONS_H
#define WOODY_WOODPACKER_FUNCTIONS_H

int32_t read_file(int32_t file, uint8_t **result, uint64_t *len);

int32_t parse_program_headers(t_bin *bin);

int32_t encryption(t_bin *bin);

__attribute__((unused)) void print_program_headers(phdr_list_t *head);

__attribute__((unused)) void print_elf_header(Elf64_Ehdr *header);

__attribute__((unused)) void hexdump(void *data, uint64_t len, int32_t row);

int32_t is_text_segment_64(const void *segment);

int32_t is_dyn_segment_64(const void *segment);

int check_relocations_presence(const t_bin *bin);

void *get_segment(const phdr_list_t *head, int32_t (*callback)(const void *));

int find_code_cave(t_bin *bin);

int32_t get_key(uint8_t *key);

int32_t save_new_file(const t_bin *ptr);

int32_t craft_payload(t_bin *bin);

uint8_t get_uint8(uint8_t *data, uint8_t endian);

uint16_t get_uint16(uint8_t *data, uint8_t endian);

uint32_t get_uint32(uint8_t *data, uint8_t endian);

uint64_t get_uint64(uint8_t *data, uint8_t endian);

__attribute__((unused)) void print_info_payload(t_bin *bin);

#endif //WOODY_WOODPACKER_FUNCTIONS_H
