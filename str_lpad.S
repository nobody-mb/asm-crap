/*
 * Prototype: void asm_str_lpad (char *dst, const char *src, size_t width, unsigned char pad)
 * Output: 
 * Input: rdi (dst), rsi (src), rdx (width), rcx (pad)
 * Clobber: rbx (i), al (transfer)
 */

.globl _asm_str_lpad
_asm_str_lpad:
lp_pad:
	movq	%rdx, %rbx
	jmp	lp_pad_mid
lp_pad_loop:
	movb	%cl, (%rdi)
	incq	%rdi
lp_pad_mid:
	decq	%rbx
	jge	lp_pad_loop
lp_iterator:
	movb	(%rsi), %al
	movb	%al, (%rdi)
	incq	%rdi
	incq	%rsi
	cmpb	$0xa, %al
	je	lp_pad
	testb	%al, %al
	jne	lp_iterator
	ret
