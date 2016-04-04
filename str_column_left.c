#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define rdtscl(val) asm volatile ("rdtsc" : "=A" (val) : : );

void old_str_lcol (char *dst, const char *src, size_t width);
void new_str_lcol (char *dst, const char *src, size_t width);
void asm_str_lcol (char *dst, const char *src, size_t width);

void test_lcol (char *dst, const char *src, int width)
{
	unsigned long t1, t2;
	
	char *r1 = malloc(strlen(src) + strlen(dst) * 2);
	char *r2 = malloc(strlen(src) + strlen(dst) * 2);
	char *r3 = malloc(strlen(src) + strlen(dst) * 2);
	
	rdtscl(t1);
	old_str_lcol (r1, src, width);
	rdtscl(t2);
	printf("old - %5lu cycles - \n%s\n", t2 - t1, r1);

	rdtscl(t1);
	new_str_lcol (r2, src, width);
	rdtscl(t2);
	printf("new - %5lu cycles - \n%s\n", t2 - t1, r2);

	rdtscl(t1);
	asm_str_lcol (r3, src, width);
	rdtscl(t2);
	printf("asm - %5lu cycles - \n%s\n\n", t2 - t1, r3);
	
	free(r1);
	free(r2);
	free(r3);
}

void old_str_lcol (char *dst, const char *src, size_t width)
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

void new_str_lcol (char *dst, const char *src, size_t width)
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
	test_lcol ("abc\n"
		   "this is a test\n", 
		   "this is another test\n"
		   "still testing\n", 2);
	
	test_lcol ("abc\n"
		   "this is a test\n", 
		   "this is another test\n"
		   "still testing\n", 2);
}
