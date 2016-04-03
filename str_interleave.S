/*
 * Prototype: void asm_str_interleave (char *output, const char *s1, const char *s2)
 * Input: output (rdi), s1 (rsi), s2 (rdx)
 * Output: 
 * Clobber: cl
 */

.globl asm_str_interleave
asm_str_interleave:

si_loop: 
	movb	(%rsi), %cl
	movb	%cl, (%rdi)
	testb	%cl, %cl
	je	si_term
	incq	%rsi
	incq	%rdi
	movb	(%rdx), %cl
	testb	%cl, %cl
	je	si_loop
	movb	%cl, (%rdi)
	incq	%rdi
	incq	%rdx
	jmp	si_loop
si_term:
	ret
