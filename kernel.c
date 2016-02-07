#include "screentext.h"
#include "alloc.h"
#include "map.h"
#include "sanders_shell.h"
#include "syscall.h"
#include "sandersio.h"
#include "stringlib.h"
#include "video.h"
#include "sandersboard.h"
#include "screentext.h"


#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

#define VERSION_STRING "Version 0.0.0"

void (*current_keyboard_handler)(char keycode);

unsigned char* keyboard_map;
unsigned char key_status[128] = {0};
extern void keyboard_handler(void);
extern void load_idt(unsigned long *idt_ptr);
unsigned char sandersin[255];
unsigned char sandersindex = 0;
dmap *syscall_map;

dmap *root_fs;

struct IDT_entry {
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];

void idt_init(void) {
    unsigned long keyboard_address;
    unsigned long idt_address;
    unsigned long idt_ptr[2];
    keyboard_address = (unsigned long)keyboard_handler;
    IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
    IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = INTERRUPT_GATE;
    IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;
    write_port(0x20 , 0x11);
    write_port(0xA0 , 0x11);
    write_port(0x21 , 0x20);
    write_port(0xA1 , 0x28);
    write_port(0x21 , 0x00);
    write_port(0xA1 , 0x00);
    write_port(0x21 , 0x01);
    write_port(0xA1 , 0x01);
    write_port(0x21 , 0xff);
    write_port(0xA1 , 0xff);
    idt_address = (unsigned long)IDT ;
    idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16 ;
    load_idt(idt_ptr);
}

void kb_init(void) {
    write_port(0x21 , 0xFD); /* enable keyboard */
    qwerty((int)0,(char**)0);
}

int systemcheck() {
    if (mm_init()) { // mm_init has failed
        return 0;
    }
    console_print("initializing system check...\n");

    console_print("    memory... ");
    char *mem = mm_alloc(128);
    if (mem) {
        console_print("OK\n");
        mm_free(mem);
    } else {
        console_print("BAD\n");
        return 0;
    }

    console_print("    filesystem... ");
    if (fs_init()) {
        console_print("NOT OK\n");
    } else {
        console_print("OK\n");
    }
    

    console_print("completed system check\n");
    return 1;
}

void kmain(void) {
    screentext_clear();
    idt_init();
    kb_init();
    console_init();
    console_clear();
	int i = 2;
    if (systemcheck()) {
        console_print("Welcome to Kernel Sanders\n\n\n\n");
        set_default_keyboard_handler(&shell_keyboard_handler);
        console_print("> ");
        while(1);
    }
}

#define SYSTEM(name) process(root(), #name, name)

int fs_init() {
    root_fs = map_new();
    SYSTEM(dvorak);
    SYSTEM(qwerty);
    SYSTEM(videorun);
    SYSTEM(touch);
    SYSTEM(ls);
    SYSTEM(si);
    SYSTEM(dog);
    SYSTEM(sanderssweeper);
    return 0;
}

dmap *root() {
    return root_fs;
}
