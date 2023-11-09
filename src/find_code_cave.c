#include "woody.h"

void *find_code_cave(t_bin *bin) {
char PAYLOAD[] = {0x77,0x6F,0x6F,0x64,0x79,0x05,0xB8,0x01,0x00,0x00,0x00,0xBF,0x01,0x00,0x00,0x00,0x48,0x8D,0x35,0xE9,0xFF,0xFF,0xFF,0xBA,0x05,0x00,0x00,0x00,0x0F,0x05,0xB8,0x3C,0x00,0x00,0x00,0xBF,0x2A,0x00,0x00,0x00,0x0F,0x05};

	// for (phdr_list_t *seg_header = bin->phdrs; seg_header != 0; seg_header = seg_header->next) {
	// 	Elf64_Phdr current = seg_header->program_header;
	// 	if (current.p_type != PT_LOAD)
	// 		continue;
	// 	if (seg_header->next == 0 &&
	// 		allign_up(current.p_offset + current.p_filesz, current.p_align) - current.p_offset + current.p_filesz ) {
				
	// 	}

	// }
	// bin->raw_data = realloc(bin->raw_data, bin->data_len + PAYLOAD_SIZE);
	Elf64_Ehdr *header = (Elf64_Ehdr *)bin->raw_data;
	Elf64_Phdr *text_segment_header = (Elf64_Phdr *)(bin->raw_data + header->e_phoff + (header->e_phentsize * 1));
	printf("filesz = %#lx\n", text_segment_header->p_filesz);
	memcpy(bin->raw_data + text_segment_header->p_offset + text_segment_header->p_filesz, PAYLOAD, PAYLOAD_SIZE);

	header->e_entry += text_segment_header->p_memsz + 6;


	void *old_addres = (void *)text_segment_header->p_vaddr;
	printf("text_segment_header = %p\n", (void *)text_segment_header);

	text_segment_header->p_flags |= PROT_WRITE;
	text_segment_header->p_filesz += PAYLOAD_SIZE;
	text_segment_header->p_memsz += PAYLOAD_SIZE;

	printf("new range segment = %p..%p\n", old_addres, old_addres + text_segment_header->p_memsz);
	printf("Entry point = %p\n", (void *)header->e_entry);
	return NULL;
}