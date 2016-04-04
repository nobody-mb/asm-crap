#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define rdtscl(val) asm volatile ("rdtsc" : "=A" (val) : : );

void old_str_lpad (char *dst, const char *src, size_t width, unsigned char pad);
void new_str_lpad (char *dst, const char *src, size_t width, unsigned char pad);
void asm_str_lpad (char *dst, const char *src, size_t width, unsigned char pad);

void test_lpad (char *src, int width, unsigned char pad)
{
	unsigned long t1, t2;
	
	char *r1 = calloc(1, 1024);
	char *r2 = calloc(1, 1024);
	char *r3 = calloc(1, 1024);
	
	memset(r1, 'a', 1023);
	memset(r2, 'a', 1023);
	memset(r3, 'a', 1023);
	
	old_str_lpad (r1, src, width, pad);
	rdtscl(t1);
	old_str_lpad (r1, src, width, pad);
	rdtscl(t2);
	printf("old - %5lu cycles - \n%s\n", t2 - t1, r1);
	
	new_str_lpad (r2, src, width, pad);
	rdtscl(t1);
	new_str_lpad (r2, src, width, pad);
	rdtscl(t2);
	printf("new - %5lu cycles - \n%s\n", t2 - t1, r2);
	
	asm_str_lpad (r3, src, width, pad);
	rdtscl(t1);
	asm_str_lpad (r3, src, width, pad);
	rdtscl(t2);
	printf("asm - %5lu cycles - \n%s\n\n", t2 - t1, r3);
	
	free(r1);
	free(r2);
	free(r3);
}

void old_str_lpad (char *dst, const char *src, size_t width, unsigned char pad)
{
	int i;
	
	do {
		for (i = 0; i < width; i++) {
			*dst++ = pad;
		}
		while (*src != '\n' && *src) {
			*dst++ = *src++;
		}
		*dst++ = *src;
	} while (*src++);
}

void new_str_lpad (char *dst, const char *src, size_t width, unsigned char pad)
{
	int i;
	
	i = (int)width;
	while (i--)
		*dst++ = pad;
	
	do {
		*dst++ = *src;
		if (*src == '\n') {
			i = (int)width;
			while (i--)
				*dst++ = pad;
		}
	} while (*src++);
}

int main (void)
{
	test_lpad ("abc\n"
		   "this is a test\n", 2, ' ');
	
	test_lpad ("abc\n"
		   "\na\0\nasdf\n", 5, ' ');
	
	test_lpad ("\na\0", 2, ' ');
	
	test_lpad ("\na\0", 0, ' ');
	
	test_lpad ("\n\0\n", 3, ' ');
	
	test_lpad ("a\0a", 2, ' ');
	
	test_lpad ("asdfadsf\naefsdf\n", 3, '0');
}
