CC = gcc -ggdb 
LIBS = -pthread
CFLAGS = -Og -std=gnu11 -Wall -Wextra -Wno-unused-parameter -Wno-unused-function $(LIBS)

all: zad1 zad2 zad3 zad6 zad7 zad10

zad1: sem.o zad1.o
	$(CC) $(CFLAGS) -o $@ $^ -ldl

zad2: sem.o zad2.o
	$(CC) $(CFLAGS) -o $@ $^ -ldl

zad3: zad3.o
	$(CC) $(CFLAGS) -o $@ $^ -ldl

zad6: zad6.o
	$(CC) $(CFLAGS) -o $@ $^ -ldl

zad7: zad7.o
	$(CC) $(CFLAGS) -o $@ $^ -ldl

zad10: zad10.o
	$(CC) $(CFLAGS) -o $@ $^ -ldl

clean:
	rm -f sem zad1 zad2 zad3 zad6 zad7 zad10
	rm -f *.o
	rm -f *~
	rm -f .gdb_history

# vim: ts=8 sw=8 noe