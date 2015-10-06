/***************************************************************************//**
* @file
* Interface Library Dependencies
*
* @version @n 1.0
* @date @n 3/16/2014
*
* @author @n Kwabena W. Agyeman
* @copyright @n (c) 2014 Kwabena W. Agyeman
* @n All rights reserved - Please see the end of the file for the terms of use
*
* @par Update History:
* @n v1.0 - Original release - 3/16/2014
*******************************************************************************/

#ifndef ILDEPENDENCIES_H
#define ILDEPENDENCIES_H

#include "ilshared.h"

#define FIFO_BUFFER_SIZE 64 // MAX 256

#define MAX_OUTSTANDING DCE_MAX_QUEUE_DEPTH

#ifndef READ_CREDIT_TIMEOUT_OVERRIDE
#define READ_CREDIT_TIMEOUT (2L * 1000L) // two seconds in milliseconds
#endif

#ifndef WRITE_CREDITS_TIMEOUT_OVERRIDE
#define WRITE_CREDITS_TIMEOUT (2L * 1000L) // two seconds in milliseconds
#endif

// Start Library Dependencies /////////////////////////////////////////////////

#define MAX_SET_CREDITS MAX_CREDITS
#define MAX_SET_LAYER_0_DATA_SIZE MAX_LAYER_0_DATA_SIZE
#define MAX_SET_BAUD_RATE DEF_BAUD_RATE

#ifdef ARDUINO

    #if (ARDUINO >= 100)
        #include "Arduino.h"
    #else
        #include "WProgram.h"
    #endif

    #undef MAX_SET_BAUD_RATE
    #define MAX_SET_BAUD_RATE MID_FAST_BAUD_RATE

    #define ARDUINO_TX_PIN 1
    #define ARDUINO_RX_PIN 0

    #ifdef RAMEND
        #if (RAMEND < 1000)

            #define HALF_DUPLEX_MODE

            // Only have a 16-1 byte buffer... ((16-1) == (1*15))

            #undef MAX_SET_CREDITS
            #define MAX_SET_CREDITS 1

            #undef MAX_SET_LAYER_0_DATA_SIZE
            #define MAX_SET_LAYER_0_DATA_SIZE \
            (15-LAYER_0_HEAD_SIZE-LAYER_0_TAIL_SIZE-1)
            // includes the credit flow control byte

            // Save RAM
            #undef FIFO_BUFFER_SIZE
            #define FIFO_BUFFER_SIZE 16

        #else

            #define FULL_DUPLEX_MODE

            // Only have a 64-1 byte buffer... ((64-1) == (3*21))

            #undef MAX_SET_CREDITS
            #define MAX_SET_CREDITS 3

            #undef MAX_SET_LAYER_0_DATA_SIZE
            #define MAX_SET_LAYER_0_DATA_SIZE \
            (21-LAYER_0_HEAD_SIZE-LAYER_0_TAIL_SIZE-1)
            // includes the credit flow control byte

            // Save RAM
            #undef FIFO_BUFFER_SIZE
            #define FIFO_BUFFER_SIZE 64

        #endif
    #endif

#elif defined __PROPELLER__

    #include "simpletools.h"
    #include "mstimer.h"
    #include "simpletext.h"
    #include "serial.h"
    #include "fdserial.h"

    #undef MAX_SET_BAUD_RATE
    #define MAX_SET_BAUD_RATE MID_FAST_BAUD_RATE

    #define PROPELLER_TX_PIN 30
    #define PROPELLER_RX_PIN 31

    #if 0

        #define HALF_DUPLEX_MODE

        // Only have a 64-1 byte buffer... ((64-1) == (3*21))

        #undef MAX_SET_CREDITS
        #define MAX_SET_CREDITS 3

        #undef MAX_SET_LAYER_0_DATA_SIZE
        #define MAX_SET_LAYER_0_DATA_SIZE \
        (21-LAYER_0_HEAD_SIZE-LAYER_0_TAIL_SIZE-1)
        // includes the credit flow control byte

        // Save RAM
        #undef FIFO_BUFFER_SIZE
        #define FIFO_BUFFER_SIZE 64

    #else

        #define FULL_DUPLEX_MODE

        // Only have a 64-1 byte buffer... ((64-1) == (3*21))

        #undef MAX_SET_CREDITS
        #define MAX_SET_CREDITS 3

        #undef MAX_SET_LAYER_0_DATA_SIZE
        #define MAX_SET_LAYER_0_DATA_SIZE \
        (21-LAYER_0_HEAD_SIZE-LAYER_0_TAIL_SIZE-1)
        // includes the credit flow control byte

        // Save RAM
        #undef FIFO_BUFFER_SIZE
        #define FIFO_BUFFER_SIZE 64

    #endif

    #define HIGH 1
    #define LOW 0

    #define OUTPUT 1
    #define INPUT 0

#else
    #include "serialdevice.h"
#endif

#define TIME_DELAY_PAUSE 10 // 10 ms

// End Library Dependencies ///////////////////////////////////////////////////

#define minMacro(a,b) (((a)<(b))?(a):(b))
#define maxMacro(a,b) (((a)>(b))?(a):(b))

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef unsigned long long uint64;

typedef signed char int8;
typedef signed short int16;
typedef signed long int32;
typedef signed long long int64;

typedef float float32;
typedef double float64;

// Helper Functions ///////////////////////////////////////////////////////////

static uint32 stringLength(const char *string)
{
    const char *anchor = string;

    if(string)
    {
        for(; *string; string++);
    }

    return (string - anchor);
}

// To Little Endian Functions /////////////////////////////////////////////////

inline uint8 toLittleEndian(uint8 value)
{
    return value; // already LittleEndian - do nothing
}

inline uint16 toLittleEndian(uint16 value)
{
    return value; // already LittleEndian - do nothing
}

inline uint32 toLittleEndian(uint32 value)
{
    return value; // already LittleEndian - do nothing
}

inline uint64 toLittleEndian(uint64 value)
{
    return value; // already LittleEndian - do nothing
}

inline int8 toLittleEndian(int8 value)
{
    return value; // already LittleEndian - do nothing
}

inline int16 toLittleEndian(int16 value)
{
    return value; // already LittleEndian - do nothing
}

inline int32 toLittleEndian(int32 value)
{
    return value; // already LittleEndian - do nothing
}

inline int64 toLittleEndian(int64 value)
{
    return value; // already LittleEndian - do nothing
}

inline float32 toLittleEndian(float32 value)
{
    return value; // already LittleEndian - do nothing
}

inline float64 toLittleEndian(float64 value)
{
    return value; // already LittleEndian - do nothing
}

// From Little Endian Functions ///////////////////////////////////////////////

inline uint8 fromLittleEndian(uint8 value)
{
    return value; // already LittleEndian - do nothing
}

inline uint16 fromLittleEndian(uint16 value)
{
    return value; // already LittleEndian - do nothing
}

inline uint32 fromLittleEndian(uint32 value)
{
    return value; // already LittleEndian - do nothing
}

inline uint64 fromLittleEndian(uint64 value)
{
    return value; // already LittleEndian - do nothing
}

inline int8 fromLittleEndian(int8 value)
{
    return value; // already LittleEndian - do nothing
}

inline int16 fromLittleEndian(int16 value)
{
    return value; // already LittleEndian - do nothing
}

inline int32 fromLittleEndian(int32 value)
{
    return value; // already LittleEndian - do nothing
}

inline int64 fromLittleEndian(int64 value)
{
    return value; // already LittleEndian - do nothing
}

inline float32 fromLittleEndian(float32 value)
{
    return value; // already LittleEndian - do nothing
}

inline float64 fromLittleEndian(float64 value)
{
    return value; // already LittleEndian - do nothing
}

// Serial Functions ///////////////////////////////////////////////////////////

inline void serialBegin()
{
    #ifdef ARDUINO
    {
        Serial.begin(DEF_BAUD_RATE);
    }
    #elif defined __PROPELLER__
    {
        simpleterm_set(fdserial_open(PROPELLER_RX_PIN, PROPELLER_TX_PIN,
                                     FDSERIAL_MODE_NONE, DEF_BAUD_RATE));
    }
    #endif
}

inline void serialEnd()
{
    #ifdef ARDUINO
    {
        // Add a null byte to fix the fifo buffer flush bug...
        Serial.write('\0');
        Serial.end();
    }
    #elif defined __PROPELLER__
    {
        extern text_t *dport_ptr;

        if(dport_ptr)
        {
            // Add a null byte to fix the fifo buffer flush bug...
            fdserial_txChar(static_cast<fdserial *>(dport_ptr), '\0');
            fdserial_close(static_cast<fdserial *>(dport_ptr));
            dport_ptr = 0;
        }
    }
    #endif
}

static void serialSetBaudRate(uint32 baudRate)
{
    #ifdef ARDUINO
    {
        delay(TIME_DELAY_PAUSE/2);
        {
            // Add a null byte to fix the fifo buffer flush bug...
            Serial.write('\0');
            Serial.end();
        }
        delay(TIME_DELAY_PAUSE);
        {
            Serial.begin(baudRate);
        }
        delay(TIME_DELAY_PAUSE/2);
    }
    #elif defined __PROPELLER__
    {
        pause(TIME_DELAY_PAUSE/2);
        {
            extern text_t *dport_ptr;

            if(dport_ptr)
            {
                // Add a null byte to fix the fifo buffer flush bug...
                fdserial_txChar(static_cast<fdserial *>(dport_ptr), '\0');
                fdserial_close(static_cast<fdserial *>(dport_ptr));
                dport_ptr = 0;
            }
        }
        pause(TIME_DELAY_PAUSE);
        {
            simpleterm_set(fdserial_open(PROPELLER_RX_PIN, PROPELLER_TX_PIN,
                                         FDSERIAL_MODE_NONE, baudRate));
        }
        pause(TIME_DELAY_PAUSE/2);
    }
    #else
    {
        Q_UNUSED(baudRate);
    }
    #endif
}

static void serialResetEvent()
{
    #ifdef ARDUINO
    {
        pinMode(ARDUINO_TX_PIN, OUTPUT);
        delay(TIME_DELAY_PAUSE);
        digitalWrite(ARDUINO_TX_PIN, LOW);
        delay(TIME_DELAY_PAUSE);
        digitalWrite(ARDUINO_TX_PIN, HIGH);
        delay(TIME_DELAY_PAUSE);
        digitalWrite(ARDUINO_TX_PIN, LOW);
        delay(TIME_DELAY_PAUSE);
        digitalWrite(ARDUINO_TX_PIN, HIGH);
        delay(TIME_DELAY_PAUSE);
        pinMode(ARDUINO_TX_PIN, INPUT);
    }
    #elif defined __PROPELLER__
    {
        set_direction(PROPELLER_TX_PIN, OUTPUT);
        pause(TIME_DELAY_PAUSE);
        set_output(PROPELLER_TX_PIN, LOW);
        pause(TIME_DELAY_PAUSE);
        set_output(PROPELLER_TX_PIN, HIGH);
        pause(TIME_DELAY_PAUSE);
        set_output(PROPELLER_TX_PIN, LOW);
        pause(TIME_DELAY_PAUSE);
        set_output(PROPELLER_TX_PIN, HIGH);
        pause(TIME_DELAY_PAUSE);
        set_direction(PROPELLER_TX_PIN, INPUT);
    }
    #else
    {
        SerialDevice::resetEventExtern();
    }
    #endif
}

// Read Data Functions ////////////////////////////////////////////////////////

inline int serialAvailable()
{
    #ifdef ARDUINO
    {
        return Serial.available();
    }
    #elif defined __PROPELLER__
    {
        if(simpleterm_pointer())
        {
            volatile fdserial_st *p =
            static_cast<volatile fdserial_st *>(simpleterm_pointer()->devst);

            if(p)
            {
                return (FDSERIAL_BUFF_MASK & (p->rx_head - p->rx_tail));
            }
        }

        return int();
    }
    #else
    {
        return SerialDevice::bytesAvailableExtern();
    }
    #endif
}

inline int serialRead()
{
    #ifdef ARDUINO
    {
        return Serial.read();
    }
    #elif defined __PROPELLER__
    {
        if(simpleterm_pointer())
        {
            fdserial *p =
            static_cast<fdserial *>(simpleterm_pointer());
            return fdserial_rxCheck(p);
        }

        return int();
    }
    #else
    {
        char data; SerialDevice::readDataExtern(&data); return data;
    }
    #endif
}

// Send Data Functions ////////////////////////////////////////////////////////

inline void serialWrite(char data)
{
    #ifdef ARDUINO
    {
        Serial.write(data);
    }
    #elif defined __PROPELLER__
    {
        if(simpleterm_pointer())
        {
            fdserial *p =
            static_cast<fdserial *>(simpleterm_pointer());
            fdserial_txChar(p, data);
        }
    }
    #else
    {
        SerialDevice::writeDataExtern(&data);
    }
    #endif
}

inline void serialWrite(const char *data, uint32 size)
{
    #ifdef ARDUINO
    {
        Serial.write(reinterpret_cast<const uint8_t *>(data), size);
    }
    #elif defined __PROPELLER__
    {
        if(simpleterm_pointer())
        {
            fdserial *p =
            static_cast<fdserial *>(simpleterm_pointer());

            for(uint32 i = 0; i < size; i++)
            {
                fdserial_txChar(p, data[i]);
            }
        }
    }
    #else
    {
        SerialDevice::writeDataExtern(data, size);
    }
    #endif
}

// Timer Functions ////////////////////////////////////////////////////////////

inline uint32 timeMicroseconds()
{
    #ifdef ARDUINO
    {
        return micros();
    }
    #elif defined __PROPELLER__
    {
        return (CNT / us);
    }
    #else
    {
        return uint32();
    }
    #endif
}

inline uint32 timeMilliseconds()
{
    #ifdef ARDUINO
    {
        return millis();
    }
    #elif defined __PROPELLER__
    {
        return (CNT / ms);
    }
    #else
    {
        return SerialDevice::millisecondsExtern();
    }
    #endif
}

// Delay Functions ////////////////////////////////////////////////////////////

inline void timeDelayMicroseconds(uint32 microseconds)
{
    #ifdef ARDUINO
    {
        delayMicroseconds(microseconds);
    }
    #elif defined __PROPELLER__
    {
        waitcnt((microseconds * us) + CNT);
    }
    #else
    {
        Q_UNUSED(microseconds);
    }
    #endif
}

inline void timeDelayMilliseconds(uint32 milliseconds)
{
    #ifdef ARDUINO
    {
        delay(milliseconds);
    }
    #elif defined __PROPELLER__
    {
        waitcnt((milliseconds * ms) + CNT);
    }
    #else
    {
        SerialDevice::waitMillisecondsExtern(milliseconds);
    }
    #endif
}

#endif // ILDEPENDENCIES_H

/***************************************************************************//**
* @file
* @par MIT License - TERMS OF USE:
* @n Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* @n
* @n The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* @n
* @n THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*******************************************************************************/
