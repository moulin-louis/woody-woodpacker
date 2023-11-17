//
// Created by louis on 11/2/23.
//

#ifndef WOODY_WOODPACKER_FUNCTIONS_H
#define WOODY_WOODPACKER_FUNCTIONS_H

//I/O FUNCTION
int32_t read_file(int32_t file, uint8_t** result, uint64_t* len);

int32_t save_new_file(const t_bin* ptr);

//PARSING FUNCTION
int32_t parse_program_headers_64(t_bin* bin);
int32_t parse_program_headers_32(t_bin* bin);

//RELOCATIONS FUNCTION
int32_t check_relocations_presence_64(const t_bin* bin);
int32_t check_relocations_presence_32(const t_bin* bin);

//ENCRYPTION FUNCTION
int32_t get_key(uint8_t* key);

int32_t encryption_64(t_bin* bin);
int32_t encryption_32(t_bin* bin);

//PAYLOAD FUNCTION
int32_t craft_payload_64(t_bin* bin);
int32_t craft_payload_32(t_bin* bin);

int find_code_cave_64(t_bin* bin);
int find_code_cave_32(t_bin* bin);

//UTILS FUNCTION
void* get_segment_64(const phdr_list_64_t* head, int32_t (*callback)(const Elf64_Phdr*));
void* get_segment_32(const phdr_list_32_t* head, int32_t (*callback)(const Elf32_Phdr*));

int32_t is_text_segment_64(const Elf64_Phdr* segment);
int32_t is_text_segment_32(const Elf32_Phdr* segment);

int32_t is_dyn_segment_64(const Elf64_Phdr* segment);
int32_t is_dyn_segment_32(const Elf32_Phdr* segment);

//DEBUG FUNCTIONS
void print_dyn_tag_64(const Elf64_Dyn* tag);
void print_dyn_tag_32(const Elf32_Dyn* tag);

__attribute__((unused)) void print_info_payload(t_bin* bin);

__attribute__((unused)) void hexdump(void* data, uint64_t len, int32_t row);

__attribute__((unused)) void print_elf_header_64(Elf64_Ehdr* header);
__attribute__((unused)) void print_elf_header_32(Elf32_Ehdr* header);

__attribute__((unused)) void print_program_headers_64(phdr_list_64_t* head);
__attribute__((unused)) void print_program_headers_32(phdr_list_32_t* head);

#endif //WOODY_WOODPACKER_FUNCTIONS_H
