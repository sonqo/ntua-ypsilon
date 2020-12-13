string_manipulation.out: string_manipulation.o
	gcc -g -Wall string_manipulation.o -o string_manipulation.out

string_manipulation.o: string_manipulation.s string_manipulation.c
	gcc -g -Wall -c string_manipulation.s string_manipulation.c

clean:
	rm *.o *.out
