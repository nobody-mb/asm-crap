#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define rdtscl(val) asm volatile ("rdtsc" : "=A" (val) : : );

void old_str_extract(const char *input, char *output, const char *start, const char *end);
void new_str_extract(const char *input, char *output, const char *start, const char *end);
void asm_str_extract(const char *input, char *output, const char *start, const char *end);

char *asm_strstr(const char *cs, const char *ct);

void old_str_extract(const char *input, char *output, const char *start, const char *end)
{
	if (!start || !end || !input)
		return;
		
	int i = 0, tmp;
	char *proc = (char *)input;
	size_t stl = strlen(start);
	size_t edl = strlen(end);
	
	while ((tmp = strncmp(proc, start, stl)) && *proc) 
		proc++;
	
	if (tmp)
		return;
	
	proc += stl;
	
	while ((tmp = strncmp(proc, end, edl)) && *proc++) 
		i++;
	
	if (tmp) 
		return; 
	
	proc -= i;
	
	strncpy(output, proc, i);
	
	output[i] = 0;
}

void new_str_extract(const char *input, char *output, const char *start, const char *end)
{
	if (!input || !start || !end)
		return;
	
	char *stp = strstr(input, start);
	char *edp = strstr(input, end);
	
	if (!stp || !edp)
		return;
		
	if (stp == edp)
		return;

	int len = (int)strlen(start);
	int cpy = (int)(edp-(stp+len));

	strncpy(output, &stp[len], cpy);
	
	output[cpy] = 0;
}

void test_extract(char *cmp, char *start, char *end)
{
	char *o1 = malloc(128);
	char *o2 = malloc(128);
	char *o3 = malloc(128);
	
	strcpy(o1, "asdfasdf");
	strcpy(o2, "asdfasdf");
	strcpy(o3, "asdfasdf");
	
	uint64_t t1, t2;
	
	rdtscl(t1);
	old_str_extract(cmp, o1, start, end); 
	rdtscl(t2);
	printf("old - (%5llu cycles) %s\n", (long long unsigned int)t2 - t1, o1);
	
	rdtscl(t1);
	new_str_extract(cmp, o2, start, end); 
	rdtscl(t2);
	printf("new - (%5llu cycles) %s\n", (long long unsigned int)t2 - t1, o2);

	rdtscl(t1);
	asm_str_extract(cmp, o3, start, end); 
	rdtscl(t2);
	printf("asm - (%5llu cycles) %s\n", (long long unsigned int)t2 - t1, o3);

	free(o1);
	free(o2);
	free(o3);
}

int main(void)
{
	
	printf("\nWorking\n");
	test_extract("asdf2dlkj<<$@3f>>", "<<", ">>");
	
	printf("\nWorking\n");
	test_extract("asdf<<value == \"hi!\">>qqqq", "<<value == \"", "\">>");
		
	printf("\nEmpty Start\n");
	test_extract("asdf<<value == \"hi!\">>qqqq", NULL, "\">>");
	
	printf("\nEmpty End\n");
	test_extract("asdf<<value == \"hi!\">>qqqq", "<<value == \"", NULL);
	
	printf("\nEmpty Source\n");
	test_extract(NULL, "<<value == \"", "\">>");
	
	printf("\nStrings Identical\n");
	test_extract("<<hi>>", "<<hi>>", "<<hi>>");
	
	printf("\nStart not in Source\n");
	test_extract("<<hi>>", "asdf", ">>");
	
	printf("\nEnd not in Source\n");
	test_extract("<<hi>>", "<<", "asdf");
	
	printf("\nNeither in Source\n");
	test_extract("<<hi>>", "j", "f");
}

