/****************************************************************************************************************************
  FakeAnalogWrite.ino
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
#define USE_TIMER_0           true          // Check if used by millis(), Servo or tone()
#define USE_TIMER_1           false         // Check if used by millis(), Servo or tone()

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

// For PWM_Value from 0-255.You can change to 1024 or 2048
#define MAX_PWM_VALUE         256

#define MAPPING_TABLE_SIZE    (MAX_PWM_VALUE / 10) + 1

// You have to calibrate and update this mapping table
float mappingTable[MAPPING_TABLE_SIZE] =
{ 0.0,     3.281,   6.860,  10.886,  15.285,  20.355,  26.096,  32.732,  40.785,  50.180,
  62.557,  79.557, 104.461, 136.075, 163.066, 181.930, 195.724, 207.132, 216.228, 223.684,
  230.395, 236.136, 241.206, 245.680, 249.781, 253.509
};

#define TIMER2_FREQUENCY_HZ     10000UL
#define TIMER2_INTERVAL_US      (1000000UL / TIMER2_FREQUENCY_HZ)

volatile uint32_t startMillis = 0;

// Depending on the board, you can select NRF52 Hardware Timer from NRF_TIMER_1,NRF_TIMER_3,NRF_TIMER_4 (1,3 and 4)
// If you select the already-used NRF_TIMER_0 or NRF_TIMER_2, it'll be auto modified to use NRF_TIMER_1

#define LED_TOGGLE_INTERVAL_MS        500L

#define NUMBER_ISR_TIMERS         16

void TimerHandler()
{
  static bool toggle  = false;
  static uint32_t timeRun  = 0;

  for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
    doingSomething(i);
  }

  // Toggle LED every LED_TOGGLE_INTERVAL_MS = 500ms = 0.5s
  if (++timeRun == ((LED_TOGGLE_INTERVAL_MS * TIMER2_FREQUENCY_HZ) / 1000) )
  {   
    timeRun = 0;

    //timer interrupt toggles pin LED_BUILTIN
    digitalWrite(LED_BUILTIN, toggle);
    toggle = !toggle;
  }
}

/////////////////////////////////////////////////

typedef void (*irqCallback)();

/////////////////////////////////////////////////


typedef struct
{
  int16_t       PWM_Value;          // Writing negative value to stop and free this PWM
  uint16_t      PWM_PremapValue;    // To detect if use the same PWM_Value setting => don't do anything
  uint16_t      pin;
  int16_t       countPWM;
  bool          beingUsed;
} ISRTimerData;

// Avoid doing something fancy in ISR, for example SerialDebug.print()
// The pure simple SerialDebug.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash

///////////////////////////////////

volatile ISRTimerData curISRTimerData[NUMBER_ISR_TIMERS] =
{
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
  { 0, 0, 0, 0, false },
};

void doingSomething(int index)
{
  // First check if enabled and pin != 0
  if ( (curISRTimerData[index].beingUsed) && (curISRTimerData[index].PWM_Value > 0) && (curISRTimerData[index].pin != 0) )
  {
    // Divide the time into MAX_PWM_VALUE slots.
    // PWM_Value = 0 => no digitalWrite(pin, 1)
    // PWM_Value > 0 => digitalWrite(pin, 1) from countPWM = 0 to countPWM = PWM_Value

    if (curISRTimerData[index].countPWM == 0)
    {
      if (curISRTimerData[index].PWM_Value > 0)
        digitalWrite(curISRTimerData[index].pin, 1);
      else
        digitalWrite(curISRTimerData[index].pin, 0);
    }
    else if (curISRTimerData[index].countPWM == curISRTimerData[index].PWM_Value)
    {
      digitalWrite(curISRTimerData[index].pin, 0);
    }
  }

  curISRTimerData[index].countPWM = (curISRTimerData[index].countPWM + 1) % MAX_PWM_VALUE;
}

///////////////////////////////////////////

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  SerialDebug.begin(115200);
  while (!SerialDebug && millis() < 5000);

  SerialDebug.print(F("\nStarting FakeAnalogWrite on ")); SerialDebug.println(BOARD_NAME);
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

  CurrentTimer.init();

  //if (CurrentTimer.attachInterruptInterval(TIMER2_INTERVAL_MS, TimerHandler))
  if (CurrentTimer.attachInterrupt(TIMER2_FREQUENCY_HZ, TimerHandler))
  {
    SerialDebug.print(F("Starting ITimer OK, millis() = ")); SerialDebug.println(millis());
  }
  else
    SerialDebug.println(F("Can't set ITimer. Select another freq. or timer"));
    
  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
    curISRTimerData[i].beingUsed      = false;
    curISRTimerData[i].pin            = 0;
    curISRTimerData[i].PWM_Value      = 0;

    //ISR_Timer.setInterval(curISRTimerData[i].TimerInterval, curISRTimerData[i].irqCallbackFunc);
  }
}

#define USING_MAPPING_TABLE   false

void fakeAnalogWrite(uint16_t pin, uint16_t value)
{
  uint16_t localValue;

#if USING_MAPPING_TABLE  
  uint16_t localIndex = 0;
#endif

  // First check if already got that pin, then just update the PWM_Value
  for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
    if ( (curISRTimerData[i].beingUsed) && (curISRTimerData[i].pin == pin) )
    {
      localValue = (value < MAX_PWM_VALUE) ? value : MAX_PWM_VALUE;

      if (curISRTimerData[i].PWM_PremapValue == localValue)
      {
#if (LOCAL_DEBUG > 0)
        SerialDebug.print(F("Ignore : Same Value for index = ")); SerialDebug.println(i);
#endif

        return;
      }
      else if (curISRTimerData[i].PWM_Value >= 0)
      {
        curISRTimerData[i].PWM_PremapValue = localValue;

        // Mapping to corect value
        if ( ( localValue == 0) || ( localValue == MAX_PWM_VALUE - 1) )
        {
          // Keep MAX_PWM_VALUE
          curISRTimerData[i].PWM_Value = localValue;
        }
        else
        {

#if USING_MAPPING_TABLE

          // Get the mapping index
          for (uint16_t j = 0; j < MAPPING_TABLE_SIZE; j++)
          {
            if ( (float) localValue < mappingTable[j])
            {
              localIndex = j - 1;
              break;
            }
          }

#if (LOCAL_DEBUG > 1)
          SerialDebug.print(F("localIndex = ")); SerialDebug.println(localIndex);
#endif

          // Can use map() function
          // Can use map() function
          curISRTimerData[i].PWM_Value = (uint16_t) ( (localIndex * 10 ) +
                                         ( (localValue - mappingTable[localIndex]) * 10 ) /  (mappingTable[localIndex + 1] - mappingTable[localIndex]) );

#else
          curISRTimerData[i].PWM_Value = localValue;
#endif

#if (LOCAL_DEBUG > 0)
          SerialDebug.print(F("Update index = ")); SerialDebug.print(i);
          SerialDebug.print(F(", pin = ")); SerialDebug.print(pin);
          SerialDebug.print(F(", input PWM_Value = ")); SerialDebug.print(value);
          SerialDebug.print(F(", mapped PWM_Value = ")); SerialDebug.println(curISRTimerData[i].PWM_Value);
#endif
        }
      }
      else
      {
        curISRTimerData[i].beingUsed      = false;
        curISRTimerData[i].pin            = 0;
        curISRTimerData[i].PWM_Value      = 0;
      }

      // Reset countPWM
      curISRTimerData[i].countPWM = 0;

      return;
    }
  }

  for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
    if (!curISRTimerData[i].beingUsed)
    {
      curISRTimerData[i].beingUsed    = true;
      curISRTimerData[i].pin          = pin;

      // Mapping to corect value
      localValue = (value < MAX_PWM_VALUE) ? value : MAX_PWM_VALUE;

      if ( ( localValue == 0) || ( localValue == MAX_PWM_VALUE - 1) )
      {
        // Keep MAX_PWM_VALUE
        curISRTimerData[i].PWM_Value = localValue;
      }
      else
      {
        curISRTimerData[i].PWM_PremapValue = localValue;

#if USING_MAPPING_TABLE

        // Get the mapping index
        for (uint16_t j = 0; j < MAPPING_TABLE_SIZE; j++)
        {
          if ( (float) localValue < mappingTable[j])
          {
            localIndex = j - 1;
            break;
          }
        }

#if (LOCAL_DEBUG > 1)
        SerialDebug.print(F("localIndex = ")); SerialDebug.println(localIndex);
#endif

        // Can use map() function
        // Can use map() function
        curISRTimerData[i].PWM_Value = (uint16_t) ( (localIndex * 10 ) +
                                       ( (localValue - mappingTable[localIndex]) * 10 ) /  (mappingTable[localIndex + 1] - mappingTable[localIndex]) );
#else
        curISRTimerData[i].PWM_Value = localValue;
#endif
      }

      curISRTimerData[i].countPWM     = 0;

      pinMode(pin, OUTPUT);

#if (LOCAL_DEBUG > 0)
      SerialDebug.print(F("Add index = ")); SerialDebug.print(i);
      SerialDebug.print(F(", pin = ")); SerialDebug.print(pin);
      SerialDebug.print(F(", input PWM_Value = ")); SerialDebug.print(value);
      SerialDebug.print(F(", mapped PWM_Value = ")); SerialDebug.println(curISRTimerData[i].PWM_Value);
#endif

      return;
    }
  }
}

#define DELAY_BETWEEN_CHANGE_MS     5000L
#define REPEAT_INTERVAL_MS          10000L

#define DIVIDER                     5

void loop()
{
  for (uint16_t i = 0; i <= MAX_PWM_VALUE / DIVIDER; i++)
  {
    // Change the pin according to board
    // UNI, Nano, etc can use pins from 2-12. Pin 13 is used for LED_BUILTIN
    // Mega can use many more pins, such as 22-53

    fakeAnalogWrite( 2, i * DIVIDER);
    fakeAnalogWrite( 3, i * DIVIDER);
    fakeAnalogWrite( 4, i * DIVIDER);
    fakeAnalogWrite( 5, i * DIVIDER);
    fakeAnalogWrite( 6, i * DIVIDER);
    fakeAnalogWrite( 7, i * DIVIDER);
    fakeAnalogWrite( 8, i * DIVIDER);
    fakeAnalogWrite( 9, i * DIVIDER);
    fakeAnalogWrite(10, i * DIVIDER);
    fakeAnalogWrite(11, i * DIVIDER);
    fakeAnalogWrite(12, i * DIVIDER);
    fakeAnalogWrite(A0, i * DIVIDER);
    fakeAnalogWrite(A1, i * DIVIDER);
    fakeAnalogWrite(A2, i * DIVIDER);
    fakeAnalogWrite(A3, i * DIVIDER);
    fakeAnalogWrite(A4, i * DIVIDER);

#if (LOCAL_DEBUG > 0)
    SerialDebug.print(F("Test PWM_Value = ")); SerialDebug.print(i * DIVIDER);
    SerialDebug.print(F(", max = ")); SerialDebug.println(MAX_PWM_VALUE - 1);
#endif

    delay(DELAY_BETWEEN_CHANGE_MS);
  }

  SerialDebug.println(F("==================="));
  delay(REPEAT_INTERVAL_MS);
}
