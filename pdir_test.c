#include <stdio.h>

/* OUTPUT: 
 [Switching to process 32872 thread 0x0]
 file path: ~/Users/nobody1/Desktop/file.jpg
 file name: file.jpg
 extension (c): .jpg
 extension (asm): .jpg
 parent dir (asm): /Desktop/file.jpg
 parent dir (c): /Desktop/file.jpg
 parent dir (c old): /Desktop/file.jpg
 Program ended with exit code: 0
 */

/* returns a pointer to the parent directory of the file specified in path dir */
const char *get_pdir_asm (const char *dir)
{
	asm volatile ("movq %%rcx, %%rdx\n"
		      "movb $2, %%al\n"
		"len_loop:\n"			/* increment RCX until reached end of string */
		      "cmpb $0, (%%rcx)\n"
		      "je main_loop\n"
		      "incq %%rcx\n"
		      "jmp len_loop\n"
		"main_loop:\n"			/* do this after the end is reached */
		      "decq %%rcx\n"
		      "cmpq %%rdx, %%rcx\n"
		      "jl insufficient\n"	/* if reached beginning, return error */
		      "cmpb $47, (%%rcx)\n"	/* check char, ascii 47 = '/' */
		      "jne main_loop\n"	
		      "decb %%al\n"	
		      "jz success\n"
		      "jmp main_loop\n"
		"insufficient:\n"
		      "xorq %%rcx, %%rcx\n"	/* set return value to NULL */
		"success:\n"
		      : "=c" (dir)
		      : "c" (dir)
		      : "%rdx", "%al"
		      );
	return dir;
}

/* returns a pointer to the parent directory of the file specified in path dir */
const char *get_last_asm (const char *dir, unsigned char cmp)
{
	asm volatile ("movq %%rcx, %%rdx\n"
		"ext_len_loop:\n"		/* increment RCX until reached end of string */
		      "cmpb $0, (%%rcx)\n"
		      "je ext_main_loop\n"
		      "incq %%rcx\n"
		      "jmp ext_len_loop\n"
		"ext_main_loop:\n"		/* do this after the end is reached */
		      "decq %%rcx\n"
		      "cmpq %%rdx, %%rcx\n"
		      "jl ext_insufficient\n"	/* if reached beginning, return error */
		      "cmpb %%bl, (%%rcx)\n"
		      "je ext_success\n"
		      "jmp ext_main_loop\n"
		"ext_insufficient:\n"
		      "xorq %%rcx, %%rcx\n"	/* set return value to NULL */
		"ext_success:\n"
		      : "=c" (dir)
		      : "c" (dir), "b" (cmp)
		      : "%rdx"
		      );
	return dir;
}

const char *get_last (const char *dir, unsigned char cmp)
{
	const char *start = dir;
	
	while (*dir)
		dir++;
	
	while (dir >= start && *dir != cmp)
		dir--;
	
	return (dir == start) ? NULL : dir;
}

const char *get_parent_dir (const char *dir)
{
	const char *start = dir;
	int levels = 2;
	
	while (*dir)
		dir++;
	
	while (levels && dir >= start)
		if (*--dir == '/')
			--levels;
	
	return (dir == start) ? NULL : (dir);
}

int main (void) 
{
	const char *testpath = "~/Users/nobody1/Desktop/file.jpg";
	
	fprintf(stderr, "file path: %s\n", testpath);
	
	fprintf(stderr, "file name (c): %s\n", get_last(testpath, '/') + 1);
	fprintf(stderr, "file name (asm): %s\n", get_last_asm(testpath, '/') + 1);
	
	fprintf(stderr, "extension (c): %s\n", get_last(testpath, '.'));
	fprintf(stderr, "extension (asm): %s\n", get_last_asm(testpath, '.'));
	
	fprintf(stderr, "parent dir (c): %s\n", get_parent_dir(testpath));
	fprintf(stderr, "parent dir (asm): %s\n", get_pdir_asm(testpath));
	
	return 0;
}



