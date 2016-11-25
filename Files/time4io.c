// Comments abput creator

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int getsw(void){
    
  int retval = ((PORTD & (0xf << 8)) >> 8);
  return retval;
}

int getbtns(void){
  
  int retval = ((PORTD & (0b111 << 5)) >> 5);
  return retval;
}