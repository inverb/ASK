CC = gcc -g
CFLAGS = -Og -Wall
LDFLAGS = 
ASFLAGS = -g

all: zad4

zad4: zad4.o fibonacci.o

%.stripped: %
	objcopy --strip-all --remove-section .note.gnu.build-id $< $@


clean:
	rm -f zad4 *.stripped *.o *~

# vim: ts=8 sw=8 noet
