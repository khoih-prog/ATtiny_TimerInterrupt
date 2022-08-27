## ATtiny_TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/ATtiny_TimerInterrupt.svg?)](https://www.ardu-badge.com/ATtiny_TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ATtiny_TimerInterrupt.svg)](https://github.com/khoih-prog/ATtiny_TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ATtiny_TimerInterrupt/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ATtiny_TimerInterrupt.svg)](http://github.com/khoih-prog/ATtiny_TimerInterrupt/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>


---
---

## Table of Contents

* [Important Note for Arduino IDE](#Important-Note-for-Arduino-IDE)
* [Why do we need this ATtiny_TimerInterrupt library](#why-do-we-need-this-ATtiny_TimerInterrupt-library)
  * [Features](#features)
  * [Why using ISR-based Hardware Timer Interrupt is better](#why-using-isr-based-hardware-timer-interrupt-is-better)
  * [Important Notes about ISR](#important-notes-about-isr)
  * [Currently supported Boards](#currently-supported-boards)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)
* [More useful Information](#more-useful-information)
  * [1. Documents](#1-documents)
  * [2. Timer TCB0-TCB1](#2-timer-tcb0-tcb1)
* [Usage](#usage)
  * [1. Using only Hardware Timer directly](#1-using-only-hardware-timer-directly)
    * [1.1 Init Hardware Timer](#11-init-hardware-timer)
    * [1.2 Set Hardware Timer Interval and attach Timer Interrupt Handler function](#12-set-hardware-timer-interval-and-attach-timer-interrupt-handler-function)
    * [1.3 Set Hardware Timer Frequency and attach Timer Interrupt Handler function](#13-set-hardware-timer-frequency-and-attach-timer-interrupt-handler-function)
  * [2. Using 16 ISR_based Timers from 1 Hardware Timer](#2-using-16-isr_based-timers-from-1-hardware-timer)
    * [2.1 Important Note](#21-important-note)
    * [2.2 Init Hardware Timer and ISR-based Timer](#22-init-hardware-timer-and-isr-based-timer)
    * [2.3 Set Hardware Timer Interval and attach Timer Interrupt Handler functions](#23-set-hardware-timer-interval-and-attach-timer-interrupt-handler-functions)
* [Examples](#examples)
  * [  1. Argument_Complex](examples/Argument_Complex)
  * [  2. Argument_None](examples/Argument_None)
  * [  3. Argument_Simple](examples/Argument_Simple)
  * [  4. Change_Interval](examples/Change_Interval)
  * [  5. FakeAnalogWrite](examples/FakeAnalogWrite)
  * [  6. **ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex).
  * [  7. ISR_RPM_Measure](examples/ISR_RPM_Measure)
  * [  8. **Change_Interval_HF**](examples/Change_Interval_HF)
  * [  9. **ISR_Timers_Array_Simple**](examples/ISR_Timers_Array_Simple).
  * [ 10. RPM_Measure](examples/RPM_Measure)
  * [ 11. SwitchDebounce](examples/SwitchDebounce)
  * [ 12. TimerDuration](examples/TimerDuration)
  * [ 13. TimerInterruptTest](examples/TimerInterruptTest)
  * [ 14. **multiFileProject**](examples/multiFileProject) **New**
* [Example ISR_16_Timers_Array_Complex](#example-isr_16_timers_array_complex)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. ISR_16_Timers_Array_Complex on AVR_CuriosityNano3217](#1-isr_16_timers_array_complex-on-AVR_CuriosityNano3217)
  * [2. ISR_16_Timers_Array_Complex on AVR_CuriosityNano3217 to show accuracy difference](#2-isr_16_timers_array_complex-on-AVR_CuriosityNano3217-to-show-accuracy-difference)
    * [2.1 TCB Clock Frequency Full clock for highest accuracy](#21-tcb-clock-frequency-full-clock-for-highest-accuracy)
    * [2.2 TCB Clock Frequency Half clock for high accuracy](#22-tcb-clock-frequency-half-clock-for-high-accuracy)
  * [3. Change_Interval on AVR_CuriosityNano3217](#3-change_interval_HF-on-AVR_CuriosityNano3217)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---

### Important Note for Arduino IDE

With some Arduino IDE versions, such as v1.8.19, upload directly via USB to some boards, such as `AVR_CuriosityNano3217` can't be done without unknown-to-me fix. We'll get the following error when uploading

```
avrdude: Version 6.3-20201216
         Copyright (c) 2000-2005 Brian Dean, http://www.bdmicro.com/
         Copyright (c) 2007-2014 Joerg Wunsch

         System wide configuration file is "/home/kh/.arduino15/packages/megaTinyCore/hardware/megaavr/2.5.11/avrdude.conf"
         User configuration file is "/home/kh/.avrduderc"
         User configuration file does not exist or is not a regular file, skipping

         Using Port                    : usb
         Using Programmer              : curiosity_updi
avrdude: usbdev_open(): Found nEDBG CMSIS-DAP, serno: MCHP3333021800000998
avrdude: usbdev_open(): WARNING: failed to set configuration 1: Device or resource busy
avrdude: Found CMSIS-DAP compliant device, using EDBG protocol
avrdude: usbdev_send(): wrote -5 out of 912 bytes, err = Input/output error
avrdude: jtag3_edbg_prepare(): failed to send command to serial port

avrdude done.  Thank you.

the selected serial port 
 does not exist or your board is not connected
```

We can use drag-and-drop method to `drag-and-drop` the compiled **hex** file to `CURIOSITY` virtual drive. 

If `success`, The LED blinks **slowly** for 2 sec. The LED will blinks **rapidly** for 2 sec if `failure`


For example, to run [Change_Interval example](https://github.com/khoih-prog/ATtiny_TimerInterrupt/tree/main/examples/Change_Interval), use Arduino IDE to compile, and get the `Change_Interval.ino.hex` file. For Ubuntu Linux, the file is stored in directory `/tmp/arduino_build_xxxxxx`


<p align="center">
    <img src="https://github.com/khoih-prog/ATtiny_TimerInterrupt/blob/main/pics/Change_Interval.png">
</p>


After drag-and-drop the `Change_Interval.ino.hex` into `CURIOSITY` virtual drive, the code will run immidiately if successfully loaded (LED blinks **slowly**)


<p align="center">
    <img src="https://github.com/khoih-prog/ATtiny_TimerInterrupt/blob/main/pics/CURIOSITY_drive.png">
</p>


---
---

### Why do we need this [ATtiny_TimerInterrupt library](https://github.com/khoih-prog/ATtiny_TimerInterrupt)

### Features

This library enables you to use Interrupt from Hardware Timers on Arduino AVR ATtiny-based boards (ATtiny3217, etc.) using megaTinyCore

As **Hardware Timers are rare, and very precious assets** of any board, this library now enables you to use up to **16 ISR-based Timers, while consuming only 1 Hardware Timer**. Timers' interval is very long (**ulong millisecs**).

Now with these new **16 ISR-based timers**, the maximum interval is **practically unlimited** (limited only by unsigned long miliseconds) while **the accuracy is nearly perfect** compared to software timers. 

The most important feature is they're ISR-based timers. Therefore, their executions are **not blocked by bad-behaving functions / tasks**. This important feature is absolutely necessary for mission-critical tasks. 

The [**ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex) example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.

Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

### Why using ISR-based Hardware Timer Interrupt is better

Imagine you have a system with a **mission-critical function**, measuring water level and control the sump pump or doing something much more important. You normally use a **software timer to poll**, or even place the function in loop(). But what if another function is blocking the loop() or setup().

**So your function might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a **Hardware Timer with Interrupt** to call your function.

**These hardware timers, using interrupt**, still work even if other functions are blocking. Moreover, they are **much more precise** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules. More to read on:

[**HOWTO Attach Interrupt**](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

### Important Notes about ISR

1. Inside the attached function, delay() won’t work and the value returned by millis() will not increment. Serial data received while in the function may be lost. You should declare as volatile any variables that you modify within the attached function.

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.

### Currently supported Boards

- **tinyAVR boards using megaTinyCore**

<p align="center">
    <img src="https://github.com/khoih-prog/ATtiny_TimerInterrupt/blob/main/pics/Curiosity_ATtiny3217.png">
</p>

---
---


## Prerequisites

1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
2. [`SpenceKonde megaTinyCore core 2.5.11+`](https://github.com/SpenceKonde/megaTinyCore) for Arduino ATtiny boards.  [![GitHub release](https://img.shields.io/github/release/SpenceKonde/megaTinyCore.svg)](https://github.com/SpenceKonde/megaTinyCore/releases/latest). Follow [**megaTinyCore Installation**](https://github.com/SpenceKonde/megaTinyCore/blob/master/Installation.md).
3. To use with certain example
   - [`SimpleTimer library`](https://github.com/jfturcot/SimpleTimer) for [ISR_Timers_Array_Simple](examples/ISR_Timers_Array_Simple) and [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex) examples.
   
---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**ATtiny_TimerInterrupt**](https://github.com/khoih-prog/ATtiny_TimerInterrupt), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/ATtiny_TimerInterrupt.svg?)](https://www.ardu-badge.com/ATtiny_TimerInterrupt) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**ATtiny_TimerInterrupt**](https://github.com/khoih-prog/ATtiny_TimerInterrupt) page.
2. Download the latest release `ATtiny_TimerInterrupt-main.zip`.
3. Extract the zip file to `ATtiny_TimerInterrupt-main` directory 
4. Copy whole `ATtiny_TimerInterrupt-main` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO:

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**ATtiny_TimerInterrupt** library](https://registry.platformio.org/libraries/khoih-prog/ATtiny_TimerInterrupt) by using [Library Manager](https://registry.platformio.org/libraries/khoih-prog/ATtiny_TimerInterrupt/installation). Search for ATtiny_TimerInterrupt in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---

### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using `xyz-Impl.h` instead of standard `xyz.cpp`, possibly creates certain `Multiple Definitions` Linker error in certain use cases.

You can include these `.hpp` files

```
// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "ATtiny_TimerInterrupt.hpp"   //https://github.com/khoih-prog/ATtiny_TimerInterrupt

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "ATtiny_ISR_Timer.hpp"        //https://github.com/khoih-prog/ATtiny_TimerInterrupt
```

in many files. But be sure to use the following `.h` files **in just 1 `.h`, `.cpp` or `.ino` file**, which must **not be included in any other file**, to avoid `Multiple Definitions` Linker Error

```
// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ATtiny_TimerInterrupt.h"     //https://github.com/khoih-prog/ATtiny_TimerInterrupt

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ATtiny_ISR_Timer.h"          //https://github.com/khoih-prog/ATtiny_TimerInterrupt
```

Check the new [**multiFileProject** example](examples/multiFileProject) for a `HOWTO` demo.

---
---

## More useful Information

### 1. Documents

1. [Arduino 101: Timers and Interrupts](https://www.robotshop.com/community/forum/t/arduino-101-timers-and-interrupts/13072)
2. [Getting Started with Timer/Counter Type B (TCB)](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ApplicationNotes/ApplicationNotes/TB3214-Getting-Started-with-TCB-DS90003214.pdf)
3. [megaTinyCore README.md](https://github.com/SpenceKonde/megaTinyCore/blob/master/README.md)
4. [ATtiny3217 Curiosity Nano Hardware User Guide](https://ww1.microchip.com/downloads/en/DeviceDoc/40002193A.pdf)
5. [AVR128DA48-Curiosity-Nano-Hardware-User Guide](https://ww1.microchip.com/downloads/en/DeviceDoc/AVR128DA48-Curiosity-Nano-UG-DS50002971B.pdf)
6. [AVR128DB48-Curiosity-Nano-Hardware-User Guide](https://ww1.microchip.com/downloads/en/DeviceDoc/AVR128DB48-Curiosity-Nano-HW-UserG-DS50003037A.pdf)




### 2. Timer TCB0-TCB1

TCB0-TCB1 are 16-bit timers

The ATtiny boards, such as `ATtiny3217`, `ATtiny1617`, will have only maximum 2 TCB timers, (TCB0-TCB1).

The ATtiny boards, such as `ATtiny817`, will have only maximum 1 TCB timer, (TCB0).

The number of TCB timers will be automatically configured by the library.

---
---

## Usage

Before using any Timer, you have to make sure the Timer has not been used by any other purpose.

### 1. Using only Hardware Timer directly

### 1.1 Init Hardware Timer

```
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

// Init timer CurrentTimer
CurrentTimer.init();
```

### 1.2 Set Hardware Timer Interval and attach Timer Interrupt Handler function

Use one of these functions with **interval in unsigned long milliseconds**

```
// interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
template<typename TArg> bool setInterval(unsigned long interval, void (*callback)(TArg), TArg params, unsigned long duration = 0);

// interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
bool setInterval(unsigned long interval, timer_callback callback, unsigned long duration = 0);

// Interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
template<typename TArg> bool attachInterruptInterval(unsigned long interval, void (*callback)(TArg), TArg params, unsigned long duration = 0);

// Interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
bool attachInterruptInterval(unsigned long interval, timer_callback callback, unsigned long duration = 0)
```

as follows

```
void TimerHandler1()
{
  // Doing something here inside ISR
}

#define TIMER1_INTERVAL_MS        50L

void setup()
{
  ....
  
  // Interval in unsigned long millisecs
    
  ITimer1.init();

  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1))
  {
    SerialDebug.print(F("Starting ITimer OK, millis() = ")); SerialDebug.println(millis());
  }
  else
    SerialDebug.println(F("Can't set ITimer. Select another freq. or timer"));  
}  
```

### 1.3 Set Hardware Timer Frequency and attach Timer Interrupt Handler function

Use one of these functions with **frequency in float Hz**

```
// frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
bool setFrequency(float frequency, timer_callback_p callback, /* void* */ uint32_t params, unsigned long duration = 0);

// frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
bool setFrequency(float frequency, timer_callback callback, unsigned long duration = 0);

// frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
template<typename TArg> bool attachInterrupt(float frequency, void (*callback)(TArg), TArg params, unsigned long duration = 0);

// frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
bool attachInterrupt(float frequency, timer_callback callback, unsigned long duration = 0);
```

as follows

```
void TimerHandler1()
{
  // Doing something here inside ISR
}

#define TIMER1_FREQ_HZ        5555.555

void setup()
{
  ....
  
  // Frequency in float Hz
  if (ITimer1.attachInterrupt(TIMER1_FREQ_HZ, TimerHandler1))
  {
    SerialDebug.print(F("Starting ITimer OK, millis() = ")); SerialDebug.println(millis());
  }
  else
    SerialDebug.println("Can't set ITimer. Select another freq. or timer");
}  
```


### 2. Using 16 ISR_based Timers from 1 Hardware Timer

### 2.1 Important Note

The 16 ISR_based Timers, designed for long timer intervals, only support using **unsigned long millisec intervals**. If you have to use much higher frequency or sub-millisecond interval, you have to use the Hardware Timers directly as in [1.3 Set Hardware Timer Frequency and attach Timer Interrupt Handler function](#13-set-hardware-timer-frequency-and-attach-timer-interrupt-handler-function)

### 2.2 Init Hardware Timer and ISR-based Timer

```
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

// Init ISR_Timer
// Each ISR_Timer can service 16 different ISR-based timers
ISR_Timer ISR_Timer1;
```

### 2.3 Set Hardware Timer Interval and attach Timer Interrupt Handler functions

```
void TimerHandler()
{
  ISR_Timer1.run();
}

#define HW_TIMER_INTERVAL_MS          50L

#define TIMER_INTERVAL_2S             2000L
#define TIMER_INTERVAL_5S             5000L
#define TIMER_INTERVAL_11S            11000L
#define TIMER_INTERVAL_101S           101000L

// In AVR, avoid doing something fancy in ISR, for example complex SerialDebug.print with String() argument
// The pure simple SerialDebug.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething2s()
{
  // Doing something here inside ISR every 2 seconds
}
  
void doingSomething5s()
{
  // Doing something here inside ISR every 5 seconds
}

void doingSomething11s()
{
  // Doing something here inside ISR  every 11 seconds
}

void doingSomething101s()
{
  // Doing something here inside ISR every 101 seconds
}

void setup()
{
  ....
  
  // Timer TCB2 is used for micros(), millis(), delay(), etc and can't be used
  CurrentTimer.init();
  
  // Interval in millisecs
  if (CurrentTimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS, TimerHandler))
  {
    lastMillis = millis();
    SerialDebug.print(F("Starting ITimer OK, millis() = ")); SerialDebug.println(millis());
  }
  else
    SerialDebug.println(F("Can't set ITimer correctly. Select another freq. or interval"));

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  ISR_Timer1.setInterval(TIMER_INTERVAL_2S, doingSomething2s);
  ISR_Timer1.setInterval(TIMER_INTERVAL_5S, doingSomething5s);
  ISR_Timer1.setInterval(TIMER_INTERVAL_11S, doingSomething11s);
  ISR_Timer1.setInterval(TIMER_INTERVAL_101S, doingSomething101s);
}  
```

---
---

### Examples: 

 1. [Argument_Complex](examples/Argument_Complex)
 2. [Argument_None](examples/Argument_None)
 3. [Argument_Simple](examples/Argument_Simple)
 4. [Change_Interval](examples/Change_Interval).
 5. [FakeAnalogWrite](examples/FakeAnalogWrite).
 6. [**ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex).
 7. [ISR_RPM_Measure](examples/ISR_RPM_Measure)
 8. [**Change_Interval_HF**](examples/Change_Interval_HF)
 9. [**ISR_Timers_Array_Simple**](examples/ISR_Timers_Array_Simple).
10. [RPM_Measure](examples/RPM_Measure)
11. [SwitchDebounce](examples/SwitchDebounce)
12. [TimerDuration](examples/TimerDuration)
13. [TimerInterruptTest](examples/TimerInterruptTest)
14. [**multiFileProject**](examples/multiFileProject)

---

### Example [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex)

https://github.com/khoih-prog/ATtiny_TimerInterrupt/blob/e844b26a8ef72e9d91c47f963f61ca01fee7fc18/examples/ISR_16_Timers_Array_Complex/ISR_16_Timers_Array_Complex.ino#L16-L376


---
---

### Debug Terminal Output Samples

### 1. ISR_16_Timers_Array_Complex on AVR_CuriosityNano3217

The following is the sample terminal output when running example [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex) on **AVR_CuriosityNano3217** to demonstrate the accuracy of ISR Hardware Timer, **especially when system is very busy**.  The ISR timer is **programmed for 2s, is activated exactly after 2.000s !!!**

While software timer, **programmed for 2s, is activated after more than 10.000s in loop().

```
Starting ISR_16_Timers_Array_Complex on AVR_CuriosityNano3217
ATtiny_TimerInterrupt v1.0.1
CPU Frequency = 20 MHz
TCB Clock Frequency = Full clock (20/16MHz, etc) for highest accuracy
Starting  ITimer OK, millis() = 13
SimpleTimer : 2, ms : 9949, Dms : 9949
Timer : 0, programmed : 5000, actual : 5015
Timer : 1, programmed : 10000, actual : 0
Timer : 2, programmed : 15000, actual : 0
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 19876, Dms : 9927
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10016
Timer : 2, programmed : 15000, actual : 15016
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
...

SimpleTimer : 2, ms : 179959, Dms : 9997
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 29996
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 39996
Timer : 8, programmed : 45000, actual : 45001
Timer : 9, programmed : 50000, actual : 49997
Timer : 10, programmed : 55000, actual : 54996
Timer : 11, programmed : 60000, actual : 60001
Timer : 12, programmed : 65000, actual : 64997
Timer : 13, programmed : 70000, actual : 69997
Timer : 14, programmed : 75000, actual : 74997
Timer : 15, programmed : 80000, actual : 79997
```

---


### 2. ISR_16_Timers_Array_Complex on AVR_CuriosityNano3217 to show accuracy difference


### 2.1. TCB Clock Frequency Full clock for highest accuracy


```
Starting ISR_16_Timers_Array_Complex on AVR_CuriosityNano3217
ATtiny_TimerInterrupt v1.0.1
CPU Frequency = 20 MHz
TCB Clock Frequency = Full clock (20/16MHz, etc) for highest accuracy
Starting  ITimer OK, millis() = 13
SimpleTimer : 2, ms : 9949, Dms : 9949
Timer : 0, programmed : 5000, actual : 5015
Timer : 1, programmed : 10000, actual : 0
Timer : 2, programmed : 15000, actual : 0
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 19876, Dms : 9927
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10016
Timer : 2, programmed : 15000, actual : 15016
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
...

SimpleTimer : 2, ms : 179959, Dms : 9997
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 29996
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 39996
Timer : 8, programmed : 45000, actual : 45001
Timer : 9, programmed : 50000, actual : 49997
Timer : 10, programmed : 55000, actual : 54996
Timer : 11, programmed : 60000, actual : 60001
Timer : 12, programmed : 65000, actual : 64997
Timer : 13, programmed : 70000, actual : 69997
Timer : 14, programmed : 75000, actual : 74997
Timer : 15, programmed : 80000, actual : 79997

```

---

### 2.2. TCB Clock Frequency Half clock for high accuracy

```
Starting ISR_16_Timers_Array_Complex on AVR_CuriosityNano3217
ATtiny_TimerInterrupt v1.0.1
CPU Frequency = 20 MHz
TCB Clock Frequency = Half clock (10/8MHz, etc.) for high accuracy
Starting  ITimer OK, millis() = 13
SimpleTimer : 2, ms : 9752, Dms : 9752
Timer : 0, programmed : 5000, actual : 5015
Timer : 1, programmed : 10000, actual : 0
Timer : 2, programmed : 15000, actual : 0
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 19810, Dms : 10058
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10016
Timer : 2, programmed : 15000, actual : 15016
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
...
SimpleTimer : 2, ms : 89713, Dms : 9998
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10001
Timer : 2, programmed : 15000, actual : 15001
Timer : 3, programmed : 20000, actual : 20001
Timer : 4, programmed : 25000, actual : 25001
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40001
Timer : 8, programmed : 45000, actual : 45017
Timer : 9, programmed : 50000, actual : 50016
Timer : 10, programmed : 55000, actual : 55017
Timer : 11, programmed : 60000, actual : 60016
Timer : 12, programmed : 65000, actual : 65017
Timer : 13, programmed : 70000, actual : 70016
Timer : 14, programmed : 75000, actual : 75017
Timer : 15, programmed : 80000, actual : 80017
```

---

### 3. Change_Interval_HF on AVR_CuriosityNano3217

The following is the sample terminal output when running example [Change_Interval_HF](examples/Change_Interval_HF) on **AVR_CuriosityNano3217** to demonstrate how to change High Frequency Timer Interval on-the-fly

```
Starting Change_Interval_HF on AVR_CuriosityNano3217
ATtiny_TimerInterrupt v1.0.1
CPU Frequency = 20 MHz
TCB Clock Frequency = Full clock (20/16MHz, etc) for highest accuracy
Starting ITimer OK, millis() = 13
Frequency, Timer = 50
Time = 1001, Timer1Count = 49
Time = 2002, Timer1Count = 99
Time = 3003, Timer1Count = 149
Time = 4004, Timer1Count = 199
Time = 5005, Timer1Count = 249
Changing Frequency, Timer = 25
Time = 6006, Timer1Count = 274
Time = 7007, Timer1Count = 299
Time = 8008, Timer1Count = 324
Time = 9009, Timer1Count = 349
Time = 10010, Timer1Count = 374
Changing Frequency, Timer = 16
Time = 11011, Timer1Count = 390
Time = 12012, Timer1Count = 406
Time = 13013, Timer1Count = 422
Time = 14014, Timer1Count = 438
Time = 15015, Timer1Count = 454
Changing Frequency, Timer = 12
Time = 16016, Timer1Count = 466
Time = 17017, Timer1Count = 478
Time = 18018, Timer1Count = 490
Time = 19019, Timer1Count = 502
Time = 20020, Timer1Count = 514
Changing Frequency, Timer = 10
Time = 21021, Timer1Count = 524
Time = 22022, Timer1Count = 534
Time = 23023, Timer1Count = 544
Time = 24024, Timer1Count = 554
Time = 25025, Timer1Count = 564
```

---
---


### Debug

Debug is enabled by default on `Serial` for `AVR_CuriosityNano3217`.

You can also change the debugging level from 0 to 4

```cpp
// These define's must be placed at the beginning before #include "ATtiny_TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.


---
---

### Issues

Submit issues to: [ATtiny_TimerInterrupt issues](https://github.com/khoih-prog/ATtiny_TimerInterrupt/issues)

---
---

### TO DO

1. Search for bug and improvement
2. Add support to 250KHz Timer Frequency


### DONE

 1. Longer Interval for timers
 2. Reduce code size if use less timers. Eliminate compiler warnings
 3. Now supporting complex object pointer-type argument
 4. 16 hardware-initiated software-enabled timers while using only 1 hardware timer
 5. Add support to **ATtiny-based boards (AVR_CuriosityNano3217, etc.) using megaTinyCore**
 6. Selectable **TCB Clock FULL, HALF** depending on necessary accuracy
 6. Fix `multiple-definitions` linker error
 7. Optimize library code by using `reference-passing` instead of `value-passing`
 8. Improve and customize examples for `Curiosity Nano AVRDA/AVRDB` boards to use on-board LED and SW
 9. Add notes `howto upload by drag-and-drop` to `CURIOSITY` virtual drive
10. Using Serial for debugging with `AVR_CuriosityNano3217`
11. Default to use `TCB0` in examples for boards having only 1 TCB Timer, such as `ATtiny817`, `ATtiny807`
12. Fix bug giving error when using TCB0 (`USE_TIMER_0 == true`)

---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library. Especially to these people who have directly or indirectly contributed to this [ATtiny_TimerInterrupt library](https://github.com/khoih-prog/ATtiny_TimerInterrupt)

1. Thanks to good work of [Spence Konde (aka Dr. Azzy)](https://github.com/SpenceKonde) for the [DxCore](https://github.com/SpenceKonde/DxCore) and [megaTinyCore](https://github.com/SpenceKonde/megaTinyCore)
2. Thanks to [LaurentR59](https://github.com/LaurentR59) to request the enhancement [Support for DX CORE CPU and MightyCORE CPU possible? #8](https://github.com/khoih-prog/TimerInterrupt_Generic/issues/8) leading to this new library

<table>
  <tr>
    <td align="center"><a href="https://github.com/SpenceKonde"><img src="https://github.com/SpenceKonde.png" width="100px;" alt="SpenceKonde"/><br/><sub><b>⭐️⭐️ Spence Konde</b></sub></a><br/></td>
    <td align="center"><a href="https://github.com/LaurentR59"><img src="https://github.com/LaurentR59.png" width="100px;" alt="LaurentR59"/><br/><sub><b>LaurentR59</b></sub></a><br/></td>
  </tr> 
</table>

---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

## License

- The library is licensed under [MIT](https://github.com/khoih-prog/ATtiny_TimerInterrupt/blob/main/LICENSE)

---

## Copyright

Copyright 2022- Khoi Hoang


