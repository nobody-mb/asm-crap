#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define rdtscl(val) asm volatile ("rdtsc" : "=A" (val) : :);

unsigned int hex (char *string)
{
    unsigned int res = 0;
    unsigned int tmp; 

    while (*string) {
        tmp = *string | 0x20;
        if (tmp >= '0' && tmp <= '9')
            tmp -= '0';
        else if (tmp >= 'a' && tmp <= 'f')
            tmp -= 0x57; //('a' - 0xa)
        else
            return -1;
        res <<= 4;
        res += tmp;
        string++;
    }

    return res;
}

unsigned int asm_hex (char *string);

void test_hex (char *conv)
{
    unsigned long t1, t2;
    unsigned int res;
    
    res = hex(conv);
    rdtscl(t1);
    res = hex(conv);
    rdtscl(t2);
    printf("old - %5lu cycles: %10s -> %u\n", t2 - t1, conv, res);
    
    res = asm_hex(conv);
    rdtscl(t1);
    res = asm_hex(conv);
    rdtscl(t2);
    printf("asm - %5lu cycles: %10s -> %u\n", t2 - t1, conv, res);
}

int main (void)
{
    printf("\n-2\n");
    test_hex("fffffffe");
    
    printf("\nlen 4\n");
    test_hex("abcd");
    
    printf("\ninvalid char\n");
    test_hex("m");
    
    printf("\n1-8\n");
    test_hex("12345678");
    
    printf("\nmixed case\n");
    test_hex("FFFFFFFE");
    
    printf("\ninvalid character\n");
    test_hex("FF FFFFFF");

    return 0;
}

