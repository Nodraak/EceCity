	.file	"test.c"
	.text
	.globl	ft_long
	.type	ft_long, @function
ft_long:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -36(%rbp)
	movsd	%xmm0, -48(%rbp)
	movsd	%xmm1, -56(%rbp)
	movsd	%xmm2, -64(%rbp)
	xorpd	%xmm0, %xmm0
	subsd	-56(%rbp), %xmm0
	movsd	%xmm0, -8(%rbp)
	cvtsi2sd	-36(%rbp), %xmm0
	divsd	-48(%rbp), %xmm0
	subsd	-56(%rbp), %xmm0
	movsd	%xmm0, -16(%rbp)
	movl	$0, -20(%rbp)
	movl	-36(%rbp), %eax
	movl	%eax, -24(%rbp)
	cvtsi2sd	-20(%rbp), %xmm1
	movsd	-64(%rbp), %xmm0
	movapd	%xmm0, %xmm2
	subsd	-8(%rbp), %xmm2
	movl	-20(%rbp), %eax
	movl	-24(%rbp), %edx
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	cvtsi2sd	%eax, %xmm0
	mulsd	%xmm2, %xmm0
	movsd	-16(%rbp), %xmm2
	subsd	-8(%rbp), %xmm2
	divsd	%xmm2, %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -72(%rbp)
	movq	-72(%rbp), %rax
	movq	%rax, -72(%rbp)
	movsd	-72(%rbp), %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	ft_long, .-ft_long
	.globl	ft_short
	.type	ft_short, @function
ft_short:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movsd	%xmm0, -16(%rbp)
	movsd	%xmm1, -24(%rbp)
	movsd	%xmm2, -32(%rbp)
	movsd	-32(%rbp), %xmm0
	addsd	-24(%rbp), %xmm0
	mulsd	-16(%rbp), %xmm0
	movsd	%xmm0, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, -40(%rbp)
	movsd	-40(%rbp), %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	ft_short, .-ft_short
	.ident	"GCC: (Debian 4.7.2-5) 4.7.2"
	.section	.note.GNU-stack,"",@progbits
