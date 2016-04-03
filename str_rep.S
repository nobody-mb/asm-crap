/*
 * Prototype : void asm_str_rep(char *dst, const char *src, const char *find, const char *rep)
 * Input     : dst (rdi), src (rsi), find (rdx), rep (rcx)	
 * Output    : none
 * Clobber   : rax (temp.), rbx (i)
 */	

.globl asm_str_rep
asm_str_rep:
	testq	%rsi, %rsi
	je	rp_out
	testq	%rdx, %rdx
	je	rp_out
rp_loop:
	xorq	%rbx, %rbx
rp_comp:
	movb	(%rdx, %rbx), %al
	testb	%al, %al
	je	rp_bgrp
	cmpb	%al, (%rsi, %rbx)
	jne	rp_copy
	incq	%rbx
	jmp	rp_comp
rp_bgrp:
	addq	%rbx, %rsi
	xorq	%rbx, %rbx
rp_repl:
	movb	(%rcx, %rbx), %al
	testb	%al, %al
	je	rp_loop
	movb	%al, (%rdi)
	incq	%rbx
	incq	%rdi
	jmp	rp_repl
rp_copy:
	movb	(%rsi), %al
	movb	%al, (%rdi)
	incq	%rsi
	incq	%rdi
	testb	%al, %al
	jne	rp_loop
rp_out:
	ret
