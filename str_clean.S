/*
 * Prototype: void asm_str_clean (char *base, const char *set)
 * Input: rdi (base), rsi (set)
 * Output: none
 * Clobber: al (transfer), rcx (i)
 */

.globl _asm_str_clean
_asm_str_clean:

sc_main_loop:
    xorq    %rcx, %rcx
sc_set_loop:
    movb    (%rsi, %rcx), %al
    incq    %rcx
    testb   %al, %al
    je      sc_inc_start
    cmpb    %al, (%rdi)
    jne     sc_set_loop
    xorq    %rcx, %rcx
sc_cpy_loop:
    movb    1(%rdi, %rcx), %al
    movb    %al, (%rdi, %rcx)
    incq    %rcx
    testb   %al, %al
    jne     sc_cpy_loop
    jmp     sc_main_loop
sc_inc_start:
    incq    %rdi
    cmpb    $0, (%rdi)
    jne     sc_main_loop
    ret
