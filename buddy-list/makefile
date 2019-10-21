all: buddytest

buddytest: buddytest.o buddy.o
	gcc -o buddytest buddytest.o buddy.o

buddytest.o: buddytest.c buddy.h
	gcc -c -o buddytest.o buddytest.c

buddy.o: buddy.c buddy.h
	gcc -c -o buddy.o buddy.c