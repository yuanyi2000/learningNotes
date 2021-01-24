	.file	"decode1.c"
	.text
	.globl	decode1
	.def	decode1;	.scl	2;	.type	32;	.endef
	.seh_proc	decode1
decode1:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$16, %rsp
	.seh_stackalloc	16
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	movq	%r8, 32(%rbp)
	movq	16(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	movq	24(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -8(%rbp)
	movq	32(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -12(%rbp)
	movq	24(%rbp), %rax
	movl	-4(%rbp), %edx
	movl	%edx, (%rax)
	movq	32(%rbp), %rax
	movl	-8(%rbp), %edx
	movl	%edx, (%rax)
	movq	16(%rbp), %rax
	movl	-12(%rbp), %edx
	movl	%edx, (%rax)
	nop
	addq	$16, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0"
