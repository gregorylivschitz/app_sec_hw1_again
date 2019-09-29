#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"




int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[])
{
	char *line_buf = NULL;
	size_t line_buf_size = 0;
	int line_count = 0;
	ssize_t line_size;
	int misspell_count = 0;
	// The problem with this is I think we can get oom by big files?
	line_size = getline(&line_buf, &line_buf_size, fp);
	while (line_size >= 0)
	{
		line_buf[strcspn(line_buf, "\n")] = 0;
		char * pch;
		pch = strtok(line_buf," ,.;:");
		while (pch != NULL)
		{
			bool is_word_spelled = check_word(pch, hashtable);
			if (!(is_word_spelled)){
				printf("Found misspelled word %s\n", pch);
				misspelled[misspell_count] = pch;
				misspell_count++;
			}
			pch = strtok(NULL, " ,.;:");
		}
		line_count++;
		line_size = getline(&line_buf, &line_buf_size, fp);
	}
//	print_misspell(misspelled);
	free(line_buf);
	line_buf = NULL;
	fclose(fp);
	return misspell_count;
}

bool check_word(const char* word, hashmap_t hashtable[]){
	int hash_code = hash_function(word);
	hashmap_t word_node_ptr = hashtable[hash_code];
	printf("the hashcode of word %s is %d\n", word, hash_code);
	while(word_node_ptr != NULL)
	{
		if (strcmp(word_node_ptr->word, word) == 0){
			printf("word found %s\n", word_node_ptr->word);
			return true;
		}
		word_node_ptr = word_node_ptr->next;
	}
//	char * lower_word = strlwr_custom(word);
//	int hash_code_lower = hash_function(lower_word);
//	hashmap_t word_node_ptr_lower = hashtable[hash_code_lower];
//	printf("the hashcode of lower word %s is %d\n", lower_word, hash_code_lower);
//	while(word_node_ptr_lower != NULL)
//	{
//		if (strcmp(word_node_ptr_lower->word, lower_word) == 0){
//			printf("word found lower case\n");
//			return true;
//		}
//		word_node_ptr_lower = word_node_ptr_lower->next;
//	}
	return false;
}

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]){
	for (int i=0; i< HASH_SIZE; i++){
		hashtable[i] = NULL;
	}
	FILE *fp = fopen(dictionary_file, "r");
	char *line_buf = NULL;
	size_t line_buf_size = 0;
	int line_count = 0;
	ssize_t line_size;
	line_size = getline(&line_buf, &line_buf_size, fp);
	while(line_size != -1){
		hashmap_t new_node_ptr = calloc(1, sizeof(node));
		line_buf[strcspn(line_buf, "\n")] = 0;
		strcpy(new_node_ptr->word, line_buf);
		int hash_code = hash_function(line_buf);
		new_node_ptr->next = NULL;
		if (hashtable[hash_code] != NULL){
			insert_node_at_end(new_node_ptr, hashtable[hash_code]);
		}else{
			hashtable[hash_code] = new_node_ptr;
		}
		line_count++;
		line_size = getline(&line_buf, &line_buf_size, fp);
	}
	return true;
}


void insert_node_at_end(hashmap_t new_node, hashmap_t head)
{
	hashmap_t temp = malloc(sizeof(hashmap_t));
	if(new_node == NULL)
	{
		printf("Unable to allocate memory.");
	}
	else
	{
		temp = head;;
		while(temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = new_node;
	}
}


void print_hashtable(hashmap_t hashtable[]){
	printf("starting printing hashtable\n");
	for(int i=0; i<HASH_SIZE; i++){
		hashmap_t blah = hashtable[i];
		while(blah != NULL){
			printf("the hashcode is %d\n", i);
			printf("the word is %s, next %p\n", blah->word, blah->next);
			blah = blah->next;
		}
	}
}

void print_hashtable_one_link(hashmap_t hashtable[], int hash){
	hashmap_t blah = hashtable[hash];
	printf("link list %p, hash %d", blah, hash);
	while(blah != NULL) {
		printf("the hashcode is %d\n", hash);
		printf("the word is %s, next %p\n", blah->word, blah->next);
		blah = blah->next;
	}
}

////Took it from here: https://ftp.samba.org/pub/unpacked/lorikeet-heimdal/lib/roken/strlwr.c
//strlwr_custom(char *str)
//{
//	printf("running tolower\n");
//	char *s;
//	for(s = str; *s; s++){
//		printf("to_lower before %d", (int)*s);
//		*s = tolower(*s);
//		printf("to_lower after %d", (int)*s);
//	}
//	return str;
//}