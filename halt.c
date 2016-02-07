#include "halt.h"
#include "sandersio.h"
void halt(void){
	sanders_print("halting");
	asm("hlt");
}
