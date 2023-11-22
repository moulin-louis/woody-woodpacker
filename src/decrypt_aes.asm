

0:  f3 0f 1e fa             endbr64
4:  48 83 ec 08             sub    rsp,0x8
8:  48 8b 05 d9 2f 00 00    mov    rax,QWORD PTR [rip+0x2fd9]        # 0x2fe8
f:  48 85 c0                test   rax,rax
12: 74 02                   je     0x16
14: ff d0                   call   rax
16: 48 83 c4 08             add    rsp,0x8
1a: c3                      ret
1b: 00 00                   add    BYTE PTR [rax],al
1d: 00 00                   add    BYTE PTR [rax],al
1f: 00 ff                   add    bh,bh
21: 35 9a 2f 00 00          xor    eax,0x2f9a
26: f2 ff 25 9b 2f 00 00    bnd jmp QWORD PTR [rip+0x2f9b]        # 0x2fc8
2d: 0f 1f 00                nop    DWORD PTR [rax]
30: f3 0f 1e fa             endbr64
34: 68 00 00 00 00          push   0x0
39: f2 e9 e1 ff ff ff       bnd jmp 0x20
3f: 90                      nop
40: f3 0f 1e fa             endbr64
44: f2 ff 25 ad 2f 00 00    bnd jmp QWORD PTR [rip+0x2fad]        # 0x2ff8
4b: 0f 1f 44 00 00          nop    DWORD PTR [rax+rax*1+0x0]
50: f3 0f 1e fa             endbr64
54: f2 ff 25 75 2f 00 00    bnd jmp QWORD PTR [rip+0x2f75]        # 0x2fd0
5b: 0f 1f 44 00 00          nop    DWORD PTR [rax+rax*1+0x0]
60: f3 0f 1e fa             endbr64
64: 48 83 ec 38             sub    rsp,0x38
68: 0f 28 05 a1 0f 00 00    movaps xmm0,XMMWORD PTR [rip+0xfa1]        # 0x1010
6f: ba 12 00 00 00          mov    edx,0x12
74: 64 48 8b 04 25 28 00    mov    rax,QWORD PTR fs:0x28
7b: 00 00
7d: 48 89 44 24 28          mov    QWORD PTR [rsp+0x28],rax
82: 31 c0                   xor    eax,eax
84: 48 8d 74 24 16          lea    rsi,[rsp+0x16]
89: 48 8d 7c 24 06          lea    rdi,[rsp+0x6]
8e: 66 c7 44 24 26 61 62    mov    WORD PTR [rsp+0x26],0x6261
95: 0f 11 44 24 16          movups XMMWORD PTR [rsp+0x16],xmm0
9a: 0f 28 05 7f 0f 00 00    movaps xmm0,XMMWORD PTR [rip+0xf7f]        # 0x1020
a1: 0f 11 44 24 06          movups XMMWORD PTR [rsp+0x6],xmm0
a6: e8 be 03 00 00          call   0x469
ab: 48 8b 44 24 28          mov    rax,QWORD PTR [rsp+0x28]
b0: 64 48 2b 04 25 28 00    sub    rax,QWORD PTR fs:0x28
b7: 00 00
b9: 74 05                   je     0xc0
bb: e8 90 ff ff ff          call   0x50
c0: 31 c0                   xor    eax,eax
c2: 48 83 c4 38             add    rsp,0x38
c6: c3                      ret
c7: 66 0f 1f 84 00 00 00    nop    WORD PTR [rax+rax*1+0x0]
ce: 00 00
d0: f3 0f 1e fa             endbr64
d4: 31 ed                   xor    ebp,ebp
d6: 49 89 d1                mov    r9,rdx
d9: 5e                      pop    rsi
da: 48 89 e2                mov    rdx,rsp
dd: 48 83 e4 f0             and    rsp,0xfffffffffffffff0
e1: 50                      push   rax
e2: 54                      push   rsp
e3: 45 31 c0                xor    r8d,r8d
e6: 31 c9                   xor    ecx,ecx
e8: 48 8d 3d 71 ff ff ff    lea    rdi,[rip+0xffffffffffffff71]        # 0x60
ef: ff 15 e3 2e 00 00       call   QWORD PTR [rip+0x2ee3]        # 0x2fd8
f5: f4                      hlt
f6: 66 2e 0f 1f 84 00 00    nop    WORD PTR cs:[rax+rax*1+0x0]
fd: 00 00 00
100:    48 8d 3d 09 2f 00 00    lea    rdi,[rip+0x2f09]        # 0x3010
107:    48 8d 05 02 2f 00 00    lea    rax,[rip+0x2f02]        # 0x3010
10e:    48 39 f8                cmp    rax,rdi
111:    74 15                   je     0x128
113:    48 8b 05 c6 2e 00 00    mov    rax,QWORD PTR [rip+0x2ec6]        # 0x2fe0
11a:    48 85 c0                test   rax,rax
11d:    74 09                   je     0x128
11f:    ff e0                   jmp    rax
121:    0f 1f 80 00 00 00 00    nop    DWORD PTR [rax+0x0]
128:    c3                      ret
129:    0f 1f 80 00 00 00 00    nop    DWORD PTR [rax+0x0]
130:    48 8d 3d d9 2e 00 00    lea    rdi,[rip+0x2ed9]        # 0x3010
137:    48 8d 35 d2 2e 00 00    lea    rsi,[rip+0x2ed2]        # 0x3010
13e:    48 29 fe                sub    rsi,rdi
141:    48 89 f0                mov    rax,rsi
144:    48 c1 ee 3f             shr    rsi,0x3f
148:    48 c1 f8 03             sar    rax,0x3
14c:    48 01 c6                add    rsi,rax
14f:    48 d1 fe                sar    rsi,1
152:    74 14                   je     0x168
154:    48 8b 05 95 2e 00 00    mov    rax,QWORD PTR [rip+0x2e95]        # 0x2ff0
15b:    48 85 c0                test   rax,rax
15e:    74 08                   je     0x168
160:    ff e0                   jmp    rax
162:    66 0f 1f 44 00 00       nop    WORD PTR [rax+rax*1+0x0]
168:    c3                      ret
169:    0f 1f 80 00 00 00 00    nop    DWORD PTR [rax+0x0]
170:    f3 0f 1e fa             endbr64
174:    80 3d 95 2e 00 00 00    cmp    BYTE PTR [rip+0x2e95],0x0        # 0x3010
17b:    75 2b                   jne    0x1a8
17d:    55                      push   rbp
17e:    48 83 3d 72 2e 00 00    cmp    QWORD PTR [rip+0x2e72],0x0        # 0x2ff8
185:    00
186:    48 89 e5                mov    rbp,rsp
189:    74 0c                   je     0x197
18b:    48 8b 3d 76 2e 00 00    mov    rdi,QWORD PTR [rip+0x2e76]        # 0x3008
192:    e8 a9 fe ff ff          call   0x40
197:    e8 64 ff ff ff          call   0x100
19c:    c6 05 6d 2e 00 00 01    mov    BYTE PTR [rip+0x2e6d],0x1        # 0x3010
1a3:    5d                      pop    rbp
1a4:    c3                      ret
1a5:    0f 1f 00                nop    DWORD PTR [rax]
1a8:    c3                      ret
1a9:    0f 1f 80 00 00 00 00    nop    DWORD PTR [rax+0x0]
1b0:    f3 0f 1e fa             endbr64
1b4:    e9 77 ff ff ff          jmp    0x130
1b9:    f3 0f 1e fa             endbr64
1bd:    40 0f b6 f6             movzx  esi,sil
1c1:    b9 04 00 00 00          mov    ecx,0x4
1c6:    89 f8                   mov    eax,edi
1c8:    29 f1                   sub    ecx,esi
1ca:    c1 e1 03                shl    ecx,0x3
1cd:    d3 e8                   shr    eax,cl
1cf:    8d 0c f5 00 00 00 00    lea    ecx,[rsi*8+0x0]
1d6:    d3 e7                   shl    edi,cl
1d8:    09 f8                   or     eax,edi
1da:    c3                      ret
1db:    f3 0f 1e fa             endbr64
1df:    89 f8                   mov    eax,edi
1e1:    89 fa                   mov    edx,edi
1e3:    c0 c2 04                rol    dl,0x4
1e6:    d0 c0                   rol    al,1
1e8:    31 d0                   xor    eax,edx
1ea:    89 fa                   mov    edx,edi
1ec:    31 f8                   xor    eax,edi
1ee:    c0 c2 02                rol    dl,0x2
1f1:    31 d0                   xor    eax,edx
1f3:    40 c0 c7 03             rol    dil,0x3
1f7:    31 f8                   xor    eax,edi
1f9:    83 f0 63                xor    eax,0x63
1fc:    c3                      ret
1fd:    f3 0f 1e fa             endbr64
201:    89 f8                   mov    eax,edi
203:    89 fa                   mov    edx,edi
205:    40 c0 c7 03             rol    dil,0x3
209:    d0 c0                   rol    al,1
20b:    c0 ca 02                ror    dl,0x2
20e:    31 d0                   xor    eax,edx
210:    31 f8                   xor    eax,edi
212:    83 f0 05                xor    eax,0x5
215:    c3                      ret
216:    f3 0f 1e fa             endbr64
21a:    51                      push   rcx
21b:    89 7c 24 04             mov    DWORD PTR [rsp+0x4],edi
21f:    48 8d 4c 24 04          lea    rcx,[rsp+0x4]
224:    48 8d 74 24 08          lea    rsi,[rsp+0x8]
229:    0f b6 39                movzx  edi,BYTE PTR [rcx]
22c:    48 ff c1                inc    rcx
22f:    e8 a7 ff ff ff          call   0x1db
234:    88 41 ff                mov    BYTE PTR [rcx-0x1],al
237:    48 39 f1                cmp    rcx,rsi
23a:    75 ed                   jne    0x229
23c:    8b 44 24 04             mov    eax,DWORD PTR [rsp+0x4]
240:    5a                      pop    rdx
241:    c3                      ret
242:    f3 0f 1e fa             endbr64
246:    0f b6 d2                movzx  edx,dl
249:    01 d2                   add    edx,edx
24b:    48 63 c2                movsxd rax,edx
24e:    ff c2                   inc    edx
250:    48 63 d2                movsxd rdx,edx
253:    48 8b 04 c6             mov    rax,QWORD PTR [rsi+rax*8]
257:    48 31 07                xor    QWORD PTR [rdi],rax
25a:    48 8b 04 d6             mov    rax,QWORD PTR [rsi+rdx*8]
25e:    48 31 47 08             xor    QWORD PTR [rdi+0x8],rax
262:    c3                      ret
263:    f3 0f 1e fa             endbr64
267:    48 89 fe                mov    rsi,rdi
26a:    31 c9                   xor    ecx,ecx
26c:    0f b6 3c 0e             movzx  edi,BYTE PTR [rsi+rcx*1]
270:    e8 88 ff ff ff          call   0x1fd
275:    88 04 0e                mov    BYTE PTR [rsi+rcx*1],al
278:    48 ff c1                inc    rcx
27b:    48 83 f9 10             cmp    rcx,0x10
27f:    75 eb                   jne    0x26c
281:    c3                      ret
282:    f3 0f 1e fa             endbr64
286:    49 89 f8                mov    r8,rdi
289:    ba 01 00 00 00          mov    edx,0x1
28e:    41 b9 04 00 00 00       mov    r9d,0x4
294:    41 8b 3c 90             mov    edi,DWORD PTR [r8+rdx*4]
298:    44 89 ce                mov    esi,r9d
29b:    29 d6                   sub    esi,edx
29d:    e8 17 ff ff ff          call   0x1b9
2a2:    41 89 04 90             mov    DWORD PTR [r8+rdx*4],eax
2a6:    48 ff c2                inc    rdx
2a9:    48 83 fa 04             cmp    rdx,0x4
2ad:    75 e5                   jne    0x294
2af:    c3                      ret
2b0:    f3 0f 1e fa             endbr64
2b4:    31 c0                   xor    eax,eax
2b6:    40 84 ff                test   dil,dil
2b9:    74 23                   je     0x2de
2bb:    40 84 f6                test   sil,sil
2be:    74 1e                   je     0x2de
2c0:    40 f6 c6 01             test   sil,0x1
2c4:    74 02                   je     0x2c8
2c6:    31 f8                   xor    eax,edi
2c8:    8d 14 3f                lea    edx,[rdi+rdi*1]
2cb:    8d 0c 3f                lea    ecx,[rdi+rdi*1]
2ce:    83 f2 1b                xor    edx,0x1b
2d1:    40 84 ff                test   dil,dil
2d4:    0f 49 d1                cmovns edx,ecx
2d7:    40 d0 ee                shr    sil,1
2da:    89 d7                   mov    edi,edx
2dc:    eb d8                   jmp    0x2b6
2de:    c3                      ret
2df:    f3 0f 1e fa             endbr64
2e3:    53                      push   rbx
2e4:    40 0f b6 de             movzx  ebx,sil
2e8:    49 89 fb                mov    r11,rdi
2eb:    41 ba 01 00 00 00       mov    r10d,0x1
2f1:    48 01 fb                add    rbx,rdi
2f4:    44 0f b6 ce             movzx  r9d,sil
2f8:    0f b6 3b                movzx  edi,BYTE PTR [rbx]
2fb:    be 0e 00 00 00          mov    esi,0xe
300:    48 83 c3 04             add    rbx,0x4
304:    e8 a7 ff ff ff          call   0x2b0
309:    be 0b 00 00 00          mov    esi,0xb
30e:    41 89 c0                mov    r8d,eax
311:    44 89 d0                mov    eax,r10d
314:    83 e0 03                and    eax,0x3
317:    41 8d 04 81             lea    eax,[r9+rax*4]
31b:    48 98                   cdqe
31d:    41 0f b6 3c 03          movzx  edi,BYTE PTR [r11+rax*1]
322:    e8 89 ff ff ff          call   0x2b0
327:    42 8d 14 95 04 00 00    lea    edx,[r10*4+0x4]
32e:    00
32f:    be 0d 00 00 00          mov    esi,0xd
334:    83 e2 0c                and    edx,0xc
337:    41 31 c0                xor    r8d,eax
33a:    44 01 ca                add    edx,r9d
33d:    48 63 d2                movsxd rdx,edx
340:    41 0f b6 3c 13          movzx  edi,BYTE PTR [r11+rdx*1]
345:    e8 66 ff ff ff          call   0x2b0
34a:    be 09 00 00 00          mov    esi,0x9
34f:    41 31 c0                xor    r8d,eax
352:    42 8d 04 95 08 00 00    lea    eax,[r10*4+0x8]
359:    00
35a:    49 ff c2                inc    r10
35d:    83 e0 0c                and    eax,0xc
360:    44 01 c8                add    eax,r9d
363:    48 98                   cdqe
365:    41 0f b6 3c 03          movzx  edi,BYTE PTR [r11+rax*1]
36a:    e8 41 ff ff ff          call   0x2b0
36f:    41 31 c0                xor    r8d,eax
372:    44 88 43 fc             mov    BYTE PTR [rbx-0x4],r8b
376:    49 83 fa 05             cmp    r10,0x5
37a:    0f 85 78 ff ff ff       jne    0x2f8
380:    5b                      pop    rbx
381:    c3                      ret
382:    f3 0f 1e fa             endbr64
386:    55                      push   rbp
387:    48 89 fd                mov    rbp,rdi
38a:    53                      push   rbx
38b:    31 db                   xor    ebx,ebx
38d:    89 de                   mov    esi,ebx
38f:    48 89 ef                mov    rdi,rbp
392:    ff c3                   inc    ebx
394:    e8 46 ff ff ff          call   0x2df
399:    83 fb 04                cmp    ebx,0x4
39c:    75 ef                   jne    0x38d
39e:    5b                      pop    rbx
39f:    5d                      pop    rbp
3a0:    c3                      ret
3a1:    f3 0f 1e fa             endbr64
3a5:    41 54                   push   r12
3a7:    ba 0a 00 00 00          mov    edx,0xa
3ac:    49 89 f4                mov    r12,rsi
3af:    55                      push   rbp
3b0:    48 89 fd                mov    rbp,rdi
3b3:    53                      push   rbx
3b4:    bb 09 00 00 00          mov    ebx,0x9
3b9:    e8 84 fe ff ff          call   0x242
3be:    48 89 ef                mov    rdi,rbp
3c1:    e8 bc fe ff ff          call   0x282
3c6:    48 89 ef                mov    rdi,rbp
3c9:    e8 95 fe ff ff          call   0x263
3ce:    89 da                   mov    edx,ebx
3d0:    4c 89 e6                mov    rsi,r12
3d3:    48 89 ef                mov    rdi,rbp
3d6:    e8 67 fe ff ff          call   0x242
3db:    e8 a2 ff ff ff          call   0x382
3e0:    ff cb                   dec    ebx
3e2:    75 da                   jne    0x3be
3e4:    48 89 ef                mov    rdi,rbp
3e7:    e8 96 fe ff ff          call   0x282
3ec:    48 89 ef                mov    rdi,rbp
3ef:    e8 6f fe ff ff          call   0x263
3f4:    5b                      pop    rbx
3f5:    4c 89 e6                mov    rsi,r12
3f8:    48 89 ef                mov    rdi,rbp
3fb:    31 d2                   xor    edx,edx
3fd:    5d                      pop    rbp
3fe:    41 5c                   pop    r12
400:    e9 3d fe ff ff          jmp    0x242
405:    f3 0f 1e fa             endbr64
409:    53                      push   rbx
40a:    0f 10 07                movups xmm0,XMMWORD PTR [rdi]
40d:    49 89 f3                mov    r11,rsi
410:    41 ba 04 00 00 00       mov    r10d,0x4
416:    41 b1 01                mov    r9b,0x1
419:    0f 11 06                movups XMMWORD PTR [rsi],xmm0
41c:    43 8b 5c 93 f0          mov    ebx,DWORD PTR [r11+r10*4-0x10]
421:    47 8b 44 93 fc          mov    r8d,DWORD PTR [r11+r10*4-0x4]
426:    41 f6 c2 03             test   r10b,0x3
42a:    74 05                   je     0x431
42c:    41 31 d8                xor    r8d,ebx
42f:    eb 29                   jmp    0x45a
431:    44 89 c7                mov    edi,r8d
434:    c1 c7 08                rol    edi,0x8
437:    e8 da fd ff ff          call   0x216
43c:    41 0f b6 f9             movzx  edi,r9b
440:    41 c1 e1 18             shl    r9d,0x18
444:    be 02 00 00 00          mov    esi,0x2
449:    41 31 d9                xor    r9d,ebx
44c:    45 89 c8                mov    r8d,r9d
44f:    41 31 c0                xor    r8d,eax
452:    e8 59 fe ff ff          call   0x2b0
457:    41 89 c1                mov    r9d,eax
45a:    47 89 04 93             mov    DWORD PTR [r11+r10*4],r8d
45e:    49 ff c2                inc    r10
461:    49 83 fa 2c             cmp    r10,0x2c
465:    75 b5                   jne    0x41c
467:    5b                      pop    rbx
468:    c3                      ret
469:    f3 0f 1e fa             endbr64
46d:    41 57                   push   r15
46f:    41 56                   push   r14
471:    41 55                   push   r13
473:    41 54                   push   r12
475:    49 89 d4                mov    r12,rdx
478:    55                      push   rbp
479:    48 89 f5                mov    rbp,rsi
47c:    53                      push   rbx
47d:    31 db                   xor    ebx,ebx
47f:    48 81 ec d8 00 00 00    sub    rsp,0xd8
486:    64 48 8b 04 25 28 00    mov    rax,QWORD PTR fs:0x28
48d:    00 00
48f:    48 89 84 24 c8 00 00    mov    QWORD PTR [rsp+0xc8],rax
496:    00
497:    31 c0                   xor    eax,eax
499:    4c 8d 74 24 18          lea    r14,[rsp+0x18]
49e:    4c 8d 7c 24 08          lea    r15,[rsp+0x8]
4a3:    4c 89 f6                mov    rsi,r14
4a6:    e8 5a ff ff ff          call   0x405
4ab:    44 0f b7 eb             movzx  r13d,bx
4af:    4d 39 e5                cmp    r13,r12
4b2:    73 34                   jae    0x4e8
4b4:    42 8a 4c 2d 00          mov    cl,BYTE PTR [rbp+r13*1+0x0]
4b9:    89 d8                   mov    eax,ebx
4bb:    48 89 da                mov    rdx,rbx
4be:    83 e2 0f                and    edx,0xf
4c1:    83 e0 0f                and    eax,0xf
4c4:    88 4c 14 08             mov    BYTE PTR [rsp+rdx*1+0x8],cl
4c8:    66 83 f8 0f             cmp    ax,0xf
4cc:    75 16                   jne    0x4e4
4ce:    4c 89 f6                mov    rsi,r14
4d1:    4c 89 ff                mov    rdi,r15
4d4:    e8 c8 fe ff ff          call   0x3a1
4d9:    0f 10 44 24 08          movups xmm0,XMMWORD PTR [rsp+0x8]
4de:    42 0f 11 44 2d f1       movups XMMWORD PTR [rbp+r13*1-0xf],xmm0
4e4:    ff c3                   inc    ebx
4e6:    eb c3                   jmp    0x4ab
4e8:    48 8b 84 24 c8 00 00    mov    rax,QWORD PTR [rsp+0xc8]
4ef:    00
4f0:    64 48 2b 04 25 28 00    sub    rax,QWORD PTR fs:0x28
4f7:    00 00
4f9:    74 05                   je     0x500
4fb:    e8 50 fb ff ff          call   0x50
500:    48 81 c4 d8 00 00 00    add    rsp,0xd8
507:    5b                      pop    rbx
508:    5d                      pop    rbp
509:    41 5c                   pop    r12
50b:    41 5d                   pop    r13
50d:    41 5e                   pop    r14
50f:    41 5f                   pop    r15
511:    c3                      ret
512:    00 00                   add    BYTE PTR [rax],al
514:    f3 0f 1e fa             endbr64
518:    48 83 ec 08             sub    rsp,0x8
51c:    48 83 c4 08             add    rsp,0x8
520:    c3                      ret
