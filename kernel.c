#include "screentext.h"
#include "alloc.h"
#include "map.h"
#include "sanders_shell.h"
#include "syscall.h"
#include "keyboard_map.h"
#include "sandersio.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define ENTER_KEY_CODE 0x1C
#define BACKSPACE_KEY_CODE 0x0E

#define VERSION_STRING "Version 0.0.0"

unsigned char* keyboard_map;
extern void keyboard_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);

unsigned char sandersin[255];
unsigned char sandersindex = 0;

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
    //dvorak((int)0, (char**)0);
}

void keyboard_handler_main(void) {

    unsigned char status;
    char keycode;
    // write EOI
    write_port(0x20, 0x20);

    status = read_port(KEYBOARD_STATUS_PORT);
    // Lowest bit of status will be set if buffer is not empty
    if (status & 0x01) {
        keycode = read_port(KEYBOARD_DATA_PORT);
        if(keycode < 0)
            return;

        if(keycode == ENTER_KEY_CODE) {
            console_print("\n");
            sandersindex = 0;
            shell_run((char*)sandersin);
            return;
        }
        if(keycode == BACKSPACE_KEY_CODE) {
            screentext_backspace();
            sandersin[sandersindex--] = 0;
            return;
        }
        console_writechar(keyboard_map[(unsigned char) keycode]);
        sandersin[sandersindex++] = keyboard_map[(unsigned char) keycode];
    }

}


void kmain(void) {
    screentext_clear();
    idt_init();
    kb_init();
    console_init();
    console_clear();
    if (!mm_init()) {
        sanders_print("memory_checking\n");
        char *mem = mm_alloc(256);
        if (mem) {
            sanders_print("memory OK\n");

            dmap *map = map_new();
            mm_copy(mem, "map is working\n", 16);
            map_put(map, "test", mem);
            char *c = (char *)map_get(map, "test");
            sanders_print(c);
            
            sanders_printf("Welcome to Kernel Sanders, %s\n\n\n\n", VERSION_STRING);
        }
    }
    while(1);
}
