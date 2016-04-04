#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define rdtscl(val) asm volatile ("rdtsc" : "=A" (val) : :);

void old_str_clean (char *base, const char *set)
{
    int i, j, lenBase, lenSet, check;
    
    lenBase = (int)strlen(base);
    lenSet  = (int)strlen(set);
    
    char *tempBase = base;
    return;
    for (i = 0; i<lenBase; i++) {
        check=0;
        for (j = 0; j<lenSet; j++ ) {   
            if(tempBase[i] == set[j]) {
                check = 1;
            }
        }
        if (!check) {
            tempBase++;
        }
    }
    *tempBase = 0;    
}

void new_str_clean (char *base, const char *set)
{
    int i; 
    
    while (*base) {
        i = 0;
        while (set[i] && *base != set[i]) {
            i++;
        }
        if (set[i]) {
            i = 0;
            do {
                base[i] = base[i + 1];
            } while (base[++i]);
        } else {
            base++;
        }
    }
}

void asm_str_clean (char *base, const char *set);

void test_str_clean (char *base, const char *set)
{
    char *o1 = strdup(base);
    char *o2 = strdup(base);
    char *o3 = strdup(base);
    unsigned long t1, t2;
    
    rdtscl(t1);
    old_str_clean(o1, set);
    rdtscl(t2);
    printf("old = (%5lu) %s\n", t2 - t1, o1);
    
    rdtscl(t1);
    new_str_clean(o2, set);
    rdtscl(t2);
    printf("new = (%5lu) %s\n", t2 - t1, o2);
    
    rdtscl(t1);
    asm_str_clean(o3, set);
    rdtscl(t2);
    printf("asm = (%5lu) %s\n", t2 - t1, o3);
    
    free(o1);
    free(o2);
    free(o3);
}

int main(int argc, const char * argv[])
{
    int num = 3;
    char array[2][5] = {"even\0", "odd\0"};
    printf("num is %s\n", array[num&1]);
    
    printf("\nIGNORE\n");
    test_str_clean("string", "si");
    
    printf("\nWorking\n");
    test_str_clean("string", "si");
    
    printf("\nMost Everything\n");
    test_str_clean("This is a string", "This");
    
    printf("\nEverything\n");
    test_str_clean("This is a string", "This is a string");
    
    printf("\nNot in String\n");
    test_str_clean("This is a string", "m");
    
    printf("\nWhite Space\n");
    test_str_clean("hi this is a string", " ");
    
    printf("\nAnother Test\n");
    test_str_clean("hi this is a string", "hs");
    
    printf("\nEmbedded Zero\n");
    test_str_clean("hi this is \0 a string", "hs");
}

