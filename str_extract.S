/*
 * Prototype : char *asm_str_extract(char *output, const char *input, 
 		const char *start, const char *end)
 * Input     : output (rdi), input (rsi), start (rdx), end (rcx)	
 * Output    : char * (rax)
 * Clobber   : rbx, rcx, rdx
 */	

.globl asm_str_extract

asm_str_extract:
	testq	%rdi, %rdi
	je	err_out
	testq	%rdx, %rdx
	je	err_out
	testq	%rcx, %rcx
	je 	err_out
	movq	%rsi, %r14	/* r14 = output */
	movq	%rdx, %rsi	/* rsi = start */
	movq	%rcx, %rax	/* rax = end */	 
	movq	$-1, %rcx
.s1_len:
	incq	%rcx
	cmpb	$0, (%rsi, %rcx, 1)
	jne	.s1_len
	movq	%rcx, %rdx
	testq	%rcx, %rcx
	je	.s1_out
.s1_loop:
	cmpq	$8, %rcx
	jl	.s1_cmp_1
.s1_cmp_8:
	movq	-8(%rdi, %rcx, 1), %rbx
	cmpq	%rbx, -8(%rsi, %rcx, 1)
	jne	.s1_inc
	subq	$8, %rcx
        cmpq	$7, %rcx
	jg	.s1_cmp_8
.s1_cmp_1:
	movb	-1(%rdi, %rcx, 1), %bl
	cmpb	%bl, -1(%rsi, %rcx, 1)
	jne	.s1_inc
	decq	%rcx
	jg	.s1_cmp_1
	jmp	.s1_out
.s1_inc:
	incq	%rdi
	cmpb 	$0, -1(%rdi)
	jne	.s1_loop
	jmp	err_out
.s1_out:
	movq	%rax, %rsi	/* rsi = end */
	movq	%rdi, %rax	/* rax = stp */
	movq	$-1, %rcx
.s2_len:
	incq	%rcx
	cmpb	$0, (%rsi, %rcx, 1)
	jne	.s2_len
	cmpq	$0, %rcx
	je	.s2_out
.s2_loop:
	cmpq	$8, %rcx
	jl	.s2_cmp_1
.s2_cmp_8:
	movq	-8(%rdi, %rcx, 1), %rbx
	cmpq	%rbx, -8(%rsi, %rcx, 1)
	jne	.s2_inc
	subq	$8, %rcx
        cmpq	$7, %rcx
	jg	.s2_cmp_8
.s2_cmp_1:
	movb	-1(%rdi, %rcx, 1), %bl
	cmpb	%bl, -1(%rsi, %rcx, 1)
	jne	.s2_inc
	decq	%rcx
	jg	.s2_cmp_1
	jmp	.s2_out
.s2_inc:
	incq	%rdi
	cmpb 	$0, -1(%rdi)
	jne	.s2_loop
	jmp	err_out
.s2_out:
	cmpq	%rdi, %rax
	je	err_out
	movq	%rdi, %rcx	/* rcx = edp */
	subq	%rax, %rcx	/* rcx -= stp */
	subq	%rdx, %rcx	/* rcx -= length of start */
	movq	%rax, %rsi	/* rsi = stp */
	addq	%rdx, %rsi	/* rsi += len */
	movq	%r14, %rdi	/* rdi = output */
	movq	%rcx, %rbx	/* rbx = copy len */
cpy_loop:
	movq	-1(%rsi, %rcx, 1), %rax
	movq	%rax, -1(%rdi, %rcx, 1)
	decq	%rcx
	jne	cpy_loop
	movb	$0, (%rdi, %rbx)
	jmp 	clc_out
err_out:
	xorq	%rdi, %rdi	
clc_out:
	movq	%rdi, %rax
	ret
