/*
 * Prototype: void asm_str_pad_to_l (char *dst, const char *src, size_t width, unsigned char pad)
 * Output: 
 * Input: rdi (dst), rsi (src), rdx (width), rcx (pad)
 * Clobber: rbx (i), al (transfer)
 */

.globl _asm_str_pad_to_l
_asm_str_pad_to_l:
pt_iterator:
	movq	%rdx, %rbx
pt_copier:
	movb	(%rsi), %al
	cmpb	$0xa, %al
	je	pt_pad_mid
	decq	%rbx
pt_continue:
	movb	%al, (%rdi)
	incq	%rsi
	incq	%rdi
	testb	%al, %al
	jne	pt_copier
	//jmp	pt_pad_line
	ret
pt_pad_line:
	movb	%cl, (%rdi)
	incq	%rdi
pt_pad_mid:
	decq	%rbx
	jge	pt_pad_line
	jmp	pt_continue