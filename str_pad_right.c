#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define rdtscl(val) asm volatile("rdtsc" : "=A" (val) : : )

void old_str_pad_to_r (char *dst, const char *src, size_t len)
{
	int line_len;
	int offset;
	
	while (*src) {
		line_len = 0;
		while (line_len < len) {                // this loop counts the number of chars in current line
			line_len++;
			if (*src == '\n' || *src == '\0')
				break;
			src++;
		}
		
		if (line_len == len) {                 // if not at the end of the string, then advance the pointer to the end
			while (*src != '\n' && *src)
				src++;
		}
		
		src -= line_len - 1;                  // back the pointer to the previous point where it used to point
		offset = (int)(len - line_len);            // set offset to the length passed in minus the length of the current line
		
		while (offset--) {                    // counts down from length of offset and sets what dst points to, to " "
			*dst++ = ' ';
		}
		
		while (line_len--) {                 // counts down from how many characters are in the line and then makes 
			*dst++ = *src++;                 // what dst points to equal to what src points to  (copying the string)
		}
	}
}

void new_str_pad_to_r (char *dst, const char *src, size_t len)
{
	int line_len;
	int offset;
	src--;
	do {
		line_len = 0;
		do {
			src++;
			if (line_len < len)
				line_len++;
		} while (*src != '\n' && *src);

		offset = (int)(len - line_len);
		src -= (line_len);

		while (offset--)
			*dst++ = ' ';

		while (line_len--) 
			*dst++ = *++src;
	} while (*src);
}

void asm_str_pad_to_r (char *dst, const char *src, size_t len);

void test_ralign (const char *src, size_t len)
{
	unsigned long t1, t2;
	char *r1 = malloc(2048);
	char *r2 = malloc(2048);
	char *r3 = malloc(2048);
	
	old_str_pad_to_r(r1, src, len);
	rdtscl(t1);
	old_str_pad_to_r(r1, src, len);
	rdtscl(t2);
	printf("old - %5lu:\n%s\n\n", t2 - t1, r1);
	
	new_str_pad_to_r(r2, src, len);
	rdtscl(t1);
	new_str_pad_to_r(r2, src, len);
	rdtscl(t2);
	printf("new - %5lu:\n%s\n\n", t2 - t1, r2);
	
	asm_str_pad_to_r(r3, src, len);
	rdtscl(t1);
	asm_str_pad_to_r(r3, src, len);
	rdtscl(t2);
	printf("asm - %5lu:\n%s\n\n", t2 - t1, r3);
	
	free(r1);
	free(r2);
	free(r3);
}

int main(int argc, const char * argv[])
{
	test_ralign("Hello! This is a test!\n"
		    "Here's a blank line:\n"
		    "\n"
		    "And short\n"
		    "q", 40);
	
	test_ralign("Hello! This is a test!\n"
		    "Here's a blank line:\n"
		    "\n"
		    "And short\n"
		    "q", 30);
	
	test_ralign("Hello! This is a test!\n"
		    "Here's a blank line:\n"
		    "\n"
		    "And short\n"
		    "q", 10);
	
	test_ralign("Hello! This is a test!\n"
		    "Here's a blank line:\n"
		    "\n"
		    "And short\n"
		    "q", 0);
	
	test_ralign("Hello! This is a test!\n"
		    "Here's a blank line:\n"
		    "\n"
		    "And short\n"
		    "Here's a blank line:\n"
		    "\n"
		    "And short\n"
		    "q", 18);
	
	return 0;
}

