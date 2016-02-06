CC = gcc -fno-stack-protector -m32 -c

all: KERNEL_C STRING_C
	nasm -f elf32 kernel.asm -o kasm.o
	ld -m elf_i386 -T link.ld -o kernel kasm.o kernel.o stringlib.o

KERNEL_C:
	$(CC) kernel.c -o kernel.o

STRING_C:
	$(CC) stringlib.c -o stringlib.o

run: all
	qemu-system-i386 -kernel kernel

clean:
	rm kernel *.o
