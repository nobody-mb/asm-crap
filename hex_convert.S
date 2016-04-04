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
    subb    $0x30, %cl
    jl      invalid_letter
    cmpb    $0x09, %cl           /* '9' - '0' */
    jle     valid_letter
check_letter:
    cmpb    $0x31, %cl          /* 'a' - '0' */
    jl      invalid_letter
    cmpb    $0x36, %cl          /* 'f' - '0' */
    jg      invalid_letter
    subb    $0x27, %cl          /* ('a' - 10) - '0' */
valid_letter:
    shlq    $0x04, %rax
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

