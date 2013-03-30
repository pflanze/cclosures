main: main.c cclosures.h
	gcc -o main -Wall -O2 main.c

expansion.c: main.c cclosures.h
	gcc -E main.c | grep -v '^#' > expansion.c

expansion: expansion.c cclosures.h
	gcc -o expansion -Wall -O2 expansion.c

test: main
	tests/t1

