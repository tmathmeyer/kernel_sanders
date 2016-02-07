bits 32
section .text
        ;multiboot spec
        align 4
        dd 0x1BADB002              ;magic
        dd 0x00                    ;flags
        dd - (0x1BADB002 + 0x00)   ;checksum. m+f+c should be zero

global start
global keyboard_handler
global read_port
global write_port
global load_idt
global video_mode
global text_mode
global get_mode

extern kmain 		;this is defined in the c file
extern keyboard_handler_main

read_port:
	mov edx, [esp + 4]
			;al is the lower 8 bits of eax
	in al, dx	;dx is the lower 16 bits of edx
	ret

write_port:
	mov   edx, [esp + 4]
	mov   al, [esp + 4 + 4]
	out   dx, al
	ret

load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	sti 				;turn on interrupts
	ret

get_protected:
;	cli          ; disable interrupts
;	lgdt [gdtr]  ; load GDT register with start address of Global Descriptor Table
;	mov eax, cr0
;	or al, 1     ; set PE (Protection Enable) bit in CR0 (Control Register 0)
;	mov cr0, eax
;	; Perform far jump to selector 08h (offset into GDT, pointing at a 32bit PM code segment descriptor)
;	; to load CS with proper PM32 descriptor)
;
;	JMP 08h:realstart
;	ret
keyboard_handler:
	call    keyboard_handler_main
	iretd

video_mode:
	mov ah, 0x00
	mov al, 0x13
	int 0x10
	ret
text_mode:
	mov ah, 0x00
	mov al, 0x3
	int 0x10
	ret
get_mode:
	mov ah, 0x0F
;	mov eax, 0x0
	int 0x10
	ret
start:
;	call get_protected
realstart:
	cli 				;block interrupts
	mov esp, stack_space
	call kmain
	hlt 				;halt the CPU

section .bss
resb 8192; 8KB for stack
stack_space:
