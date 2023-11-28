#include "woody.h"

Elf64_Shdr	*get_symtab_header_64(t_bin *bin) {
	Elf64_Shdr *s_headers = (Elf64_Shdr *)(bin->raw_data + bin->elf64_header->e_shoff);
	for (uint16_t idx = 0; idx < bin->elf64_header->e_shnum; idx++) {
		if (s_headers->sh_type == SHT_SYMTAB) {
			printf ("symbole table found !!!\n");
			return s_headers;
		}
		s_headers = (Elf64_Shdr *)((void *)s_headers + bin->elf64_header->e_shentsize);
	}
	return 0;
}

Elf32_Shdr	*get_symtab_header_32(t_bin *bin) {
	Elf32_Shdr *s_headers = (Elf32_Shdr *)(bin->raw_data + bin->elf32_header->e_shoff);
	for (uint16_t idx = 0; idx < bin->elf32_header->e_shnum; idx++) {
		if (s_headers->sh_type == SHT_SYMTAB) {
			printf ("symbole table found !!!\n");
			return s_headers;
		}
		s_headers = (Elf32_Shdr *)((void *)s_headers + bin->elf32_header->e_shentsize);
	}
	return 0;
}

void offset_symtab_64(t_bin *bin, Elf64_Shdr *symtab_header, const uint64_t cave_begin, const uint64_t resize_needed) {
	Elf64_Sym *symtab = (Elf64_Sym *)(bin->raw_data + symtab_header->sh_offset);
	for (uint32_t idx = 0; idx < 7; idx++) {
		if (symtab->st_value > cave_begin)
			symtab->st_value += resize_needed;
		symtab = (Elf64_Sym *)((void *)symtab + symtab_header->sh_entsize);
	}
}


void offset_symtab_32(t_bin *bin, Elf32_Shdr *symtab_header, const uint64_t cave_begin, const uint64_t resize_needed) {
	Elf32_Sym *symtab = (Elf32_Sym *)(bin->raw_data + symtab_header->sh_offset);
	for (uint32_t idx = 0; idx < 7; idx++) {
		if (symtab->st_value > cave_begin)
			symtab->st_value += resize_needed;
		symtab = (Elf32_Sym *)((void *)symtab + symtab_header->sh_entsize);
	}
}

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
		return -1;
	}
	if (cave_begin + bin->len_payload < bin->data_len)
		return 0;
	return cave_begin + bin->len_payload - bin->data_len;
}

int32_t reinit_bin_ptr_64(t_bin* bin) {
	bin->elf64_header = (Elf64_Ehdr *)bin->raw_data;
	size_t curr_offset = bin->elf64_header->e_phoff;
	phdr_list_64_t* current_phdrs = bin->phdrs_64;
	for (uint64_t idx = 0; idx != bin->elf64_header->e_phnum; idx++) {
		current_phdrs->program_header = (Elf64_Phdr *)(bin->raw_data + curr_offset);
		curr_offset += bin->elf64_header->e_phentsize;
		current_phdrs = current_phdrs->next;
	}
	return 0;
}

int32_t reinit_bin_ptr_32(t_bin* bin) {
	bin->elf32_header = (Elf32_Ehdr *)bin->raw_data;
	size_t curr_offset = bin->elf32_header->e_phoff;
	phdr_list_32_t* current_phdrs = bin->phdrs_32;
	for (uint64_t idx = 0; idx != bin->elf32_header->e_phnum; idx++) {
		current_phdrs->program_header = (Elf32_Phdr *)(bin->raw_data + curr_offset);
		curr_offset += bin->elf32_header->e_phentsize;
		current_phdrs = current_phdrs->next;
	}
	return 0;
}

void modify_header_64(t_bin* bin, const uint64_t cave_begin, const uint64_t resize_needed) {
	for (const phdr_list_64_t* seg_h = bin->phdrs_64; seg_h != 0; seg_h = seg_h->next) {
		if (seg_h->program_header->p_offset > cave_begin) {
			seg_h->program_header->p_offset += resize_needed;
			seg_h->program_header->p_vaddr += resize_needed;
			seg_h->program_header->p_paddr += resize_needed;
		}
	}
	bin->elf64_header->e_shoff += resize_needed;
	Elf64_Shdr* shdr = (Elf64_Shdr *)(bin->raw_data + bin->elf64_header->e_shoff);
	for (size_t i = 0; i != bin->elf64_header->e_shnum; i++) {
		shdr = (Elf64_Shdr *)((void *)shdr + bin->elf64_header->e_shentsize);
		if (shdr->sh_offset > cave_begin) {
			shdr->sh_offset += resize_needed;
			shdr->sh_addr += resize_needed;
		}
	}
	uint64_t vaddr_offset = bin->phdrs_64->program_header->p_vaddr - bin->phdrs_64->program_header->p_offset;
	Elf64_Shdr *symtab_header = get_symtab_header_64(bin);
	if (!symtab_header)
		return ;
	offset_symtab_64(bin, symtab_header, vaddr_offset + cave_begin, resize_needed);
}

void modify_header_32(t_bin* bin, const uint64_t cave_begin, const uint64_t resize_needed) {
	for (const phdr_list_32_t* seg_h = bin->phdrs_32; seg_h != 0; seg_h = seg_h->next) {
		if (seg_h->program_header->p_offset > cave_begin) {
			seg_h->program_header->p_offset += resize_needed;
			seg_h->program_header->p_vaddr += resize_needed;
			seg_h->program_header->p_paddr += resize_needed;
		}
	}
	bin->elf32_header->e_shoff += resize_needed;
	Elf32_Shdr* shdr = (Elf32_Shdr *)(bin->raw_data + bin->elf32_header->e_shoff);
	for (size_t i = 0; i != bin->elf32_header->e_shnum; i++) {
		shdr = (Elf32_Shdr *)((void *)shdr + bin->elf32_header->e_shentsize);
		if (shdr->sh_offset > cave_begin) {
			shdr->sh_offset += resize_needed;
			shdr->sh_addr += resize_needed;
		}
	}
	uint32_t vaddr_offset = bin->phdrs_32->program_header->p_vaddr - bin->phdrs_32->program_header->p_offset;
	Elf32_Shdr *symtab_header = get_symtab_header_32(bin);
	if (!symtab_header)
		return ;
	offset_symtab_32(bin, symtab_header, vaddr_offset + cave_begin, resize_needed);
}

int32_t resize_file_64(t_bin* bin, const uint64_t cave_begin, const uint64_t resize_needed) {
	bin->raw_data = realloc(bin->raw_data, bin->data_len + resize_needed);
	if (!bin->raw_data) {
		perror("realloc");
		return 1;
	}
	memcpy(bin->raw_data + cave_begin + resize_needed, bin->raw_data + cave_begin, bin->data_len - cave_begin);
	bin->data_len += resize_needed;
	reinit_bin_ptr_64(bin);
	modify_header_64(bin, cave_begin, resize_needed);
	return 0;
}

int32_t resize_file_32(t_bin* bin, const uint64_t cave_begin, const uint64_t resize_needed) {
	bin->raw_data = realloc(bin->raw_data, bin->data_len + resize_needed);
	if (!bin->raw_data) {
		perror("realloc");
		return 1;
	}
	memcpy(bin->raw_data + cave_begin + resize_needed, bin->raw_data + cave_begin, bin->data_len - cave_begin);
	bin->data_len += resize_needed;
	reinit_bin_ptr_32(bin);
	modify_header_32(bin, cave_begin, resize_needed);
	return 0;
}


int find_code_cave_64(t_bin* bin) {
	Elf64_Ehdr* header = bin->elf64_header;
	Elf64_Phdr* txt_segment_h = get_segment_64(bin->phdrs_64, is_text_segment_64);
	uint64_t offset = txt_segment_h->p_offset + txt_segment_h->p_filesz;
	uint64_t aligned_offset = ALIGN_UP(offset, 4);
	offset = aligned_offset - offset;
	uint64_t resize_needed = get_resize_64(bin, aligned_offset);
	if (resize_needed == (uint64_t)-1) {
		fprintf(stderr, "Resize file needed!");
		return 1;
	}
	if (resize_needed) {
		resize_needed = ALIGN_UP(resize_needed, 4096);
		if (resize_file_64(bin, aligned_offset, resize_needed)) {
			return 1;
		}
		header = bin->elf64_header;
		txt_segment_h = get_segment_64(bin->phdrs_64, is_text_segment_64);
		offset = txt_segment_h->p_offset + txt_segment_h->p_filesz;
		aligned_offset = ALIGN_UP(offset, 4);
		offset = aligned_offset - offset;
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
	if (resize_needed == (uint32_t)-1) {
		fprintf(stderr, "Resize file needed!");
		return 1;
	}
	if (resize_needed) {
		resize_needed = ALIGN_UP(resize_needed, 4096);
		if (resize_file_32(bin, aligned_offset, resize_needed)) {
			return 1;
		}
		header = bin->elf32_header;
		txt_segment_h = get_segment_32(bin->phdrs_32, is_text_segment_32);
		offset = txt_segment_h->p_offset + txt_segment_h->p_filesz;
		aligned_offset = ALIGN_UP(offset, 4);
		offset = aligned_offset - offset;
	}
	memcpy(bin->raw_data + aligned_offset, bin->payload, bin->len_payload);
	const uint32_t entry_offset = header->e_entry - txt_segment_h->p_vaddr;
	header->e_entry += -entry_offset + txt_segment_h->p_memsz + OFFSET_ENTRY + offset;
	txt_segment_h->p_flags |= PROT_WRITE;
	txt_segment_h->p_filesz += bin->len_payload + offset;
	txt_segment_h->p_memsz += bin->len_payload + offset;
	return 0;
}