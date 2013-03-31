
main: main.c list.c cclosures.h Makefile
	gcc -o main -Wall -O2 main.c

maindebug: main.c list.c cclosures.h Makefile
	gcc -o maindebug -Wall -O0 -gdwarf-4 -g3 main.c

debug: maindebug
	gdb ./maindebug

expansion.c: main.c list.c cclosures.h
	gcc -E main.c | grep -v '^#' > expansion.c

expansion: expansion.c cclosures.h
	gcc -o expansion -Wall -O2 expansion.c

test: main
	tests/t1
	tests/t2
