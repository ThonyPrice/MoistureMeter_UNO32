/* mipslabwork.c
   Original file written 2015 by F Lundevall
   Modified by Thony Price for course IS1500, KTH 
   Last revision: 2016-11-25          

   Current to do:
   
    Read analog input from moisture sensor at port A1 by;
        Set the corresponding bits in the AD1PCFG register = 0
        Set the corresponding TRIS bit = 1 (input). 

*/

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#define TMR2PERIOD ((80000000 / 256) / 10)  // Set time-out-period to 10 ms
#if TMR2PERIOD > 0xffff
#error "Timer period is too big."
#endif
int timeoutcount = 0;

// This code will no longer be used(?)
int mytime = 0x5957;
volatile int* myTRISE;
volatile int* myPORTE;
// Declare variable where which later should refer to an adress
volatile int* add;

/* Interrupt Service Routine */
void user_isr( void ) {
  
  // check which flag's up
  int t2_flag   = IFS(0) & 256; // Get status of T2 flag
  int int1_flag = IFS(0) & 128; // Get status of INT1 flag  
  
  if (t2_flag == 256){ 
    IFS(0) ^= 256;         
    int btns  = getbtns();
    if (btns != 0) {
        int sw = getsw();
            
        if (btns == 1){
          mytime &= 0xff0f;
          int bitmask = (sw << 4);
          mytime |= bitmask;
        }
        if (btns == 2){
          mytime &= 0xf0ff;
          int bitmask = (sw << 8);
          mytime |= bitmask;
        }
        if (btns == 4){
          mytime &= 0x0fff;
          int bitmask = (sw << 12);
          mytime |= bitmask;
        }
    }
    
    (timeoutcount)++;
    if (timeoutcount == 10){
      // time2string( textstring, mytime );
      // display_string( 3, textstring );
      // display_update();
      add = (volatile int*) 0xbf809070;
      display_debug(add);          // Display what's read at A0
      display_image(96, icon);
      tick( &mytime );
      timeoutcount = 0;
    }
  }
  
  if (int1_flag == 128){
    (*myPORTE)++;
    IFS(0) ^= 128;
  }
}

/* Initialize */
void labinit( void )
{ 
  AD1PCFG = 0x0000;           // Enable all analog inputs
  AD1CHS  |= 0x01010000;      // Set A1 to analog MUX' in CH0SB and CH0SA 
  AD1CON1 |= 0x00000200;      // Chose output format as 16 bit signed integer
  AD1CON1 |= 0x000000e0;      // Chose auto-convert for SSRC
  AD1CON3 |= 0x00000300;      // Rate of auto-convert SAMC, 15 TAD
  AD1CON3 |= 0x00008000;      // Set internal clock, ADRC, as RC ocilator clock
  AD1CON3 |= 0x0000000f;      // Set aquisiton period
  
  AD1CON1 |= 0x00008000;      // Turn on the ADC
  AD1CON1 |= 0x00000004;      // Enable auto sampling
  
  // AD1CHS  |= 0x00030000;    // Choose A1 for analog to digital conversion(?)
  // myTRISA = (volatile int*) 0xbf886000;
  // *myTRISA |= 0x2;
  TRISA   |= 0x00000002;             // Set A1 as input instead of output (default)
  // PORTA  &= 0xffffffff;     // Set 
   
  myTRISE   = (volatile int*) 0xbf886100;
  myPORTE   = (volatile int*) 0xbf886110;
  *myTRISE &= ~0x00;
  PORTE = 0;
  
  int bitmask = (0x3f << 5);
  TRISD |= bitmask;
  
  T2CON   = 0x8070;                   // Set prescale to 1:256
  PR2     = TMR2PERIOD;               // Initialize the timer
  TMR2    = 0;                        // Reset timer 2
  T2CONSET = 0x8000;                  // Start the timer 
  
  IPC(2) = 4;                         // Interrupt priority control for TMR2
  IPC(1) = 28;                        // IPC for 
  IFS(0) = 0;                         // Reset all event flags (crude!)
  
  IEC(0) = 384;
  enable_interrupts();                // Call funtion in labwork.S

}

/* This function is called repetitively from the main program */
void labwork( void ) {  
  // add = (volatile int*) 0xbf809070;
  // display_debug(add);          // Display what's read at A0
  int foo = getsw();
  foo &= 0b0001;
  if (getsw() == 1){
    IFS(1) = 1;               
  }
  
  // prime = nextprime( prime );
  // display_string( 0, itoaconv( prime ) );
  // display_update();
  // display_image(96, icon);
}