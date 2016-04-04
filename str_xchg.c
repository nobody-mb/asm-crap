#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>

#define rdtscl(val) asm volatile ("rdtsc" : "=A" (val) : : );

void old_str_xchg(char *dst, const char *src, const char *s1, const char *s2);
void new_str_xchg(char *dst, const char *src, const char *s1, const char *s2);
void asm_str_xchg(char *dst, const char *src, const char *s1, const char *s2);

void old_str_xchg(char *dst, const char *src, const char *s1, const char *s2)
{
	if (!src || !s1 || !s2 || !*s1 || !*s2)
		return;

	size_t s1len = strlen(s1);
	size_t s2len = strlen(s2);

	while (*src) {
		if (!memcmp(src, s1, s1len)) {
			memcpy(dst, s2, s2len);
			dst += s2len;
			src += s1len;
		} else if (!memcmp(src, s2, s2len)) {
			memcpy(dst, s1, s1len);
			dst += s1len;
			src += s2len;
		} else {
			*dst++ = *src++;
		}
	}
	
	*dst = '\0';
}

void new_str_xchg(char *dst, const char *src, const char *s1, const char *s2)
{
	if (!src || !s1 || !s2 || !*s1 || !*s2)
		return;

	size_t i;

	while (*src) {
		
		for (i = 0; s1[i] == src[i] && s1[i]; ) {
			i++; 
		}
		if (!s1[i]) {
			src += i;
			for (i = 0; s2[i]; i++) 
				*dst++ = s2[i];
			continue;
		}
		*dst++ = *src++; 
	}
	*dst = '\0';
}

void test_str(const char *src, const char *s1, const char *s2)
{
	char *o1 = malloc(128);
	char *o2 = malloc(128);
	char *o3 = malloc(128);
	
	uint64_t t1, t2;

	rdtscl(t1);
	old_str_xchg(o1, src, s1, s2); 
	rdtscl(t2);
	printf("old - (%5llu cycles) %s\n", (long long unsigned int)t2 - t1, o1);
	
	rdtscl(t1);
	new_str_xchg(o2, src, s1, s2); 
	rdtscl(t2);
	printf("new - (%5llu cycles) %s\n", (long long unsigned int)t2 - t1, o2);

	rdtscl(t1);
	asm_str_xchg(o3, src, s1, s2); 
	rdtscl(t2);
	printf("asm - (%5llu cycles) %s\n", (long long unsigned int)t2 - t1, o3);
	
	free(o1);
	free(o2);
	free(o3);
}

int main (void)
{
	test_str("Hello!", "el", "qq");

	printf("\n2 to 2\n");
	test_str("Hello!", "el", "qq");
	
	printf("\n2 to 1\n");
	test_str("Hello!", "el", "q");

	printf("\n1 to 2\n");
	test_str("Hello!", "e", "qq");
	
	printf("\nMore than 1\n");
	test_str("Hello!", "l", "q");
	
	printf("\nMore than 1 (long)\n");
	test_str("Hello! Hello!", "Hello", "Hi");

	printf("\nNone\n");
	test_str("Hello!", "f", "q");
	
	printf("\nReplace with Nothing\n");
	test_str("Hello!", "l", "");
	
	printf("\nFull Word\n");
	test_str("Hello!", "Hello!", "Hello!");
	
	printf("\nFull Word Long\n");
	test_str("Hello! Hi!", "Hello!", "Hi!");
	
	printf("\nOther Characters in String\n");
	test_str("This string array is a Test!", "string", "array");
}

