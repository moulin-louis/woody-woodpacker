

aes_decrypt:
	push   r8
  mov    r12, rsi
  mov    rbp, rdx
	sub		 rbp, 0x10
  xor    ebx, ebx
  sub    rsp, 0xc8
  xor    eax, eax
  lea    r13, [rsp+0x8]
  mov    rsi, r13
  call   generate_keys
  cmp    rbx, rbp ; data_offset < data_len    jmp 24
  jae    aes_decrypt+0x3b
  lea    rdi, [r12+rbx*1]
  mov    rsi, r13
  add    rbx, 0x10 ; data_offset += 16
  call   decrypt_segment
  jmp    aes_decrypt+0x24 ; end loop
  add    rsp, 0xc8   ; jmp 3b
	pop		 r8
  jmp    r8      

sub_letter: 
  mov    eax, edi
  mov    edx, edi
  rol    dl, 0x4
  rol    al, 1
  xor    eax, edx
  mov    edx, edi
  xor    eax, edi
  rol    dl, 0x2
  xor    eax, edx
  rol    dil, 0x3
  xor    eax, edi
  xor    eax, 0x63
  ret    

inv_sub_letter:
	mov    eax, edi
	mov    edx, edi
	rol    dil, 0x3
	rol    al, 1
	ror    dl, 0x2
	xor    eax, edx
	xor    eax, edi
	xor    eax, 0x5
	ret    

sub_word:
  push   rcx
  mov    dword [rsp+0x4], edi
  lea    rcx, [rsp+0x4]
  lea    rsi, [rsp+0x8]
  movzx  edi, byte [rcx]   ; jmp f
  inc    rcx
  call   sub_letter
  mov    byte [rcx-0x1], al
  cmp    rcx, rsi
  jne    sub_word+0xf		
  mov    eax, dword [rsp+0x4]
  pop    rdx
  ret    

add_round_key:
  movzx  edx, dl
  add    edx, edx
  movsxd rax, edx
  inc    edx
  movsxd rdx, edx
  mov    rax, qword [rsi+rax*8]
  xor    qword [rdi], rax
  mov    rax, qword [rsi+rdx*8]
  xor    qword [rdi+0x8], rax
  ret    

inv_sub_bytes:
  mov    rsi, rdi
  xor    ecx, ecx
  movzx  edi, byte [rsi+rcx*1] ; jmp 5
  call   inv_sub_letter
  mov    byte [rsi+rcx*1], al
  inc    rcx
  cmp    rcx, 0x10
  jne    inv_sub_bytes+0x5
  ret    

;inv_shift_rows:
;  ror    dword [rdi+0x4], 0x8
;  rol    dword [rdi+0x8], 0x10
;  rol    dword [rdi+0xc], 0x8
;  ret    

galois_mul:
  xor    eax, eax
  test   dil, dil			; jmp 2
  je     galois_mul+0x2a
  test   sil, sil
  je     galois_mul+0x2a
  test   sil, 0x1
  je     galois_mul+0x14
  xor    eax, edi
  lea    edx, [rdi+rdi*1] ; jmp 14
  lea    ecx, [rdi+rdi*1]
  xor    edx, 0x1b
  test   dil, dil
  cmovns edx, ecx
  shr    sil, 1
  mov    edi, edx
  jmp    galois_mul+0x2
  ret    						; jmp 2a

inv_mix_columns:
  sub    rsp, 0x28
  mov    r10, rdi
  xor    r9d, r9d
  xor    eax, eax
  movzx  edi, byte [r10+r9*1] ; begin loop jmp c
  mov    esi, 0xe
  call   galois_mul
  mov    esi, 0xb
  mov    r8d, eax
  lea    eax, [r9+0x4]
  and    eax, 0xf
  movzx  edi, byte [r10+rax*1]
  call   galois_mul
  mov    esi, 0xd
  xor    r8d, eax
  lea    eax, [r9+0x8]
  and    eax, 0xf
  movzx  edi, byte [r10+rax*1]
  call   galois_mul
  mov    esi, 0x9
  xor    r8d, eax
  lea    eax, [r9+0xc]
  and    eax, 0xf
  movzx  edi, byte [r10+rax*1]
  call   galois_mul
  xor    r8d, eax
  mov    byte [rsp+r9*1+0x8], r8b
  inc    r9
  cmp    r9, 0x10
  jne    inv_mix_columns+0xc ; jump to begin loop
  movups xmm0, [rsp+0x8]
  movups [r10], xmm0
  add    rsp, 0x28
  ret    

decrypt_segment:
  push   r12
  mov    edx, 0xa
  mov    r12, rsi
  push   rbp
  mov    rbp, rdi
  push   rbx
  mov    ebx, 0x9
  call   add_round_key
  mov    rdi, rbp			; jmp 19
  ror    dword [rdi+0x4], 0x8
  rol    dword [rdi+0x8], 0x10
  rol    dword [rdi+0xc], 0x8
  call   inv_sub_bytes
  mov    edx, ebx
  mov    rsi, r12
  mov    rdi, rbp
  call   add_round_key
  call   inv_mix_columns
  dec    ebx
  jne    decrypt_segment+0x19
  mov    rdi, rbp
  ror    dword [rdi+0x4], 0x8
  rol    dword [rdi+0x8], 0x10
  rol    dword [rdi+0xc], 0x8
  call   inv_sub_bytes
  pop    rbx
  mov    rsi, r12
  mov    rdi, rbp
  xor    edx, edx
  pop    rbp
  pop    r12
  jmp    add_round_key

generate_keys:
  push   rbx
	movups xmm0, [rdi]
	mov    r11, rsi
	mov    r10d, 0x4
	mov    r9b, 0x1
	movups [rsi], xmm0
	mov    ebx, dword [r11+r10*4-0x10] ; jmp 13
	mov    r8d, dword [r11+r10*4-0x4]
	test   r10b, 0x3
	je     generate_keys+0x28
	xor    r8d, ebx
	jmp    generate_keys+0x51
	mov    edi, r8d			; jmp 28
	rol    edi, 0x8
	call   sub_word
	movzx  edi, r9b
	shl    r9d, 0x18
	mov    esi, 0x2
	xor    r9d, ebx
	mov    r8d, r9d
	xor    r8d, eax
	call   galois_mul
	mov    r9d, eax
	mov    dword [r11+r10*4], r8d ; jmp 51
	inc    r10
	cmp    r10, 0x2c
	jne    generate_keys+0x13
	pop    rbx
	ret
