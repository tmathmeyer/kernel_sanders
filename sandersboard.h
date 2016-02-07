
#ifndef KEYBOARD_HEADER
#define KEYBOARD_HEADER

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define ENTER_KEY_CODE 0x1C
#define BACKSPACE_KEY_CODE 0x0E
#define CTRL_KEY_CODE 0x1D
#define SHIFT_KEY_CODE 0x2A

extern unsigned char key_status[128];
extern void (*current_keyboard_handler)(char keycode);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);

void set_keyboard_handler(void (*new_keyboard_handler)(char keycode));
void set_default_keyboard_handler();
void keyboard_handler_main(void);


#endif // KEYBOARD_HEADER
