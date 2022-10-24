all: ifj_string.o scanner.o
	gcc -g scanner_test.c ifj_string.o scanner.o -o st

ifj_string.o: ifj_string.c
	gcc -c ifj_string.c -o ifj_string.o

scanner.o: scanner.c
	gcc -c scanner.c -o scanner.o

clean:
	rm -rf *.o
