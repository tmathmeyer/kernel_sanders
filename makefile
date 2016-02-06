CC = gcc -fno-stack-protector -m32 -c

CFLAGS = -Wall

OBJECTS = $(wildcard *.c)

all: $(OBJECTS:.c=.o)
	nasm -f elf32 kernel.asm -o kasm.o
	ld -m elf_i386 -T link.ld -o kernel kasm.o $(OBJECTS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

run: all
	qemu-system-i386 -kernel kernel

clean:
	rm kernel *.o
