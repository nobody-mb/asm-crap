#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define rdtscl(val) asm volatile ("rdtsc" : "=A" (val) : : );

void old_str_ltrim(char *output, const char *input, size_t offset)
{
	if (!input) {
		*output = 0;
		return;
	}
	int q;

	while (*input) {
		q = offset;
		while (*input != '\n' && *input && q) {
			q--;
			input++;
		}
		while (*input != '\n' && *input) {
			*output++ = *input++;
		}
		if (!q) 
			*output++ = '\n';
		input++;
	}
	
	*output++ = 0;
}

void new_str_ltrim(char *output, const char *input, size_t offset)
{
	int len;
	char *op;

	if (!input) {
		*output = 0;
		return;
	}

	do {
		op = (char *)input;
		do {
			input++;
		} while (*input != '\n' && *input != '\0');

		op += offset;
		
		if (input <= op)
			continue; 

		do {
			*output++ = *op++; 
		} while (input > op);
		
		*output++ = '\n';
		
	} while (*(input++));
	
	*output++ = 0;
}

void asm_str_ltrim(char *output, const char *input, size_t offset);

void test_trim(char *cmp, size_t offset)
{
	char *o1 = malloc(128);
	char *o2 = malloc(128);
	char *o3 = malloc(128);
	
	strcpy(o1, "asdfasdf");
	strcpy(o2, "asdfasdf");
	strcpy(o3, "asdfasdf");
	
	uint64_t t1, t2;
	
	rdtscl(t1);
	old_str_ltrim(o1, cmp, offset); 
	rdtscl(t2);
	printf("old - (%5llu cycles) \n%s\n", (long long unsigned int)t2 - t1, o1);
	
	rdtscl(t1);
	new_str_ltrim(o2, cmp, offset); 
	rdtscl(t2);
	printf("new - (%5llu cycles) \n%s\n", (long long unsigned int)t2 - t1, o2);

	rdtscl(t1);
	asm_str_ltrim(o3, cmp, offset); 
	rdtscl(t2);
	printf("asm - (%5llu cycles) \n%s\n", (long long unsigned int)t2 - t1, o3);

	free(o1);
	free(o2);
	free(o3);
}


int main(void)
{
	printf("\n\t\tNull Source\n");
	test_trim(NULL, 14);
	
	printf("\n\t\tWorking\n");
	test_trim("asdfasdf\n"
		"asdfasdf\n"
		"1 3 5 2 6\n"
		"\n"
		"2\n"
		"2223232323\0", 3);
	
	printf("\n\t\tZero Offset\n");
	test_trim("asdfasdf\n"
		"asdfasdf\n"
		"1 3 5 2 6\n"
		"\n"
		"2\n"
		"2223232323\0", 0);
		
	printf("\n\t\t14 Offset\n");
	test_trim("asdfasdf\n"
		"asdfasdf\n"
		"1 3 5 2 6\n"
		"\n"
		"2\n"
		"222323232334534535\0", 14);
	
	printf("\n\t\tOne Line\n");
	test_trim("Hi! Test line!\0", 3);
	
	
	printf("\n\t\tAnother Test\n");
	test_trim("1234567890\n"
		"1234567890\n"
		"12345678901\n"
		"123456789\n", 10);
}



