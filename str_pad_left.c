#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define rdtscl(val) asm volatile ("rdtsc" : "=A" (val) : : );

void old_str_pad_to_l (char *dst, const char *src, size_t width, unsigned char pad);
void new_str_pad_to_l (char *dst, const char *src, size_t width, unsigned char pad);
void asm_str_pad_to_l (char *dst, const char *src, size_t width, unsigned char pad);

void test_pad_to (const char *src, size_t width, unsigned char pad)
{
	unsigned long t1, t2;
	
	char *r1 = malloc(1024);
	char *r2 = malloc(1024);
	char *r3 = malloc(1024);
	
	rdtscl(t1);
	old_str_pad_to_l (r1, src, width, pad);
	rdtscl(t2);
	printf("old - %5lu cycles - \n%s\n\n", t2 - t1, r1);

	rdtscl(t1);
	new_str_pad_to_l (r2, src, width, pad);
	rdtscl(t2);
	printf("new - %5lu cycles - \n%s\n\n", t2 - t1, r2);

	rdtscl(t1);
	asm_str_pad_to_l (r3, src, width, pad);
	rdtscl(t2);
	printf("asm - %5lu cycles - \n%s\n\n\n\n", t2 - t1, r3);
	
	free(r1);
	free(r2);
	free(r3);
}

void old_str_pad_to_l (char *dst, const char *src, size_t width, unsigned char pad)
{
	int i;
	
	do {
		i = (int)width;
		while (*src != '\n' && *src) {
			*dst++ = *src++;
			i--;
		}
		while (i-- > 0)
			*dst++ = pad;
		*dst++ = *src;
	} while (*src++);
}

void new_str_pad_to_l (char *dst, const char *src, size_t width, unsigned char pad)
{
	int i = (int)width;
	
	do {
		if (*src == '\n') {
			while (i-- > 0)
				*dst++ = pad;
			i = (int)width;
		} else {
			i--;
		}
	} while ((*dst++ = *src++));
	
	dst--;	
	while (i-- >= 0)
		*dst++ = pad;
}

int main (void)
{
	test_pad_to ("abc\n"
		     "this is a test", 20, 0x26);
	test_pad_to ("abcadfadsfasdfasdfagf\n"
		     "this is a test\n"
		     "and another!\n"
		     "\n"
		     "another!!!!", 20, 0x26);
}
