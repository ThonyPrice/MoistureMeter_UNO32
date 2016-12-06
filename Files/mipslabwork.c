/* mipslabwork.c
   Original file written 2015 by F Lundevall
   Modified by Thony Price for course IS1500, KTH 
   Last revision: 2016-12-06          

*/

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#define TMR2PERIOD ((80000000 / 256) / 10)  // Set time-out-period to 10 ms
#if TMR2PERIOD > 0xffff                     // Fail-safe TMR2PERIOD size
#error "Timer period is too big."
#endif
int mytime = 0x5957;  // Delete?
int timeoutcount = 0; // Counter it interrupt timeouts 
volatile int* add;    // Declare variable which refers to adress
int* value;           // [Text]

/* Interrupt Service Routine */
void user_isr( void ) {
  
  add = (volatile int*) 0xbf809070;
  display_debug(add);          // Display what's read at A0
  value = *add;
  
  if (value > 260){
    PORTE = 3;
  }
  if (value < 260){
    PORTE = 0;
  }
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

      // display_image(96, icon);
      tick( &mytime );
      timeoutcount = 0;
    }
  }
  
  if (int1_flag == 128){
    // PORTE++;
    IFS(0) ^= 128;
  }
}

/* Initialize */
void labinit( void )
{ 
                                // Initialize ADC
  AD1CON1CLR= 0x00008000;       // Turn off the ADC before operating on it
  AD1PCFG   = 0x0000;           // Enable all analog inputs
  TRISASET  = 4;                // Set coresponding TRISbit (A2)

  AD1CHSSET = 0x00040000;       // Set A2 to positive analog input in MUX' for CH0SB and CH0SA 
  AD1CON1CLR= 0x00000700;       // Set output format as 16 bit integer
  AD1CON1SET= 0x000000e0;       // Chose auto-convert for SSRC
  // AD1CON3SET= 0x00000c00;       // Rate of auto-convert SAMC, 12 TAD <- Increase?
  AD1CON3SET= 0x00000300;       // Rate of auto-convert SAMC, 3 TAD 
  AD1CHSCLR = 0x00800000;       // Make sure scanning mode is off (read multiple pins)
  AD1CON2CLR= 0x0000003c;       // Set SMPI-bits to 0 -> All ADC convertions writes to BUF0
                                // Also sets convertions per interrupt to 1
  AD1CON2CLR= 0x00000001;       // Clr ALTS bit i.e. only one input source
  AD1CON3SET= 0x00008000;       // Set internal clock, ADRC, as RC ocilator clock <- Change?
  AD1CON3CLR= 0x00008000;       // Set internal clock, ADRC, as PBCLK
  AD1CON3SET= 0x00000002;       // Set ADCS to 2
  // AD1CON3SER= 0x0000000f;      // Set aquisiton period
  
  AD1CON1SET= 0x00008000;       // Turn on the ADC
  AD1CON1SET= 0x00000004;       // Enable auto sampling

                                // Inititalize timer
  T2CON   = 0x8070;             // Set prescale to 1:256
  PR2     = TMR2PERIOD;         // Initialize the timer
  TMR2    = 0;                  // Reset timer 2
  T2CONSET = 0x8000;            // Start the timer 
  
  IPC(2) = 4;                   // Interrupt priority control for TMR2
  IPC(1) = 28;                  // Set IPC1 to same priority
  IFS(0) = 0;                   // Reset all event flags 
  IEC(0) = 384;
  enable_interrupts();          // Call funtion in labwork.S

}

/* This function is called repetitively from the main program */
void labwork( void ) {  
  
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