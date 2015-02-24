	.file	"test.c"
	.text
	.p2align 4,,15
	.globl	ft_long
	.type	ft_long, @function
ft_long:
.LFB0:
	.cfi_startproc
	xorpd	%xmm3, %xmm3
	cvtsi2sd	%edi, %xmm5
	movapd	%xmm3, %xmm4
	subsd	%xmm1, %xmm4
	subsd	%xmm4, %xmm2
	mulsd	%xmm5, %xmm2
	divsd	%xmm0, %xmm5
	movapd	%xmm5, %xmm0
	subsd	%xmm1, %xmm0
	subsd	%xmm4, %xmm0
	divsd	%xmm0, %xmm2
	addsd	%xmm3, %xmm2
	movapd	%xmm2, %xmm0
	ret
	.cfi_endproc
.LFE0:
	.size	ft_long, .-ft_long
	.p2align 4,,15
	.globl	ft_short
	.type	ft_short, @function
ft_short:
.LFB1:
	.cfi_startproc
	addsd	%xmm2, %xmm1
	mulsd	%xmm0, %xmm1
	movapd	%xmm1, %xmm0
	ret
	.cfi_endproc
.LFE1:
	.size	ft_short, .-ft_short
	.ident	"GCC: (Debian 4.7.2-5) 4.7.2"
	.section	.note.GNU-stack,"",@progbits
