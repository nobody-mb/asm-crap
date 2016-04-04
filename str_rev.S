/*
 * Prototype: void asm_str_rev (char *input)
 * Input: rdi (input)
 * Output: 
 * Clobber: rsi (tmp), ax (transfer)
 */

.globl asm_str_rev
asm_str_rev:
    movq    %rdi, %rsi
    jmp     rv_mid_a
rv_add:
    incq    %rsi
rv_mid_a:
    cmpb    $0, (%rsi)
    jne     rv_add
    jmp     rv_mid_s
rv_loop:
    movb    (%rsi), %al
    movb    (%rdi), %ah
    movb    %al, (%rdi)
    movb    %ah, (%rsi)
    incq    %rdi
rv_mid_s:
    decq    %rsi
    cmpq    %rsi, %rdi
    jl     rv_loop
    ret