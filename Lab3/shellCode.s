00000000000005fa <main>:
 5fa:	55                   	push   %rbp
 5fb:	48 89 e5             	mov    %rsp,%rbp
 5fe:	48 c7 c2 00 00 00 00 	mov    $0x0,%rdx
 605:	48 c7 c6 00 00 00 00 	mov    $0x0,%rsi
 60c:	48 bf 2f 62 69 6e 2f 	movabs $0x68732f6e69622f,%rdi
 613:	73 68 00 
 616:	57                   	push   %rdi
 617:	48 89 e7             	mov    %rsp,%rdi
 61a:	48 c7 c0 3b 00 00 00 	mov    $0x3b,%rax
 621:	0f 05                	syscall 
 623:	b8 00 00 00 00       	mov    $0x0,%eax
 628:	5d                   	pop    %rbp
 629:	c3                   	retq   
 62a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000000630 <__libc_csu_init>:
 630:	41 57                	push   %r15
 632:	41 56                	push   %r14
 634:	49 89 d7             	mov    %rdx,%r15
 637:	41 55                	push   %r13
 639:	41 54                	push   %r12
 63b:	4c 8d 25 ae 07 20 00 	lea    0x2007ae(%rip),%r12        # 200df0 <__frame_dummy_init_array_entry>
 642:	55                   	push   %rbp
 643:	48 8d 2d ae 07 20 00 	lea    0x2007ae(%rip),%rbp        # 200df8 <__init_array_end>
 64a:	53                   	push   %rbx
 64b:	41 89 fd             	mov    %edi,%r13d
 64e:	49 89 f6             	mov    %rsi,%r14
 651:	4c 29 e5             	sub    %r12,%rbp
 654:	48 83 ec 08          	sub    $0x8,%rsp
 658:	48 c1 fd 03          	sar    $0x3,%rbp
 65c:	e8 57 fe ff ff       	callq  4b8 <_init>
 661:	48 85 ed             	test   %rbp,%rbp
 664:	74 20                	je     686 <__libc_csu_init+0x56>
 666:	31 db                	xor    %ebx,%ebx
 668:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
 66f:	00 
 670:	4c 89 fa             	mov    %r15,%rdx
 673:	4c 89 f6             	mov    %r14,%rsi
 676:	44 89 ef             	mov    %r13d,%edi
 679:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
 67d:	48 83 c3 01          	add    $0x1,%rbx
 681:	48 39 dd             	cmp    %rbx,%rbp
 684:	75 ea                	jne    670 <__libc_csu_init+0x40>
 686:	48 83 c4 08          	add    $0x8,%rsp
 68a:	5b                   	pop    %rbx
 68b:	5d                   	pop    %rbp
 68c:	41 5c                	pop    %r12
 68e:	41 5d                	pop    %r13
 690:	41 5e                	pop    %r14
 692:	41 5f                	pop    %r15
 694:	c3                   	retq   
 695:	90                   	nop
 696:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 69d:	00 00 00 

00000000000006a0 <__libc_csu_fini>:
 6a0:	f3 c3                	repz retq 

Disassembly of section .fini:

00000000000006a4 <_fini>:
 6a4:	48 83 ec 08          	sub    $0x8,%rsp
 6a8:	48 83 c4 08          	add    $0x8,%rsp
 6ac:	c3                   	retq   
