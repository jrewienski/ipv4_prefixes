CFLAGS = --coverage -Wall -fprofile-arcs -ftest-coverage

test : main.o ipv4_prefix.o
		cc -o test.out $(CFLAGS) main.o ipv4_prefix.o

main.o : main.c ipv4_prefix.h
		cc $(CFLAGS) -c main.c

ipv4_prefix.o : ipv4_prefix.c ipv4_prefix.h
		cc $(CFLAGS) -c ipv4_prefix.c

coverage :
	gcov main.c ipv4_prefix.c

clean :
		rm test.out main.o ipv4_prefix.o *.gcno *.gcda *.gcov 