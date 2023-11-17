//
// Created by loumouli on 11/9/23.
//

#include "woody.h"

#define AES_KEY_LEN 16
#define AES_ROUNDS 11
//expose symbol for asm code
void encrypt(const uint8_t* key, const uint64_t len_key, uint8_t* data, const uint64_t len_data);

uint32_t rot_word_left(uint32_t word, uint8_t nb) {
	return (word << 8 * nb) | (word >> (32 - 8 * nb));
}

uint8_t sub_letter(uint8_t letter) {
	return (letter ^
				((letter << 1) | (letter >> 7)) ^
				((letter << 2) | (letter >> 6)) ^
				((letter << 3) | (letter >> 5)) ^
				((letter << 4) | (letter >> 4)) ^
				0x63);
}

uint32_t sub_word(uint32_t word) {
	uint8_t *letter_word = (uint8_t *)&word;
	for (uint8_t id = 0; id < 4; id++) {
		letter_word[id] = sub_letter(letter_word[id]);
	}
	return word;
}

const uint8_t *generate_keys(const uint8_t* key) {
	uint8_t *expanded_key = calloc(1, AES_KEY_LEN * AES_ROUNDS);
	if (!expanded_key)
		return 0;
	const uint32_t round_constant[10] = {
		0x1 << 24,
		0x2 << 24,
		0x4 << 24,
		0x8 << 24,
		0x10 << 24,
		0x20 << 24,
		0x40 << 24,
		0x80 << 24,
		0x1b << 24,
		0x36 << 24
	};
	memcpy(expanded_key, key, AES_KEY_LEN);
	uint32_t *words_keys = (uint32_t *)expanded_key;
	for (uint8_t i = 4; i < 4 * AES_ROUNDS; i++) {
		if (i % 4)
			words_keys[i] = words_keys[i - 4] ^ words_keys[i - 1];
		else
			words_keys[i] = words_keys[i - 4] ^ sub_word(rot_word_left(words_keys[i - 1], 1)) ^ (round_constant[i / 4]);
	}
	return expanded_key;
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

void shift_rows(uint8_t *current_segment) {
	uint32_t *words_segment = (uint32_t *)current_segment;
	for (uint8_t i = 1; i < 4; i++) {
		words_segment[i] = rot_word_left(words_segment[i], i);
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

void mix_columns(uint8_t *current_segment) {
	for (uint8_t column = 0; column < 4; column++) {
		multiply_column(current_segment, column);
	}
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
	add_round_key(current_segment, expanded_key, 10);
}

uint8_t aes_encrypt(const uint8_t* key, uint8_t* data, const uint64_t len_data) {
	const uint8_t* expanded_key = generate_keys(key);
	uint8_t current_segment[16];

	if (!expanded_key)
		return 1;
	for (uint16_t data_offset = 0; data_offset < len_data; data_offset++) {
		current_segment[data_offset % 16] = data[data_offset];
		if (data_offset % 16 == 15) {
			encrypt_segment(current_segment, expanded_key);
			memcpy(data + data_offset, current_segment, 16);
		}
	}
	return 0;
}

//temporary xor encrypt
void xor_encrypt(const uint8_t* key, const uint64_t len_key, uint8_t* data, const uint64_t len_data) {
  for (size_t i = 0; i < len_data; i++)
    data[i] ^= key[i % len_key];
}

//Encrypt the text segment base on a random key provided by urandom or the user in the ENV
int32_t encryption_64(t_bin* bin) {
  //get text segment
  const Elf64_Phdr* text_segment = NULL;
  text_segment = get_segment_64(bin->phdrs_64, is_text_segment_64);
  //get key from urandom
  bin->key = calloc(1, KEY_SIZE);
  bin->len_key = 32;
  const char* key = getenv("KEY");
  if (key != NULL) {
    printf("Found key in ENV\n");
    bin->key = (uint8_t *)strdup(key);
    if (bin->key == NULL) {
      return 1;
    }
    bin->len_key = strlen((char *)bin->key);
  }
  else if (get_key(bin->key)) {
    return 1;
  }
  void* data = bin->raw_data + text_segment->p_offset + (bin->elf64_header->e_entry - text_segment->p_vaddr);
	aes_encrypt(bin->key, data, text_segment->p_filesz - (bin->elf64_header->e_entry - text_segment->p_vaddr));
  // xor_encrypt(bin->key, bin->len_key, data, text_segment->p_filesz - (bin->elf64_header->e_entry - text_segment->p_vaddr));
  return 0;
}
int32_t encryption_32(t_bin *bin) {
  //get text segment
  const Elf32_Phdr* text_segment = NULL;
  text_segment = get_segment_32(bin->phdrs_32, is_text_segment_32);
  //get key from urandom
  bin->key = calloc(1, KEY_SIZE);
  bin->len_key = 32;
  const char* key = getenv("KEY");
  if (key != NULL) {
    printf("Found key in ENV\n");
    bin->key = (uint8_t *)strdup(key);
    if (bin->key == NULL) {
      return 1;
    }
    bin->len_key = strlen((char *)bin->key);
  }
  else if (get_key(bin->key)) {
    return 1;
  }
  void* data = bin->raw_data + text_segment->p_offset + (bin->elf32_header->e_entry - text_segment->p_vaddr);
  xor_encrypt(bin->key, bin->len_key, data, text_segment->p_filesz - (bin->elf32_header->e_entry - text_segment->p_vaddr));
  return 0;
}
