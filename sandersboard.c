
#include "sandersboard.h"
#include "sanders_shell.h"

void set_keyboard_handler(void (*new_keyboard_handler)(char keycode)) {
    current_keyboard_handler = new_keyboard_handler;
}
void set_default_keyboard_handler() {
    current_keyboard_handler = shell_keyboard_handler;
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
        if (keycode < 0) {
            key_status[keycode + 128] = 0;
        } else {
            key_status[(int)keycode] = 1;
        }
        current_keyboard_handler(keycode);
    }
}

