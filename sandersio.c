#include "sandersio.h"
#include "screentext.h"

int print(const char *string) {
	int charc = 0;
	const char *head = string;
	while(*head) {
		switch (*head) {
			case '\n':
				screentext_newline();
				break;
			case '\b':
				screentext_backspace();
				break;
			case '\r':
				screentext_clear();
				break;
			default:
				screentext_writechar(*head);
		}
		charc++;
		head++;
	}

    return charc;
}
