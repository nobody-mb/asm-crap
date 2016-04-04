#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define rdtscl(val) asm volatile ("rdtsc" : "=A" (val) : : );

char * old_str_insidesep(char *dst, char **base, char open, char close);
char * new_str_insidesep(char *dst, char **base, char open, char close);
char * asm_str_insidesep(char *dst, char **base, char open, char close);

char * old_str_insidesep(char *dst, char **base, char open, char close)
{
	int i = 0;
	int ns;
	char *new_base = *base;
	char *tmp_base;

	new_base += strlen(*base) - 1;
	
	while (*new_base != open && *new_base) {
		new_base--;
		i++; 
	}
	new_base++;
	
	ns = strlen(*base) - i - 1;
	tmp_base = new_base;
	new_base += ns;
	//printf("tb=%s\n", tmp_base);
	
	strcpy((*base) + ns, new_base);
	
	//printf("copy %d bytes nb = %s tb=%s ns=%d\n", i, new_base, tmp_base, ns);

	while (*tmp_base != close && *tmp_base) {
		//printf("%s", tmp_base);
		*dst++ = *tmp_base++;
		i++;
	}
	//printf("%d\n", i);
}

char *new_str_insidesep(char *dst, char **base, char open, char close)
{
	char *tmp = *base;
	char *tmp2 = *base;
	char *dst2 = dst;
	
	int len = 0;
	int j = 0;
	
	do {
		len++;
		tmp++;
	} while (*tmp);

	do {
		tmp--;
		j++;
	} while (j <= len && *tmp != open);
	
	tmp++;
	
	if (tmp == tmp2)
		return NULL;

	do {
		if (!*tmp) {
			dst = dst2;
			*dst = 0;
			return NULL;
		}
		*dst++ = *tmp++;
	} while (*tmp != close);

	tmp2 += (len - j);
	
	do {
		*tmp2++ = *++tmp;
	} while (*tmp);
	
	*dst = 0;
	*tmp2 = 0;
	return dst;
}

void test_insidesep(char *base, char open, char close)
{
	char *o1 = calloc(128,1);
	char *o2 = calloc(128,1);
	char *o3 = calloc(128,1);
	
	char *b1 = strdup(base);
	char *b2 = strdup(base);
	char *b3 = strdup(base);

	uint64_t t1, t2;

	rdtscl(t1);
	old_str_insidesep(o1, &b1, open, close); 
	rdtscl(t2);
	printf("old - (%5llu cycles) %s -> %s\n", 
		(long long unsigned int)t2 - t1, b1, o1);
		
	rdtscl(t1);
	new_str_insidesep(o2, &b2, open, close); 
	rdtscl(t2);
	printf("new - (%5llu cycles) %s -> %s\n", 
		(long long unsigned int)t2 - t1, b2, o2);
		
	rdtscl(t1);
	asm_str_insidesep(o3, &b3, open, close); 
	rdtscl(t2);
	printf("asm - (%5llu cycles) %s -> %s\n", 
		(long long unsigned int)t2 - t1, b3, o3);
	free(o1);
	free(o2);
	free(o3);
	
	free(b1);
	free(b2);
	free(b3);
}

int main (void)
{
	printf("\nWorking\n");
	test_insidesep("(xx(yy)zz)", '(', ')');
	
	printf("\nWorking 2\n");
	test_insidesep("(xx(aa(bb)cc)zz)", '(', ')');
	
	printf("\nWorking All\n");
	test_insidesep("(ca)", '(', ')');
	
	printf("\nDifferent Side Balance\n");
	test_insidesep("(ca))", '(', ')');
	
	printf("\nNo Seperators\n");
	test_insidesep("ca", '(', ')');
	
	printf("\nNo 1\n");
	test_insidesep("(ca", '(', ')');
	
	printf("\nNo 2\n");
	test_insidesep("ca)", '(', ')');
	
	printf("\nTwo Seperators\n");
	test_insidesep("(asdf) (ghjk)", '(', ')');
	
	char *exp = malloc(128);
	char *dst = malloc(128); 
	strcpy(exp, "(asdf(qq)j(fn)kl)");
	printf("\n%s\n", exp);
	while (asm_str_insidesep(dst, &exp, '(', ')')) {
		printf("%s -> %s\n", exp, dst);
	}	
	free(exp);
}

