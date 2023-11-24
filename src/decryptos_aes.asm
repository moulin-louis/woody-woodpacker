
a.out:     file format elf64-x86-64


Disassembly of section .init:

0000000000001000 <_init>:
    1000:	f3 0f 1e fa          	endbr64 
    1004:	48 83 ec 08          	sub    rsp,0x8
    1008:	48 8b 05 d9 2f 00 00 	mov    rax,QWORD PTR [rip+0x2fd9]        # 3fe8 <__gmon_start__@Base>
    100f:	48 85 c0             	test   rax,rax
    1012:	74 02                	je     1016 <_init+0x16>
    1014:	ff d0                	call   rax
    1016:	48 83 c4 08          	add    rsp,0x8
    101a:	c3                   	ret    

Disassembly of section .plt:

0000000000001020 <.plt>:
    1020:	ff 35 92 2f 00 00    	push   QWORD PTR [rip+0x2f92]        # 3fb8 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:	f2 ff 25 93 2f 00 00 	bnd jmp QWORD PTR [rip+0x2f93]        # 3fc0 <_GLOBAL_OFFSET_TABLE_+0x10>
    102d:	0f 1f 00             	nop    DWORD PTR [rax]
    1030:	f3 0f 1e fa          	endbr64 
    1034:	68 00 00 00 00       	push   0x0
    1039:	f2 e9 e1 ff ff ff    	bnd jmp 1020 <_init+0x20>
    103f:	90                   	nop
    1040:	f3 0f 1e fa          	endbr64 
    1044:	68 01 00 00 00       	push   0x1
    1049:	f2 e9 d1 ff ff ff    	bnd jmp 1020 <_init+0x20>
    104f:	90                   	nop

Disassembly of section .plt.got:

0000000000001050 <__cxa_finalize@plt>:
    1050:	f3 0f 1e fa          	endbr64 
    1054:	f2 ff 25 9d 2f 00 00 	bnd jmp QWORD PTR [rip+0x2f9d]        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    105b:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]

Disassembly of section .plt.sec:

0000000000001060 <__stack_chk_fail@plt>:
    1060:	f3 0f 1e fa          	endbr64 
    1064:	f2 ff 25 5d 2f 00 00 	bnd jmp QWORD PTR [rip+0x2f5d]        # 3fc8 <__stack_chk_fail@GLIBC_2.4>
    106b:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]

0000000000001070 <calloc@plt>:
    1070:	f3 0f 1e fa          	endbr64 
    1074:	f2 ff 25 55 2f 00 00 	bnd jmp QWORD PTR [rip+0x2f55]        # 3fd0 <calloc@GLIBC_2.2.5>
    107b:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]

Disassembly of section .text:

0000000000001080 <main>:
    1080:	f3 0f 1e fa          	endbr64 
    1084:	48 83 ec 28          	sub    rsp,0x28
    1088:	be 01 00 00 00       	mov    esi,0x1
    108d:	bf 00 e1 f5 05       	mov    edi,0x5f5e100
    1092:	64 48 8b 04 25 28 00 	mov    rax,QWORD PTR fs:0x28
    1099:	00 00 
    109b:	48 89 44 24 18       	mov    QWORD PTR [rsp+0x18],rax
    10a0:	31 c0                	xor    eax,eax
    10a2:	e8 c9 ff ff ff       	call   1070 <calloc@plt>
    10a7:	b9 00 e1 f5 05       	mov    ecx,0x5f5e100
    10ac:	ba 00 e1 f5 05       	mov    edx,0x5f5e100
    10b1:	0f 28 05 58 0f 00 00 	movaps xmm0,XMMWORD PTR [rip+0xf58]        # 2010 <_IO_stdin_used+0x10>
    10b8:	48 89 c6             	mov    rsi,rax
    10bb:	b0 69                	mov    al,0x69
    10bd:	48 89 f7             	mov    rdi,rsi
    10c0:	0f 11 44 24 08       	movups XMMWORD PTR [rsp+0x8],xmm0
    10c5:	f3 aa                	rep stos BYTE PTR es:[rdi],al
    10c7:	48 8d 7c 24 08       	lea    rdi,[rsp+0x8]
    10cc:	e8 87 03 00 00       	call   1458 <aes_decrypt>
    10d1:	48 8b 44 24 18       	mov    rax,QWORD PTR [rsp+0x18]
    10d6:	64 48 2b 04 25 28 00 	sub    rax,QWORD PTR fs:0x28
    10dd:	00 00 
    10df:	74 05                	je     10e6 <main+0x66>
    10e1:	e8 7a ff ff ff       	call   1060 <__stack_chk_fail@plt>
    10e6:	31 c0                	xor    eax,eax
    10e8:	48 83 c4 28          	add    rsp,0x28
    10ec:	c3                   	ret    
    10ed:	0f 1f 00             	nop    DWORD PTR [rax]

00000000000010f0 <_start>:
    10f0:	f3 0f 1e fa          	endbr64 
    10f4:	31 ed                	xor    ebp,ebp
    10f6:	49 89 d1             	mov    r9,rdx
    10f9:	5e                   	pop    rsi
    10fa:	48 89 e2             	mov    rdx,rsp
    10fd:	48 83 e4 f0          	and    rsp,0xfffffffffffffff0
    1101:	50                   	push   rax
    1102:	54                   	push   rsp
    1103:	45 31 c0             	xor    r8d,r8d
    1106:	31 c9                	xor    ecx,ecx
    1108:	48 8d 3d 71 ff ff ff 	lea    rdi,[rip+0xffffffffffffff71]        # 1080 <main>
    110f:	ff 15 c3 2e 00 00    	call   QWORD PTR [rip+0x2ec3]        # 3fd8 <__libc_start_main@GLIBC_2.34>
    1115:	f4                   	hlt    
    1116:	66 2e 0f 1f 84 00 00 	cs nop WORD PTR [rax+rax*1+0x0]
    111d:	00 00 00 

0000000000001120 <deregister_tm_clones>:
    1120:	48 8d 3d e9 2e 00 00 	lea    rdi,[rip+0x2ee9]        # 4010 <__TMC_END__>
    1127:	48 8d 05 e2 2e 00 00 	lea    rax,[rip+0x2ee2]        # 4010 <__TMC_END__>
    112e:	48 39 f8             	cmp    rax,rdi
    1131:	74 15                	je     1148 <deregister_tm_clones+0x28>
    1133:	48 8b 05 a6 2e 00 00 	mov    rax,QWORD PTR [rip+0x2ea6]        # 3fe0 <_ITM_deregisterTMCloneTable@Base>
    113a:	48 85 c0             	test   rax,rax
    113d:	74 09                	je     1148 <deregister_tm_clones+0x28>
    113f:	ff e0                	jmp    rax
    1141:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]
    1148:	c3                   	ret    
    1149:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]

0000000000001150 <register_tm_clones>:
    1150:	48 8d 3d b9 2e 00 00 	lea    rdi,[rip+0x2eb9]        # 4010 <__TMC_END__>
    1157:	48 8d 35 b2 2e 00 00 	lea    rsi,[rip+0x2eb2]        # 4010 <__TMC_END__>
    115e:	48 29 fe             	sub    rsi,rdi
    1161:	48 89 f0             	mov    rax,rsi
    1164:	48 c1 ee 3f          	shr    rsi,0x3f
    1168:	48 c1 f8 03          	sar    rax,0x3
    116c:	48 01 c6             	add    rsi,rax
    116f:	48 d1 fe             	sar    rsi,1
    1172:	74 14                	je     1188 <register_tm_clones+0x38>
    1174:	48 8b 05 75 2e 00 00 	mov    rax,QWORD PTR [rip+0x2e75]        # 3ff0 <_ITM_registerTMCloneTable@Base>
    117b:	48 85 c0             	test   rax,rax
    117e:	74 08                	je     1188 <register_tm_clones+0x38>
    1180:	ff e0                	jmp    rax
    1182:	66 0f 1f 44 00 00    	nop    WORD PTR [rax+rax*1+0x0]
    1188:	c3                   	ret    
    1189:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]

0000000000001190 <__do_global_dtors_aux>:
    1190:	f3 0f 1e fa          	endbr64 
    1194:	80 3d 75 2e 00 00 00 	cmp    BYTE PTR [rip+0x2e75],0x0        # 4010 <__TMC_END__>
    119b:	75 2b                	jne    11c8 <__do_global_dtors_aux+0x38>
    119d:	55                   	push   rbp
    119e:	48 83 3d 52 2e 00 00 	cmp    QWORD PTR [rip+0x2e52],0x0        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    11a5:	00 
    11a6:	48 89 e5             	mov    rbp,rsp
    11a9:	74 0c                	je     11b7 <__do_global_dtors_aux+0x27>
    11ab:	48 8b 3d 56 2e 00 00 	mov    rdi,QWORD PTR [rip+0x2e56]        # 4008 <__dso_handle>
    11b2:	e8 99 fe ff ff       	call   1050 <__cxa_finalize@plt>
    11b7:	e8 64 ff ff ff       	call   1120 <deregister_tm_clones>
    11bc:	c6 05 4d 2e 00 00 01 	mov    BYTE PTR [rip+0x2e4d],0x1        # 4010 <__TMC_END__>
    11c3:	5d                   	pop    rbp
    11c4:	c3                   	ret    
    11c5:	0f 1f 00             	nop    DWORD PTR [rax]
    11c8:	c3                   	ret    
    11c9:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]

00000000000011d0 <frame_dummy>:
    11d0:	f3 0f 1e fa          	endbr64 
    11d4:	e9 77 ff ff ff       	jmp    1150 <register_tm_clones>

00000000000011d9 <rot_word_left>:
    11d9:	f3 0f 1e fa          	endbr64 
    11dd:	89 f8                	mov    eax,edi
    11df:	89 f1                	mov    ecx,esi
    11e1:	d3 c0                	rol    eax,cl
    11e3:	c3                   	ret    

00000000000011e4 <sub_letter>:
    11e4:	f3 0f 1e fa          	endbr64 
    11e8:	89 f8                	mov    eax,edi
    11ea:	89 fa                	mov    edx,edi
    11ec:	c0 c2 04             	rol    dl,0x4
    11ef:	d0 c0                	rol    al,1
    11f1:	31 d0                	xor    eax,edx
    11f3:	89 fa                	mov    edx,edi
    11f5:	31 f8                	xor    eax,edi
    11f7:	c0 c2 02             	rol    dl,0x2
    11fa:	31 d0                	xor    eax,edx
    11fc:	40 c0 c7 03          	rol    dil,0x3
    1200:	31 f8                	xor    eax,edi
    1202:	83 f0 63             	xor    eax,0x63
    1205:	c3                   	ret    

0000000000001206 <inv_sub_letter>:
    1206:	f3 0f 1e fa          	endbr64 
    120a:	89 f8                	mov    eax,edi
    120c:	89 fa                	mov    edx,edi
    120e:	40 c0 c7 03          	rol    dil,0x3
    1212:	d0 c0                	rol    al,1
    1214:	c0 ca 02             	ror    dl,0x2
    1217:	31 d0                	xor    eax,edx
    1219:	31 f8                	xor    eax,edi
    121b:	83 f0 05             	xor    eax,0x5
    121e:	c3                   	ret    

000000000000121f <sub_word>:
    121f:	f3 0f 1e fa          	endbr64 
    1223:	51                   	push   rcx
    1224:	89 7c 24 04          	mov    DWORD PTR [rsp+0x4],edi
    1228:	48 8d 4c 24 04       	lea    rcx,[rsp+0x4]
    122d:	48 8d 74 24 08       	lea    rsi,[rsp+0x8]
    1232:	0f b6 39             	movzx  edi,BYTE PTR [rcx]
    1235:	48 ff c1             	inc    rcx
    1238:	e8 a7 ff ff ff       	call   11e4 <sub_letter>
    123d:	88 41 ff             	mov    BYTE PTR [rcx-0x1],al
    1240:	48 39 f1             	cmp    rcx,rsi
    1243:	75 ed                	jne    1232 <sub_word+0x13>
    1245:	8b 44 24 04          	mov    eax,DWORD PTR [rsp+0x4]
    1249:	5a                   	pop    rdx
    124a:	c3                   	ret    

000000000000124b <add_round_key>:
    124b:	f3 0f 1e fa          	endbr64 
    124f:	0f b6 d2             	movzx  edx,dl
    1252:	01 d2                	add    edx,edx
    1254:	48 63 c2             	movsxd rax,edx
    1257:	ff c2                	inc    edx
    1259:	48 63 d2             	movsxd rdx,edx
    125c:	48 8b 04 c6          	mov    rax,QWORD PTR [rsi+rax*8]
    1260:	48 31 07             	xor    QWORD PTR [rdi],rax
    1263:	48 8b 04 d6          	mov    rax,QWORD PTR [rsi+rdx*8]
    1267:	48 31 47 08          	xor    QWORD PTR [rdi+0x8],rax
    126b:	c3                   	ret    

000000000000126c <inv_sub_bytes>:
    126c:	f3 0f 1e fa          	endbr64 
    1270:	48 89 fe             	mov    rsi,rdi
    1273:	31 c9                	xor    ecx,ecx
    1275:	0f b6 3c 0e          	movzx  edi,BYTE PTR [rsi+rcx*1]
    1279:	e8 88 ff ff ff       	call   1206 <inv_sub_letter>
    127e:	88 04 0e             	mov    BYTE PTR [rsi+rcx*1],al
    1281:	48 ff c1             	inc    rcx
    1284:	48 83 f9 10          	cmp    rcx,0x10
    1288:	75 eb                	jne    1275 <inv_sub_bytes+0x9>
    128a:	c3                   	ret    

000000000000128b <inv_shift_rows>:
    128b:	f3 0f 1e fa          	endbr64 
    128f:	c1 4f 04 08          	ror    DWORD PTR [rdi+0x4],0x8
    1293:	c1 47 08 10          	rol    DWORD PTR [rdi+0x8],0x10
    1297:	c1 47 0c 08          	rol    DWORD PTR [rdi+0xc],0x8
    129b:	c3                   	ret    

000000000000129c <galois_mul>:
    129c:	f3 0f 1e fa          	endbr64 
    12a0:	31 c0                	xor    eax,eax
    12a2:	40 84 ff             	test   dil,dil
    12a5:	74 23                	je     12ca <galois_mul+0x2e>
    12a7:	40 84 f6             	test   sil,sil
    12aa:	74 1e                	je     12ca <galois_mul+0x2e>
    12ac:	40 f6 c6 01          	test   sil,0x1
    12b0:	74 02                	je     12b4 <galois_mul+0x18>
    12b2:	31 f8                	xor    eax,edi
    12b4:	8d 14 3f             	lea    edx,[rdi+rdi*1]
    12b7:	8d 0c 3f             	lea    ecx,[rdi+rdi*1]
    12ba:	83 f2 1b             	xor    edx,0x1b
    12bd:	40 84 ff             	test   dil,dil
    12c0:	0f 49 d1             	cmovns edx,ecx
    12c3:	40 d0 ee             	shr    sil,1
    12c6:	89 d7                	mov    edi,edx
    12c8:	eb d8                	jmp    12a2 <galois_mul+0x6>
    12ca:	c3                   	ret    

00000000000012cb <inv_mix_columns>:
    12cb:	f3 0f 1e fa          	endbr64 
    12cf:	48 83 ec 28          	sub    rsp,0x28
    12d3:	49 89 fa             	mov    r10,rdi
    12d6:	45 31 c9             	xor    r9d,r9d
    12d9:	64 48 8b 04 25 28 00 	mov    rax,QWORD PTR fs:0x28
    12e0:	00 00 
    12e2:	48 89 44 24 18       	mov    QWORD PTR [rsp+0x18],rax
    12e7:	31 c0                	xor    eax,eax
    12e9:	43 0f b6 3c 0a       	movzx  edi,BYTE PTR [r10+r9*1]
    12ee:	be 0e 00 00 00       	mov    esi,0xe
    12f3:	e8 a4 ff ff ff       	call   129c <galois_mul>
    12f8:	be 0b 00 00 00       	mov    esi,0xb
    12fd:	41 89 c0             	mov    r8d,eax
    1300:	41 8d 41 04          	lea    eax,[r9+0x4]
    1304:	83 e0 0f             	and    eax,0xf
    1307:	41 0f b6 3c 02       	movzx  edi,BYTE PTR [r10+rax*1]
    130c:	e8 8b ff ff ff       	call   129c <galois_mul>
    1311:	be 0d 00 00 00       	mov    esi,0xd
    1316:	41 31 c0             	xor    r8d,eax
    1319:	41 8d 41 08          	lea    eax,[r9+0x8]
    131d:	83 e0 0f             	and    eax,0xf
    1320:	41 0f b6 3c 02       	movzx  edi,BYTE PTR [r10+rax*1]
    1325:	e8 72 ff ff ff       	call   129c <galois_mul>
    132a:	be 09 00 00 00       	mov    esi,0x9
    132f:	41 31 c0             	xor    r8d,eax
    1332:	41 8d 41 0c          	lea    eax,[r9+0xc]
    1336:	83 e0 0f             	and    eax,0xf
    1339:	41 0f b6 3c 02       	movzx  edi,BYTE PTR [r10+rax*1]
    133e:	e8 59 ff ff ff       	call   129c <galois_mul>
    1343:	41 31 c0             	xor    r8d,eax
    1346:	46 88 44 0c 08       	mov    BYTE PTR [rsp+r9*1+0x8],r8b
    134b:	49 ff c1             	inc    r9
    134e:	49 83 f9 10          	cmp    r9,0x10
    1352:	75 95                	jne    12e9 <inv_mix_columns+0x1e>
    1354:	0f 10 44 24 08       	movups xmm0,XMMWORD PTR [rsp+0x8]
    1359:	41 0f 11 02          	movups XMMWORD PTR [r10],xmm0
    135d:	48 8b 44 24 18       	mov    rax,QWORD PTR [rsp+0x18]
    1362:	64 48 2b 04 25 28 00 	sub    rax,QWORD PTR fs:0x28
    1369:	00 00 
    136b:	74 05                	je     1372 <inv_mix_columns+0xa7>
    136d:	e8 ee fc ff ff       	call   1060 <__stack_chk_fail@plt>
    1372:	48 83 c4 28          	add    rsp,0x28
    1376:	c3                   	ret    

0000000000001377 <decrypt_segment>:
    1377:	f3 0f 1e fa          	endbr64 
    137b:	41 54                	push   r12
    137d:	ba 0a 00 00 00       	mov    edx,0xa
    1382:	49 89 f4             	mov    r12,rsi
    1385:	55                   	push   rbp
    1386:	48 89 fd             	mov    rbp,rdi
    1389:	53                   	push   rbx
    138a:	bb 09 00 00 00       	mov    ebx,0x9
    138f:	e8 b7 fe ff ff       	call   124b <add_round_key>
    1394:	48 89 ef             	mov    rdi,rbp
    1397:	e8 ef fe ff ff       	call   128b <inv_shift_rows>
    139c:	e8 cb fe ff ff       	call   126c <inv_sub_bytes>
    13a1:	89 da                	mov    edx,ebx
    13a3:	4c 89 e6             	mov    rsi,r12
    13a6:	48 89 ef             	mov    rdi,rbp
    13a9:	e8 9d fe ff ff       	call   124b <add_round_key>
    13ae:	e8 18 ff ff ff       	call   12cb <inv_mix_columns>
    13b3:	ff cb                	dec    ebx
    13b5:	75 dd                	jne    1394 <decrypt_segment+0x1d>
    13b7:	48 89 ef             	mov    rdi,rbp
    13ba:	e8 cc fe ff ff       	call   128b <inv_shift_rows>
    13bf:	e8 a8 fe ff ff       	call   126c <inv_sub_bytes>
    13c4:	5b                   	pop    rbx
    13c5:	4c 89 e6             	mov    rsi,r12
    13c8:	48 89 ef             	mov    rdi,rbp
    13cb:	31 d2                	xor    edx,edx
    13cd:	5d                   	pop    rbp
    13ce:	41 5c                	pop    r12
    13d0:	e9 76 fe ff ff       	jmp    124b <add_round_key>

00000000000013d5 <generate_keys>:
    13d5:	f3 0f 1e fa          	endbr64 
    13d9:	0f 10 07             	movups xmm0,XMMWORD PTR [rdi]
    13dc:	49 89 f2             	mov    r10,rsi
    13df:	41 b9 04 00 00 00    	mov    r9d,0x4
    13e5:	41 b0 01             	mov    r8b,0x1
    13e8:	0f 11 06             	movups XMMWORD PTR [rsi],xmm0
    13eb:	47 8b 5c 8a f0       	mov    r11d,DWORD PTR [r10+r9*4-0x10]
    13f0:	43 8b 7c 8a fc       	mov    edi,DWORD PTR [r10+r9*4-0x4]
    13f5:	41 f6 c1 03          	test   r9b,0x3
    13f9:	74 11                	je     140c <generate_keys+0x37>
    13fb:	41 31 fb             	xor    r11d,edi
    13fe:	47 89 1c 8a          	mov    DWORD PTR [r10+r9*4],r11d
    1402:	49 ff c1             	inc    r9
    1405:	49 83 f9 2c          	cmp    r9,0x2c
    1409:	75 e0                	jne    13eb <generate_keys+0x16>
    140b:	c3                   	ret    
    140c:	50                   	push   rax
    140d:	c1 c7 08             	rol    edi,0x8
    1410:	e8 0a fe ff ff       	call   121f <sub_word>
    1415:	41 0f b6 f8          	movzx  edi,r8b
    1419:	41 c1 e0 18          	shl    r8d,0x18
    141d:	be 02 00 00 00       	mov    esi,0x2
    1422:	45 31 d8             	xor    r8d,r11d
    1425:	41 31 c0             	xor    r8d,eax
    1428:	47 89 04 8a          	mov    DWORD PTR [r10+r9*4],r8d
    142c:	e8 6b fe ff ff       	call   129c <galois_mul>
    1431:	41 89 c0             	mov    r8d,eax
    1434:	49 ff c1             	inc    r9
    1437:	49 83 f9 2c          	cmp    r9,0x2c
    143b:	74 19                	je     1456 <generate_keys+0x81>
    143d:	47 8b 5c 8a f0       	mov    r11d,DWORD PTR [r10+r9*4-0x10]
    1442:	43 8b 7c 8a fc       	mov    edi,DWORD PTR [r10+r9*4-0x4]
    1447:	41 f6 c1 03          	test   r9b,0x3
    144b:	74 c0                	je     140d <generate_keys+0x38>
    144d:	41 31 fb             	xor    r11d,edi
    1450:	47 89 1c 8a          	mov    DWORD PTR [r10+r9*4],r11d
    1454:	eb de                	jmp    1434 <generate_keys+0x5f>
    1456:	5a                   	pop    rdx
    1457:	c3                   	ret    

0000000000001458 <aes_decrypt>:
    1458:	f3 0f 1e fa          	endbr64 
    145c:	41 55                	push   r13
    145e:	41 54                	push   r12
    1460:	49 89 f4             	mov    r12,rsi
    1463:	55                   	push   rbp
    1464:	48 89 d5             	mov    rbp,rdx
    1467:	53                   	push   rbx
    1468:	31 db                	xor    ebx,ebx
    146a:	48 81 ec c8 00 00 00 	sub    rsp,0xc8
    1471:	64 48 8b 04 25 28 00 	mov    rax,QWORD PTR fs:0x28
    1478:	00 00 
    147a:	48 89 84 24 b8 00 00 	mov    QWORD PTR [rsp+0xb8],rax
    1481:	00 
    1482:	31 c0                	xor    eax,eax
    1484:	4c 8d 6c 24 08       	lea    r13,[rsp+0x8]
    1489:	4c 89 ee             	mov    rsi,r13
    148c:	e8 44 ff ff ff       	call   13d5 <generate_keys>
    1491:	48 39 eb             	cmp    rbx,rbp
    1494:	73 12                	jae    14a8 <aes_decrypt+0x50>
    1496:	49 8d 3c 1c          	lea    rdi,[r12+rbx*1]
    149a:	4c 89 ee             	mov    rsi,r13
    149d:	48 83 c3 10          	add    rbx,0x10
    14a1:	e8 d1 fe ff ff       	call   1377 <decrypt_segment>
    14a6:	eb e9                	jmp    1491 <aes_decrypt+0x39>
    14a8:	48 8b 84 24 b8 00 00 	mov    rax,QWORD PTR [rsp+0xb8]
    14af:	00 
    14b0:	64 48 2b 04 25 28 00 	sub    rax,QWORD PTR fs:0x28
    14b7:	00 00 
    14b9:	74 05                	je     14c0 <aes_decrypt+0x68>
    14bb:	e8 a0 fb ff ff       	call   1060 <__stack_chk_fail@plt>
    14c0:	48 81 c4 c8 00 00 00 	add    rsp,0xc8
    14c7:	5b                   	pop    rbx
    14c8:	5d                   	pop    rbp
    14c9:	41 5c                	pop    r12
    14cb:	41 5d                	pop    r13
    14cd:	c3                   	ret    

Disassembly of section .fini:

00000000000014d0 <_fini>:
    14d0:	f3 0f 1e fa          	endbr64 
    14d4:	48 83 ec 08          	sub    rsp,0x8
    14d8:	48 83 c4 08          	add    rsp,0x8
    14dc:	c3                   	ret    
