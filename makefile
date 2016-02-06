CC = gcc -fno-stack-protector -m32 -c

CFLAGS = -Wall

OBJECTS = $(wildcard *.c)

all: clear $(OBJECTS:.c=.o)
	nasm -f elf32 kernel.asm -o kasm.o
	ld -m elf_i386 -T link.ld -o kernel kasm.o $(OBJECTS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

run: all
	qemu-system-i386 -kernel kernel

fontcompiler: font.c font.h
	gcc font.c -o fontcompiler -D FONTCOMPILER -lm

clean:
	rm kernel *.o

clear:
	clear
