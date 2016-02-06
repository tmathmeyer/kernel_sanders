#include "sandersio.h"
#include "screentext.h"
#include "stringlib.h"

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

int sanders_printf(char fmt[], ...) {
	int *arg;
  	char c;
  	arg = (int *) &fmt ; // argument pointer
  	arg++; // ap now points to first optional argument

	c = *fmt;
	while (c) {
		switch (c) {
			case '\n':
				screentext_newline();
				break;
			case '\b':
				screentext_backspace();
				break;
			case '\r':
				screentext_clear();
				break;
			case '%':
				fmt++;
				c = *fmt;
				if (arg) {
					if (c) {
						char *temp;
						switch(c) {
							case 'd':
								temp = itoa(*arg);
								print(temp);
								mm_free(temp);
								break;
							case 's':
								print((char *) *arg);
								break;
						}
					}
					arg++;
				}
				break;
			default:
				screentext_writechar(c);
		}
		if (c) {
			fmt++;
			c = *fmt;
		}
	}
  	return 0;
}
