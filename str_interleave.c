#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define rdtscl(val) asm volatile ("rdtsc" : "=A" (val) : : );

void old_str_interleave (char *output, const char *s1, const char *s2)
{
	int i = 0;
	while (1) {
		if ((i % 2 == 0) && *s1)
			*output++ = *s1++;
		else if ((i % 2 != 0) && *s2)
			*output++ = *s2++;
		else if (!*s2 && *s1) 
			*output++ = *s1++;
		else
			break;
		i++;
	}
	*output = '\0';
}

void new_str_interleave (char *output, const char *s1, const char *s2)
{ 
	while (*s1) {
		*output++ = *s1++;
		if (*s2) 
			*output++ = *s2++;
	}
	*output = '\0';
}

void asm_str_interleave (char *output, const char *s1, const char *s2);

void test_str_interleave (const char *s1, const char *s2)
{
	char *output = calloc(128, 1);
	unsigned long t1, t2;
	
	rdtscl(t1);
	old_str_interleave(output, s1, s2);
	rdtscl(t2);
	printf("old - (%5lu) %s\n", t2 - t1, output);
	
	rdtscl(t1);
	new_str_interleave(output, s1, s2);
	rdtscl(t2);
	printf("new - (%5lu) %s\n", t2 - t1, output);
	
	rdtscl(t1);
	asm_str_interleave(output, s1, s2);
	rdtscl(t2);
	printf("asm - (%5lu) %s\n", t2 - t1, output);
	
	free(output);
}


int main (int argc, const char * argv[])
{
	printf("\nEqual 1\n");
	test_str_interleave("abcdefghijklmnopqrstuvwxyz", "12345678901234567890123456");
	
	printf("\nEqual 2\n");
	test_str_interleave("abcdefg", "1234567");
	
	printf("\nShorter s2\n");
	test_str_interleave("abcdefg", "1234");
	
	printf("\nShorter s1\n");
	test_str_interleave("abcd", "1234567");
	
	printf("\nEmpty s2\n");
	test_str_interleave("abcdefg", "");
	
	printf("\nEmpty s1\n");
	test_str_interleave("", "1234567");
	
	printf("\n1 char s2\n");
	test_str_interleave("abcdefg", "1");
	
	printf("\n1 char s1\n");
	test_str_interleave("a", "1234567");
	
}
