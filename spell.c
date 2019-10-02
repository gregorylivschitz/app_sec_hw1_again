#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "dictionary.h"


bool is_number(const char* word);
char * strip_punc(char * pch);
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
	// The problem with this is I think we can get oom by big files?
	line_size = getline(&line_buf, &line_buf_size, fp);
	while (line_size >= 0 && misspell_count < MAX_MISSPELLED)
	{
		line_buf[strcspn(line_buf, "\n")] = 0;
		char * pch;
		char * str_delim = " ";
		pch = strtok(line_buf, str_delim);
		while (pch != NULL && (strlen(pch) > 0) && misspell_count < MAX_MISSPELLED)
		{
			char * stripped_word = strip_punc(pch);
			if (strlen(stripped_word) > 0){
				bool is_word_spelled = check_word(stripped_word, hashtable);
				if (!(is_word_spelled)){
					char* new_word = malloc(LENGTH);
					strncpy(new_word, stripped_word, LENGTH);
					misspelled[misspell_count] = new_word;
					misspell_count++;
				}

			};
			pch = strtok(NULL, str_delim);
		}
		line_count++;
		line_size = getline(&line_buf, &line_buf_size, fp);
	}
	deallocate_hashtable(hashtable);
	free(line_buf);
	line_buf = NULL;
	fclose(fp);
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
		if (hash_code_lower > 0 ){
			hashmap_t word_node_ptr_lower = hashtable[hash_code_lower];
			while (word_node_ptr_lower != NULL) {
				if (strncmp(word_node_ptr_lower->word, lower_word, LENGTH) == 0) {
					return true;
				}
				word_node_ptr_lower = word_node_ptr_lower->next;
			}
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
			hashmap_t new_node = malloc(sizeof(node));
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
	hashmap_t new_node = malloc(sizeof(node));
	strncpy(new_node->word, word, LENGTH);
	new_node->next = NULL;
	temp->next = new_node;
}


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
	const char *s = word;
	while (*s) {
		if (isdigit(*s) == 0){
			return false;
		}
		s++;
	}
	return true;
}

char * strip_punc(char * pch){

	char *s = pch + strlen(pch) - 1;
	while (ispunct(*s) && s >= pch) {

		*s = '\0';
		s--;
	}
	if (s > pch) {
		s = pch;
		bool strip_punc = ispunct(*s);
		while (strip_punc && *s != '\0') {
			//*s = '\0';
			s++;
			strip_punc = ispunct(*s);
		}
	}
	return s;
}
