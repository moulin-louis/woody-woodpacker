//
// Created by loumouli on 11/10/23.
//

#include "woody.h"

unsigned char blueprint_payload[80] = {
    // Offset 0x00001000 to 0x0000104F
    0x2E, 0x2E, 0x2E, 0x2E, 0x57, 0x4F, 0x4F, 0x44, 0x59, 0x2E, 0x2E, 0x2E, 0x2E,
    0x0A, 0x0E, 0xB8, 0x01, 0x00, 0x00, 0x00, 0xBF, 0x01, 0x00, 0x00, 0x00,

    0x48,0x8D, 0x35, 0xE0, 0xFF, 0xFF, 0xFF, 0xBA, 0x0E, 0x00, 0x00, 0x00, 0x0F, 0x05,
    0x48, 0x8D, 0x3D, 0x1D, 0x00, 0x00, 0x00, 0xBE, 0x01, 0x00, 0x00, 0x00, 0x48,
    0x8D, 0x15, 0x11, 0x00, 0x00, 0x00, 0xB9, 0x01, 0x00, 0x00, 0x00, 0x4C, 0x8D,
    0x05, 0x05, 0x00, 0x00, 0x00, 0xE9, 0xFD, 0xFF, 0xFF, 0xFF, 0xE9, 0xFD, 0xFF,
    0xFF, 0xFF
};

unsigned char decrypt_fn[49] = {
    // Offset 0x00001000 to 0x00001030
    0x41, 0x50, 0x49, 0x89, 0xF9, 0x48, 0x89, 0xD7, 0x49, 0x89, 0xC8, 0xB9, 0x00,
    0x00, 0x00, 0x00, 0x48, 0x89, 0xC8, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x48, 0xF7,
    0xF6, 0x41, 0x0F, 0xB6, 0x04, 0x11, 0x30, 0x04, 0x0F, 0x48, 0x83, 0xC1, 0x01,
    0x49, 0x39, 0xC8, 0x75, 0xE4, 0x41, 0x58, 0x41, 0xFF, 0xE0
};

int craft_payload(t_bin *bin) {
  Elf64_Phdr *text_segment = get_segment(bin->phdrs, is_text_segment_64);
  //init payload
  bin->payload = malloc(sizeof(blueprint_payload) + sizeof (decrypt_fn) + bin->len_key);

  memcpy(bin->payload, blueprint_payload, sizeof(blueprint_payload));

  memcpy(bin->payload + sizeof(blueprint_payload), bin->key, bin->len_key);

  memcpy(bin->payload + sizeof(blueprint_payload) + bin->len_key, decrypt_fn, sizeof(decrypt_fn));
  bin->len_payload = sizeof(blueprint_payload) + sizeof(decrypt_fn) + bin->len_key;
//  change key offset
  *((uint32_t *)(bin->payload + PAYLOAD_OFFSET_KEY)) = ((uint32_t)0x22);
//  change key_len
  *((uint32_t *)(bin->payload + PAYLOAD_OFFSET_KEY_LEN)) = bin->len_key;
//  change data offset
  uint32_t size_to_data = 0x3a;
  printf("text_segment->p_memsz = %#lx\n", text_segment->p_memsz);
  printf("size to data = %#x\n", size_to_data);
  printf("text_segment->p_memsz + size_to_data = %#lx\n", text_segment->p_memsz + size_to_data);
  *((uint32_t *)(bin->payload + PAYLOAD_OFFSET_DATA)) = -(size_to_data + text_segment->p_memsz);
//  change data_len
  *((uint32_t *)(bin->payload + PAYLOAD_OFFSET_DATA_LEN)) = text_segment->p_filesz;
//  change decrypt_fn jmp offset
  *((uint32_t *)(bin->payload + PAYLOAD_OFFSET_DECRYPT_FN)) = 0x25;
//  TODO !!!!!
//  change og_entry jmp offset
//  printf("diff to jmp = %#lx\n", -(sizeof(blueprint_payload) + text_segment->p_filesz - (bin->elf_header->e_entry - text_segment->p_offset)));
//  *((uint32_t *)(bin->payload + PAYLOAD_OFFSET_OG_ENTRY)) = -(sizeof(blueprint_payload) + text_segment->p_filesz - (bin->elf_header->e_entry - text_segment->p_offset));
  print_info_payload(bin);
  return 0;
}

__attribute__((unused)) void print_info_payload(t_bin *bin) {
//  printf("key_ptr in payload = %#x\n", get_uint32(bin->payload + PAYLOAD_OFFSET_KEY, bin->elf_header->e_ident[EI_DATA]));
//  printf("key_len in payload = %#x\n", get_uint32(bin->payload + PAYLOAD_OFFSET_KEY_LEN, bin->elf_header->e_ident[EI_DATA]));
  printf("data offset in payload = %#x\n", get_uint32(bin->payload + PAYLOAD_OFFSET_DATA, bin->elf_header->e_ident[EI_DATA]));
//  printf("data_len in payload = %#x\n", get_uint32(bin->payload + PAYLOAD_OFFSET_DATA_LEN, bin->elf_header->e_ident[EI_DATA]));
//  printf("HEXDUMP payload: \n");
//  hexdump(bin->payload, sizeof(blueprint_payload), 0);
//  printf("HEXDUMP key: \n");
//  hexdump(bin->payload + sizeof(blueprint_payload), bin->len_key, 0);
//  printf("HEXDUMP decrypt_fn: \n");
//  hexdump(bin->payload + sizeof(blueprint_payload) + bin->len_key, sizeof(decrypt_fn), 0);
  printf("total hexdump: \n");
  hexdump(bin->payload, bin->len_payload, 0);
}