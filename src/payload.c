//
// Created by loumouli on 11/10/23.
//

#include "woody.h"

unsigned char PAYLOAD[73] = {
    0x2E, 0x2E, 0x2E, 0x2E, 0x57, 0x4F, 0x4F, 0x44, 0x59, 0x2E, 0x2E, 0x2E, 0x2E,
    0x0A, 0x0E, 0xB8, 0x01, 0x00, 0x00, 0x00, 0xBF, 0x01, 0x00, 0x00, 0x00, 0x48,
    0x8D, 0x35, 0xE0, 0xFF, 0xFF, 0xFF, 0xBA, 0x0E, 0x00, 0x00, 0x00, 0x0F, 0x05,
    0x48, 0x8D, 0x35, 0xD2, 0xFF, 0xFF, 0xFF, 0xBF, 0x01, 0x00, 0x00, 0x00, 0x48,
    0x8D, 0x15, 0xC6, 0xFF, 0xFF, 0xFF, 0xB9, 0x01, 0x00, 0x00, 0x00, 0xE9, 0xFD,
    0xFF, 0xFF, 0xFF, 0xE9, 0xFD, 0xFF, 0xFF, 0xFF
};

int craft_payload(t_bin *bin) {
  //init payload
  bin->payload = malloc(sizeof(PAYLOAD) + bin->len_key);
  memcpy(bin->payload, PAYLOAD, sizeof(PAYLOAD));
//  memcpy(bin->payload + sizeof(PAYLOAD), bin->key, bin->len_key);
  bin->len_payload = sizeof(PAYLOAD) + bin->len_key;
  //print info payload
  *((uint32_t *)(bin->payload + PAYLOAD_OFFSET_KEY)) = 0x1B;
  //change key_len
  *((uint32_t *)(bin->payload + PAYLOAD_OFFSET_KEY_LEN)) = bin->len_key;
  //change data_ptr
  *((uint32_t *)(bin->payload + PAYLOAD_OFFSET_DATA)) = bin->elf_header->e_entry;
  //change data_len
  Elf64_Phdr *text_segment = get_segment(bin->phdrs, is_text_segment_64);
  *((uint32_t *)(bin->payload + PAYLOAD_OFFSET_DATA_LEN)) = text_segment->p_filesz;
  printf("key_ptr in payload = %#x\n", get_uint32(bin->payload + PAYLOAD_OFFSET_KEY, bin->elf_header->e_ident[EI_DATA]));
  printf("key_len in payload = %#x\n", get_uint32(bin->payload + PAYLOAD_OFFSET_KEY_LEN, bin->elf_header->e_ident[EI_DATA]));
  printf("data_ptr in payload = %#x\n", get_uint32(bin->payload + PAYLOAD_OFFSET_DATA, bin->elf_header->e_ident[EI_DATA]));
  printf("data_len in payload = %#x\n", get_uint32(bin->payload + PAYLOAD_OFFSET_DATA_LEN, bin->elf_header->e_ident[EI_DATA]));
  printf("decrypt_fn in payload = %#x\n", get_uint32(bin->payload + PAYLOAD_OFFSET_DECRYPT_FN, bin->elf_header->e_ident[EI_DATA]));
  printf("og_entry in payload = %#x\n", get_uint32(bin->payload + PAYLOAD_OFFSET_OG_ENTRY, bin->elf_header->e_ident[EI_DATA]));
  printf("HEXDUMP PAYLOAD: \n");
  hexdump(bin->payload, bin->len_payload, 0);
  //change key
  return 0;
}