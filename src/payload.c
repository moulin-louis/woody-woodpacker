//
// Created by loumouli on 11/10/23.
//

#include "woody.h"

//shellcode of payload, modified during runtime to suit the binary
uint8_t blueprint_payload_64[96] = {
  0x2E, 0x2E, 0x2E, 0x2E, 0x57, 0x4F, 0x4F, 0x44, 0x59, 0x2E, 0x2E, 0x2E,
  0x2E, 0x0A, 0x0E, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0xB8,
  0x01, 0x00, 0x00, 0x00, 0xBF, 0x01, 0x00, 0x00, 0x00, 0x48, 0x8D, 0x35,
  0xD8, 0xFF, 0xFF, 0xFF, 0xBA, 0x0E, 0x00, 0x00, 0x00, 0x0F, 0x05, 0x48,
  0x8D, 0x3D, 0x1D, 0x00, 0x00, 0x00, 0xBE, 0x01, 0x00, 0x00, 0x00, 0x48,
  0x8D, 0x15, 0x11, 0x00, 0x00, 0x00, 0xB9, 0x01, 0x00, 0x00, 0x00, 0x4C,
  0x8D, 0x05, 0x05, 0x00, 0x00, 0x00, 0xE9, 0xFD, 0xFF, 0xFF, 0xFF, 0x5F,
  0x5E, 0x5D, 0x5C, 0x5B, 0x5A, 0x59, 0x58, 0xE9, 0xFD, 0xFF, 0xFF, 0xFF
};

//shellcode for the xor_decrypt function
uint8_t decrypt_fn_64[49] = {
  0x41, 0x50, 0x49, 0x89, 0xF9, 0x48, 0x89, 0xD7, 0x49, 0x89, 0xC8, 0xB9, 0x00,
  0x00, 0x00, 0x00, 0x48, 0x89, 0xC8, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x48, 0xF7,
  0xF6, 0x41, 0x0F, 0xB6, 0x04, 0x11, 0x30, 0x04, 0x0F, 0x48, 0x83, 0xC1, 0x01,
  0x49, 0x39, 0xC8, 0x75, 0xE4, 0x41, 0x58, 0x41, 0xFF, 0xE0
};

unsigned char blueprint_payload_32[101] = {
  0x2E, 0x2E, 0x2E, 0x2E, 0x57, 0x4F, 0x4F, 0x44, 0x59, 0x2E, 0x2E, 0x2E, 0x2E, 0x0A, 0x0F, 0x50, 0x51, 0x52, 0x53,
  0x54, 0x55, 0x56, 0x57, 0xB8, 0x04, 0x00, 0x00, 0x00, 0xBB, 0x01, 0x00, 0x00, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x00,
  0x59, 0x81, 0xE9, 0x26, 0x00, 0x00, 0x00, 0xBA, 0x0E, 0x00, 0x00, 0x00, 0xCD, 0x80, 0xE8, 0x00, 0x00, 0x00, 0x00,
  0x5F, 0x83, 0xC7, 0x29, 0xBE, 0x01, 0x00, 0x00, 0x00, 0x8D, 0x15, 0x4F, 0x90, 0x04, 0x08, 0xB9, 0x01, 0x00, 0x00,
  0x00, 0x8D, 0x1D, 0x4F, 0x90, 0x04, 0x08, 0xE9, 0xB2, 0x6F, 0xFB, 0xF7, 0x5F, 0x5E, 0x5D, 0x5C, 0x5B, 0x5A, 0x59,
  0x58, 0xE9, 0xA5, 0x6F, 0xFB, 0xF7
};

unsigned char decrypt_fn_32[36] = {
  // Offset 0x00001000 to 0x00001023
  0x56, 0x57, 0x51, 0x8B, 0x74, 0x24, 0x0C, 0x8B, 0x4C, 0x24, 0x14, 0x8B, 0x7C,
  0x24, 0x10, 0x31, 0xC0, 0x89, 0xC2, 0xF7, 0x74, 0x24, 0x14, 0x30, 0x14, 0x07,
  0x40, 0x39, 0xC1, 0x75, 0xF2, 0x59, 0x5F, 0x5E, 0xFF, 0xE3
};

//craft the full payload with the blueprint, the key and the decrypt function
int32_t craft_payload_64(t_bin* bin) {
  const Elf64_Phdr* text_segment = get_segment_64(bin->phdrs_64, is_text_segment_64);
  const uint32_t size_to_data = 0x3a + 8;
  const uint32_t size_to_entry = 0x50 + 16;
  const uint32_t offset_jmp_key = 0x22 + 8;
  const uint32_t offset_jmp_decrypt = 0x25 + 8;

  //init payload
  bin->payload = malloc(sizeof(blueprint_payload_64) + sizeof (decrypt_fn_64) + bin->len_key);
  if (bin->payload == NULL)
    return 1;
  memcpy(bin->payload, blueprint_payload_64, sizeof(blueprint_payload_64));
  memcpy(bin->payload + sizeof(blueprint_payload_64), bin->key, bin->len_key);
  memcpy(bin->payload + sizeof(blueprint_payload_64) + bin->len_key, decrypt_fn_64, sizeof(decrypt_fn_64));
  bin->len_payload = sizeof(blueprint_payload_64) + sizeof(decrypt_fn_64) + bin->len_key;

  size_t offset = text_segment->p_offset + text_segment->p_filesz;
  offset = (ALIGN_UP(offset, 4)) - offset;

  //  change key offset
  *(uint32_t *)(bin->payload + OFFSET_KEY_64 + 8) = offset_jmp_key;

  //  change key_len
  *(uint32_t *)(bin->payload + OFFSET_KEY_LEN_64 + 8) = bin->len_key;

  //  change data offset
  *(uint32_t *)(bin->payload + OFFSET_DATA_64 + 8) = -(size_to_data + text_segment->p_memsz - (bin->elf64_header->e_entry - text_segment->p_vaddr) + offset);

  //  change data_len
  *(uint32_t *)(bin->payload + OFFSET_DATA_LEN_64 + 8) = text_segment->p_filesz - (bin->elf64_header->e_entry - text_segment->p_vaddr);

  //  change decrypt_fn jmp offset
  *(uint32_t *)(bin->payload + OFFSET_DECRYPT_FN_64 + 8) = offset_jmp_decrypt; // + bin->len_key;

  //  change og_entry jmp offset
  *(uint32_t *)(bin->payload + OFFSET_OG_ENTRY_64 + 16) = -(size_to_entry + text_segment->p_memsz - (bin->elf64_header->e_entry - text_segment->p_vaddr) + offset);
  // print_info_payload(bin);
  return 0;
}

size_t len_blueprint_32 = 0x61;

int32_t craft_payload_32(t_bin* bin) {
  const Elf32_Phdr* text_segment = get_segment_32(bin->phdrs_32, is_text_segment_32);
  const uint32_t size_to_data = 0x3a + 8;
  const uint32_t size_to_entry = 0x50 + 16;
  const uint32_t offset_jmp_key = 0x22 + 8;
  const uint32_t offset_jmp_decrypt = 0x25 + 8;

  //init payload
  bin->payload = malloc(len_blueprint_32 + sizeof (decrypt_fn_32) + bin->len_key);
  if (bin->payload == NULL)
    return 1;
  memcpy(bin->payload, blueprint_payload_32,  len_blueprint_32);
  memcpy(bin->payload + sizeof(blueprint_payload_32), bin->key, bin->len_key);
  memcpy(bin->payload + sizeof(blueprint_payload_32) + bin->len_key, decrypt_fn_32, sizeof(decrypt_fn_32));
  bin->len_payload = sizeof(blueprint_payload_32) + sizeof(decrypt_fn_32) + bin->len_key;

  size_t offset = text_segment->p_offset + text_segment->p_filesz;
  offset = (ALIGN_UP(offset, 4)) - offset;

  //  change key offset
  *(uint32_t *)(bin->payload + OFFSET_KEY_32 + 8) = offset_jmp_key;

  //  change key_len
  *(uint32_t *)(bin->payload + OFFSET_KEY_LEN_32 + 8) = bin->len_key;

  //  change data offset
  *(uint32_t *)(bin->payload + OFFSET_DATA_32 + 8) = -(size_to_data + text_segment->p_memsz - (bin->elf32_header->e_entry - text_segment->p_vaddr) + offset);

  //  change data_len
  *(uint32_t *)(bin->payload + OFFSET_DATA_LEN_32 + 8) = text_segment->p_filesz - (bin->elf32_header->e_entry - text_segment->p_vaddr);

  //  change decrypt_fn jmp offset
  *(uint32_t *)(bin->payload + OFFSET_DECRYPT_FN_32 + 8) = offset_jmp_decrypt; // + bin->len_key;

  //  change og_entry jmp offset
  *(uint32_t *)(bin->payload + OFFSET_OG_ENTRY_32 + 16) = -(size_to_entry + text_segment->p_memsz - (bin->elf32_header->e_entry - text_segment->p_vaddr) + offset);
  print_info_payload(bin);
  return 0;
}