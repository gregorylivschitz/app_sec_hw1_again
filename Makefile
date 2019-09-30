default: prog

get-deps:
	# Assuming Debian or Ubuntu here
	sudo apt-get install -y build-essential check

dictionary.o: dictionary.c
	afl-clang -Wall -c dictionary.c dictionary.h

spell.o: spell.c
	afl-clang -Wall -c spell.c

test.o: test_main.c
	afl-clang -Wall -c test_main.c

main.o: main.c
	afl-clang -Wall -c main.c

test: dictionary.o spell.o test_main.o
	afl-clang -Wall -o test_main test_main.o spell.o dictionary.o -lcheck -lm -lrt -lpthread -lsubunit
	./test_main

prog: dictionary.o spell.o main.o
	afl-clang -Wall -o spell_check dictionary.o spell.o main.o

clean:
	rm dictionary.o spell.o main.o test_main.o check_spell.o

cleanall:clean
	rm spell_check
	
