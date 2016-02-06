#include "halt.h"
#include "sandersio.h"
void halt(void){
	print("halting");
	asm("hlt");
}
