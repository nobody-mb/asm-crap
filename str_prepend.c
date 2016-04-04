#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define rdtscl(val) asm volatile ("rdtsc" : "=A" (val) : : );

void old_str_prepend(char *base, const char *app);
void new_str_prepend(char *base, const char *app);
void asm_str_prepend(char *base, const char *app);

void old_str_prepend(char *base, const char *app)
{
	int plen = strlen(app);
	int blen = strlen(base);
	memmove(base + plen, base, blen);
	memcpy(base, app, plen);
	base[blen + plen] = 0;
}

void new_str_prepend(char *base, const char *app)
{
	int plen = strlen(app);
	int blen = strlen(base);
	char *tmp = base;
	
	tmp[blen + plen] = 0;
	while (blen--) {		
		tmp[blen + plen] = tmp[blen]; 
	}
	while (plen--) {		
		tmp[plen] = app[plen];
	}
}

void test_prepend(char *_base, const char *app)
{
	char *o1 = malloc(128);
	char *o2 = malloc(128);
	char *o3 = malloc(128);
	
	strcpy(o1, _base);
	strcpy(o2, _base);
	strcpy(o3, _base);
	
	uint64_t t1, t2;
	
	rdtscl(t1);
	old_str_prepend(o1, app); 
	rdtscl(t2);
	printf("old - (%5llu cycles) %s\n", (long long unsigned int)t2 - t1, o1);
	
	rdtscl(t1);
	new_str_prepend(o2, app); 
	rdtscl(t2);
	printf("new - (%5llu cycles) %s\n", (long long unsigned int)t2 - t1, o2);

	rdtscl(t1);
	asm_str_prepend(o3, app); 
	rdtscl(t2);
	printf("asm - (%5llu cycles) %s\n", (long long unsigned int)t2 - t1, o3);

	free(o1);
	free(o2);
	free(o3);
}

int main (void)
{
	printf("\nWorking\n");
	test_prepend("asdf", "asdf");
		
	printf("\nEmpty Start\n");
	test_prepend("", "asdf");
	
	printf("\nEmpty End\n");
	test_prepend("asdf", "");
	
	printf("\nWorking 2\n");
	test_prepend("Hello!", "Hi! ");
	
	printf("\nEmbedded Zero\n");
	test_prepend("asdf\0ghjk", "qwerty");
}

