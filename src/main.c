#include "woody.h"

uint64_t base_address;

int init(t_bin *bin, char **av) {
  int file = open(av[1], O_RDONLY);
  if (file == -1) {
    printf("Error opening file\n");
    return 2;
  }
  if (read_file(file, (char **) &bin->raw_data, &bin->data_len) != 0) {
    printf("Error reading file\n");
    return 3;
  }
  close(file);
  if (bin->data_len < sizeof(elf_header_t)) {
    printf("File too small\n");
    return 4;
  }
  return 0;
}

int main(int ac, char **av) {
  t_bin bin = {};

  base_address = BASE_ADDRESS;
  if (ac != 2) {
    printf("Wrong usage\n");
    return 1;
  }
  if (init(&bin, av)) {
    printf("Error init\n");
    return 1;
  }
  memcpy(&bin.header, bin.raw_data, sizeof(elf_header_t));
  if (memcmp(bin.header.magic, "\x7F" "ELF", 4) != 0) {
    printf("This is not an ELF file\n");
    return 4;
  }
  if (bin.header.e_type == 0x02) {
    base_address = 0;
  }
  if (parse_program_headers(&bin)) {
    printf("Error parsing program headers\n");
    return 5;
  }
  parse_dynamic_segment(&bin);
  making_relocations(&bin);
  // print_elf_header(&bin.header);
  // print_program_headers(bin.program_headers);
  print_dynamic_segments(bin.dynamic_segment);
  // if (second_stage(&bin)) {
  //   printf("Error in second stage\n");
  // }
  return 0;
}