/*
 * Prototype: void asm_str_pad_to_r (char *dst, const char *src, size_t len)
 * Output: 
 * Input: rdi (dst), rsi (src), rdx (len)
 * Clobber: rcx (line_len), rbx (offset), al (transfer)
 */

.globl _asm_str_pad_to_r
_asm_str_pad_to_r:
	decq	%rsi
pr_loop:
	xorq	%rcx, %rcx

pr_line_loop:
	incq	%rsi
	movb	(%rsi), %al

	cmpq	%rdx, %rcx
	jge	pr_maxed_len
	incq	%rcx
pr_maxed_len:
	cmpb	$0xa, %al
	je	pr_end_line
	cmpb	$0x0, %al
	jne	pr_line_loop

pr_end_line:
	movq	%rdx, %rbx
	subq	%rcx, %rbx

	subq	%rcx, %rsi
	jmp	pr_pad_loop
pr_pad_mid:
	movb	$0x20, (%rdi)
	incq	%rdi
pr_pad_loop:
	decq	%rbx
	jne	pr_pad_mid

	jmp	pr_cpy_loop
pr_cpy_mid:
	incq	%rsi
	movb	(%rsi), %al
	movb	%al, (%rdi)
	incq	%rdi
pr_cpy_loop:
	decq	%rcx
	jne	pr_cpy_mid

	testb	%al, %al
	jne	pr_loop
 
	ret
