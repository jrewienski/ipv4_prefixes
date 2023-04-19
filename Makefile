CFLAGS = --coverage -Wall -fprofile-arcs -ftest-coverage -g -pedantic

test : main.o ipv4_prefix.o utils.o
		cc -o test.out $(CFLAGS) main.o ipv4_prefix.o utils.o

main.o : main.c ipv4_prefix.h utils.h
		cc $(CFLAGS) -c main.c

ipv4_prefix.o : ipv4_prefix.c ipv4_prefix.h utils.h
		cc $(CFLAGS) -c ipv4_prefix.c

utils.o : utils.c utils.h
		cc $(CFLAGS) -c utils.c

coverage :
	gcov main.c ipv4_prefix.c utis.c

clean :
		rm test.out main.o ipv4_prefix.o utils.o *.gcno *.gcda *.gcov 