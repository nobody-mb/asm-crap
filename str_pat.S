/*
 * Prototype : void asm_find_pattern(char *output, char *input)
 * Input     : output (rdi), input (rsi)
 * Output    : none
 * Clobber   : rax, rbx, rcx, rdx, r8, r9
 */	
.globl asm_find_pattern
asm_find_pattern:
	movq	$0, %rcx
fp_out_loop:
	addq	$1, %rcx
	movq	%rsi, %rdx
fp_inf_loop:
	movq	%rcx, %rbx
	movq	%rsi, %r8
	movq	%rdx, %r9
fp_cmp_loop:
	addq	$1, %r8
	addq	$1, %r9
	movb	-1(%r8), %al
	cmpb	%al, -1(%r9)
	jne	fp_out_loop
	subq	$1, %rbx
	jne	fp_cmp_loop
	addq	%rcx, %rdx
	cmpb	$0, (%rdx)
	jne	fp_inf_loop
	subq	%rcx, %rdx
fp_cpy_loop: 
	movb	(%rdx), %al
	movb	%al, (%rdi)
	addq	$1, %rdx
	addq	$1, %rdi
	testb	%al, %al
	jne	fp_cpy_loop
	movb	$0, (%rdi)
	ret
