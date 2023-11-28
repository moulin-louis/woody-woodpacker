//
// Created by loumouli on 11/9/23.
//

#include "woody.h"
// #include <stdio.h>
// #include <stdint.h>
// #include <string.h>
#define AES_KEY_LEN 16
#define AES_ROUNDS 11
//expose symbol for asm code
// void encrypt(const uint8_t *key, const uint64_t len_key, uint8_t *data, const uint64_t len_data);



uint32_t rot_word_left(uint32_t word, uint16_t nb) {
	return (word << nb) | (word >> (32 - nb));
}

uint8_t sub_letter(uint8_t letter) {
	return (letter ^
				((letter << 1) | (letter >> 7)) ^
				((letter << 2) | (letter >> 6)) ^
				((letter << 3) | (letter >> 5)) ^
				((letter << 4) | (letter >> 4)) ^
				0x63);
}

uint8_t inv_sub_letter(uint8_t letter) {
	return (((letter << 1) | (letter >> 7)) ^
				((letter << 3) | (letter >> 5)) ^
				((letter << 6) | (letter >> 2)) ^
				0x5);
}

uint32_t sub_word(uint32_t word) {
	uint8_t *letter_word = (uint8_t *)&word;
	for (uint8_t id = 0; id < 4; id++) {
		letter_word[id] = sub_letter(letter_word[id]);
	}
	return word;
}

void add_round_key(uint8_t *current_segment, const uint8_t *expanded_key, uint8_t round) {
	uint64_t *segment64 = (uint64_t *)current_segment;
	uint64_t *key64 = (uint64_t *)(expanded_key);

	segment64[0] ^= key64[0 + round * 2];
	segment64[1] ^= key64[1 + round * 2];
}

void sub_bytes(uint8_t *current_segment) {
	for (uint8_t id = 0; id < 16; id++) {
		current_segment[id] = sub_letter(current_segment[id]);
	}
}

void inv_sub_bytes(uint8_t *current_segment) {
	for (uint8_t id = 0; id < 16; id++) {
		current_segment[id] = inv_sub_letter(current_segment[id]);
	}
}

void shift_rows(uint8_t *current_segment) {
	uint32_t *words_segment = (uint32_t *)current_segment;
	for (uint8_t i = 1; i < 4; i++) {
		words_segment[i] = rot_word_left(words_segment[i], 8 * i);
	}
}

void	inv_shift_rows(uint8_t *current_segment) {
	uint32_t *words_segment = (uint32_t *)current_segment;
	for (uint8_t i = 1; i < 4; i++) {
		words_segment[i] = rot_word_left(words_segment[i], 8 * (4 - i));
	}
}

void multiply_column(uint8_t *current_segment, uint8_t column) {
	uint8_t	current_col[4];
	uint8_t	double_col[4];
	uint8_t	new_col[4];

	for (uint8_t i = 0; i < 4; i++) {
		current_col[i] = current_segment[i * 4 + column];
		double_col[i] = current_col[i] << 1;
		double_col[i] ^= (current_col[i] >> 7) * 0x1b; // xor with 1b if result would be larger than ff
	}
	new_col[0] = double_col[0] ^ current_col[3] ^ current_col[2] ^ double_col[1] ^ current_col[1]; /* 2 * a0 + a3 + a2 + 3 * a1 */
	new_col[1] = double_col[1] ^ current_col[0] ^ current_col[3] ^ double_col[2] ^ current_col[2]; /* 2 * a1 + a0 + a3 + 3 * a2 */
	new_col[2] = double_col[2] ^ current_col[1] ^ current_col[0] ^ double_col[3] ^ current_col[3]; /* 2 * a2 + a1 + a0 + 3 * a3 */
	new_col[3] = double_col[3] ^ current_col[2] ^ current_col[1] ^ double_col[0] ^ current_col[0]; /* 2 * a3 + a2 + a1 + 3 * a0 */
	for (uint8_t i = 0; i < 4; i++) {
		current_segment[i * 4 + column] = new_col[i];
	}
}

uint8_t	galois_mul(uint8_t a, uint8_t b) {
	uint8_t res = 0;
	while (a != 0 && b != 0) {
		if (b & 1)
			res ^= a;
		if (a & 0x80)
			a = (a << 1) ^ 0x1b;
		else
			a <<= 1;
		b >>= 1;
	}
	return res;
}

void mix_columns(uint8_t *current_segment) {
	for (uint8_t column = 0; column < 4; column++) {
		multiply_column(current_segment, column);
	}
}

void	inv_mix_columns(uint8_t *current_segment) {
	uint8_t new_segment[16];
	for (uint8_t i = 0; i < 16; i++) {
			new_segment[i] = galois_mul(current_segment[i], 14) ^
				galois_mul(current_segment[((i + 4) % 16)], 11) ^
				galois_mul(current_segment[((i + 8) % 16)], 13) ^
				galois_mul(current_segment[((i + 12) % 16)], 9);
	}
	memcpy(current_segment, new_segment, 16);
}

void	encrypt_segment(uint8_t *current_segment, const uint8_t *expanded_key) {
	add_round_key(current_segment, expanded_key, 0);
	for (uint8_t i = 1; i < AES_ROUNDS - 1; i++) {
		sub_bytes(current_segment);
		shift_rows(current_segment);
		mix_columns(current_segment);
		add_round_key(current_segment, expanded_key, i);
	}
	sub_bytes(current_segment);
	shift_rows(current_segment);
	add_round_key(current_segment, expanded_key, AES_ROUNDS - 1);
}

void	decrypt_segment(uint8_t *current_segment, const uint8_t *expanded_key) {
	add_round_key(current_segment, expanded_key, AES_ROUNDS - 1);
	for (uint8_t i = 1; i < AES_ROUNDS - 1; i++) {
		inv_shift_rows(current_segment);
		inv_sub_bytes(current_segment);
		add_round_key(current_segment, expanded_key, AES_ROUNDS - 1 - i);
		inv_mix_columns(current_segment);
	}
	inv_shift_rows(current_segment);
	inv_sub_bytes(current_segment);
	add_round_key(current_segment, expanded_key, 0);
}

void generate_keys(const uint8_t *key, uint8_t expanded_key[AES_KEY_LEN * AES_ROUNDS]) {
	uint8_t round_constant = 0x1;
	memcpy(expanded_key, key, AES_KEY_LEN);
	uint32_t *words_keys = (uint32_t *)expanded_key;
	for (uint8_t i = 4; i < 4 * AES_ROUNDS; i++) {
		if (i % 4)
			words_keys[i] = words_keys[i - 4] ^ words_keys[i - 1];
		else {
			words_keys[i] = words_keys[i - 4] ^ sub_word(rot_word_left(words_keys[i - 1], 8)) ^ (round_constant << 24);
			round_constant = galois_mul(round_constant, 2);
		}
	}
}

void aes_encrypt(const uint8_t *key, uint8_t *data, const uint64_t len_data) {
	uint8_t expanded_key[AES_KEY_LEN * AES_ROUNDS];

	generate_keys(key, expanded_key);
	for (uint64_t data_offset = 0; data_offset + 16 < len_data; data_offset += 16) {
		encrypt_segment(data + data_offset, expanded_key);
	}
}

void aes_decrypt(const uint8_t *key, uint8_t *data, const uint64_t len_data) {
	uint8_t expanded_key[AES_KEY_LEN * AES_ROUNDS];

	generate_keys(key, expanded_key);
	for (uint64_t data_offset = 0; data_offset + 16 < len_data; data_offset += 16) {
			decrypt_segment(data + data_offset, expanded_key);
	}
}

// int main() {
// 	// uint64_t data_len = 100000000;
// 	// uint8_t c = 105;
// 	// // uint8_t *data_base = calloc(data_len, 1);
// 	// uint8_t *data = calloc(data_len, 1);
// 	unsigned char data[37] = {
// 	// Offset 0x00001000 to 0x00001024
// 	0xBF, 0x01, 0x00, 0x00, 0x00, 0x48, 0xBE, 0x00, 0x20, 0x40, 0x00, 0x00,
// 	0x00, 0x00, 0x00, 0xBA, 0x09, 0x00, 0x00, 0x00, 0xB8, 0x01, 0x00, 0x00,
// 	0x00, 0x0F, 0x05, 0x48, 0x31, 0xFF, 0xB8, 0x3C, 0x00, 0x00, 0x00, 0x0F,
// 	0x05
// };

// 	// memset(data_base, c, data_len);
// 	// memset(data, c, data_len);
	
// 	// uint8_t data[901] = "bonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourababbonjourbonjourabab";
// 	unsigned char key[16] = {
// 	// Offset 0x00001088 to 0x00001097
// 	0xCA, 0xFC, 0xB4, 0xB5, 0xD6, 0x77, 0xED, 0x6F, 0xED, 0xAD, 0xB9, 0x13,
// 	0xD0, 0x1A, 0x39, 0xCC
// };

// 	// aes_encrypt(key, data, 37);
// 	// for(int i = 0; i < 37; i++) {
// 	// 	printf("%2x ", data[i]);
// 	// }
// 	// printf("\n");
// 	aes_decrypt(key, data, 37);
// 	// for(int i = 0; i < 37; i++) {
// 	// 	printf("%2x ", data[i]);
// 	// }
// 	// printf("\n");
// 	// printf("is ok: %d\n", memcmp(data, data_base, data_len));
	
// }

// // //temporary xor encrypt
// void xor_encrypt(const uint8_t *key, const uint64_t len_key, uint8_t *data, const uint64_t len_data) {
//   for (size_t i = 0; i < len_data; i++)
//     data[i] ^= key[i % len_key];
// }

// Encrypt the text segment base on a random key provided by urandom or the user in the ENV
int32_t encryption_64(t_bin* bin) {
  //get text segment
  const Elf64_Phdr* text_segment = NULL;
  text_segment = get_segment_64(bin->phdrs_64, is_text_segment_64);
  //get key from urandom
  bin->key = calloc(1, KEY_SIZE);
  bin->len_key = 16;
  const char* key = getenv("KEY");
  if (key != NULL) {
    bin->key = (uint8_t *)strdup(key);
    if (bin->key == NULL) {
      return 1;
    }
    bin->len_key = strlen((char *)bin->key);
	if (bin->len_key != KEY_SIZE) {
		return 1;
	}
  }
  else if (get_key(bin->key)) {
    return 1;
  }

  void* data = bin->raw_data + text_segment->p_offset;
  // xor_encrypt(bin->key, bin->len_key, data, text_segment->p_filesz);
  aes_encrypt(bin->key, data, text_segment->p_filesz);
  return 0;
}

void xor_encrypt(const uint8_t* key, const uint64_t len_key, uint8_t* data, const uint64_t len_data) {
	for (size_t i = 0; i < len_data; i++)
		data[i] ^= key[i % len_key];
}

int32_t encryption_32(t_bin* bin) {
  //get text segment
  const Elf32_Phdr* text_segment = NULL;
  text_segment = get_segment_32(bin->phdrs_32, is_text_segment_32);
  //get key from urandom
  bin->key = calloc(1, KEY_SIZE);
  bin->len_key = 16;
  const char* key = getenv("KEY");
  if (key != NULL) {
    bin->key = (uint8_t *)strdup(key);
    if (bin->key == NULL)
      return 1;
    bin->len_key = strlen((char *)bin->key);
  }
  else if (get_key(bin->key))
  	return 1;
  void* data = bin->raw_data + text_segment->p_offset;
  xor_encrypt(bin->key, bin->len_key,data, text_segment->p_filesz);
  return 0;
}
