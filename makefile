CC = gcc -fno-stack-protector -m32 -c

OBJECTS = $(wildcard *.c)

all: clear $(OBJECTS:.c=.o)
	nasm -f elf32 vga.asm -o vga.o
	nasm -f elf32 kernel.asm -o kasm.o
	ld -m elf_i386 -T link.ld -o sanders kasm.o vga.o $(OBJECTS:.c=.o) -static ./libm.a ##./libc.a

%.o: %.c
	$(CC) -o $@ $<

run: all
	qemu-system-i386 -kernel sanders 

fontcompiler: font.c font.h
	gcc font.c -o fontcompiler -D FONTCOMPILER -lm

clean:
	rm -f sanders *.o kernel

clear:
	clear
