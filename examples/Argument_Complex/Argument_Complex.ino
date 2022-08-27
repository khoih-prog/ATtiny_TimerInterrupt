/****************************************************************************************************************************
  Argument_Complex.ino
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

// These define's must be placed at the beginning before #include "megaAVR_TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

// Select USING_FULL_CLOCK      == true for  20/16MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_HALF_CLOCK      == true for  10/ 8MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_250KHZ          == true for 250KHz to Timer TCBx => longer timer,  but worse  accuracy
// Not select for default 250KHz to Timer TCBx => longer timer,  but worse accuracy
#define USING_FULL_CLOCK      true
#define USING_HALF_CLOCK      false
#define USING_250KHZ          false         // Not supported now

// Try to use RTC, TCA0 or TCD0 for millis()
#define USE_TIMER_0           false         // Check if used by millis(), Servo or tone()
#define USE_TIMER_1           true          // Check if used by millis(), Servo or tone()

#if USE_TIMER_0
  #define CurrentTimer   ITimer0
#elif USE_TIMER_1
  #define CurrentTimer   ITimer1
#else
  #error You must select one Timer  
#endif

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ATtiny_TimerInterrupt.h"

#ifdef LED_BUILTIN
  #undef LED_BUILTIN

  // To modify according to your board
  // For Curiosity Nano ATtiny3217 => PIN_PA3
  #if defined(ARDUINO_AVR_CuriosityNano3217) 
    #define LED_BUILTIN   PIN_PA3
  #else
    // standard Arduino pin 13
    #define LED_BUILTIN   PIN_PA3
  #endif
#endif

// standard Serial
#define SerialDebug       Serial


struct pinStruct
{
  unsigned int Pin1;
  unsigned int Pin2;
  unsigned int Pin3;
};

volatile pinStruct myOutputPins = { LED_BUILTIN, A0, A1 };

void TimerHandler1(unsigned int outputPinsAddress)
{
  static bool toggle = false;

  //timer interrupt toggles pins
#if (TIMER_INTERRUPT_DEBUG > 1)
  SerialDebug.print("Toggle pin1 = "); SerialDebug.println( ((pinStruct *) outputPinsAddress)->Pin1 );
#endif
  
  digitalWrite(((pinStruct *) outputPinsAddress)->Pin1, toggle);

#if (TIMER_INTERRUPT_DEBUG > 1)
  SerialDebug.print("Read pin2 A0 ("); SerialDebug.print(((pinStruct *) outputPinsAddress)->Pin2 );
  SerialDebug.print(") = ");
  SerialDebug.println(digitalRead(((pinStruct *) outputPinsAddress)->Pin2) ? "HIGH" : "LOW" );                          

  SerialDebug.print("Read pin3 A1 ("); SerialDebug.print(((pinStruct *) outputPinsAddress)->Pin3 );
  SerialDebug.print(") = ");
  SerialDebug.println(digitalRead(((pinStruct *) outputPinsAddress)->Pin3) ? "HIGH" : "LOW" );  
#endif
                 
  toggle = !toggle;
}

#define TIMER1_INTERVAL_MS    1000

void setup()
{
  pinMode(myOutputPins.Pin1, OUTPUT);
  pinMode(myOutputPins.Pin2, INPUT_PULLUP);
  pinMode(myOutputPins.Pin3, INPUT_PULLUP);
    
  SerialDebug.begin(115200);
  while (!SerialDebug && millis() < 5000);

  SerialDebug.print(F("\nStarting Argument_Complex on ")); SerialDebug.println(BOARD_NAME);
  SerialDebug.println(AT_TINY_TIMER_INTERRUPT_VERSION);
  SerialDebug.print(F("CPU Frequency = ")); SerialDebug.print(F_CPU / 1000000); SerialDebug.println(F(" MHz"));

  SerialDebug.print(F("TCB Clock Frequency = ")); 

#if USING_FULL_CLOCK  
  SerialDebug.println(F("Full clock (20/16MHz, etc) for highest accuracy"));
#elif USING_HALF_CLOCK  
  SerialDebug.println(F("Half clock (10/8MHz, etc.) for high accuracy"));
#else
  SerialDebug.println(F("250KHz for lower accuracy but longer time"));
#endif

  // Timer TCB2 is used for micros(), millis(), delay(), etc and can't be used
  CurrentTimer.init();

  if (CurrentTimer.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1, (unsigned int) &myOutputPins))
  {
    SerialDebug.print(F("Starting ITimer OK, millis() = ")); SerialDebug.println(millis());
  }
  else
    SerialDebug.println(F("Can't set ITimer. Select another freq. or timer"));
}

void loop()
{
}
