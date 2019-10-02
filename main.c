#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <linux/limits.h>
#include "dictionary.h"

#define DICTIONARY "wordlist.txt"

int main(int argc, char *argv[])
{
	if (PATH_MAX > strlen(argv[1])){
		hashmap_t hashtable[HASH_SIZE];
		bool success = load_dictionary(DICTIONARY, hashtable);
		if (success){
			FILE *fp = fopen(argv[1], "r");
			char* misspell[MAX_MISSPELLED];
			int misspelled_words = check_words(fp, hashtable, misspell);
			printf("misspelled words %d\n", misspelled_words);
			for (int i=0; i< misspelled_words; i++){
				char* my_word = misspell[i];
				free(my_word);
			}
		}
	}

	return 0;
}