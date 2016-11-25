/* mipslabwork.c
   This file written 2015 by F Lundevall
   This file should be changed by YOU! So add something here:
   This file modified 2015-12-24 by Ture Teknolog 
   Latest update 2015-08-28 by F Lundevall
   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#define TMR2PERIOD ((80000000 / 256) / 10)  // Set time-out-period to 10 ms
#if TMR2PERIOD > 0xffff
#error "Timer period is too big."
#endif
int timeoutcount = 0;

int prime = 1234567;
int mytime = 0x5957;
volatile int* myTRISE;
volatile int* myPORTE;

char textstring[] = "text, more text, and even more text!";

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
      time2string( textstring, mytime );
      display_string( 3, textstring );
      display_update();
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

/* Lab-specific initialization goes here */
void labinit( void )
{
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
  
  int foo = getsw();
  foo &= 0b0001;
  if (getsw() == 1){
    IFS(1) = 1;               
  }
  
  prime = nextprime( prime );
  display_string( 0, itoaconv( prime ) );
  display_update();
  display_image(96, icon);
}