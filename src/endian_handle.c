//
// Created by loumouli on 11/10/23.
//

#include "woody.h"

uint8_t get_uint8(uint8_t *data, uint8_t endian) {
  if (endian == ELFDATA2LSB) {
    return *data;
  } else {
    return *(data + 7);
  }
}

uint16_t get_uint16(uint8_t *data, uint8_t endian) {
  if (endian == ELFDATA2LSB) {
    return *(uint16_t *)data;
  } else {
    return *(uint16_t *)(data + 6);
  }
}

uint32_t get_uint32(uint8_t *data, uint8_t endian) {
  if (endian == ELFDATA2LSB) {
    return *(uint32_t *)data;
  } else {
    return *(uint32_t *)(data + 4);
  }
}

uint64_t get_uint64(uint8_t *data, uint8_t endian) {
  if (endian == ELFDATA2LSB) {
    return *(uint64_t *)data;
  } else {
    return *(uint64_t *)(data);
  }
}