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

/* Decide how many leds to represent value of sensor */
int getLeds(int value, int waterVal, int plantMode){
  
  float prop = ((float)value/(float)waterVal)*0.5;  
  if (plantMode == 1){
    prop = prop * 1.5;
  }
  
  if (prop > ((float)7/(float)8)){     
    return 255; }
  if (prop <= ((float)7/(float)8) && prop > ((float)6/(float)8)){
    return 127; }
  if (prop <= ((float)6/(float)8) && prop > ((float)5/(float)8)){
    return 63;  }
  if (prop <= ((float)5/(float)8) && prop > ((float)4/(float)8)){
    return 31;  }
  if (prop <= ((float)4/(float)8) && prop > ((float)3/(float)8)){
    return 15;  }
  if (prop <= ((float)3/(float)8) && prop > ((float)2/(float)8)){
    return 7;   }
  if (prop <= ((float)2/(float)8) && prop > ((float)1/(float)8)){
    return 3;   }
  if (prop <= ((float)1/(float)8)){
    return 1;   }
}
