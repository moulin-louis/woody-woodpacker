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
uint8_t decrypt_fn_64[38] = {
  0x41, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x4C, 0x89, 0xC8, 0x52, 0xBA, 0x00, 0x00,
  0x00, 0x00, 0x48, 0xF7, 0xF6, 0x0F, 0xB6, 0x04, 0x17, 0x5A, 0x42, 0x30, 0x04,
  0x0A, 0x49, 0xFF, 0xC1, 0x4C, 0x39, 0xC9, 0x75, 0xE3, 0x41, 0xFF, 0xE0
};

unsigned char blueprint_payload_32[103] = {
  0x2E, 0x2E, 0x2E, 0x2E, 0x57, 0x4F, 0x4F, 0x44, 0x59, 0x2E, 0x2E, 0x2E, 0x2E, 0x0A, 0x0F, 0x50, 0x51, 0x52, 0x53,
  0x54, 0x55, 0x56, 0x57, 0xB8, 0x04, 0x00, 0x00, 0x00, 0xBB, 0x01, 0x00, 0x00, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x00,
  0x59, 0x83, 0xE9, 0x26, 0xBA, 0x0E, 0x00, 0x00, 0x00, 0xCD, 0x80, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x5F, 0x83, 0xC7,
  0x15, 0xBE, 0x24, 0x00, 0x00, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x5A, 0x81, 0xEA, 0x15, 0x00, 0x00, 0x00, 0xB9,
  0x15, 0x00, 0x00, 0x00, 0xe8, 0x00, 0x00, 0x00, 0x00,0xE9, 0xFF, 0xFF, 0xFF, 0xFF, 0x5F, 0x5E, 0x5D, 0x5C, 0x5B,
  0x5A, 0x59, 0x58, 0xE9, 0xFF, 0xFF, 0xFF, 0xFF
};

unsigned char decrypt_fn_32[34] = {
  0xBB, 0x00, 0x00, 0x00, 0x00, 0x89, 0xD8, 0x52, 0xBA, 0x00, 0x00, 0x00,
  0x00, 0xF7, 0xF6, 0x0F, 0xB6, 0x04, 0x17, 0x5A, 0x30, 0x04, 0x1A, 0x43,
  0x39, 0xD9, 0x75, 0xE9, 0x58, 0x83, 0xC0, 0x05 ,0xFF, 0xE0,
};

//craft the full payload with the blueprint, the key and the decrypt function
int32_t craft_payload_64(t_bin* bin) {
  const Elf64_Phdr* text_segment = get_segment_64(bin->phdrs_64, is_text_segment_64);
  const uint32_t start_payload_data = 0x42;
  const uint32_t start_payload_entry = 0x60;
  const uint32_t offset_key = 0x2A;
  const uint32_t offset_decrypt = 13;

  //init payload
  bin->payload = malloc(sizeof(blueprint_payload_64) + bin->len_key + sizeof(decrypt_fn_64));
  if (bin->payload == NULL)
    return 1;
  printf("\n");
  memcpy(bin->payload, blueprint_payload_64, sizeof(blueprint_payload_64));
  memcpy(bin->payload + sizeof(blueprint_payload_64), bin->key, bin->len_key);
  memcpy(bin->payload + sizeof(blueprint_payload_64) + bin->len_key, decrypt_fn_64, sizeof(decrypt_fn_64));
  bin->len_payload = sizeof(blueprint_payload_64) + sizeof(decrypt_fn_64) + bin->len_key;

  size_t offset = text_segment->p_offset + text_segment->p_filesz;
  offset = (ALIGN_UP(offset, 4)) - offset;
  printf("offset = %#lx\n", offset);
  printf("text filesz = %#lx\n", text_segment->p_filesz);

  //  change key offset
  *(uint32_t *)(bin->payload + OFFSET_KEY_64 + 8) = offset_key;

  //  change key_len
  *(uint32_t *)(bin->payload + OFFSET_KEY_LEN_64 + 8) = bin->len_key;

  //  change data offset
  *(uint32_t *)(bin->payload + OFFSET_DATA_64 + 8) = -(start_payload_data + text_segment->p_filesz + offset);

  //  change data_len
  *(uint32_t *)(bin->payload + OFFSET_DATA_LEN_64 + 8) = text_segment->p_filesz;

  //  change decrypt_fn jmp offset
  *(uint32_t *)(bin->payload + OFFSET_DECRYPT_FN_64 + 8) = offset_decrypt + bin->len_key;

  //  change og_entry jmp offset
  *(uint32_t *)(bin->payload + OFFSET_OG_ENTRY_64 + 16) = -(start_payload_entry + text_segment->p_memsz - (bin->elf64_header->e_entry - text_segment->p_vaddr) + offset);;
  // print_info_payload(bin);
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
  printf("\n");
  printf("offset = %#lx\n", offset);
  printf("txt segment filezs = %#x\n", text_segment->p_filesz);
  //init payload
  bin->payload = malloc(sizeof(blueprint_payload_32) + sizeof (decrypt_fn_32) + bin->len_key);
  if (bin->payload == NULL)
    return 1;
  memcpy(bin->payload, blueprint_payload_32,  sizeof(blueprint_payload_32));
  memcpy(bin->payload + sizeof(blueprint_payload_32), bin->key, bin->len_key);
  memcpy(bin->payload + sizeof(blueprint_payload_32) + bin->len_key, decrypt_fn_32, sizeof(decrypt_fn_32));

  bin->len_payload = sizeof(blueprint_payload_32) + sizeof(decrypt_fn_32) + bin->len_key;

  *(bin->payload + OFFSET_KEY_32) = offset_key;

  *(uint32_t *)(bin->payload + OFFSET_KEY_LEN_32) = bin->len_key;

  printf("sub to esp -> %#x\n", start_payload_data + text_segment->p_filesz);
  *(uint32_t *)(bin->payload + OFFSET_DATA_32) = start_payload_data + text_segment->p_filesz + offset;

  *(uint32_t *)(bin->payload + OFFSET_DATA_LEN_32) = text_segment->p_filesz;

  *(uint32_t *)(bin->payload + OFFSET_DECRYPT_FN_32) = offset_decrypt + bin->len_key;

  *(uint32_t *)(bin->payload + OFFSET_OG_ENTRY_32) = -(start_payload_entry + text_segment->p_memsz - (bin->elf32_header->e_entry - text_segment->p_vaddr) + offset);;

  printf("HEXDUMP PAYLOAD:\n");
  hexdump(bin->payload, bin->len_payload, 0);
  // print_info_payload(bin);
  return 0;
}