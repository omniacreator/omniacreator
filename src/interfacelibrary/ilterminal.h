/***************************************************************************//**
* @file
* Interface Library Terminal
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

#ifndef ILTERMINAL_H
#define ILTERMINAL_H

#include "ilcore.h"

class ILTerminal : public ILWindow
{

public:

    ILTerminal() : ILWindow()
    {
        // UNUSED //
    }

    virtual ILNodeType type() const
    {
        return NT_WINDOW_TERMINAL;
    }

    bool initUndocked(ILCore *parent, const char *name,
                      bool waitForResponse = false)
    {
        bool result = (!m_enabled) && parent && parent->enabled() && name;

        if(result)
        {
            construct(parent);

            if(!(result = sendPacket(TERMINAL_NEW_UNDOCKED_WINDOW,
            name, waitForResponse)))
            {
                deconstruct();
            }
        }

        return result;
    }

    bool initDocked(ILCore *parent, const char *name,
                    bool waitForResponse = false)
    {
        return init(parent, name, waitForResponse);
    }

//    bool initDocked(ILWindow *parent, const char *name,
//                    bool waitForResponse = false)
//    {
//        bool result = (!m_enabled) && parent && parent->enabled() && name;

//        if(result)
//        {
//            construct(parent->parent());

//            if(!(result = sendPacket(TERMINAL_NEW_DOCKED_WINDOW,
//            parent->handle(), name, waitForResponse)))
//            {
//                deconstruct();
//            }
//        }

//        return result;
//    }

    bool init(ILCore *parent, const char *name,
              bool waitForResponse = false)
    {
        bool result = (!m_enabled) && parent && parent->enabled() && name;

        if(result)
        {
            construct(parent);

            if(!(result = sendPacket(TERMINAL_NEW_MAIN_DOCKED_WINDOW,
            name, waitForResponse)))
            {
                deconstruct();
            }
        }

        return result;
    }

    // The wrong overload could be called with print() - but not with write().

    bool write(char character,
               bool waitForResponse = false)
    {
        char string[] = {character, '\0'};

        return sendPacket(TERMINAL_PRINT,
        string, waitForResponse);
    }

    // The wrong overload could be called with print() - but not with write().

    bool write(const char *text,
               bool waitForResponse = false)
    {
        return sendPacket(TERMINAL_PRINT,
        text, waitForResponse);
    }

    bool print(char character,
               bool waitForResponse = false)
    {
        char string[] = {character, '\0'};

        return sendPacket(TERMINAL_PRINT,
        string, waitForResponse);
    }

    bool print(const char *text,
               bool waitForResponse = false)
    {
        return sendPacket(TERMINAL_PRINT,
        text, waitForResponse);
    }

//    bool print(uint8 value, uint16 base,
//               bool waitForResponse = false)
//    {
//        return print(itoa(value, base, false),
//        waitForResponse);
//    }

//    bool print(int8 value, uint16 base,
//               bool waitForResponse = false)
//    {
//        return print(itoa(value, base, true),
//        waitForResponse);
//    }

//    bool print(uint16 value, uint16 base = DEC,
//               bool waitForResponse = false)
//    {
//        return print(itoa(value, base, false),
//        waitForResponse);
//    }

//    bool print(int16 value, uint16 base = DEC,
//               bool waitForResponse = false)
//    {
//        return print(itoa(value, base, true),
//        waitForResponse);
//    }

//    bool print(uint32 value, uint16 base = DEC,
//               bool waitForResponse = false)
//    {
//        return print(itoa(value, base, false),
//        waitForResponse);
//    }

    bool print(int32 value, uint16 base = DEC,
               bool waitForResponse = false)
    {
        return print(itoa(value, base, true),
        waitForResponse);
    }

    // TODO: Real float support...
    bool printF(float32 value,
                bool waitForResponse = false)
    {
        return print(int32(value), DEC,
        waitForResponse);
    }

    bool println(char character,
                 bool waitForResponse = false)
    {
        char string[] = {character, '\0'};

        return sendPacket(TERMINAL_PRINTLN,
        string, waitForResponse);
    }

    bool println(const char *text,
                 bool waitForResponse = false)
    {
        return sendPacket(TERMINAL_PRINTLN,
        text, waitForResponse);
    }

//    bool println(uint8 value, uint16 base,
//                 bool waitForResponse = false)
//    {
//        return println(itoa(value, base, false),
//        waitForResponse);
//    }

//    bool println(int8 value, uint16 base,
//                 bool waitForResponse = false)
//    {
//        return println(itoa(value, base, true),
//        waitForResponse);
//    }

//    bool println(uint16 value, uint16 base = DEC,
//                 bool waitForResponse = false)
//    {
//        return println(itoa(value, base, false),
//        waitForResponse);
//    }

//    bool println(int16 value, uint16 base = DEC,
//                 bool waitForResponse = false)
//    {
//        return println(itoa(value, base, true),
//        waitForResponse);
//    }

//    bool println(uint32 value, uint16 base = DEC,
//                 bool waitForResponse = false)
//    {
//        return println(itoa(value, base, false),
//        waitForResponse);
//    }

    bool println(int32 value, uint16 base = DEC,
                 bool waitForResponse = false)
    {
        return println(itoa(value, base, true),
        waitForResponse);
    }

    // TODO: Real float support...
    bool printlnF(float32 value,
                  bool waitForResponse = false)
    {
        return println(int32(value), DEC,
        waitForResponse);
    }

private:

    ILTerminal(const ILTerminal &);
    ILTerminal &operator=(const ILTerminal &);
};

#endif // ILTERMINAL_H

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
