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
  return 0;
}

int check_elf_header(Elf64_Ehdr *elf64Ehdr) {
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

int save_new_file(t_bin *ptr) {
  int fd = open("./woody", O_WRONLY | O_CREAT , 0777);
  if (fd == -1) {
    printf("Error opening file\n");
    return 1;
  }
  write(fd, ptr->raw_data, ptr->data_len);
  close(fd);
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
//   memcpy(&bin.elf_header, bin.raw_data, sizeof(Elf64_Ehdr));
  bin.elf_header = (Elf64_Ehdr *)bin.raw_data;	
  if (check_elf_header(bin.elf_header)) {
    printf("Error checking elf header\n");
    return 1;
  }
  parse_program_headers(&bin);
  //encryption
  if (encryption(&bin)) {
    printf("Error encryption\n");
    return 1;
  }
  find_code_cave(&bin);
  //prepare payload
  //inject payload
  //save new file
  if (save_new_file(&bin)) {
    printf("Error saving new file\n");
    return 1;
  }
  return 0;
}