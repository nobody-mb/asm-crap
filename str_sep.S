/*
 * Prototype: void asm_str_sep (char **output, const char *input, char sep)
 * Input: rdi (output), rsi (input), rdx (sep)
 * Output: 
 * Clobber: rcx (tmp), al (transfer)
 */

.globl asm_str_sep
asm_str_sep:
    movq    (%rdi), %rcx
ss_loop:
    movb    (%rsi), %al
    incq    %rsi
    testb   %al, %al
    je      ss_out
    cmpb    %dl, %al
    je      ss_next
    movb    %al, (%rcx)
    incq    %rcx
    jmp     ss_loop
ss_next:
    addq    $8, %rdi
    movq    (%rdi), %rcx
    jmp     ss_loop
ss_out:
    ret