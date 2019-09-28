#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "dictionary.h"

extern hashmap_t * hashtable[];
int main()
{
	bool success = load_dictionary("test_worlist.txt", hashtable);
	print_hashtable(hashtable);
//	if (success){
//		char cwd[PATH_MAX];
//		getcwd(cwd, sizeof(cwd));
//		strcat(cwd, "/test1.txt");
//		FILE *fp = fopen(cwd, "r");
//		char* misspell[256];
//		check_words(fp, hashtable, misspell);
//	}
//	return 0;
}