#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>

#define rdtscl(val) asm volatile ("rdtsc" : "=A" (val) : : );

void old_str_rep(char *dst, const char *src, const char *find, const char *rep);
void new_str_rep(char *dst, const char *src, const char *find, const char *rep);
void asm_str_rep(char *dst, const char *src, const char *find, const char *rep);

void old_str_rep(char *dst, const char *src, const char *find, const char *rep)
{
	if (!src || !find)
		return;
		
	char *q; 
	char *r; 

	size_t flen = strlen(find);
	size_t rlen = strlen(rep);
	
	ptrdiff_t len;
	
	while (*src) {
		q = strstr(src, find);
		if (!q) {
			while (*src) 
				*dst++ = *src++;
			break;
		}
		len = q - src;
		while (len) {
			*dst++ = *src++; 
			len--;
		}
		strncpy(dst, rep, rlen);
		dst += rlen;
		src += flen;
	}
	
	*dst = '\0';
}

void new_str_rep(char *dst, const char *src, const char *find, const char *rep)
{
	if (!src || !find)
		return;

	size_t i;

	while (*src) {
		for (i = 0; find[i] == src[i] && find[i]; ) {
			i++; 
		}
		if (!find[i]) {
			src += i;
			for (i = 0; rep[i]; i++) 
				*dst++ = rep[i];
			continue;
		}
		*dst++ = *src++; 
	}
	*dst = '\0';
}

void test_str(const char *src, const char *find, const char *rep)
{
	char *o1 = malloc(128);
	char *o2 = malloc(128);
	char *o3 = malloc(128);
	
	uint64_t t1, t2;

	rdtscl(t1);
	old_str_rep(o1, src, find, rep); 
	rdtscl(t2);
	printf("old - (%5llu cycles) %s\n", (long long unsigned int)t2 - t1, o1);
	
	rdtscl(t1);
	new_str_rep(o2, src, find, rep); 
	rdtscl(t2);
	printf("new - (%5llu cycles) %s\n", (long long unsigned int)t2 - t1, o2);

	rdtscl(t1);
	asm_str_rep(o3, src, find, rep); 
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
	test_str("Hello!", "Hello!", "q");
	
	printf("\nFull Word Long\n");
	test_str("Hello!", "Hello!", "Another Word!");
	
	printf("\nOther Characters in String\n");
	test_str("This string is a Test!", "string", "array");
}

