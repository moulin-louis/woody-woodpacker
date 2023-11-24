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
uint8_t decrypt_fn_64_2[38] = {
  0x41, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x4C, 0x89, 0xC8, 0x52, 0xBA, 0x00, 0x00,
  0x00, 0x00, 0x48, 0xF7, 0xF6, 0x0F, 0xB6, 0x04, 0x17, 0x5A, 0x42, 0x30, 0x04,
  0x0A, 0x49, 0xFF, 0xC1, 0x4C, 0x39, 0xC9, 0x75, 0xE3, 0x41, 0xFF, 0xE0
};

unsigned char decrypt_fn_64[590] = {
	0x41, 0x50, 0x49, 0x89, 0xF4, 0x48, 0x89, 0xD5, 0x31, 0xDB, 0x48, 0x81,
	0xEC, 0xC8, 0x00, 0x00, 0x00, 0x31, 0xC0, 0x4C, 0x8D, 0x6C, 0x24, 0x08,
	0x4C, 0x89, 0xEE, 0xE8, 0xCE, 0x01, 0x00, 0x00, 0x48, 0x39, 0xEB, 0x73,
	0x2B, 0x49, 0x8D, 0x3C, 0x1C, 0x4C, 0x89, 0xEE, 0x48, 0x83, 0xC3, 0x10,
	0xE8, 0x51, 0x01, 0x00, 0x00, 0xEB, 0x02, 0x48, 0x81, 0xC4, 0xC8, 0x00,
	0x00, 0x00, 0x41, 0x58, 0x41, 0xFF, 0xE0, 0x89, 0xF8, 0x89, 0xFA, 0xC0,
	0xC2, 0x04, 0xD0, 0xC0, 0x31, 0xD0, 0x89, 0xFA, 0x31, 0xF8, 0xC0, 0xC2,
	0x02, 0x31, 0xD0, 0x40, 0xC0, 0xC7, 0x03, 0x31, 0xF8, 0x83, 0xF0, 0x63,
	0xC3, 0x89, 0xF8, 0x89, 0xFA, 0x40, 0xC0, 0xC7, 0x03, 0xD0, 0xC0, 0xC0,
	0xCA, 0x02, 0x31, 0xD0, 0x31, 0xF8, 0x83, 0xF0, 0x05, 0xC3, 0x51, 0x89,
	0x7C, 0x24, 0x04, 0x48, 0x8D, 0x4C, 0x24, 0x04, 0x48, 0x8D, 0x74, 0x24,
	0x08, 0x0F, 0xB6, 0x39, 0x48, 0xFF, 0xC1, 0xE8, 0xB3, 0xFF, 0xFF, 0xFF,
	0x88, 0x41, 0xFF, 0x48, 0x39, 0xF1, 0x75, 0xF1, 0x8B, 0x44, 0x24, 0x04,
	0x5A, 0xC3, 0x0F, 0xB6, 0xD2, 0x01, 0xD2, 0x48, 0x63, 0xC2, 0xFF, 0xC2,
	0x48, 0x63, 0xD2, 0x48, 0x8B, 0x04, 0xC6, 0x48, 0x31, 0x07, 0x48, 0x8B,
	0x04, 0xD6, 0x48, 0x31, 0x47, 0x08, 0xC3, 0x48, 0x89, 0xFE, 0x31, 0xC9,
	0x0F, 0xB6, 0x3C, 0x0E, 0xE8, 0x98, 0xFF, 0xFF, 0xFF, 0x88, 0x04, 0x0E,
	0x48, 0xFF, 0xC1, 0x48, 0x83, 0xF9, 0x10, 0x75, 0xEF, 0xC3, 0x31, 0xC0,
	0x40, 0x84, 0xFF, 0x74, 0x27, 0x40, 0x84, 0xF6, 0x74, 0x22, 0x40, 0xF6,
	0xC6, 0x01, 0x74, 0x06, 0x31, 0xF8, 0x8D, 0x14, 0x3F, 0x8D, 0x0C, 0x3F,
	0x83, 0xF2, 0x1B, 0x40, 0x84, 0xFF, 0x0F, 0x49, 0xD1, 0x40, 0xD0, 0xEE,
	0x89, 0xD7, 0xEB, 0xDC, 0xC3, 0x48, 0x83, 0xEC, 0x28, 0x49, 0x89, 0xFA,
	0x45, 0x31, 0xC9, 0x31, 0xC0, 0x43, 0x0F, 0xB6, 0x3C, 0x0A, 0xBE, 0x0E,
	0x00, 0x00, 0x00, 0xE8, 0xBA, 0xFF, 0xFF, 0xFF, 0xBE, 0x0B, 0x00, 0x00,
	0x00, 0x41, 0x89, 0xC0, 0x41, 0x8D, 0x41, 0x04, 0x83, 0xE0, 0x0F, 0x41,
	0x0F, 0xB6, 0x3C, 0x02, 0xE8, 0xA1, 0xFF, 0xFF, 0xFF, 0xBE, 0x0D, 0x00,
	0x00, 0x00, 0x41, 0x31, 0xC0, 0x41, 0x8D, 0x41, 0x08, 0x83, 0xE0, 0x0F,
	0x41, 0x0F, 0xB6, 0x3C, 0x02, 0xE8, 0x88, 0xFF, 0xFF, 0xFF, 0xBE, 0x09,
	0x00, 0x00, 0x00, 0x41, 0x31, 0xC0, 0x41, 0x8D, 0x41, 0x0C, 0x83, 0xE0,
	0x0F, 0x41, 0x0F, 0xB6, 0x3C, 0x02, 0xE8, 0x6F, 0xFF, 0xFF, 0xFF, 0x41,
	0x31, 0xC0, 0x46, 0x88, 0x44, 0x0C, 0x08, 0x49, 0xFF, 0xC1, 0x49, 0x83,
	0xF9, 0x10, 0x75, 0xA7, 0x0F, 0x10, 0x44, 0x24, 0x08, 0x41, 0x0F, 0x11,
	0x02, 0x48, 0x83, 0xC4, 0x28, 0xC3, 0x41, 0x54, 0xBA, 0x0A, 0x00, 0x00,
	0x00, 0x49, 0x89, 0xF4, 0x55, 0x48, 0x89, 0xFD, 0x53, 0xBB, 0x09, 0x00,
	0x00, 0x00, 0xE8, 0xFF, 0xFE, 0xFF, 0xFF, 0x48, 0x89, 0xEF, 0xC1, 0x4F,
	0x04, 0x08, 0xC1, 0x47, 0x08, 0x10, 0xC1, 0x47, 0x0C, 0x08, 0xE8, 0x08,
	0xFF, 0xFF, 0xFF, 0x89, 0xDA, 0x4C, 0x89, 0xE6, 0x48, 0x89, 0xEF, 0xE8,
	0xDE, 0xFE, 0xFF, 0xFF, 0xE8, 0x3C, 0xFF, 0xFF, 0xFF, 0xFF, 0xCB, 0x75,
	0xDA, 0x48, 0x89, 0xEF, 0xC1, 0x4F, 0x04, 0x08, 0xC1, 0x47, 0x08, 0x10,
	0xC1, 0x47, 0x0C, 0x08, 0xE8, 0xDE, 0xFE, 0xFF, 0xFF, 0x5B, 0x4C, 0x89,
	0xE6, 0x48, 0x89, 0xEF, 0x31, 0xD2, 0x5D, 0x41, 0x5C, 0xE9, 0xB0, 0xFE,
	0xFF, 0xFF, 0x53, 0x0F, 0x10, 0x07, 0x49, 0x89, 0xF3, 0x41, 0xBA, 0x04,
	0x00, 0x00, 0x00, 0x41, 0xB1, 0x01, 0x0F, 0x11, 0x06, 0x43, 0x8B, 0x5C,
	0x93, 0xF0, 0x47, 0x8B, 0x44, 0x93, 0xFC, 0x41, 0xF6, 0xC2, 0x03, 0x74,
	0x09, 0x41, 0x31, 0xD8, 0xEB, 0x2D, 0x44, 0x89, 0xC7, 0xC1, 0xC7, 0x08,
	0xE8, 0x55, 0xFE, 0xFF, 0xFF, 0x41, 0x0F, 0xB6, 0xF9, 0x41, 0xC1, 0xE1,
	0x18, 0xBE, 0x02, 0x00, 0x00, 0x00, 0x41, 0x31, 0xD9, 0x45, 0x89, 0xC8,
	0x41, 0x31, 0xC0, 0xE8, 0x9A, 0xFE, 0xFF, 0xFF, 0x41, 0x89, 0xC1, 0x47,
	0x89, 0x04, 0x93, 0x49, 0xFF, 0xC2, 0x49, 0x83, 0xFA, 0x2C, 0x75, 0xB9,
	0x5B, 0xC3
};



unsigned char blueprint_payload_32[103] = {
  0x2E, 0x2E, 0x2E, 0x2E, 0x57, 0x4F, 0x4F, 0x44, 0x59, 0x2E, 0x2E, 0x2E, 0x2E, 0x0A, 0x0F, 0x50, 0x51, 0x52, 0x53,
  0x54, 0x55, 0x56, 0x57, 0xB8, 0x04, 0x00, 0x00, 0x00, 0xBB, 0x01, 0x00, 0x00, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x00,
  0x59, 0x83, 0xE9, 0x26, 0xBA, 0x0E, 0x00, 0x00, 0x00, 0xCD, 0x80, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x5F, 0x83, 0xC7,
  0x15, 0xBE, 0x24, 0x00, 0x00, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x5A, 0x81, 0xEA, 0x15, 0x00, 0x00, 0x00, 0xB9,
  0x15, 0x00, 0x00, 0x00, 0xe8, 0x00, 0x00, 0x00, 0x00, 0xE9, 0xFF, 0xFF, 0xFF, 0xFF, 0x5F, 0x5E, 0x5D, 0x5C, 0x5B,
  0x5A, 0x59, 0x58, 0xE9, 0xFF, 0xFF, 0xFF, 0xFF
};

unsigned char decrypt_fn_32[34] = {
  0xBB, 0x00, 0x00, 0x00, 0x00, 0x89, 0xD8, 0x52, 0xBA, 0x00, 0x00, 0x00,
  0x00, 0xF7, 0xF6, 0x0F, 0xB6, 0x04, 0x17, 0x5A, 0x30, 0x04, 0x1A, 0x43,
  0x39, 0xD9, 0x75, 0xE9, 0x58, 0x83, 0xC0, 0x05, 0xFF, 0xE0,
};

//craft the full payload with the blueprint, the key and the decrypt function
int32_t craft_payload_64(t_bin* bin) {
  const Elf64_Phdr* text_segment = get_segment_64(bin->phdrs_64, is_text_segment_64);
  const uint32_t start_payload_data = 0x42;
  const uint32_t start_payload_entry = 0x60;
  const uint32_t offset_key = 0x2A;
  const uint32_t offset_decrypt = 13;
  bin->payload = malloc(sizeof(blueprint_payload_64) + bin->len_key + sizeof(decrypt_fn_64));
  if (bin->payload == NULL)
    return 1;
  memcpy(bin->payload, blueprint_payload_64, sizeof(blueprint_payload_64));
  memcpy(bin->payload + sizeof(blueprint_payload_64), bin->key, bin->len_key);
  memcpy(bin->payload + sizeof(blueprint_payload_64) + bin->len_key, decrypt_fn_64, sizeof(decrypt_fn_64));
  bin->len_payload = sizeof(blueprint_payload_64) + sizeof(decrypt_fn_64) + bin->len_key;
  size_t offset = text_segment->p_offset + text_segment->p_filesz;
  offset = (ALIGN_UP(offset, 4)) - offset;
  *(uint32_t *)(bin->payload + OFFSET_KEY_64 + 8) = offset_key;
  *(uint32_t *)(bin->payload + OFFSET_KEY_LEN_64 + 8) = bin->len_key;
  *(uint32_t *)(bin->payload + OFFSET_DATA_64 + 8) = -(start_payload_data + text_segment->p_filesz + offset);
  *(uint32_t *)(bin->payload + OFFSET_DATA_LEN_64 + 8) = text_segment->p_filesz;
  *(uint32_t *)(bin->payload + OFFSET_DECRYPT_FN_64 + 8) = offset_decrypt + bin->len_key;
  *(uint32_t *)(bin->payload + OFFSET_OG_ENTRY_64 + 16) = -(
    start_payload_entry + text_segment->p_memsz - (bin->elf64_header->e_entry - text_segment->p_vaddr) + offset);;
  return 0;
}

int32_t craft_payload_32(t_bin* bin) {
  const Elf32_Phdr* text_segment = get_segment_32(bin->phdrs_32, is_text_segment_32);
  const uint32_t start_payload_entry = 0x67;
  const uint32_t start_payload_data = 0x44;
  const uint32_t offset_key = 0x31;
  const uint32_t offset_decrypt = 0xD;
  size_t offset = text_segment->p_offset + text_segment->p_filesz;
  offset = (ALIGN_UP(offset, 4)) - offset;
  bin->payload = malloc(sizeof(blueprint_payload_32) + sizeof (decrypt_fn_32) + bin->len_key);
  if (bin->payload == NULL)
    return 1;
  memcpy(bin->payload, blueprint_payload_32, sizeof(blueprint_payload_32));
  memcpy(bin->payload + sizeof(blueprint_payload_32), bin->key, bin->len_key);
  memcpy(bin->payload + sizeof(blueprint_payload_32) + bin->len_key, decrypt_fn_32, sizeof(decrypt_fn_32));
  bin->len_payload = sizeof(blueprint_payload_32) + sizeof(decrypt_fn_32) + bin->len_key;
  *(bin->payload + OFFSET_KEY_32) = offset_key;
  *(uint32_t *)(bin->payload + OFFSET_KEY_LEN_32) = bin->len_key;
  *(uint32_t *)(bin->payload + OFFSET_DATA_32) = start_payload_data + text_segment->p_filesz + offset;
  *(uint32_t *)(bin->payload + OFFSET_DATA_LEN_32) = text_segment->p_filesz;
  *(uint32_t *)(bin->payload + OFFSET_DECRYPT_FN_32) = offset_decrypt + bin->len_key;
  *(uint32_t *)(bin->payload + OFFSET_OG_ENTRY_32) = -(
    start_payload_entry + text_segment->p_memsz - (bin->elf32_header->e_entry - text_segment->p_vaddr) + offset);;
  return 0;
}
