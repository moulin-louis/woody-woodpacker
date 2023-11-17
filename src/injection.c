#include "woody.h"

Elf64_Shdr	*get_symtab_header(t_bin *bin) {
	Elf64_Shdr *s_headers = (Elf64_Shdr *)(bin->raw_data + bin->elf_header->e_shoff);
	for (uint16_t idx = 0; idx < bin->elf_header->e_shnum; idx++) {
		if (s_headers->sh_type == SHT_SYMTAB) {
			printf ("symbole table found !!!\n");
			return s_headers;
		}
		s_headers = (Elf64_Shdr *)((void *)s_headers + bin->elf_header->e_shentsize);
	}
	return 0;
}

void offset_symtab(t_bin *bin, Elf64_Shdr *symtab_header, const uint64_t cave_begin, const uint64_t resize_needed) {
	Elf64_Sym *symtab = (Elf64_Sym *)(bin->raw_data + symtab_header->sh_offset);
	for (uint16_t idx = 0; idx < 7; idx++) {
		if (symtab->st_value > cave_begin)
			symtab->st_value += resize_needed;
		symtab = (Elf64_Sym *)((void *)symtab + symtab_header->sh_entsize);
	}
}

uint64_t get_resize(const t_bin *bin, const uint64_t cave_begin) {
  for (const phdr_list_t *seg_h = bin->phdrs; seg_h != 0; seg_h = seg_h->next) {
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

int32_t reinit_bin_ptr(t_bin *bin) {
	bin->elf_header = (Elf64_Ehdr *)bin->raw_data;
	size_t curr_offset = bin->elf_header->e_phoff;
	phdr_list_t *current_phdrs = bin->phdrs;
	for (uint16_t idx = 0; idx != bin->elf_header->e_phnum; idx++) {
		current_phdrs->program_header = (Elf64_Phdr *)(bin->raw_data + curr_offset);
		curr_offset += bin->elf_header->e_phentsize;
		current_phdrs = current_phdrs->next;
	}

	return 0;
}

void modify_header(t_bin *bin, const uint64_t cave_begin, const uint64_t resize_needed) {
	for (const phdr_list_t *seg_h = bin->phdrs; seg_h != 0; seg_h = seg_h->next) {
		if (seg_h->program_header->p_offset > cave_begin) {
			seg_h->program_header->p_offset += resize_needed;
			seg_h->program_header->p_vaddr += resize_needed;
			seg_h->program_header->p_paddr += resize_needed;
		}
	}
	bin->elf_header->e_shoff += resize_needed;
	Elf64_Shdr *shdr = (Elf64_Shdr *)(bin->raw_data + bin->elf_header->e_shoff);
	for (size_t i = 0; i != bin->elf_header->e_shnum; i++) {
		shdr = (Elf64_Shdr *)((void *)shdr + bin->elf_header->e_shentsize);
		if (shdr->sh_offset > cave_begin) {
			shdr->sh_offset += resize_needed;
			shdr->sh_addr += resize_needed;
		}
	}
	uint64_t vaddr_offset = bin->phdrs->program_header->p_vaddr - bin->phdrs->program_header->p_offset;
	Elf64_Shdr *symtab_header = get_symtab_header(bin);
	offset_symtab(bin, symtab_header, vaddr_offset + cave_begin, resize_needed);
}

int32_t resize_file(t_bin *bin, const uint64_t cave_begin, const uint64_t resize_needed) {
	bin->raw_data = realloc(bin->raw_data, bin->data_len + resize_needed);
  if (!bin->raw_data) {
    perror("realloc");
    return 1;
  }
  // bin->raw_data = tmp;
  memcpy(bin->raw_data + cave_begin + resize_needed, bin->raw_data + cave_begin, bin->data_len - cave_begin);
  // memcpy(bin->raw_data + cave_begin, bin->payload, bin->len_payload);
  bin->data_len += resize_needed;
  reinit_bin_ptr(bin);
  modify_header(bin, cave_begin, resize_needed);
  return 0;
}

int find_code_cave(t_bin *bin) {
  Elf64_Ehdr *header = bin->elf_header;
  Elf64_Phdr *txt_segment_h = get_segment(bin->phdrs, is_text_segment_64);
  uint64_t offset = txt_segment_h->p_offset + txt_segment_h->p_filesz;
  uint64_t aligned_offset = ALIGN_UP(offset, 4);
	offset = aligned_offset - offset;
  // if (aligned_offset < bin->elf_header->e_shoff &&
	// 	bin->elf_header->e_shoff < aligned_offset + bin->len_payload) {
	// 	printf("injection after section headers\n");
	// 	offset = bin->elf_header->e_shoff + bin->elf_header->e_shnum * bin->elf_header->e_shnum;
	// 	aligned_offset = ALIGN_UP(offset, 4);
  // }
  uint64_t resize_needed = get_resize(bin, aligned_offset);
  if (resize_needed) {
		printf("cave too small, resizing file by %lu\n", resize_needed);
		resize_needed = ALIGN_UP(resize_needed, 4096);
		if (resize_file(bin, aligned_offset, resize_needed)) {
			printf("Error while resizing file\n");
			return 1;
		}
		printf("new file size: %lu\n", bin->data_len);
		header = bin->elf_header;
		txt_segment_h = get_segment(bin->phdrs, is_text_segment_64);
		offset = txt_segment_h->p_offset + txt_segment_h->p_filesz;
		aligned_offset = ALIGN_UP(offset, 4);
		offset = aligned_offset - offset;
  }
  // offset = txt_segment_h->p_offset + txt_segment_h->p_filesz;
  // aligned_offset = ALIGN_UP(offset, 4);
  // offset = aligned_offset - offset;
  printf("will write to this offset: %#lx\n", aligned_offset);
  printf("offset %% 4 = %lu\n", (aligned_offset - (txt_segment_h->p_offset + txt_segment_h->p_filesz)) % 4);
  // printf("offset %% 4 = %lu\n", offset % 4);
  // printf("will write to this offset: %#lx\n", offset);
  memcpy(bin->raw_data + aligned_offset, bin->payload, bin->len_payload);
//  printf("e entry = %#lx\n", header->e_entry);
//  printf("txt segment offset = %#lx\n", txt_segment_h->p_offset);
//  printf("txt mensz = %#lx\n", txt_segment_h->p_memsz);
//  printf("adding thsi to e entry = %#lx\n", -(header->e_entry - txt_segment_h->p_offset) + txt_segment_h->p_memsz);
//   header->e_entry = 0x20c0;
//   txt_segment_h->p_memsz = 0x1149;
//   u_int64_t neg_offset = ~txt_segment_h->p_offset;
//   printf ("neg offset: %lx\n", neg_offset);
printf("e entry: %#lx\n", header->e_entry);
printf("txt segment virtual address: %#lx\n", txt_segment_h->p_vaddr);
  const uint64_t entry_offset = header->e_entry - txt_segment_h->p_vaddr;
 printf ("entry offset: %#lx\n", entry_offset);
 printf("text segment memory size: %#lx\n", txt_segment_h->p_memsz);
//   entry_offset &= 
  header->e_entry += -entry_offset + txt_segment_h->p_memsz + PAYLOAD_OFFSET_ENTRY + offset;
//   rewrite because c'est cheum
//  printf("new entry point: %#lx\n", header->e_entry);
  txt_segment_h->p_flags |= PROT_WRITE;
  txt_segment_h->p_filesz += bin->len_payload + offset;
  txt_segment_h->p_memsz += bin->len_payload + offset;
  return 0;
}