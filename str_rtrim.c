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

void old_str_rtrim(char *output, const char *input, size_t offset)
{
	int len;
	char *op;

	if (!input) {
		*output = 0;
		return;
	}

	while (*input) {
		op = (char *)input;
		do {
			input++;
		} while (*input != '\n' && *input != '\0');
		len = (int)(input - op) - offset;
		if (len > 0) {
			
			while (len--) {
				*output++ = *op++; 
			}
			*output++ = '\n';
		}
		input++;
	}
	*output++ = 0;
}

void new_str_rtrim(char *output, const char *input, size_t offset)
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

		input -= offset; 
		
		if (input <= op)
			continue; 

		do {
			*output++ = *op++; 
		} while (input > op); 
		
		*output++ = '\n';
		
	} while (*(input += (offset + 1)));
	
	*output++ = 0;
}

void asm_str_rtrim(char *output, const char *input, size_t offset);

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
	old_str_rtrim(o1, cmp, offset); 
	rdtscl(t2);
	printf("old - (%5llu cycles) \n%s\n", (long long unsigned int)t2 - t1, o1);
	
	rdtscl(t1);
	new_str_rtrim(o2, cmp, offset); 
	rdtscl(t2);
	printf("new - (%5llu cycles) \n%s\n", (long long unsigned int)t2 - t1, o2);

	rdtscl(t1);
	asm_str_rtrim(o3, cmp, offset); 
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
		"2223232342342342323\0", 14);
	
	printf("\n\t\tOne Line\n");
	test_trim("Hi! Test line!\n", 3);
	
	
	printf("\n\t\tAnother Test\n");
	test_trim("1234567890\n"
		"1234567890\n"
		"12345678901\n"
		"123456789\n", 10);
}



