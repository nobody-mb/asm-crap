/*
 * Prototype: void asm_str_erase (char *base, int start, int end)
 * Input: rdi (base), rsi (start), rdx (end)
 * Output: none
 * Clobber: al (transfer), rbx (off), rcx (len), rdx (tmp)
 */

.globl asm_str_erase
asm_str_erase:
    movq    %rdx, %rbx
    subq    %rsi, %rbx
    jl      er_out
    xorq    %rcx, %rcx
er_len:
    incq    %rcx
    cmpb    $0, (%rdi, %rcx)
    jne     er_len
    subq    %rbx, %rcx
    addq    %rsi, %rdi
    movq    %rdi, %rdx
    addq    %rbx, %rdx
er_cpy:
    incq    %rdx
    movb    (%rdx), %al
    movb    %al, (%rdi)
    incq    %rdi
    decq    %rcx
    jg      er_cpy
    movb    $0, -1(%rdi)
er_out:
    ret