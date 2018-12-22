CC = gcc -g
CFLAGS = -Og -Wall
LDFLAGS = 
ASFLAGS = -g

all: zad3

zad3: zad3.o insert_sort.o

%.stripped: %
	objcopy --strip-all --remove-section .note.gnu.build-id $< $@


clean:
	rm -f zad3 *.stripped *.o *~

# vim: ts=8 sw=8 noet
