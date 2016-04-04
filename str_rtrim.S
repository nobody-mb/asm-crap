/*
 * Prototype : void asm_str_rtrim(char *output, const char *input, int offset)
 * Input     : output (rdi), input (rsi), offset (rdx)
 * Output    : none
 * Clobber   : transfer (al), op (rbx), len (rcx)
 */

.globl asm_str_rtrim
asm_str_rtrim:
	testq	%rsi, %rsi
	je	sr_out
sr_loop:
	movq	%rsi, %rbx
sr_len:
	incq	%rsi
	cmpb	$0xa, (%rsi)
	je	sr_calc
	cmpb	$0x0, (%rsi)
	je	sr_calc
	jmp	sr_len
sr_calc:
	subq	%rdx, %rsi
	cmpq	%rbx, %rsi
	jle	sr_inc
sr_copy:
	movb	(%rbx), %al
	movb	%al, (%rdi)
	incq	%rbx
	incq	%rdi
	cmpq	%rbx, %rsi
	jg	sr_copy
	movq	$0xa, (%rdi)
	incq	%rdi
sr_inc:
	addq	%rdx, %rsi
	incq	%rsi
	cmpb	$0x0, (%rsi)
	jne	sr_loop
sr_out:
	movb	$0x0, (%rdi)
	ret