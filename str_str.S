/*
 * Prototype : char *asm_str_str(const char *s1, const char *s2)
 * Input     : s1 (rdi), s2 (rsi)	
 * Output    : char * (rax)
 * Clobber   : rbx, rcx
 */	

.globl asm_str_str
asm_str_str:
	movq	$-0x1, %rcx
.s_len:
	incq	%rcx
	cmpb	$0x0, (%rsi, %rcx, 0x1)
	jne	.s_len
	cmpq	$0x0, %rcx
	je	.s_out
.s_loop:
	cmpq	$0x8, %rcx
	jl	.s_cmp_0x1
.s_cmp_0x8:
	movq	-0x8(%rdi, %rcx, 0x1), %rbx
	cmpq	%rbx, -0x8(%rsi, %rcx, 0x1)
	jne	.s_inc
	subq	$0x8, %rcx
        cmpq	$0x7, %rcx
	jg	.s_cmp_0x8
.s_cmp_0x1:
	movb	-0x1(%rdi, %rcx, 0x1), %bl
	cmpb	%bl, -0x1(%rsi, %rcx, 0x1)
	jne	.s_inc
	decq	%rcx
	jg	.s_cmp_0x1
	jmp	.s_out
.s_inc:
	incq	%rdi
	cmpb 	$0x0, -0x1(%rdi)
	jne	.s_loop
	xorq 	%rdi, %rdi
.s_out:
	movq	%rdi, %rax
	ret
