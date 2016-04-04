#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define rdtscl(val) asm volatile ("rdtsc" : "=A" (val) : :);

void old_str_erase (char *base, int start, int end)
{
    int i = (int)strlen(base);
    int k = start;
    int j;
    for (j = 0; j < i; j++) {
        if (j >= start) {            
            if (j > end) {
                base[k] = base[j]; 
                k++;
            }
        }        
    }
    base[k]=0;
}

void new_str_erase (char *base, int start, int end)
{
    int off, len;
    char *tmp;
    
    off = (end - start);
    
    if (off < 0) {
        return;
    }
    
    len = 0;
    
    do {
        len++;
    } while (*++base);
    
    base -= len;
    
    len -= off;

    base += start;
    tmp = base + off;

    while (len-- > 0) {
        *base++ = *++tmp;
    }
    
    *base = 0;
}

void asm_str_erase (char *base, int start, int end);

void test_str_erase (char *base, int start, int end)
{
    char *o1 = strdup(base);
    char *o2 = strdup(base);
    char *o3 = strdup(base);
    unsigned long t1, t2;
    
    rdtscl(t1);
    old_str_erase(o1, start, end);
    rdtscl(t2);
    printf("old = (%5lu) %s\n", t2 - t1, o1);
    
    rdtscl(t1);
    new_str_erase(o2, start, end);
    rdtscl(t2);
    printf("new = (%5lu) %s\n", t2 - t1, o2);
    
    rdtscl(t1);
    asm_str_erase(o3, start, end);
    rdtscl(t2);
    printf("asm = (%5lu) %s\n", t2 - t1, o3);
    
    free(o1);
    free(o2);
    free(o3);
}

int main(int argc, const char * argv[])
{
    printf("\nIGNORE\n");
    test_str_erase("string", 2, 3);
    
    printf("\nWorking\n");
    test_str_erase("string", 2, 3);

    printf("\nEnd > Length\n");
    test_str_erase("string", 1, 20);
    
    printf("\nStart = End\n");
    test_str_erase("string", 4, 4);
    
    printf("\nStart > End\n");
    test_str_erase("string", 4, 2);
    
    printf("\nEntire String\n");
    test_str_erase("string", 0, 6);   
    
    printf("\nStart > Length\n");
    test_str_erase("string", 7, 8); 

}

