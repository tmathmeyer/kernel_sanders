#include "boopt.h"
#include "speaks.h"

 //Play sound using built in speaker
 static void play_sound(unsigned int nFrequence) {
 	unsigned int Div;
 	unsigned char tmp;
 
        //Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	outb(0x43, 0xb6);
 	outb(0x42, (unsigned char) (Div) );
 	outb(0x42, (unsigned char) (Div >> 8));
 
        //And play the sound using the PC speaker
 	tmp = inb(0x61);
  	if (tmp != (tmp | 3)) {
 		outb(0x61, tmp | 3);
 	}
 }
 
 //make it shutup
 static void nosound() {
 	unsigned char tmp = inb(0x61) & 0xFC;
 
 	outb(0x61, tmp);
 }
 
 //Make a beep
 void beep() {
 	 play_sound(1000);
// 	 timer_wait(10);
 //	 nosound();
          //set_PIT_2(old_frequency);
 }
