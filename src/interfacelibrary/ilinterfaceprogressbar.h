/***************************************************************************//**
* @file
* Interface Library Interface Progress Bar
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

#ifndef ILINTERFACEPROGRESSBAR_H
#define ILINTERFACEPROGRESSBAR_H

#include "ilinterfacegroupbox.h"

class ILInterfaceProgressBar : public ILInterfaceIBox
{

public:

    ILInterfaceProgressBar() : ILInterfaceIBox()
    {
        m_minimum = int32(); m_maximum = int32();
        m_value = int32();
    }

    virtual ILNodeType type() const
    {
        return NT_WINDOW_INTERFACE_IBOX_PROGRESS_BAR;
    }

    bool init(ILInterfaceGBox *parent, const char *name,
              bool waitForResponse = false)
    {
        bool result = (!m_enabled) && parent && parent->enabled() && name;

        if(result)
        {
            construct(parent);

            if(!(result = sendPacket(INTERFACE_NEW_PROGRESS_BAR,
            name, waitForResponse)))
            {
                deconstruct();
            }
        }

        return result;
    }

    // Properties /////////////////////////////////////////////////////////////

    bool reset(bool waitForResponse = false)
    {
        return sendPacket(INTERFACE_RESET_PROGRESS_BAR,
        waitForResponse);
    }

    bool setName(const char *name,
                 bool waitForResponse = false)
    {
        return sendPacket(INTERFACE_SET_PROGRESS_BAR_NAME,
        name, waitForResponse);
    }

    bool getName(char *name,
                 uint16 size)
    {
        return receivePacket(INTERFACE_GET_PROGRESS_BAR_NAME,
        name, size);
    }

    bool setMinimum(int32 minimum,
                    bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(INTERFACE_SET_PROGRESS_BAR_MINIMUM,
        int32(minimum), waitForResponse)))
        {
            m_minimum = minimum;
        }

        return result;
    }

    int32 getMinimum(bool bypassCache = false,
                     bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(INTERFACE_GET_PROGRESS_BAR_MINIMUM,
            &response)))
            {
               m_minimum = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_minimum : int32();
    }

    bool setMaximum(int32 maximum,
                    bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(INTERFACE_SET_PROGRESS_BAR_MAXIMUM,
        int32(maximum), waitForResponse)))
        {
            m_maximum = maximum;
        }

        return result;
    }

    int32 getMaximum(bool bypassCache = false,
                     bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(INTERFACE_GET_PROGRESS_BAR_MAXIMUM,
            &response)))
            {
               m_maximum = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_maximum : int32();
    }

    bool setValue(int32 value,
                  bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(INTERFACE_SET_PROGRESS_BAR_VALUE,
        int32(value), waitForResponse)))
        {
            m_value = value;
        }

        return result;
    }

    int32 getValue(bool bypassCache = false,
                   bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(INTERFACE_GET_PROGRESS_BAR_VALUE,
            &response)))
            {
               m_value = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_value : int32();
    }

private:

    ILInterfaceProgressBar(const ILInterfaceProgressBar &);
    ILInterfaceProgressBar &operator=(const ILInterfaceProgressBar &);

    int32 m_minimum; int32 m_maximum;
    int32 m_value;
};

#endif // ILINTERFACEPROGRESSBAR_H

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
