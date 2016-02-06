CC = gcc -fno-stack-protector -m32 -c

CFLAGS = -Wall

OBJECTS = kernel.o stringlib.o


all: $(OBJECTS)
	nasm -f elf32 kernel.asm -o kasm.o
	ld -m elf_i386 -T link.ld -o kernel kasm.o kernel.o stringlib.o

%.o: %.c
	$(CC) $(CFLAGS) -o $(notdir $@) $<

run: all
	qemu-system-i386 -kernel kernel

clean:
	rm kernel *.o
