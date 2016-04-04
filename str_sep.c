#include <stdlib.h>
#include <stdio.h>

#define rdtscl(val) asm volatile ("rdtsc" : "=A" (val) : :);

void old_str_sep (char **output, const char *input, char sep)
{
    int i = 0;
    int j = 0;

    while (*input) {
        if (*input != sep) {
            output[i][j++] = *input++;
        } else {
            output[i][j] = '\0';
            input++;
            i++;
            j = 0;
        }
    }
}

void new_str_sep (char **output, const char *input, char sep)
{
    char *tmp = *output;
    
    while (*input) {
        if (*input != sep) {
            *tmp++ = *input++;
        } else {
            input++;
            output++;
            tmp = *output;
        }
    }
}

void asm_str_sep (char **output, const char *input, char sep);

void test_str_sep (char *input, char sep)
{
    int i;
    char **output = malloc(6 * sizeof(char *));
    for (i = 0; i < 6; i++) {
        output[i] = malloc(32);
        *output[i] = '\0';
    }
    unsigned long t1, t2;
    
    rdtscl(t1);
    old_str_sep(output, input, sep);
    rdtscl(t2);
    printf("old = (%5lu) |%s|%s|%s|%s|%s|%s|\n", t2 - t1, 
           output[0], output[1], output[2], 
           output[3], output[4], output[5]);
    
    rdtscl(t1);
    new_str_sep(output, input, sep);
    rdtscl(t2);
    printf("new = (%5lu) |%s|%s|%s|%s|%s|%s|\n", t2 - t1, 
           output[0], output[1], output[2], 
           output[3], output[4], output[5]);
    
    rdtscl(t1);
    asm_str_sep(output, input, sep);
    rdtscl(t2);
    printf("asm = (%5lu) |%s|%s|%s|%s|%s|%s|\n", t2 - t1, 
           output[0], output[1], output[2], 
           output[3], output[4], output[5]);
    
    for (i = 0; i < 6; i++) {
        free(output[i]);
    }
    free(output);
}

int main(int argc, const char * argv[])
{
    printf("\nWorking\n");
    test_str_sep("hi:this", ':');

    printf("\nWorking (long)\n");
    test_str_sep("hi:this:is:a:string", ':');
    
    printf("\nNo Seperators\n");
    test_str_sep("hi this is a string", ':');
    
    printf("\nOnly Seperators\n");
    test_str_sep("::::", ':');
    
    printf("\nNull Between\n");
    test_str_sep("Hi:::h:", ':');
    
    printf("\nStarts with\n");
    test_str_sep(":hi:this:is:a:string", ':');
}

