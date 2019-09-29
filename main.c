#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "dictionary.h"

#define DICTIONARY "wordlist.txt"

extern hashmap_t * hashtable[];
int main()
{
	bool success = load_dictionary("wordlist.txt", hashtable);
	if (success){
		char cwd[PATH_MAX];
		getcwd(cwd, sizeof(cwd));
		strcat(cwd, "/test1.txt");
		FILE *fp = fopen(cwd, "r");
		char* misspell[MAX_MISSPELLED];
		check_words(fp, hashtable, misspell);
	}
	return 0;
}