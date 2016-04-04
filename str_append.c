#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define rdtscl(val) asm volatile ("rdtsc" : "=A" (val) : : );

void old_str_append(char *base, const char *app);
void new_str_append(char *base, const char *app);
void asm_str_append(char *base, const char *app);

void old_str_append(char *base, const char *app)
{
	base += strlen(base);
	strcpy(base, app);
}

void new_str_append(char *base, const char *app)
{
	int len = 0;
	while (*base)
		base++;
	while (*app)
		*base++ = *app++;
}

void test_append(char *_base, const char *app)
{
	char *o1 = malloc(128);
	char *o2 = malloc(128);
	char *o3 = malloc(128);
	
	strcpy(o1, _base);
	strcpy(o2, _base);
	strcpy(o3, _base);
	
	uint64_t t1, t2;
	
	rdtscl(t1);
	old_str_append(o1, app); 
	rdtscl(t2);
	printf("old - (%5llu cycles) %s\n", (long long unsigned int)t2 - t1, o1);
	
	rdtscl(t1);
	new_str_append(o2, app); 
	rdtscl(t2);
	printf("new - (%5llu cycles) %s\n", (long long unsigned int)t2 - t1, o2);

	rdtscl(t1);
	asm_str_append(o3, app); 
	rdtscl(t2);
	printf("asm - (%5llu cycles) %s\n", (long long unsigned int)t2 - t1, o3);

	free(o1);
	free(o2);
	free(o3);
}

int main (void)
{
	printf("\nWorking\n");
	test_append("asdf", "asdf");
		
	printf("\nEmpty Start\n");
	test_append("", "asdf");
	
	printf("\nEmpty End\n");
	test_append("asdf", "");
	
	printf("\nWorking 2\n");
	test_append("Hello!", " Hi!");
	
	printf("\nEmbedded Zero\n");
	test_append("asdf\0ghjk", "qwerty");
}

