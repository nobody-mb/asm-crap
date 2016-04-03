#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define rdtscl(val) asm volatile ("rdtsc" : "=A" (val) : :);

void old_str_rev (char *input)
{
    size_t r = strlen(input);
    char new[r + 1];
    size_t q = 0;
    while (r--) {
        new[q++] = input[r];
    }
    strcpy(input, new);
}

void new_str_rev (char *input)
{
    char *tmp = input;
    while (*tmp) {
        tmp++;
    }

    while (--tmp > input) {
        *input ^= *tmp;
        *tmp ^= *input;
        *input ^= *tmp;
        input++;
    }
}

void asm_str_rev (char *input);

void test_str_rev (char *_input)
{
    char *input1 = strdup(_input);
    char *input2 = strdup(_input);
    char *input3 = strdup(_input);
    unsigned long t1, t2;
    
    rdtscl(t1);
    old_str_rev(input1);
    rdtscl(t2);
    printf("old = (%5lu) %s\n", t2 - t1, input1);
    
    rdtscl(t1);
    new_str_rev(input2);
    rdtscl(t2);
    printf("new = (%5lu) %s\n", t2 - t1, input2);
    
    rdtscl(t1);
    asm_str_rev(input3);
    rdtscl(t2);
    printf("asm = (%5lu) %s\n", t2 - t1, input3);

    free(input1);
    free(input2);
    free(input3);
    input1 = NULL;
    input2 = NULL;
    input3 = NULL;
}

int main(int argc, const char * argv[])
{
    printf("\nEven\n");
    test_str_rev("123456");
    
    printf("\nOdd\n");
    test_str_rev("1234567");
    
    printf("\nTwo\n");
    test_str_rev("Hi");
    
    printf("\nOne\n");
    test_str_rev("H");
}

