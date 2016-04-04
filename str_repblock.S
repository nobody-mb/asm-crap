/*
 * Prototype: void asm_str_repblock(char *output, const char *input, char find, char rep)
 * Input: rdi (output), rsi (input), rdx (find), rcx (rep)
 * Output: none
 * Clobber: al
 */

.globl _asm_str_repblock
_asm_str_repblock:
	testq	%rsi, %rsi
	je	rb_term
rb_cmp_loop:
	movb	(%rsi), %al
	cmpb	%dl, %al
	je	rb_found
	movb	%al, (%rdi)
	testb	%al, %al
	je	rb_term
	incq	%rsi
	incq	%rdi
	jmp 	rb_cmp_loop
rb_found:
	movb	%cl, (%rdi)
	incq	%rdi
rb_inc_loop:
	cmpb	%dl, (%rsi)
	jne	rb_cmp_loop
	incq	%rsi
	jmp	rb_inc_loop
rb_term:
	ret