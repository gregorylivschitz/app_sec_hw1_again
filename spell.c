#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "dictionary.h"



int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[])
{
	char *line_buf = NULL;
	size_t line_buf_size = 0;
	int line_count = 0;
	ssize_t line_size;
	/* The problem with this is I think we can get oom by big files? */
	line_size = getline(&line_buf, &line_buf_size, fp);
	while (line_size >= 0)
	{
		line_count++;
		line_size = getline(&line_buf, &line_buf_size, fp);
		char * pch;
		pch = strtok(line_buf," ,.-");
		while (pch != NULL)
		{
			check_word(pch, hashtable);
			pch = strtok(NULL, " ,.-");
		}
	}
	free(line_buf);
	line_buf = NULL;
	fclose(fp);
	return 1;
}

bool check_word(const char* word, hashmap_t hashtable[]){
//	int hash_code = hash_function(tolower(word));
//	printf("the word is %s\n", word);
//	printf("the hashcode is %d\n", hash_code);
//	node* word_node = hashtable[hash_code];
//	if (word_node != NULL){
//		node seen_node_value = *word_node;
//		printf("found one %s\n", seen_node_value.word);
//		printf("the end of found\n");
//	}
//	return false;
	int hash_code = 539;
	node* blah = hashtable[hash_code];
	node blah_value = *blah;
	printf("looked at word in hashtable %s\n", blah_value.word);
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
	hashmap_t first_node_ptr = malloc(sizeof(hashmap_t));
	node first_node = *first_node_ptr;
	line_buf[strcspn(line_buf, "\n")] = 0;
	strcpy(first_node.word, line_buf);
	printf("putting word into hashtable %s\n", first_node.word);
	int hash_code = hash_function(line_buf);
	printf("the hashcode is %d\n", hash_code);
	hashtable[hash_code] = &first_node;
	while(line_size != -1){
		line_count++;
		line_size = getline(&line_buf, &line_buf_size, fp);
		printf("There is a word and it is %s\n", line_buf);
		hashmap_t new_node_ptr = malloc(sizeof(hashmap_t));
		node new_node = *new_node_ptr;
		new_node.next = NULL;
		line_buf[strcspn(line_buf, "\n")] = 0;
		strcpy(new_node.word, line_buf);
		printf("putting word into hashtable %s\n", new_node.word);
		int hash_code = hash_function(line_buf);
		printf("the hashcode is %d\n", hash_code);
		if (hashtable[hash_code] != NULL){
			printf("entered none null hashcode\n");
			break;
//			node* seen_node = hashtable[hash_code];
//			node seen_node_value = *seen_node;
//			while (seen_node_value.next != NULL){
//				seen_node_value = *seen_node_value.next;
//			}
//			seen_node_value.next = &new_node;
		}else{
			hashtable[hash_code] = &new_node;
		}
		node* blah = hashtable[hash_code];
		node blah_value = *blah;
		printf("looked at word in hashtable %s\n", blah_value.word);
	}
//	int hash_code2 = 636;
//	node* blah = hashtable[hash_code2];
//	node blah_value = *blah;
//	printf("looked at word in hashtable %s\n", blah_value.word);
	return true;
}


void print_hashtable(hashmap_t hashtable[]){
	for(int i; i<1000; i++){
		node* blah = hashtable[i];
		if (blah != NULL){
			node blah_value = *blah;
			char word = blah_value.word;
			printf("the hashcode was %d and the word is %s\n", i, blah_value.word);
			printf("the node pointer is %p\n", &blah_value);
			printf("the word pointer is %p\n", &word);
		}
	}
}