
findme:     file format elf64-x86-64


Disassembly of section .init:

00000000000005e0 <.init>:
 5e0:	48 83 ec 08          	sub    rsp,0x8
 5e4:	48 8b 05 f5 09 20 00 	mov    rax,QWORD PTR [rip+0x2009f5]        # 200fe0 <malloc@plt+0x2009a0>
 5eb:	48 85 c0             	test   rax,rax
 5ee:	74 02                	je     5f2 <strcpy@plt-0x1e>
 5f0:	ff d0                	call   rax
 5f2:	48 83 c4 08          	add    rsp,0x8
 5f6:	c3                   	ret    

Disassembly of section .plt:

0000000000000600 <strcpy@plt-0x10>:
 600:	ff 35 02 0a 20 00    	push   QWORD PTR [rip+0x200a02]        # 201008 <malloc@plt+0x2009c8>
 606:	ff 25 04 0a 20 00    	jmp    QWORD PTR [rip+0x200a04]        # 201010 <malloc@plt+0x2009d0>
 60c:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]

0000000000000610 <strcpy@plt>:
 610:	ff 25 02 0a 20 00    	jmp    QWORD PTR [rip+0x200a02]        # 201018 <malloc@plt+0x2009d8>
 616:	68 00 00 00 00       	push   0x0
 61b:	e9 e0 ff ff ff       	jmp    600 <strcpy@plt-0x10>

0000000000000620 <puts@plt>:
 620:	ff 25 fa 09 20 00    	jmp    QWORD PTR [rip+0x2009fa]        # 201020 <malloc@plt+0x2009e0>
 626:	68 01 00 00 00       	push   0x1
 62b:	e9 d0 ff ff ff       	jmp    600 <strcpy@plt-0x10>

0000000000000630 <strcmp@plt>:
 630:	ff 25 f2 09 20 00    	jmp    QWORD PTR [rip+0x2009f2]        # 201028 <malloc@plt+0x2009e8>
 636:	68 02 00 00 00       	push   0x2
 63b:	e9 c0 ff ff ff       	jmp    600 <strcpy@plt-0x10>

0000000000000640 <malloc@plt>:
 640:	ff 25 ea 09 20 00    	jmp    QWORD PTR [rip+0x2009ea]        # 201030 <malloc@plt+0x2009f0>
 646:	68 03 00 00 00       	push   0x3
 64b:	e9 b0 ff ff ff       	jmp    600 <strcpy@plt-0x10>

Disassembly of section .plt.got:

0000000000000650 <.plt.got>:
 650:	ff 25 a2 09 20 00    	jmp    QWORD PTR [rip+0x2009a2]        # 200ff8 <malloc@plt+0x2009b8>
 656:	66 90                	xchg   ax,ax

Disassembly of section .text:

0000000000000660 <.text>:
 660:	48 83 ec 08          	sub    rsp,0x8
 664:	ff cf                	dec    edi
 666:	48 8d 3d 57 02 00 00 	lea    rdi,[rip+0x257]        # 8c4 <malloc@plt+0x284>
 66d:	74 1b                	je     68a <malloc@plt+0x4a>
 66f:	48 8b 7e 08          	mov    rdi,QWORD PTR [rsi+0x8]
 673:	e8 7d 01 00 00       	call   7f5 <malloc@plt+0x1b5>
 678:	85 c0                	test   eax,eax
 67a:	48 8d 3d 55 02 00 00 	lea    rdi,[rip+0x255]        # 8d6 <malloc@plt+0x296>
 681:	75 07                	jne    68a <malloc@plt+0x4a>
 683:	48 8d 3d 4f 02 00 00 	lea    rdi,[rip+0x24f]        # 8d9 <malloc@plt+0x299>
 68a:	e8 91 ff ff ff       	call   620 <puts@plt>
 68f:	31 c0                	xor    eax,eax
 691:	5a                   	pop    rdx
 692:	c3                   	ret    
 693:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
 69a:	00 00 00 
 69d:	0f 1f 00             	nop    DWORD PTR [rax]
 6a0:	31 ed                	xor    ebp,ebp
 6a2:	49 89 d1             	mov    r9,rdx
 6a5:	5e                   	pop    rsi
 6a6:	48 89 e2             	mov    rdx,rsp
 6a9:	48 83 e4 f0          	and    rsp,0xfffffffffffffff0
 6ad:	50                   	push   rax
 6ae:	54                   	push   rsp
 6af:	4c 8d 05 fa 01 00 00 	lea    r8,[rip+0x1fa]        # 8b0 <malloc@plt+0x270>
 6b6:	48 8d 0d 83 01 00 00 	lea    rcx,[rip+0x183]        # 840 <malloc@plt+0x200>
 6bd:	48 8d 3d 9c ff ff ff 	lea    rdi,[rip+0xffffffffffffff9c]        # 660 <malloc@plt+0x20>
 6c4:	ff 15 0e 09 20 00    	call   QWORD PTR [rip+0x20090e]        # 200fd8 <malloc@plt+0x200998>
 6ca:	f4                   	hlt    
 6cb:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]
 6d0:	48 8d 3d 79 09 20 00 	lea    rdi,[rip+0x200979]        # 201050 <malloc@plt+0x200a10>
 6d7:	48 8d 05 79 09 20 00 	lea    rax,[rip+0x200979]        # 201057 <malloc@plt+0x200a17>
 6de:	55                   	push   rbp
 6df:	48 29 f8             	sub    rax,rdi
 6e2:	48 89 e5             	mov    rbp,rsp
 6e5:	48 83 f8 0e          	cmp    rax,0xe
 6e9:	76 15                	jbe    700 <malloc@plt+0xc0>
 6eb:	48 8b 05 de 08 20 00 	mov    rax,QWORD PTR [rip+0x2008de]        # 200fd0 <malloc@plt+0x200990>
 6f2:	48 85 c0             	test   rax,rax
 6f5:	74 09                	je     700 <malloc@plt+0xc0>
 6f7:	5d                   	pop    rbp
 6f8:	ff e0                	jmp    rax
 6fa:	66 0f 1f 44 00 00    	nop    WORD PTR [rax+rax*1+0x0]
 700:	5d                   	pop    rbp
 701:	c3                   	ret    
 702:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]
 706:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
 70d:	00 00 00 
 710:	48 8d 3d 39 09 20 00 	lea    rdi,[rip+0x200939]        # 201050 <malloc@plt+0x200a10>
 717:	48 8d 35 32 09 20 00 	lea    rsi,[rip+0x200932]        # 201050 <malloc@plt+0x200a10>
 71e:	55                   	push   rbp
 71f:	48 29 fe             	sub    rsi,rdi
 722:	48 89 e5             	mov    rbp,rsp
 725:	48 c1 fe 03          	sar    rsi,0x3
 729:	48 89 f0             	mov    rax,rsi
 72c:	48 c1 e8 3f          	shr    rax,0x3f
 730:	48 01 c6             	add    rsi,rax
 733:	48 d1 fe             	sar    rsi,1
 736:	74 18                	je     750 <malloc@plt+0x110>
 738:	48 8b 05 b1 08 20 00 	mov    rax,QWORD PTR [rip+0x2008b1]        # 200ff0 <malloc@plt+0x2009b0>
 73f:	48 85 c0             	test   rax,rax
 742:	74 0c                	je     750 <malloc@plt+0x110>
 744:	5d                   	pop    rbp
 745:	ff e0                	jmp    rax
 747:	66 0f 1f 84 00 00 00 	nop    WORD PTR [rax+rax*1+0x0]
 74e:	00 00 
 750:	5d                   	pop    rbp
 751:	c3                   	ret    
 752:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]
 756:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
 75d:	00 00 00 
 760:	80 3d e2 08 20 00 00 	cmp    BYTE PTR [rip+0x2008e2],0x0        # 201049 <malloc@plt+0x200a09>
 767:	75 27                	jne    790 <malloc@plt+0x150>
 769:	48 83 3d 87 08 20 00 	cmp    QWORD PTR [rip+0x200887],0x0        # 200ff8 <malloc@plt+0x2009b8>
 770:	00 
 771:	55                   	push   rbp
 772:	48 89 e5             	mov    rbp,rsp
 775:	74 0c                	je     783 <malloc@plt+0x143>
 777:	48 8b 3d c2 08 20 00 	mov    rdi,QWORD PTR [rip+0x2008c2]        # 201040 <malloc@plt+0x200a00>
 77e:	e8 cd fe ff ff       	call   650 <malloc@plt+0x10>
 783:	e8 48 ff ff ff       	call   6d0 <malloc@plt+0x90>
 788:	5d                   	pop    rbp
 789:	c6 05 b9 08 20 00 01 	mov    BYTE PTR [rip+0x2008b9],0x1        # 201049 <malloc@plt+0x200a09>
 790:	f3 c3                	repz ret 
 792:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]
 796:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
 79d:	00 00 00 
 7a0:	48 8d 3d 41 06 20 00 	lea    rdi,[rip+0x200641]        # 200de8 <malloc@plt+0x2007a8>
 7a7:	48 83 3f 00          	cmp    QWORD PTR [rdi],0x0
 7ab:	75 0b                	jne    7b8 <malloc@plt+0x178>
 7ad:	e9 5e ff ff ff       	jmp    710 <malloc@plt+0xd0>
 7b2:	66 0f 1f 44 00 00    	nop    WORD PTR [rax+rax*1+0x0]
 7b8:	48 8b 05 29 08 20 00 	mov    rax,QWORD PTR [rip+0x200829]        # 200fe8 <malloc@plt+0x2009a8>
 7bf:	48 85 c0             	test   rax,rax
 7c2:	74 e9                	je     7ad <malloc@plt+0x16d>
 7c4:	55                   	push   rbp
 7c5:	48 89 e5             	mov    rbp,rsp
 7c8:	ff d0                	call   rax
 7ca:	5d                   	pop    rbp
 7cb:	e9 40 ff ff ff       	jmp    710 <malloc@plt+0xd0>
 7d0:	48 83 ec 08          	sub    rsp,0x8
 7d4:	0f b6 3d 6d 08 20 00 	movzx  edi,BYTE PTR [rip+0x20086d]        # 201048 <malloc@plt+0x200a08>
 7db:	e8 60 fe ff ff       	call   640 <malloc@plt>
 7e0:	c6 00 6b             	mov    BYTE PTR [rax],0x6b
 7e3:	c6 40 01 65          	mov    BYTE PTR [rax+0x1],0x65
 7e7:	c6 40 02 65          	mov    BYTE PTR [rax+0x2],0x65
 7eb:	c6 40 03 70          	mov    BYTE PTR [rax+0x3],0x70
 7ef:	c6 40 0a 00          	mov    BYTE PTR [rax+0xa],0x0
 7f3:	5a                   	pop    rdx
 7f4:	c3                   	ret    
 7f5:	55                   	push   rbp
 7f6:	53                   	push   rbx
 7f7:	48 89 fd             	mov    rbp,rdi
 7fa:	48 83 ec 08          	sub    rsp,0x8
 7fe:	0f b6 3d 43 08 20 00 	movzx  edi,BYTE PTR [rip+0x200843]        # 201048 <malloc@plt+0x200a08>
 805:	e8 36 fe ff ff       	call   640 <malloc@plt>
 80a:	48 89 ee             	mov    rsi,rbp
 80d:	48 89 c7             	mov    rdi,rax
 810:	48 89 c3             	mov    rbx,rax
 813:	e8 f8 fd ff ff       	call   610 <strcpy@plt>
 818:	31 c0                	xor    eax,eax
 81a:	e8 b1 ff ff ff       	call   7d0 <malloc@plt+0x190>
 81f:	48 89 de             	mov    rsi,rbx
 822:	48 89 c7             	mov    rdi,rax
 825:	e8 06 fe ff ff       	call   630 <strcmp@plt>
 82a:	85 c0                	test   eax,eax
 82c:	0f 94 c0             	sete   al
 82f:	5a                   	pop    rdx
 830:	0f b6 c0             	movzx  eax,al
 833:	5b                   	pop    rbx
 834:	5d                   	pop    rbp
 835:	c3                   	ret    
 836:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
 83d:	00 00 00 
 840:	41 57                	push   r15
 842:	41 56                	push   r14
 844:	41 89 ff             	mov    r15d,edi
 847:	41 55                	push   r13
 849:	41 54                	push   r12
 84b:	4c 8d 25 86 05 20 00 	lea    r12,[rip+0x200586]        # 200dd8 <malloc@plt+0x200798>
 852:	55                   	push   rbp
 853:	48 8d 2d 86 05 20 00 	lea    rbp,[rip+0x200586]        # 200de0 <malloc@plt+0x2007a0>
 85a:	53                   	push   rbx
 85b:	49 89 f6             	mov    r14,rsi
 85e:	49 89 d5             	mov    r13,rdx
 861:	4c 29 e5             	sub    rbp,r12
 864:	48 83 ec 08          	sub    rsp,0x8
 868:	48 c1 fd 03          	sar    rbp,0x3
 86c:	e8 6f fd ff ff       	call   5e0 <strcpy@plt-0x30>
 871:	48 85 ed             	test   rbp,rbp
 874:	74 20                	je     896 <malloc@plt+0x256>
 876:	31 db                	xor    ebx,ebx
 878:	0f 1f 84 00 00 00 00 	nop    DWORD PTR [rax+rax*1+0x0]
 87f:	00 
 880:	4c 89 ea             	mov    rdx,r13
 883:	4c 89 f6             	mov    rsi,r14
 886:	44 89 ff             	mov    edi,r15d
 889:	41 ff 14 dc          	call   QWORD PTR [r12+rbx*8]
 88d:	48 83 c3 01          	add    rbx,0x1
 891:	48 39 dd             	cmp    rbp,rbx
 894:	75 ea                	jne    880 <malloc@plt+0x240>
 896:	48 83 c4 08          	add    rsp,0x8
 89a:	5b                   	pop    rbx
 89b:	5d                   	pop    rbp
 89c:	41 5c                	pop    r12
 89e:	41 5d                	pop    r13
 8a0:	41 5e                	pop    r14
 8a2:	41 5f                	pop    r15
 8a4:	c3                   	ret    
 8a5:	90                   	nop
 8a6:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
 8ad:	00 00 00 
 8b0:	f3 c3                	repz ret 

Disassembly of section .fini:

00000000000008b4 <.fini>:
 8b4:	48 83 ec 08          	sub    rsp,0x8
 8b8:	48 83 c4 08          	add    rsp,0x8
 8bc:	c3                   	ret    
