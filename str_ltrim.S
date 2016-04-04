/*
 * Prototype : void asm_str_ltrim(char *output, const char *input, int offset)
 * Input     : output (rdi), input (rsi), offset (rdx)
 * Output    : none
 * Clobber   : al, rbx
 */
 
.globl asm_str_ltrim
asm_str_ltrim:
	testq	%rsi, %rsi
	je	sl_out
sl_loop:
	movq	%rsi, %rbx
sl_len:
	incq	%rsi
	cmpb	$0xa, (%rsi)
	je	sl_calc
	cmpb	$0x0, (%rsi)
	je	sl_calc
	jmp	sl_len
sl_calc:
	addq	%rdx, %rbx
	cmpq	%rbx, %rsi
	jle	sl_inc
sl_copy:
	movb	(%rbx), %al
	movb	%al, (%rdi)
	incq	%rbx
	incq	%rdi
	cmpq	%rbx, %rsi
	jg	sl_copy
	movq	$0xa, (%rdi)
	incq	%rdi
sl_inc:
	incq	%rsi
	cmpb	$0x0, (%rsi)
	jne	sl_loop
sl_out:
	movb	$0x0, (%rdi)
	ret
	