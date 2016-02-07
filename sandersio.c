#include "sandersio.h"
#include "screentext.h"
#include "stringlib.h"

int sanders_print(const char *string) {
    return sanders_printf(string);
}

// TODO: Remove duplicated code
int sanders_printf(const char fmt[], ...) {
	int chars = 0;
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
						char temp[MAX_ITOA_STR];
						switch(c) {
							case 'd':
							case 'i':
							case 'p':
								itoa(temp, (int) *arg);
								chars += string_len(temp);
								console_print(temp);
								break;
							case 's':
								chars += string_len((char *) *arg);
								console_print((char *) *arg);
								break;
							case 'c':
								console_writechar(*arg);
								chars++;
								break;
						}
						arg++;
					}
				}
				break;
			default:
				console_writechar(c);
				chars++;
		}
		if (c) {
			fmt++;
			c = *fmt;
		}
	}
  	return chars;
}

int sanders_nprintf(const int n, const char fmt[], ...) {
	char str[n];
	int chars = sanders_snprintf(str, n, fmt, *((&fmt)+1));
	console_print(str);
	return chars;
}

// TODO: Remove duplicated code
int sanders_sprintf(char *str, const char fmt[], ...) {
	int chars = 0;
	char *head = str;
	int *arg;
  	char c;
  	arg = (int *) &fmt ; // argument pointer
  	arg++; // ap now points to first optional argument

	c = *fmt;
	while (c) {
		switch (c) {
			case '%':
				fmt++;
				c = *fmt;
				if (arg) {
					if (c) {
						int temp_c;
						char temp[MAX_ITOA_STR];
						switch(c) {
							case 'd':
							case 'i':
							case 'p':
								itoa(temp, (int) *arg);
								temp_c = sanders_sprintf(head, temp);
								chars += temp_c;
								head += temp_c;
								break;
							case 's':
								temp_c = sanders_sprintf(head, (char *) *arg);
								chars += temp_c;
								head += temp_c;
								break;
							case 'c':
								*head = (char) *arg;
								head++;
								chars++;
								break;
						}
						arg++;
					}
				}
				break;
			default:
				*head = c;
				head++;
				chars++;
		}
		if (c) {
			fmt++;
			c = *fmt;
		}
	}

	return chars;
}

// TODO: Remove duplicated code
int sanders_snprintf(char *str, const int n, const char fmt[], ...) {
	int rem_chars = n-1;
	char *head = str;
	int *arg;
  	char c;
  	arg = (int *) &fmt ; // argument pointer
  	arg++; // ap now points to first optional argument

	c = *fmt;
	while (c && rem_chars) {
		switch (c) {
			case '%':
				fmt++;
				c = *fmt;
				if (arg) {
					if (c) {
						int chars;
						char temp[MAX_ITOA_STR];
						switch(c) {
							case 'd':
							case 'i':
							case 'p':
								itoa(temp, (int) *arg);
								chars = sanders_snprintf(head, rem_chars, temp);
								rem_chars -= chars;
								head += chars;
								break;
							case 's':
								chars = sanders_snprintf(head, rem_chars, (char *) *arg);
								rem_chars -= chars;
								head += chars;
								break;
							case 'c':
								*head = (char) *arg;
								rem_chars--;
								head++;
								break;
						}
						arg++;
					}
				}
				break;
			default:
				*head = c;
				head++;
				rem_chars--;
		}
		if (c) {
			fmt++;
			c = *fmt;
		}
	}

	return (n-rem_chars);
}




