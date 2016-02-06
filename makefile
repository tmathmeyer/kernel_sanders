all:
	nasm -f elf32 kernel.asm -o kasm.o
	gcc -fno-stack-protector -m32 -c kernel.c -o kc.o
	ld -m elf_i386 -T link.ld -o kernel kasm.o kc.o

run: all
	qemu-system-i386 -kernel kernel

clean:
	rm kernel *.o
