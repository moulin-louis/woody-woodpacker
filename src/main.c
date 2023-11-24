#include "woody.h"
int32_t runtime_32(t_bin *bin);
//init the bin struct
int32_t init(t_bin* bin, char** av) {
  //open the infile in readonly
  const int32_t file = open(av[1], O_RDONLY);
  if (file == -1) {
    printf("Error opening file\n");
    return 2;
  }
  //read the whole file and put in in raw_data
  if (read_file(file, &bin->raw_data, &bin->data_len) != 0) {
    printf("Error reading file\n");
    return 3;
  }
  //close the file
  close(file);
  return 0;
}

//check health elf header from file
int32_t check_elf_header_64(const Elf64_Ehdr* elf64Ehdr) {
  //check for the magic number
  if (memcmp(elf64Ehdr, "\x7F" "ELF", 4) != 0) {
    printf("This is not an ELF file\n");
    return 1;
  }
  //check for the elf version
  if (elf64Ehdr->e_version != 1) {
    printf("Wrong ELF version\n");
    return 1;
  }
  //check for the elf class
  if (elf64Ehdr->e_ident[EI_CLASS] != ELFCLASS64) {
    printf("Not a 64 bits ELF file\n");
    return 1;
  }
  //check for the elf endianness
  if (elf64Ehdr->e_ident[EI_DATA] != ELFDATA2LSB) {
    printf("Not a little endian ELF file\n");
    return 1;
  }
  //check for the elf type
  if (elf64Ehdr->e_type != ET_EXEC && elf64Ehdr->e_type != ET_DYN) {
    printf("Not an executable ELF file\n");
    return 1;
  }
  return 0;
}

int32_t check_elf_header_32(const Elf32_Ehdr* elf32Ehdr) {
  //check for the magic number
  if (memcmp(elf32Ehdr, "\x7F" "ELF", 4) != 0) {
    printf("This is not an ELF file\n");
    return 1;
  }
  //check for the elf version
  if (elf32Ehdr->e_version != 1) {
    printf("Wrong ELF version\n");
    return 1;
  }
  //check for the elf class
  if (elf32Ehdr->e_ident[EI_CLASS] != ELFCLASS32) {
    printf("Not a 64 bits ELF file\n");
    return 1;
  }
  //check for the elf endianness
  if (elf32Ehdr->e_ident[EI_DATA] != ELFDATA2LSB) {
    printf("Not a little endian ELF file\n");
    return 1;
  }
  //check for the elf type
  if (elf32Ehdr->e_type != ET_EXEC && elf32Ehdr->e_type != ET_DYN) {
    printf("Not an executable ELF file\n");
    return 1;
  }
  return 0;
}

int main(int ac, char** av) {
  t_bin bin = {};

  if (ac != 2) {
    printf("Wrong usage\n");
    return 1;
  }

  printf(ANSI_GREEN "LOG: Initialization...........: ");
  //initialization
  if (init(&bin, av)) {
    //print emoji error
    printf(ANSI_RED ANSI_CROSS "\n");
    printf("Error init\n" ANSI_RESET);
    return 1;
  }
  printf(ANSI_CHECK "\n" ANSI_RESET);

  //heatlh check
  if (bin.raw_data[0x4] == 1) {
    return runtime_32(&bin);
  }
  printf(ANSI_GREEN "LOG: Health check.............: ");
  bin.elf64_header = (Elf64_Ehdr *)bin.raw_data;
  if (check_elf_header_64(bin.elf64_header)) {
    printf(ANSI_RED ANSI_CROSS "\n");
    printf("Error checking elf header\n" ANSI_RESET);
    return 1;
  }
  printf(ANSI_CHECK "\n" ANSI_RESET);

  //parse program headers
  printf(ANSI_GREEN "LOG: Parsing headers..........: ");
  if (parse_program_headers_64(&bin)) {
    printf(ANSI_RED ANSI_CROSS "\n");
    printf("Error parsing program headers\n" ANSI_RESET);
    return 1;
  }
  printf(ANSI_CHECK "\n" ANSI_RESET);

  //check if their some relocation
  if (get_segment_64(bin.phdrs_64, is_dyn_segment_64) != NULL) {
    printf(ANSI_YELLOW "WARNING: Found DYNAMIC segment, checking if there is relocation\n");
    if (check_relocations_presence_64(&bin)) {
      printf(ANSI_RED "ERROR: Cant encrypt an executables with relocation in it!!\n" ANSI_RESET);
      return 0;
    }
  }
  else
    printf(ANSI_GREEN "LOG: No DYNAMIC segment found.: " ANSI_CHECK "\n");

  //encryption
  printf(ANSI_GREEN "LOG: Encryption...............: ");
  if (encryption_64(&bin)) {
    printf(ANSI_RED ANSI_CROSS "\n");
    printf("Error encryption\n" ANSI_RESET);
    return 1;
  }
  printf(ANSI_CHECK "\n" ANSI_RESET);

  //prepare payload
  printf(ANSI_GREEN "LOG: Crafting payload.........: ");
  if (craft_payload_64(&bin)) {
    printf(ANSI_RED ANSI_CROSS "\n");
    printf("Error crafting payload\n" ANSI_RESET);
    return 1;
  }
  printf(ANSI_CHECK "\n" ANSI_RESET);

  //inject payload
  printf(ANSI_GREEN "LOG: Injecting payload........: ");
  if (find_code_cave_64(&bin)) {
    printf(ANSI_RED ANSI_CROSS "\n");
    printf("Error finding code cave\n" ANSI_RESET);
    return 1;
  }
  printf(ANSI_CHECK "\n" ANSI_RESET);

  //save new file
  printf(ANSI_GREEN "LOG: Creating woody file......: ");
  if (save_new_file(&bin)) {
    printf(ANSI_RESET ANSI_CROSS "\n");
    printf("Error saving new file\n" ANSI_RESET);
    return 1;
  }
  printf(ANSI_CHECK "\n" ANSI_RESET);

  //cleanup
  printf(ANSI_GREEN "LOG: Cleaning up ressources...: ");
  free(bin.key);
  free(bin.raw_data);
  free(bin.payload);
  for (phdr_list_64_t* tmp = bin.phdrs_64; tmp != NULL;) {
    phdr_list_64_t* next = tmp->next;
    free(tmp);
    tmp = next;
  }
  printf(ANSI_CHECK ANSI_RESET "\n");

  return 0;
}

int32_t runtime_32(t_bin *bin) {
  printf(ANSI_RED "WARNING: THIS IS A 32 BITS ELF\n" ANSI_RESET);
  printf(ANSI_GREEN "LOG: Health check.............: ");
  bin->elf32_header = (Elf32_Ehdr *)bin->raw_data;
  if (check_elf_header_32(bin->elf32_header)) {
    printf(ANSI_RED ANSI_CROSS "\n");
    printf("Error checking elf header\n" ANSI_RESET);
    return 1;
  }
  printf(ANSI_CHECK "\n" ANSI_RESET);

  //parse program headers
  printf(ANSI_GREEN "LOG: Parsing headers..........: ");
  if (parse_program_headers_32(bin)) {
    printf(ANSI_RED ANSI_CROSS "\n");
    printf("Error parsing program headers\n" ANSI_RESET);
    return 1;
  }
  printf(ANSI_CHECK "\n" ANSI_RESET);

  //check if their some relocation
  if (get_segment_32(bin->phdrs_32, is_dyn_segment_32) != NULL) {
    printf(ANSI_YELLOW "WARNING: Found DYNAMIC segment, checking if there is relocation\n");
    if (check_relocations_presence_32(bin)) {
      printf(ANSI_RED "ERROR: Cant encrypt an executables with relocation in it!!\n" ANSI_RESET);
      return 0;
    }
  }
  else
    printf(ANSI_GREEN "LOG: No DYNAMIC segment found.: " ANSI_CHECK "\n");

  //encryption
  printf(ANSI_GREEN "LOG: Encryption...............: ");
  if (encryption_32(bin)) {
    printf(ANSI_RED ANSI_CROSS "\n");
    printf("Error encryption\n" ANSI_RESET);
    return 1;
  }
  printf(ANSI_CHECK "\n" ANSI_RESET);

  //prepare payload
  printf(ANSI_GREEN "LOG: Crafting payload.........: ");
  if (craft_payload_32(bin)) {
    printf(ANSI_RED ANSI_CROSS "\n");
    printf("Error crafting payload\n" ANSI_RESET);
    return 1;
  }
  printf(ANSI_CHECK "\n" ANSI_RESET);

  //inject payload
  printf(ANSI_GREEN "LOG: Injecting payload........: ");
  if (find_code_cave_32(bin)) {
    printf(ANSI_RED ANSI_CROSS "\n");
    printf("Error finding code cave\n" ANSI_RESET);
    return 1;
  }
  printf(ANSI_CHECK "\n" ANSI_RESET);

  //save new file
  printf(ANSI_GREEN "LOG: Creating woody file......: ");
  if (save_new_file(bin)) {
    printf(ANSI_RESET ANSI_CROSS "\n");
    printf("Error saving new file\n" ANSI_RESET);
    return 1;
  }
  printf(ANSI_CHECK "\n" ANSI_RESET);

  //cleanup
  printf(ANSI_GREEN "LOG: Cleaning up ressources...: ");
  free(bin->key);
  free(bin->raw_data);
  free(bin->payload);
  for (phdr_list_32_t* tmp = bin->phdrs_32; tmp != NULL;) {
    phdr_list_32_t* next = tmp->next;
    free(tmp);
    tmp = next;
  }
  printf(ANSI_CHECK ANSI_RESET "\n");
  return 0;
}
