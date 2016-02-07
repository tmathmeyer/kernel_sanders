#ifndef __syscall_h__
#define __syscall_h__

extern unsigned char* keyboard_map;
typedef void *(*syscall)(void *);

int dvorak(int argc, char* argv[]);
int qwerty(int argc, char* argv[]);
int touch(int argc, char *argv[]);
int dog(int argc, char *argv[]);
int ls(int argc, char *argv[]);
int si(int argc, char *argv[]);
int sanders_exit(int argc, char* argv[]);
int sanders_sweeper(int argc, char* argv[]);
int communism(int argc, char* argv[]);

void *execute(char *exe);

#endif // __syscall_h__
