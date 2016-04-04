/*
 * Prototype: void asm_str_blockset (char *base, const char *block, int len)
 * Input: rdi (base), rsi (block), rdx (len)
 * Output: none
 * Clobber: al (transfer), rbx (cpy)
 */

.globl asm_str_blockset
asm_str_blockset:
    movq    %rsi, %rbx
    jmp     bs_loop_mid
bs_loop:
    movb    (%rbx), %al
    testb   %al, %al
    jne     bs_copy
    movq    %rsi, %rbx
    movb    (%rbx), %al
bs_copy:
    movb    %al, (%rdi)
    incq    %rbx
    incq    %rdi
bs_loop_mid:
    decq    %rdx
    jge     bs_loop
    ret