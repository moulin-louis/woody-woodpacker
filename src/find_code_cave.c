#include "woody.h"

uint64_t cave_too_small(t_bin *bin, uint64_t cave_begin) {
  for (phdr_list_t *seg_h = bin->phdrs; seg_h != 0; seg_h = seg_h->next) {
    if (seg_h->program_header->p_type != PT_LOAD)
      continue;
    if (seg_h->program_header->p_offset < cave_begin)
      continue;
    if (cave_begin + bin->len_payload < seg_h->program_header->p_offset)
      return 0;
    return (cave_begin + bin->len_payload - seg_h->program_header->p_offset);
  }
  if (cave_begin + bin->len_payload < bin->data_len)
    return 0;
  return (cave_begin + bin->len_payload - bin->data_len);
}

int reinit_bin_ptr(t_bin *bin) {
	bin->elf_header = (Elf64_Ehdr *)bin->raw_data;
	size_t curr_offset = bin->elf_header->e_phoff;
	phdr_list_t *current_phdrs = bin->phdrs;
	for (uint16_t idx = 0; idx != bin->elf_header->e_phnum; idx++) {
		curr_offset += bin->elf_header->e_phentsize;
		current_phdrs->program_header = (Elf64_Phdr *)(bin->raw_data + curr_offset);
	}
	return 0;
}

int	modify_header(t_bin *bin, uint64_t cave_begin, uint64_t resize_needed) {
	for (phdr_list_t *seg_h = bin->phdrs; seg_h != 0; seg_h = seg_h->next) {
		if (seg_h->program_header->p_offset > cave_begin) {
			seg_h->program_header->p_offset += resize_needed;
			seg_h->program_header->p_vaddr += resize_needed;
		}
	}
	return 0;
}

int resize_file(t_bin *bin, uint64_t cave_begin, uint64_t resize_needed) {
  void *tmp = realloc(bin->raw_data, bin->data_len + resize_needed);
  if (!tmp) {
    perror("realloc");
    return 1;
  }
  bin->raw_data = tmp;
  memcpy(bin->raw_data + cave_begin + resize_needed, bin->raw_data + cave_begin, bin->data_len - cave_begin);
  memcpy(bin->raw_data + cave_begin, bin->payload, bin->len_payload);
  bin->data_len += resize_needed;
  reinit_bin_ptr(bin);
  modify_header(bin, cave_begin, resize_needed);
  return 0;
}

void *find_code_cave(t_bin *bin) {
  Elf64_Ehdr *header = bin->elf_header;
  Elf64_Phdr *txt_segment_h = (Elf64_Phdr *) get_segment(bin->phdrs, is_text_segment_64);
  uint64_t resize_needed = cave_too_small(bin, txt_segment_h->p_offset + txt_segment_h->p_filesz);
  if (resize_needed) {
    resize_needed *= 2;
    printf("resize needed: %lu\n", resize_needed);
    printf("Cave too small\n");
	resize_needed = ALIGN_UP(resize_needed, 4096);
    if (resize_file(bin, txt_segment_h->p_offset + txt_segment_h->p_filesz, resize_needed)) {
      printf("Error while resizing file\n");
      return NULL;
    }
  }
  memcpy(bin->raw_data + txt_segment_h->p_offset + txt_segment_h->p_filesz, bin->payload, bin->len_payload);

  header->e_entry += txt_segment_h->p_memsz + PAYLOAD_OFFSET_ENTRY;
  txt_segment_h->p_flags |= PROT_WRITE;
  txt_segment_h->p_filesz += bin->len_payload;
  txt_segment_h->p_memsz += bin->len_payload;
  return NULL;
}