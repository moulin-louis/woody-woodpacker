#include "woody.h"

char PAYLOAD[] = {0x77,0x6F,0x6F,0x64,0x79,0x05,0xB8,0x01,0x00,0x00,0x00,0xBF,0x01,0x00,0x00,0x00,0x48,0x8D,0x35,0xE9,0xFF,0xFF,0xFF,0xBA,0x05,0x00,0x00,0x00,0x0F,0x05,0xB8,0x3C,0x00,0x00,0x00,0xBF,0x2A,0x00,0x00,0x00,0x0F,0x05};
#define PAYLOAD_SIZE sizeof(PAYLOAD)

uint64_t	cave_too_small(t_bin *bin, uint64_t cave_begin, uint64_t payload_size) {
	for (phdr_list_t *seg_h = bin->phdrs; seg_h != 0; seg_h = seg_h->next) {
		if (seg_h->program_header->p_type != PT_LOAD)
			continue;
		if (seg_h->program_header->p_offset < cave_begin)
			continue ;
		if (cave_begin + payload_size < seg_h->program_header->p_offset)
			return 0;
		return (cave_begin + payload_size - seg_h->program_header->p_offset);
	}
	if (cave_begin + payload_size < bin->data_len)
		return 0;
	return (cave_begin + payload_size - bin->data_len);
}

int resize_file(t_bin *bin, uint64_t cave_begin, uint64_t resize_needed) {
	bin->raw_data = realloc(bin->raw_data, bin->data_len + resize_needed);
	memcpy(bin->raw_data + cave_begin + resize_needed, bin->raw_data + cave_begin, bin->data_len - cave_begin);
	memcpy(bin->raw_data + cave_begin, PAYLOAD, PAYLOAD_SIZE);
	bin->data_len += resize_needed;
	// redo ptr on headers
	// reinit_headers(bin, )
	return 0;
}

void *find_code_cave(t_bin *bin) {

	// for (phdr_list_t *seg_header = bin->phdrs; seg_header != 0; seg_header = seg_header->next) {
	// 	Elf64_Phdr current = seg_header->program_header;
	// 	if (current.p_type != PT_LOAD)
	// 		continue;
	// 	if (seg_header->next == 0 &&
	// 		allign_up(current.p_offset + current.p_filesz, current.p_align) - current.p_offset + current.p_filesz ) {
				
	// 	}

	// }
	// bin->raw_data = realloc(bin->raw_data, bin->data_len + PAYLOAD_SIZE);
	Elf64_Ehdr *header = bin->elf_header;
	Elf64_Phdr *txt_segment_h = (Elf64_Phdr *)get_segment(bin->phdrs, is_text_segment_64);
	printf("filesz = %#lx\n", txt_segment_h->p_filesz);
	uint64_t resize_needed = cave_too_small(bin, txt_segment_h->p_offset + txt_segment_h->p_filesz, PAYLOAD_SIZE);
	if (resize_needed) {
		printf ("Cave too small\n");
		resize_file(bin, txt_segment_h->p_offset + txt_segment_h->p_filesz, resize_needed);
	}
	memcpy(bin->raw_data + txt_segment_h->p_offset + txt_segment_h->p_filesz, PAYLOAD, PAYLOAD_SIZE);
	
	header->e_entry += txt_segment_h->p_memsz + 6;


	void *old_addres = (void *)txt_segment_h->p_vaddr;
	printf("txt_segment_h = %p\n", (void *)txt_segment_h);

	txt_segment_h->p_flags |= PROT_WRITE;
	txt_segment_h->p_filesz += PAYLOAD_SIZE;
	txt_segment_h->p_memsz += PAYLOAD_SIZE;

	printf("new range segment = %p..%p\n", old_addres, old_addres + txt_segment_h->p_memsz);
	printf("Entry point = %p\n", (void *)header->e_entry);
	return NULL;
}