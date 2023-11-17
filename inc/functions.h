//
// Created by louis on 11/2/23.
//

#ifndef WOODY_WOODPACKER_FUNCTIONS_H
#define WOODY_WOODPACKER_FUNCTIONS_H

//I/O FUNCTION
int32_t read_file(int32_t file, uint8_t** result, uint64_t* len);

int32_t save_new_file(const t_bin* ptr);

//PARSING FUNCTION
int32_t parse_program_headers(t_bin* bin);

//RELOCATIONS FUNCTION
int check_relocations_presence(const t_bin* bin);

//ENCRYPTION FUNCTION
int32_t get_key(uint8_t* key);

int32_t encryption(t_bin* bin);

//PAYLOAD FUNCTION
int32_t craft_payload(t_bin* bin);

int find_code_cave(t_bin* bin);

//UTILS FUNCTION
void* get_segment(const phdr_list_t* head, int32_t (*callback)(const void*));

int32_t is_text_segment_64(const void* segment);

int32_t is_dyn_segment_64(const void* segment);

//DEBUG FUNCTIONS
void print_dyn_tag(const Elf64_Dyn* tag);

__attribute__((unused)) void print_info_payload(t_bin* bin);

__attribute__((unused)) void hexdump(void* data, uint64_t len, int32_t row);

__attribute__((unused)) void print_elf_header(Elf64_Ehdr* header);

__attribute__((unused)) void print_program_headers(phdr_list_t* head);

#endif //WOODY_WOODPACKER_FUNCTIONS_H
