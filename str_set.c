#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define rdtscl(val) asm volatile ("rdtsc" : "=A" (val) : : );

size_t asm_bzero(char *block, size_t len);

size_t old_bzero(char *block, size_t len)
{
	while (len--) {
		*block++ = 0;
	}
	return 0;
}

size_t new_bzero(char *block, size_t len)
{
	asm volatile ("xorb	%%al, %%al\n"
		      "loop:"
		      "movb	%%al, (%%rdi)\n"
		      "incq	%%rdi\n"
		      "decq	%%rsi\n"
		      "jnz	loop\n"
		      :
		      : "D" (block), "S" (len)
		      :
		      );
	return 0;
}

int main (void)
{
	char *block1 = malloc(32768 + 1);
	char *block2 = malloc(32768 + 1);
	char *block3 = malloc(32768 + 1);
	char *block4 = malloc(32768 + 1);
	char *block5 = malloc(32768 + 1);
	
	memset(block1, 0x26, 32768);
	memset(block2, 0x26, 32768);
	memset(block3, 0x26, 32768);
	memset(block4, 0x26, 32768);
	memset(block5, 0x26, 32768);
	
	unsigned long t1, t2;
	
	rdtscl(t1);
	old_bzero(block1, 32768);
	rdtscl(t2);
	printf("old - (%6lu) %s\n", t2 - t1, block1);

	rdtscl(t1);
	old_bzero(block2, 32768);
	rdtscl(t2);
	printf("old - (%6lu) %s\n", t2 - t1, block2);
	
	rdtscl(t1);
	new_bzero(block3, 32768);
	rdtscl(t2);
	printf("new - (%6lu) %s\n", t2 - t1, block3);
	
	rdtscl(t1);
	asm_bzero(block4, 32768);
	rdtscl(t2);
	printf("asm - (%6lu) %s\n", t2 - t1, block4);
	
	rdtscl(t1);
	bzero(block5, 32768);
	rdtscl(t2);
	printf("std - (%6lu) %s\n", t2 - t1, block5);
	
	free(block1);
	free(block2);
	free(block3);
	free(block4);
	free(block5);
}