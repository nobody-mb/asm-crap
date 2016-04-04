
.globl _asm_bzero
_asm_bzero:
	xorq	%rax, %rax
loop_64:
	cmpq	$64, %rsi
	jl	loop_32
	movq	%rax, 0(%rdi)
	movq	%rax, 8(%rdi)
	movq	%rax, 16(%rdi)
	movq	%rax, 24(%rdi)
	movq	%rax, 32(%rdi)
	movq	%rax, 40(%rdi)
	movq	%rax, 48(%rdi)
	movq	%rax, 54(%rdi)
	addq	$64, %rdi
	subq	$64, %rsi
	jmp	loop_64
loop_32:
	cmpq	$32, %rsi
	jl	loop_8
	movq	%rax, 0(%rdi)
	movq	%rax, 8(%rdi)
	movq	%rax, 16(%rdi)
	movq	%rax, 24(%rdi)
	addq	$32, %rdi
	subq	$32, %rsi
	jmp	loop_32
loop_8:
	cmpq	$8, %rsi
	jl	loop_1
	movq	%rax, (%rdi)
	addq	$8, %rdi
	subq	$8, %rsi
	jmp	loop_8
loop_1:
	cmpq	$1, %rsi
	jl	loop_0
	movb	%al, (%rdi)
	addq	$1, %rdi
	subq	$1, %rsi
	jmp	loop_1
loop_0:
	ret