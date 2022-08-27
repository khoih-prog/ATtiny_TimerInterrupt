/****************************************************************************************************************************
  ATtiny_TimerInterrupt.h
  For Arduino AVR ATtiny-based boards (ATtiny3217, etc.) using megaTinyCore
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ATtiny_TimerInterrupt
  Licensed under MIT license

  Now with we can use these new 16 ISR-based timers, while consuming only 1 hwarware Timer.
  Their independently-selected, maximum interval is practically unlimited (limited only by unsigned long miliseconds)
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Version: 1.0.1

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K.Hoang      27/08/2022 Initial coding to support AVR ATtiny (ATtiny3217, etc.) using megaTinyCore
  1.0.1   K.Hoang      27/08/2022 Fix TCB0 disable bug
****************************************************************************************************************************/

#pragma once

#ifndef AT_TINY_TIMERINTERRUPT_H
#define AT_TINY_TIMERINTERRUPT_H

#include "ATtiny_TimerInterrupt.hpp"
#include "ATtiny_TimerInterrupt_Impl.h"

#endif      //#ifndef AT_TINY_TIMERINTERRUPT_H
