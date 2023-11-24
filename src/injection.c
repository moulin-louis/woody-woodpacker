#include "woody.h"

uint64_t get_resize_64(const t_bin* bin, const uint64_t cave_begin) {
	for (const phdr_list_64_t* seg_h = bin->phdrs_64; seg_h != 0; seg_h = seg_h->next) {
		if (seg_h->program_header->p_type != PT_LOAD)
			continue;
		if (seg_h->program_header->p_offset < cave_begin)
			continue;
		if (cave_begin + bin->len_payload < seg_h->program_header->p_offset)
			return 0;
		return -1;
	}
	if (cave_begin + bin->len_payload < bin->data_len)
		return 0;
	return cave_begin + bin->len_payload - bin->data_len;
}

uint64_t get_resize_32(const t_bin* bin, const uint64_t cave_begin) {
	for (const phdr_list_32_t* seg_h = bin->phdrs_32; seg_h != 0; seg_h = seg_h->next) {
		if (seg_h->program_header->p_type != PT_LOAD)
			continue;
		if (seg_h->program_header->p_offset < cave_begin)
			continue;
		if (cave_begin + bin->len_payload < seg_h->program_header->p_offset)
			return 0;
		return cave_begin + bin->len_payload - seg_h->program_header->p_offset;
	}
	if (cave_begin + bin->len_payload < bin->data_len)
		return 0;
	return cave_begin + bin->len_payload - bin->data_len;
}


int find_code_cave_64(t_bin* bin) {
	Elf64_Ehdr* header = bin->elf64_header;
	Elf64_Phdr* txt_segment_h = get_segment_64(bin->phdrs_64, is_text_segment_64);
	uint64_t offset = txt_segment_h->p_offset + txt_segment_h->p_filesz;
	uint64_t aligned_offset = ALIGN_UP(offset, 4);
	offset = aligned_offset - offset;
	uint64_t resize_needed = get_resize_64(bin, aligned_offset);
	if (resize_needed) {
		fprintf(stderr, "Resize file needed!");
		return 1;
	}
	memcpy(bin->raw_data + aligned_offset, bin->payload, bin->len_payload);
	const uint64_t entry_offset = header->e_entry - txt_segment_h->p_vaddr;
	header->e_entry += -entry_offset + txt_segment_h->p_memsz + OFFSET_ENTRY + offset;
	txt_segment_h->p_flags |= PROT_WRITE;
	txt_segment_h->p_filesz += bin->len_payload + offset;
	txt_segment_h->p_memsz += bin->len_payload + offset;
	return 0;
}

int find_code_cave_32(t_bin* bin) {
	Elf32_Ehdr* header = bin->elf32_header;
	Elf32_Phdr* txt_segment_h = get_segment_32(bin->phdrs_32, is_text_segment_32);
	uint32_t offset = txt_segment_h->p_offset + txt_segment_h->p_filesz;
	uint32_t aligned_offset = ALIGN_UP(offset, 4);
	offset = aligned_offset - offset;
	uint32_t resize_needed = get_resize_32(bin, aligned_offset);
	if (resize_needed) {
		fprintf(stderr, "Resize file needed!");
		return 1;
	}
	memcpy(bin->raw_data + aligned_offset, bin->payload, bin->len_payload);
	const uint32_t entry_offset = header->e_entry - txt_segment_h->p_vaddr;
	header->e_entry += -entry_offset + txt_segment_h->p_memsz + OFFSET_ENTRY + offset;
	txt_segment_h->p_flags |= PROT_WRITE;
	txt_segment_h->p_filesz += bin->len_payload + offset;
	txt_segment_h->p_memsz += bin->len_payload + offset;
	return 0;
}