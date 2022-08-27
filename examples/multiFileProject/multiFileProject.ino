/****************************************************************************************************************************
  multiFileProject.ino
  For Arduino AVR ATtiny-based boards (ATtiny3217, etc.) using megaTinyCore
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ATtiny_TimerInterrupt
  Licensed under MIT license

  Now with we can use these new 16 ISR-based timers, while consuming only 1 hwarware Timer.
  Their independently-selected, maximum interval is practically unlimited (limited only by unsigned long miliseconds)
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.
*****************************************************************************************************************************/

// Important Note: To use drag-and-drop into CURIOSITY virtual drive if you can program via Arduino IDE
// For example, check https://ww1.microchip.com/downloads/en/DeviceDoc/40002193A.pdf

#if !( defined(MEGATINYCORE) )
  #error This is designed only for MEGATINYCORE megaAVR board! Please check your Tools->Board setting
#endif

#include "multiFileProject.h"

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ATtiny_TimerInterrupt.h"

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ATtiny_ISR_Timer.h"

void setup() 
{
  // put your setup code here, to run once:
}

void loop() 
{
  // put your main code here, to run repeatedly:
}
