#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <termios.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>

#define rdtscl(val) \
		__asm__ __volatile__ ("rdtsc" : "=A" (val) : : );

#define TRIALS		128

void old_find_pattern(char *output, char *input)
{
	int j = (int)strlen(input);
	char *q;
	while (j) {
		q = input;
		while (!strncmp(q, input, j)) {
			q += j;
			if (!*q) {
				q -= j;
				strcpy(output, q);
				break;
			}
		} 
		j--;
	}
}

void new_find_pattern(char *output, char *input)
{
	int i = 0, j;
	char *s1, *s2;
	char *c1, *c2;

	while (1) {
		i++;
		c1 = input;
		while (1) {
			j = i;
			s1 = c1;
			s2 = input;
			while (j) {
				if (*s1++ != *s2++)
					break;
				j--;
			}
			if (j) {
				break;
			}
			c1 += i;
			if (!*c1) {
				c1 -= i;
				for (c2 = output; *c1; c2++)
					*c2 = *c1++;
				*c2 = '\0';
				return;
			}
		} 
	}
}

void asm_find_pattern(char *output, char *input);

void test_find_pattern(char *output, char *input)
{
	unsigned long long t1, t2;
	
	*output = '\0';
	rdtscl(t1);
	old_find_pattern(output, input);
	rdtscl(t2);
	printf("old - (%5llu) %s\n", t2 - t1, output);

	*output = '\0';
	rdtscl(t1);
	new_find_pattern(output, input);
	rdtscl(t2);
	printf("new - (%5llu) %s\n", t2 - t1, output);
	
	*output = '\0';
	rdtscl(t1);
	asm_find_pattern(output, input);
	rdtscl(t2);
	printf("asm - (%5llu) %s\n\n", t2 - t1, output);
}

int main(int argc, char *argv[])
{
	char *pattern = malloc(TRIALS);
	
	printf("\nNull String\n");
	test_find_pattern(pattern, "");
	
	printf("\nDouble Pattern\n");
	test_find_pattern(pattern, "asdfasdf");
	
	printf("\nQuadruple Pattern\n");
	test_find_pattern(pattern, "qwertyqwertyqwertyqwerty");
	
	printf("\nNo Pattern\n");
	test_find_pattern(pattern, "asdf");
	
	printf("\nOne Letter\n");
	test_find_pattern(pattern, "a");
	
	printf("\nOne Letter Repeated\n");
	test_find_pattern(pattern, "aaaaaaaa");
	
	printf("\nZero Char Embedded\n");
	test_find_pattern(pattern, "asdfasdf\0asdfqwert");
	
	free(pattern);
}