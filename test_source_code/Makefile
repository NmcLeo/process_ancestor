all: program1 program2

program1: array_state_test.c
	gcc -std=c99 -D _GNU_SOURCE -static array_state_test.c -o array_state_test
program2: process_ancestor_test.c
	gcc -std=c99 -D _GNU_SOURCE -static process_ancestor_test.c -o process_ancestor_test