//
// Created by loumouli on 11/10/23.
//

#include "woody.h"

uint8_t blueprint_payload[80] = {
    0x2E, 0x2E, 0x2E, 0x2E, 0x57, 0x4F, 0x4F, 0x44, 0x59, 0x2E, 0x2E, 0x2E, 0x2E,
    0x0A, 0x0E, 0xB8, 0x01, 0x00, 0x00, 0x00, 0xBF, 0x01, 0x00, 0x00, 0x00,
    0x48,0x8D, 0x35, 0xE0, 0xFF, 0xFF, 0xFF, 0xBA, 0x0E, 0x00, 0x00, 0x00, 0x0F, 0x05,
    0x48, 0x8D, 0x3D, 0x1D, 0x00, 0x00, 0x00, 0xBE, 0x01, 0x00, 0x00, 0x00, 0x48,
    0x8D, 0x15, 0x11, 0x00, 0x00, 0x00, 0xB9, 0x01, 0x00, 0x00, 0x00, 0x4C, 0x8D,
    0x05, 0x05, 0x00, 0x00, 0x00, 0xE9, 0xFD, 0xFF, 0xFF, 0xFF, 0xE9, 0xFD, 0xFF,
    0xFF, 0xFF
};

uint8_t decrypt_fn[49] = {
    0x41, 0x50, 0x49, 0x89, 0xF9, 0x48, 0x89, 0xD7, 0x49, 0x89, 0xC8, 0xB9, 0x00,
    0x00, 0x00, 0x00, 0x48, 0x89, 0xC8, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x48, 0xF7,
    0xF6, 0x41, 0x0F, 0xB6, 0x04, 0x11, 0x30, 0x04, 0x0F, 0x48, 0x83, 0xC1, 0x01,
    0x49, 0x39, 0xC8, 0x75, 0xE4, 0x41, 0x58, 0x41, 0xFF, 0xE0
};

int32_t craft_payload(t_bin *bin) {
  Elf64_Phdr *text_segment = get_segment(bin->phdrs, is_text_segment_64);
  uint32_t size_to_data = 0x3a;
  uint32_t size_to_entry = 0x50;
  uint32_t offset_jmp_key = 0x22;
  uint32_t offset_jmp_decrypt = 0x25;

  //init payload
  bin->payload = malloc(sizeof(blueprint_payload) + sizeof (decrypt_fn) + bin->len_key);
  memcpy(bin->payload, blueprint_payload, sizeof(blueprint_payload));
  memcpy(bin->payload + sizeof(blueprint_payload), bin->key, bin->len_key);
  memcpy(bin->payload + sizeof(blueprint_payload) + bin->len_key, decrypt_fn, sizeof(decrypt_fn));
  bin->len_payload = sizeof(blueprint_payload) + sizeof(decrypt_fn) + bin->len_key;

//  change key offset
  *((uint32_t *)(bin->payload + PAYLOAD_OFFSET_KEY)) = offset_jmp_key;

//  change key_len
  *((uint32_t *)(bin->payload + PAYLOAD_OFFSET_KEY_LEN)) = bin->len_key;

//  change data offset
  *((uint32_t *)(bin->payload + PAYLOAD_OFFSET_DATA)) = -(size_to_data + text_segment->p_memsz);

//  change data_len
  *((uint32_t *)(bin->payload + PAYLOAD_OFFSET_DATA_LEN)) = text_segment->p_filesz;

//  change decrypt_fn jmp offset
  *((uint32_t *)(bin->payload + PAYLOAD_OFFSET_DECRYPT_FN)) = offset_jmp_decrypt;

//  change og_entry jmp offset
 *((uint32_t *)(bin->payload + PAYLOAD_OFFSET_OG_ENTRY)) = -(size_to_entry + text_segment->p_memsz -(bin->elf_header->e_entry - text_segment->p_vaddr));
//  print_info_payload(bin);
  return 0;
}