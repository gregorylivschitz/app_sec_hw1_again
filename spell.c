#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "dictionary.h"


bool is_number(const char* word);
char * strip_punc(const char * pch);
void insert_node_at_end(char * word, hashmap_t head);
void str_custom(char * temp);
void deallocate_hashtable(hashmap_t hashtable[]);

int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[])
{
	if (hashtable == NULL){
		return false;
	}
	char *line_buf = NULL;
	size_t line_buf_size = 0;
	int line_count = 0;
	ssize_t line_size;
	int misspell_count = 0;
	bool max_misspell = false;
	// The problem with this is I think we can get oom by big files?
	line_size = getline(&line_buf, &line_buf_size, fp);
	while (line_size >= 0 && misspell_count < MAX_MISSPELLED)
	{
		line_buf[strcspn(line_buf, "\n")] = 0;
		char * pch;
		char * str_delim = " ";
		pch = strtok(line_buf, str_delim);
		while (pch != NULL && misspell_count < MAX_MISSPELLED)
		{
			pch = strip_punc(pch);
			if (strlen(pch) > 0){
				bool is_word_spelled = check_word(pch, hashtable);
				if (!(is_word_spelled)){
//					char new_word[LENGTH];
					char* new_word = malloc(LENGTH);
					strncpy(new_word, pch, LENGTH);
					misspelled[misspell_count] = new_word;
					misspell_count++;
				}

			};
			pch = strtok(NULL, str_delim);
		}
		line_count++;
		line_size = getline(&line_buf, &line_buf_size, fp);
	}
	free(line_buf);
	line_buf = NULL;
	fclose(fp);
	deallocate_hashtable(hashtable);
//	printf("misspelled %d\n", count);
//	for(int i=0; i<misspell_count; i++){
//		printf("%d. %s\n", i, misspelled[i]);
//	}
//	printf("misspelled count is %d\n", misspell_count);
//	print_char_array(misspelled, misspell_count);
	return misspell_count;
}

bool check_word(const char* word, hashmap_t hashtable[]){
	if (hashtable == NULL){
		return false;
	}
	if (is_number(word)){
		return true;
	}
	int hash_code = hash_function(word);
	hashmap_t word_node_ptr = hashtable[hash_code];
	if (hash_code >= 0) {
		while (word_node_ptr != NULL) {
			if (strncmp(word_node_ptr->word, word, LENGTH) == 0) {
				return true;
			}
			word_node_ptr = word_node_ptr->next;
		}
		char lower_word[LENGTH + 1];
		strncpy(lower_word, word, LENGTH);
		str_custom(lower_word);
		int hash_code_lower = hash_function(lower_word);
		hashmap_t word_node_ptr_lower = hashtable[hash_code_lower];
		while (word_node_ptr_lower != NULL) {
			if (strncmp(word_node_ptr_lower->word, lower_word, LENGTH) == 0) {
				return true;
			}
			word_node_ptr_lower = word_node_ptr_lower->next;
		}
	}

	return false;
}

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]){
	if (hashtable == NULL){
		return false;
	}
	for (int i=0; i< HASH_SIZE; i++){
		hashtable[i] = NULL;
	}
	FILE *fp = fopen(dictionary_file, "r");
	char *line_buf = NULL;
	size_t line_buf_size = 0;
	int line_count = 0;
	ssize_t line_size;
	line_size = getline(&line_buf, &line_buf_size, fp);
	while(line_size >= 0){
		line_buf[strcspn(line_buf, "\n")] = 0;
		int hash_code = hash_function(line_buf);
		if (hashtable[hash_code] != NULL){
			insert_node_at_end(line_buf, hashtable[hash_code]);
		}else{
			hashmap_t new_node = calloc(1, sizeof(node));
			strncpy(new_node->word, line_buf, LENGTH);
			new_node->next = NULL;
			hashtable[hash_code] = new_node;
		}
		line_count++;
		line_size = getline(&line_buf, &line_buf_size, fp);
	}
	free(line_buf);
	line_buf = NULL;
	fclose(fp);
	return true;
}


void insert_node_at_end(char * word, hashmap_t head)
{
	hashmap_t temp = head;
	//Need to check to make sure we aren't placing same elements into the linkedlist back to back i.e. greg,greg
	if(strncmp(temp->word, word, LENGTH) == 0){
		return;
	}
	while(temp->next != NULL)
	{
		if(strncmp(temp->word, word, LENGTH) == 0){
			return;
		}
		temp = temp->next;
	}
	hashmap_t new_node = calloc(1, sizeof(node));
	strncpy(new_node->word, word, LENGTH);
	new_node->next = NULL;
	temp->next = new_node;
}




//void print_hashtable(hashmap_t hashtable[]){
//	printf("starting printing hashtable\n");
//	for(int i=0; i<HASH_SIZE; i++){
//		hashmap_t blah = hashtable[i];
//		while(blah != NULL){
//			printf("the hashcode is %d\n", i);
//			printf("the word is %s, next %p\n", blah->word, blah->next);
//			blah = blah->next;
//		}
//	}
//}

//void print_hashtable_one_link(hashmap_t hashtable[], int hash){
//	hashmap_t blah = hashtable[hash];
//	printf("link list %p, hash %d", blah, hash);
//	while(blah != NULL) {
//		printf("the hashcode is %d\n", hash);
//		printf("the word is %s, next %p\n", blah->word, blah->next);
//		blah = blah->next;
//	}
//}


void str_custom(char * temp) {
	char *s = temp;
	while (*s) {
		*s = tolower((unsigned char) *s);
		s++;
	}
}

void deallocate_hashtable(hashmap_t hashtable[]){
	for(int i=0; i<HASH_SIZE; i++){
		hashmap_t node = hashtable[i];
		hashmap_t temp;
		while(node != NULL){
			temp = node->next;
			free(node);
			node = temp;
		}
		hashtable[i] = NULL;
	}
}

bool is_number(const char* word){
	char *s = word;
	while (*s) {
		if (isdigit(*s) == 0){
			return false;
		}
		s++;
	}
	return true;
}


char * strip_punc(const char * pch){
	char *s = pch + strlen(pch) - 1;
	while (ispunct(*s)) {
		*s = '\0';
		s--;
	}

	s = pch;
	while (ispunct(*s)) {
		*s = '\0';
		s++;
	}

	return s;
}