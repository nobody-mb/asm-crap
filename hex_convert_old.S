/*
 * Prototype: unsigned int asm_hex (char *string)
 * Output: rax (res)
 * Input: rdi (string)
 * Clobber: cx (transfer), rdx (tmp)
 */

.globl _asm_hex
_asm_hex:
    xorq    %rax, %rax
    jmp     loop_chk
iterate_string:
    orb     $0x20, %cl
    cmpb    $0x30, %cl
    jl      invalid_letter
    cmpb    $0x39, %cl
    jg      check_letter
    subb    $0x30, %cl
    jmp     valid_letter
check_letter:
    cmpb    $0x61, %cl
    jl      invalid_letter
    cmpb    $0x66, %cl
    jg      invalid_letter
    subb    $0x57, %cl
valid_letter:
    shlq    $0x4, %rax
    addb    %cl, %al
loop_chk:
    movb    (%rdi), %cl
    incq    %rdi
    testb   %cl, %cl
    jne     iterate_string
    ret
invalid_letter:
    movq    $-1, %rax
    ret