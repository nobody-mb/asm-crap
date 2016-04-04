/*
 * Prototype : void asm_str_xchg(char *dst, const char *src, const char *find, const char *rep)
 * Input     : dst (rdi), src (rsi), find (rdx), rep (rcx)	
 * Output    : none
 * Clobber   : rax (temp.), rbx (i)
 */	

.globl asm_str_xchg
asm_str_xchg:
	testq	%rsi, %rsi
	je	sx_out
	testq	%rdx, %rdx
	je	sx_out
sx_loop:
	xorq	%rbx, %rbx
sx_comp:
	movb	(%rdx, %rbx), %al
	testb	%al, %al
	je	sx_bgrp
	cmpb	%al, (%rsi, %rbx)
	jne	sx_copy
	incq	%rbx
	jmp	sx_comp
sx_bgrp:
	addq	%rbx, %rsi
	xorq	%rbx, %rbx
sx_repl:
	movb	(%rcx, %rbx), %al
	testb	%al, %al
	je	sx_loop
	movb	%al, (%rdi)
	incq	%rbx
	incq	%rdi
	jmp	sx_repl
sx_copy:
	movb	(%rsi), %al
	movb	%al, (%rdi)
	incq	%rsi
	incq	%rdi
	testb	%al, %al
	jne	sx_loop
sx_out:
	ret