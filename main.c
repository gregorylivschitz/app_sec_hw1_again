#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "dictionary.h"

#define DICTIONARY "wordlist.txt"

extern hashmap_t * hashtable[];
int main(int argc, char *argv[])
{
	if( argc == 2 ) {
		printf("The argument supplied is %s\n", argv[1]);
	}
	else if( argc > 2 ) {
		printf("Too many arguments supplied.\n");
	}
	else {
		printf("One argument expected.\n");
	}
	bool success = load_dictionary("wordlist.txt", hashtable);
	if (success){
//		char cwd[PATH_MAX];
//		getcwd(cwd, sizeof(cwd));
//		strcat(cwd, "/test1.txt");
		FILE *fp = fopen(argv[1], "r");
		char* misspell[MAX_MISSPELLED];
		int misspelled_words = check_words(fp, hashtable, misspell);
		printf("misspelled words %d\n", misspelled_words);
	}
	//https://stackoverflow.com/questions/31061326/why-valgrind-report-my-memory-as-definitely-lost
	exit(0);
}