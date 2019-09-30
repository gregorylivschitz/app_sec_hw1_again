#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "dictionary.h"

#define DICTIONARY "/mnt/c/Users/greg1/dev/app_sec_assignment1/wordlist.txt"

extern hashmap_t * hashtable[];
int main(int argc, char *argv[])
{
	bool success = load_dictionary(DICTIONARY, hashtable);
	if (success){
		FILE *fp = fopen(argv[1], "r");
		char* misspell[MAX_MISSPELLED];
		printf("Checking misspelled words");
		int misspelled_words = check_words(fp, hashtable, misspell);
		printf("misspelled words %d\n", misspelled_words);
	}

	return 0;
	//https://stackoverflow.com/questions/31061326/why-valgrind-report-my-memory-as-definitely-lost
	// Need to do exit otherwise there might be memory leak according to valgrind.
}