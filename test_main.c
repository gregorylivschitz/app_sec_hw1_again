#include <check.h>
#include "dictionary.h"
#include <stdlib.h>

#define DICTIONARY "wordlist.txt"
#define TESTDICT "test_worlist.txt"
#define TESTDICT2 "test_dict2.txt"

START_TEST(test_dictionary_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    hashmap_t hashtable2[HASH_SIZE];
    hashmap_t hashtable3[HASH_SIZE];
    ck_assert(load_dictionary(TESTDICT, hashtable));
    ck_assert(load_dictionary(TESTDICT2, hashtable2));
    //Test that windows files work
    hashmap_t node =  hashtable2[1290];
    ck_assert(strcmp(node->word, "orthographic") == 0);
    ck_assert(strcmp((node->next->word), "abolitionism") == 0);

}
END_TEST

START_TEST(test_check_word_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* correct_word = "ethiiopian's";
    const char* punctuation_word_2 = "pl.ace";
    char * int_as_char = "155";
    char * quoted_word = "\"Justice\"";
	char * overflow_char = "“in";
    ck_assert(check_word(correct_word, hashtable));
    ck_assert(!check_word(punctuation_word_2, hashtable));
    ck_assert(!check_word(quoted_word, hashtable));
    ck_assert(check_word(int_as_char, hashtable));
	ck_assert(!check_word(overflow_char, hashtable));
}
END_TEST

START_TEST(test_check_words_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char* expected[3];
    expected[0] = "sogn";
    expected[1] = "skyn";
    expected[2] = "betta";
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("test1.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 3);
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
	hashmap_t hashtable2[HASH_SIZE];
	load_dictionary(DICTIONARY, hashtable2);
	char *misspelled2[MAX_MISSPELLED];
	FILE *fp2 = fopen("test_no_misspelled_words.txt", "r");
	int num_misspelled2 = check_words(fp2, hashtable2, misspelled2);
	ck_assert(num_misspelled2 == 0);
	hashmap_t hashtable3[HASH_SIZE];
	load_dictionary(DICTIONARY, hashtable3);
	char *misspelled3[MAX_MISSPELLED];
	FILE *fp3 = fopen("test_full_punc.txt", "r");
	int num_misspelled3 = check_words(fp3, hashtable3, misspelled3);
	ck_assert(num_misspelled3 == 0);
}
END_TEST

START_TEST(test_afl_input_overflow_lower_hashcode)
{
	//This test overflows overflows the hashcode making it negative
	hashmap_t hashtable_afl[HASH_SIZE];
	load_dictionary(DICTIONARY, hashtable_afl);
	char *misspelled_afl[MAX_MISSPELLED];
	FILE *fp_afl = fopen("afl_crashes/crash_2.txt", "r");
	int num_misspelled_afl = check_words(fp_afl, hashtable_afl, misspelled_afl);
	ck_assert(num_misspelled_afl == 1);
}
END_TEST

Suite *
check_word_suite(void)
{
    Suite * suite;
    TCase * check_word_case;
    suite = suite_create("check_word");
    check_word_case = tcase_create("Core");
    tcase_add_test(check_word_case, test_dictionary_normal);
    tcase_add_test(check_word_case, test_check_word_normal);
    tcase_add_test(check_word_case, test_check_words_normal);
	tcase_add_test(check_word_case, test_afl_input_overflow_lower_hashcode);
    suite_add_tcase(suite, check_word_case);
    return suite;
}

int
main(void)
{
    int failed;
    Suite *suite;
    SRunner *runner;
    
    suite = check_word_suite();
    runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

