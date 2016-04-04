#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define rdtscl(val) asm volatile ("rdtsc" : "=A" (val) : : );

char *old_str_str(const char *s1, const char *s2);
char *new_str_str(const char *s1, const char *s2);
char *asm_str_str(const char *s1, const char *s2);

char *old_str_str(const char *s1, const char *s2)
{
	size_t l1, l2;
	
	l2 = strlen(s2);
	l1 = strlen(s1);
	
	if (!l2)
		return (char *)s1;
		
	while (l1 >= l2) {
		l1--;
		if (!memcmp(s1, s2, l2))
			return (char *)s1;
		s1++;
	}
	return NULL;
}

char *new_str_str(const char *s1, const char *s2)
{
	int rcx = -1; 
	
	do {
		rcx++;
	} while (s2[rcx]);
	 
	if (!rcx) {
		return (char *)s1;
	}
	while (*s1) {
		while (rcx > 0) {
			if (s2[rcx - 1] != s1[rcx - 1]) {
				break;
			}
			rcx--;
		}
		if (!rcx)
			return (char *)s1; 
		s1++; 
	}
	
	return NULL; 
}

void test_str(const char *s1, const char *s2)
{
	char *o1, *o2, *o3;

	uint64_t t1, t2;
	
	rdtscl(t1);
	o1 = old_str_str(s1, s2); 
	rdtscl(t2);
	printf("old - (%5llu cycles) %s\n", (long long unsigned int)t2 - t1, o1);
	
	rdtscl(t1);
	o2 = new_str_str(s1, s2); 
	rdtscl(t2);
	printf("new - (%5llu cycles) %s\n", (long long unsigned int)t2 - t1, o2);

	rdtscl(t1);
	o3 = asm_str_str(s1, s2); 
	rdtscl(t2);
	printf("asm - (%5llu cycles) %s\n", (long long unsigned int)t2 - t1, o3);
}

int main (void)
{
	printf("\nWorking\n");
	test_str("asdfghjk", "f");

	printf("\nEnd\n");
	test_str("asdf<<value == \"hi!\">>qqqq", "qqq");

	printf("\nStrings Identical\n");
	test_str("<<hi>>", "<<hi>>");
	
	printf("\nStart not in Source\n");
	test_str("<<hi>>", "asdf");

	printf("\nSmall Source\n");
	test_str("s", "sdf");
	
	printf("\nTwice\n");
	test_str("QQ<<JJ<<FF<<JJ", "JJ");
	
	printf("\nNull Termination Only\n");
	test_str("mbk", "");
}

