#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define rdtscl(val) 0 // asm volatile ("rdtsc" : "=A" (val) : : );

void old_str_repblock(char *output, const char *input, char find, char rep);
void new_str_repblock(char *output, const char *input, char find, char rep);
void asm_str_repblock(char *output, const char *input, char find, char rep);

void old_str_repblock(char *output, const char *input, 
			char find, char rep)
{
	int pos = 0;
	if (!input)
		return;
	char *n = (char *)input;
	while (*n) {
		while (*n != find && *n)
			output[pos++] = *n++;
		output[pos++] = rep;
		while (*n == find)
			n++;
	}

	output[pos-1] = 0;
}

void new_str_repblock(char *output, const char *input, 
			char find, char rep)
{
	if (!input)
		return;
	while (*input) {
		if (*input == find) {
			*output++ = rep;
			while (*input == find)
				input++;
		}
		*output++ = *input++;
	}

	*(output) = 0;
}

void test_repblock(char *cmp, char find, char rep)
{
	char *o1 = malloc(128);
	char *o2 = malloc(128);
	char *o3 = malloc(128);
	
	strcpy(o1, "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq");
	strcpy(o2, "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq");
	strcpy(o3, "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq");
	
	uint64_t t1, t2;
	
	rdtscl(t1);
	old_str_repblock(o1, cmp, find, rep); 
	rdtscl(t2);
	printf("old - (%5llu cycles) %s\n", (long long unsigned int)t2 - t1, o1);
	
	rdtscl(t1);
	new_str_repblock(o2, cmp, find, rep); 
	rdtscl(t2);
	printf("new - (%5llu cycles) %s\n", (long long unsigned int)t2 - t1, o2);

	rdtscl(t1);
	asm_str_repblock(o3, cmp, find, rep); 
	rdtscl(t2);
	printf("asm - (%5llu cycles) %s\n", (long long unsigned int)t2 - t1, o3);

	free(o1);
	free(o2);
	free(o3);
}

int main (void)
{
	printf("\nWorking:\n");
	test_repblock("asdf asdf  asdf   asdf    asdf", ' ', 'y');
		
	printf("\nFind not in Source\n");
	test_repblock("asdf asdf", 'y', ' ');
	
	printf("\nSource = End\n");
	test_repblock("asdf          asdf", ' ', ' ');
	
	printf("\nEmpty Source\n");
	test_repblock(NULL, ' ', ' ');
	
	printf("\nReplace\n");
	test_repblock("asdf     asdf", ' ', 'y');
	
	printf("\nTrailing 4\n");
	test_repblock("asdf     asdf    ", ' ', 'y');
}

