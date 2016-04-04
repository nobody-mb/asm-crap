/*
 * Prototype: char * asm_str_insidestep(char *dst, char **base, 
 					char open, char close)
 * Input: rdi (dst), rsi (base), rdx (open), rcx (close)
 * Output: rax (dst)
 * Clobber: al (transfer), bh (open), bl (close), rcx (len), 
 				r8 (j), rdx (tmp), r9 (tmp2), r10 (dst2)
 */

.globl asm_str_insidesep
asm_str_insidesep:
	movb	%dl, %bh
	movb	%cl, %bl
	movq	%rdi, %r10
	
	movq	(%rsi), %rdx
	movq	(%rsi), %r9
	
	xorq	%rcx, %rcx
	xorq 	%r8, %r8
is_len:
	incq	%rdx
	incq	%rcx
	cmpb	$0, (%rdx)
	jne	is_len
is_back:
	decq	%rdx
	incq	%r8
	cmpb	(%rdx), %bh
	je	is_chk
	cmpq	%rcx, %r8
	jle	is_back
is_chk:
	incq	%rdx
	cmpq	%rdx, %r9
	je	is_rest_dst
is_loop:
	movb	(%rdx), %al
	testb	%al, %al
	je	is_rest_dst
	movb	%al, (%rdi)
	incq	%rdi
	incq	%rdx
	cmpb	(%rdx), %bl
	jne	is_loop
	
	addq	%rcx, %r9
	subq	%r8, %r9
is_copy:
	incq	%rdx
	movb	(%rdx), %al
	movb	%al, (%r9)
	incq	%r9
	testb	%al, %al
	jne	is_copy
	jmp	is_out
is_rest_dst:
	movq	%r10, %rdi
	movq	$0, (%rdi)
	movq	$0, %rax
	ret
is_out:
	movb	$0, (%rdi)
	movq	%rdi, %rax
	ret

