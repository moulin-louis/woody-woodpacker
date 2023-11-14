#include "woody.h"

int32_t init(t_bin *bin, char **av) {
  const int32_t file = open(av[1], O_RDONLY);
  if (file == -1) {
    printf("Error opening file\n");
    return 2;
  }
  if (read_file(file, &bin->raw_data, &bin->data_len) != 0) {
    printf("Error reading file\n");
    return 3;
  }
  close(file);
  return 0;
}

int32_t check_elf_header(const Elf64_Ehdr *elf64Ehdr) {
  if (memcmp(elf64Ehdr, "\x7F" "ELF", 4) != 0) {
    printf("This is not an ELF file\n");
    return 1;
  }
  if (elf64Ehdr->e_version != 1) {
    printf("Wrong ELF version\n");
    return 1;
  }
  if (elf64Ehdr->e_ident[EI_CLASS] != ELFCLASS64) {
    printf("Not a 64 bits ELF file\n");
    return 1;
  }
  if (elf64Ehdr->e_ident[EI_DATA] != ELFDATA2LSB) {
    printf("Not a little endian ELF file\n");
    return 1;
  }
  if (elf64Ehdr->e_type != ET_EXEC && elf64Ehdr->e_type != ET_DYN) {
    printf("Not an executable ELF file\n");
    return 1;
  }
  return 0;
}

int main(int ac, char **av) {
  t_bin bin = {};

  if (ac != 2) {
    printf("Wrong usage\n");
    return 1;
  }
  if (init(&bin, av)) {
    printf("Error init\n");
    return 1;
  }
  bin.elf_header = (Elf64_Ehdr *) bin.raw_data;
  if (check_elf_header(bin.elf_header)) {
    printf("Error checking elf header\n");
    return 1;
  }
  parse_program_headers(&bin);
  //check if their some relocation
  if (get_segment(bin.phdrs, is_dyn_segment_64) != NULL) {
    printf("LOG: Found DYNAMIC segment, checking if there is relocation\n");
    check_relocations_presence(&bin);
  } else
    printf("LOG: No DYNAMIC segment found\n");

  if (get_segment(bin.phdrs, is_text_segment_64) == NULL) {
    printf("No text segment found\n");
    return 1;
  }
  //encryption
  if (encryption(&bin)) {
    printf("Error encryption\n");
    return 1;
  }
  //prepare payload
  if (craft_payload(&bin)) {
    printf("Error crafting payload\n");
    return 1;
  }
  //inject payload
  if (find_code_cave(&bin)) {
    printf("Error finding code cave\n");
    return 1;
  }
  //save new file
  if (save_new_file(&bin)) {
    printf("Error saving new file\n");
    return 1;
  }
  return 0;
}