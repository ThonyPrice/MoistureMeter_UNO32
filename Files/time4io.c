/* mipslabwork.c
   Author: Thony Price for course IS1500, KTH 
   Last revision: 2016-12-07          

*/
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

/***** Return value of which switch is up *****/
int getsw(void){
    
  int retval = ((PORTD & (0xf << 8)) >> 8);
  return retval;
}

/***** Return value of which button is pressed *****/
int getbtns(void){
  
  int retval = ((PORTD & (0b111 << 5)) >> 5);
  return retval;
}

/***** Decide how many leds to represent value of sensor *****/
/***** Raise flag needs watering if below critical value *****/
int getLeds(int value, int inAirVal, int waterVal, int plantMode){
  // Calucate proportion between current value and possible values
  int span_in_range = inAirVal - waterVal;
  int measuredValue = value - waterVal;
  float prop = ((float)measuredValue/(float)span_in_range);  
  // Adjust for plantMode (cactuses doesn't need as much water)
  if (plantMode == 1){
    prop = prop * 0.7;
  }
  // Raise flag if below a critical pecentage (plantMode dependent)
  if (prop < (float)6/(float)8 && plantMode == 1){
    IFS(1) = 1;
  } if (prop < (float)6/(float)8 && plantMode == 2){
    IFS(1) = 1;
  }
  // Return amount of LEDs to light up by register PORTE 
  // Note that high proportions return low values because 
  // the higher value, the more dry the soil is
  if (prop > ((float)7/(float)8)){     
    return 1;   }
  if (prop <= ((float)7/(float)8) && prop > ((float)6/(float)8)){
    return 3;   }
  if (prop <= ((float)6/(float)8) && prop > ((float)5/(float)8)){
    return 7;   }
  if (prop <= ((float)5/(float)8) && prop > ((float)4/(float)8)){
    return 15;  }
  if (prop <= ((float)4/(float)8) && prop > ((float)3/(float)8)){
    return 31;  }
  if (prop <= ((float)3/(float)8) && prop > ((float)2/(float)8)){
    return 63;  }
  if (prop <= ((float)2/(float)8) && prop > ((float)1/(float)8)){
    return 127; }
  if (prop <= ((float)1/(float)8)){
    return 255; }
}
