#include "sandersio.h"
#include "screentext.h"
#include "stringlib.h"

int sanders_print(const char *string) {
	int charc = 0;
	const char *head = string;
	while(*head) {
		switch (*head) {
			case '\b':
				// screentext_backspace();
				break;
			case '\r':
				console_clear();
				break;
			default:
				console_writechar(*head);
		}
		charc++;
		head++;
	}

    return charc;
}

int sanders_printf(char fmt[], ...) {
	int *arg;
  	char c;
  	arg = (int *) &fmt ; // argument pointer
  	arg++; // ap now points to first optional argument

	c = *fmt;
	while (c) {
		switch (c) {
			case '\b':
				// screentext_backspace();
				break;
			case '\r':
				console_clear();
				break;
			case '%':
				fmt++;
				c = *fmt;
				if (arg) {
					if (c) {
						char *temp;
						switch(c) {
							case 'd':
							case 'i':
							case 'p':
								temp = itoa((int) *arg);
								sanders_print(temp);
								mm_free(temp);
								break;
							case 's':
								sanders_print((char *) *arg);
								break;
						}
					}
					arg++;
				}
				break;
			default:
				console_writechar(c);
		}
		if (c) {
			fmt++;
			c = *fmt;
		}
	}
  	return 0;
}
