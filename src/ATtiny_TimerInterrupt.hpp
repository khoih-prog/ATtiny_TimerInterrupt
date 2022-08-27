/****************************************************************************************************************************
  ATtiny_TimerInterrupt.hpp
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

#ifndef AT_TINY_TIMERINTERRUPT_HPP
#define AT_TINY_TIMERINTERRUPT_HPP

#if defined(MEGATINYCORE)

  #define TIMER_INTERRUPT_USING_MEGATINYCORE        true
  
  #if !defined(BOARD_NAME)
  
    ////////////////////////// Curiosity Nano //////////////////////////
  
    #if ( defined(ARDUINO_AVR_CuriosityNano3217) )
      #define BOARD_NAME      F("AVR_CuriosityNano3217" )    
    #elif ( defined(ARDUINO_AVR_CuriosityNano1627) )
      #define BOARD_NAME      F("AVR_CuriosityNano1627")
    #elif ( defined(ARDUINO_AVR_CuriosityNano1607) )
      #define BOARD_NAME      F("AVR_CuriosityNano1607")
    #elif ( defined(ARDUINO_AVR_Xplained_Pro_3217) )
      #define BOARD_NAME      F("AVR_Xplained_Pro_3217")
    #elif ( defined(ARDUINO_AVR_Xplained_Pro_817) )
      #define BOARD_NAME      F("AVR_Xplained_Pro_817" )    
    #elif ( defined(ARDUINO_AVR_XplainedMini817) )
      #define BOARD_NAME      F("AVR_XplainedMini817")
    #elif ( defined(ARDUINO_AVR_XplainedNano416) )
      #define BOARD_NAME      F("AVR_XplainedNano416")
  
    ////////////////////////// tinyAVR 0/1 series (atxy7) //////////////////////////
  
    #elif ( defined(ARDUINO_AVR_ATtiny3217) )
      #define BOARD_NAME      F("AVR_ATtiny3217" )    
    #elif ( defined(ARDUINO_AVR_ATtiny1617) )
      #define BOARD_NAME      F("AVR_ATtiny1617")
    #elif ( defined(ARDUINO_AVR_ATtiny817) )
      #define BOARD_NAME      F("AVR_ATtiny817")
    #elif ( defined(ARDUINO_AVR_ATtiny417) )
      #define BOARD_NAME      F("AVR_ATtiny417")
    #elif ( defined(ARDUINO_AVR_ATtiny3227) )
      #define BOARD_NAME      F("AVR_ATtiny3227" )    
    #elif ( defined(ARDUINO_AVR_ATtiny1627) )
      #define BOARD_NAME      F("AVR_ATtiny1627")
    #elif ( defined(ARDUINO_AVR_ATtiny827) )
      #define BOARD_NAME      F("AVR_ATtiny827")
    #elif ( defined(ARDUINO_AVR_ATtiny427) )
      #define BOARD_NAME      F("AVR_ATtiny427")  
    #elif ( defined(ARDUINO_AVR_ATtiny1607) )
      #define BOARD_NAME      F("AVR_ATtiny1607")
    #elif ( defined(ARDUINO_AVR_ATtiny807) )
      #define BOARD_NAME      F("AVR_ATtiny807")  
             
    ////////////////////////// tinyAVR 0/1 series (atxy6) //////////////////////////
  
    #elif ( defined(ARDUINO_AVR_ATtiny3216) )
      #define BOARD_NAME      F("AVR_ATtiny3216" )    
    #elif ( defined(ARDUINO_AVR_ATtiny1616) )
      #define BOARD_NAME      F("AVR_ATtiny1616")
    #elif ( defined(ARDUINO_AVR_ATtiny816) )
      #define BOARD_NAME      F("AVR_ATtiny816")
    #elif ( defined(ARDUINO_AVR_ATtiny416) )
      #define BOARD_NAME      F("AVR_ATtiny416")
    #elif ( defined(ARDUINO_AVR_ATtiny3226) )
      #define BOARD_NAME      F("AVR_ATtiny3226" )    
    #elif ( defined(ARDUINO_AVR_ATtiny1626) )
      #define BOARD_NAME      F("AVR_ATtiny1626")
    #elif ( defined(ARDUINO_AVR_ATtiny826) )
      #define BOARD_NAME      F("AVR_ATtiny826")
    #elif ( defined(ARDUINO_AVR_ATtiny426) )
      #define BOARD_NAME      F("AVR_ATtiny426")  
    #elif ( defined(ARDUINO_AVR_ATtiny1606) )
      #define BOARD_NAME      F("AVR_ATtiny1606")
    #elif ( defined(ARDUINO_AVR_ATtiny806) )
      #define BOARD_NAME      F("AVR_ATtiny806")  
    #elif ( defined(ARDUINO_AVR_ATtiny406) )
      #define BOARD_NAME      F("AVR_ATtiny406")  
    
    
    ////////////////////////// tinyAVR 0/1 series (atxy4) //////////////////////////
     
    #elif ( defined(ARDUINO_AVR_ATtiny1614) )
      #define BOARD_NAME      F("AVR_ATtiny1614")
    #elif ( defined(ARDUINO_AVR_ATtiny814) )
      #define BOARD_NAME      F("AVR_ATtiny814")
    #elif ( defined(ARDUINO_AVR_ATtiny414) )
      #define BOARD_NAME      F("AVR_ATtiny414")
    #elif ( defined(ARDUINO_AVR_ATtiny214) )
      #define BOARD_NAME      F("AVR_ATtiny214")  
    #elif ( defined(ARDUINO_AVR_ATtiny3224) )
      #define BOARD_NAME      F("AVR_ATtiny3224" )    
    #elif ( defined(ARDUINO_AVR_ATtiny1624) )
      #define BOARD_NAME      F("AVR_ATtiny1624")
    #elif ( defined(ARDUINO_AVR_ATtiny824) )
      #define BOARD_NAME      F("AVR_ATtiny824")
    #elif ( defined(ARDUINO_AVR_ATtiny424) )
      #define BOARD_NAME      F("AVR_ATtiny424")  
    #elif ( defined(ARDUINO_AVR_ATtiny1604) )
      #define BOARD_NAME      F("AVR_ATtiny1604")
    #elif ( defined(ARDUINO_AVR_ATtiny804) )
      #define BOARD_NAME      F("AVR_ATtiny804")  
    #elif ( defined(ARDUINO_AVR_ATtiny404) )
      #define BOARD_NAME      F("AVR_ATtiny404")
    #elif ( defined(ARDUINO_AVR_ATtiny204) )
      #define BOARD_NAME      F("AVR_ATtiny204")
      
    ////////////////////////// tinyAVR 0/1 series (atxy2) //////////////////////////
     
    #elif ( defined(ARDUINO_AVR_ATtiny412) )
      #define BOARD_NAME      F("AVR_ATtiny412")
    #elif ( defined(ARDUINO_AVR_ATtiny212) )
      #define BOARD_NAME      F("AVR_ATtiny212"
    #elif ( defined(ARDUINO_AVR_ATtiny402) )
      #define BOARD_NAME      F("AVR_ATtiny402")
    #elif ( defined(ARDUINO_AVR_ATtiny202) )
      #define BOARD_NAME      F("AVR_ATtiny202")    
     
    ////////////////////////////////////////////////////////////////

    #endif
    
  #endif  // #if !defined(BOARD_NAME)
            
#else
 
  #error This is designed only for AVR_ATtiny boards using megaTinyCore

#endif

///////////////////////////////////////////////////////////////////////////////

#ifndef TIMER_INTERRUPT_DEBUG
  #define TIMER_INTERRUPT_DEBUG      0
#endif

#include "TimerInterrupt_Generic_Debug.h"

#ifndef AT_TINY_TIMER_INTERRUPT_VERSION
  #define AT_TINY_TIMER_INTERRUPT_VERSION       			F("ATtiny_TimerInterrupt v1.0.1")
  
  #define AT_TINY_TIMER_INTERRUPT_VERSION_MAJOR      1
  #define AT_TINY_TIMER_INTERRUPT_VERSION_MINOR      0
  #define AT_TINY_TIMER_INTERRUPT_VERSION_PATCH      1

  #define AT_TINY_TIMER_INTERRUPT_VERSION_INT        1000001
#endif

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "pins_arduino.h"

#define MAX_COUNT_16BIT           65535UL

typedef void (*timer_callback)();
typedef void (*timer_callback_p)(void *);


enum
{
  HW_TIMER_0 = 0,
  
#if defined(TCB1)  
  HW_TIMER_1,
#endif  
  
  NUM_HW_TIMERS
};

#if ( defined(MILLIS_USE_TIMERB0) && USE_TIMER_0)
  #error TCB0 has been used by millis()
#elif ( defined(MILLIS_USE_TIMERB1) && USE_TIMER_1)
  #error TCB1 has been used by millis()  
#endif

class TimerInterrupt
{
  private:

    bool            _timerDone;
    int8_t          _timer;
    uint32_t        _CCMPValue;
    uint32_t        _CCMPValueRemaining;
    volatile long   _toggle_count;
    double           _frequency;

    void*           _callback;        // pointer to the callback function
    void*           _params;          // function parameter

    void set_CCMP();

  public:

    TimerInterrupt()
    {
      _timer              = -1;
      _frequency          = 0;
      _callback           = NULL;
      _params             = NULL;
      _timerDone          = false;
      _CCMPValue           = 0;
      _CCMPValueRemaining  = 0;
      _toggle_count       = -1;
    };

    explicit TimerInterrupt(const uint8_t& timerNo)
    {
      _timer              = timerNo;
      _frequency          = 0;
      _callback           = NULL;
      _params             = NULL;
      _timerDone          = false;
      _CCMPValue           = 0;
      _CCMPValueRemaining  = 0;
      _toggle_count       = -1;
    };

    void callback() __attribute__((always_inline))
    {
      if (_callback != NULL)
      {
        if (_params != NULL)
          (*(timer_callback_p)_callback)(_params);
        else
          (*(timer_callback)_callback)();
      }
    }

    void init(const int8_t& timer);

    void init()
    {
      init(_timer);
    };

    // frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    bool setFrequency(const float& frequency, timer_callback_p callback, const uint32_t& params, const unsigned long& duration = 0);

    // frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    bool setFrequency(const float& frequency, timer_callback callback, const unsigned long& duration = 0)
    {
      return setFrequency(frequency, reinterpret_cast<timer_callback_p>(callback), /*NULL*/ 0, duration);
    }

    // interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    template<typename TArg>
    bool setInterval(const unsigned long& interval, void (*callback)(TArg), const TArg& params, const unsigned long& duration = 0)
    {
      static_assert(sizeof(TArg) <= sizeof(uint32_t), "setInterval() callback argument size must be <= 4 bytes");
      return setFrequency((float) (1000.0f / interval), reinterpret_cast<timer_callback_p>(callback), (uint32_t) params, duration);
    }

    // interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    bool setInterval(const unsigned long& interval, timer_callback callback, const unsigned long& duration = 0)
    {
      return setFrequency((float) (1000.0f / interval), reinterpret_cast<timer_callback_p>(callback), /*NULL*/ 0, duration);
    }

    template<typename TArg>
    bool attachInterrupt(const float& frequency, void (*callback)(TArg), const TArg& params, const unsigned long& duration = 0)
    {
      static_assert(sizeof(TArg) <= sizeof(uint32_t), "attachInterrupt() callback argument size must be <= 4 bytes");
      return setFrequency(frequency, reinterpret_cast<timer_callback_p>(callback), (uint32_t) params, duration);
    }

    bool attachInterrupt(const float& frequency, timer_callback callback, const unsigned long& duration = 0)
    {
      return setFrequency(frequency, reinterpret_cast<timer_callback_p>(callback), /*NULL*/ 0, duration);
    }

    // Interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    template<typename TArg>
    bool attachInterruptInterval(const unsigned long& interval, void (*callback)(TArg), const TArg& params, const unsigned long& duration = 0)
    {
      static_assert(sizeof(TArg) <= sizeof(uint32_t), "attachInterruptInterval() callback argument size must be <= 4 bytes");
      return setFrequency( (float) ( 1000.0f / interval), reinterpret_cast<timer_callback_p>(callback), (uint32_t) params, duration);
    }

    // Interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    bool attachInterruptInterval(const unsigned long& interval, timer_callback callback, const unsigned long& duration = 0)
    {
      return setFrequency( (float) ( 1000.0f / interval), reinterpret_cast<timer_callback_p> (callback), /*NULL*/ 0, duration);
    }

    void detachInterrupt();

    void disableTimer()
    {
      detachInterrupt();
    }

    // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    void reattachInterrupt(const unsigned long& duration = 0);

    // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    void enableTimer(const unsigned long& duration = 0) __attribute__((always_inline))
    {
      reattachInterrupt(duration);
    }

    // Just stop clock source, still keep the count
    void pauseTimer();

    // Just reconnect clock source, continue from the current count
    void resumeTimer();

    // Just stop clock source, clear the count
    void stopTimer()
    {
      detachInterrupt();
    }

    // Just reconnect clock source, start current count from 0
    void restartTimer(const unsigned long& duration = 0)
    {
      reattachInterrupt(duration);
    }

    int8_t getTimer() __attribute__((always_inline))
    {
      return _timer;
    };

    long getCount() __attribute__((always_inline))
    {
      return _toggle_count;
    };

    void setCount(const long& countInput) __attribute__((always_inline))
    {
      //noInterrupts();

      _toggle_count = countInput;

      //interrupts();
    };

    uint32_t /*long*/ get_CCMPValue() __attribute__((always_inline))
    {
      return _CCMPValue;
    };

    uint32_t /*long*/ get_CCMPValueRemaining() __attribute__((always_inline))
    {
      return _CCMPValueRemaining;
    };

    void adjust_CCMPValue() //__attribute__((always_inline))
    {
      noInterrupts();
      
      if (_CCMPValueRemaining < MAX_COUNT_16BIT)
      {
        set_CCMP();
      }
        
      interrupts();  

      _CCMPValueRemaining -= min(MAX_COUNT_16BIT, _CCMPValueRemaining);

      if (_CCMPValueRemaining <= 0)
      {
        // Reset value for next cycle
        _CCMPValueRemaining = _CCMPValue;
      
        _timerDone = true;
      }
      else
        _timerDone = false;
    };

    void reload_CCMPValue() //__attribute__((always_inline))
    {
      noInterrupts();

      // Reset value for next cycle, have to deduct the value already loaded to CCMP register 
      _CCMPValueRemaining = _CCMPValue;
      set_CCMP();
      
      _timerDone = false;

      interrupts();
    };

    bool checkTimerDone() //__attribute__((always_inline))
    {
      return _timerDone;
    };

}; // class TimerInterrupt

//////////////////////////////////////////////

#endif      //#ifndef AT_TINY_TIMERINTERRUPT_HPP
