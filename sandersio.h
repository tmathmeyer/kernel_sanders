#ifndef SANDERSIO_HEADER
#define SANDERSIO_HEADER

#define MAX_ITOA_STR 12

int sanders_print(const char *string);
int sanders_printf(const char fmt[], ...);
int sanders_nprintf(const int n, const char fmt[], ...);
int sanders_sprintf(char *str, const char fmt[], ...);
int sanders_snprintf(char *str, const int n, const char fmt[], ...);

#endif // SANDERSIO_HEADER
