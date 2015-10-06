/***************************************************************************//**
* @file
* Interface Library Interface Push Button
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

#ifndef ILINTERFACEPUSHBUTTON_H
#define ILINTERFACEPUSHBUTTON_H

#include "ilinterfacegroupbox.h"

class ILInterfacePushButton : public ILInterfaceIBox
{

public:

    ILInterfacePushButton() : ILInterfaceIBox()
    {
        m_buttonSignal = int32();

        m_wasPressed = false; m_wasReleased = false; m_wasClicked = false;
        m_signalBuffer = reinterpret_cast<uint8 *>(&m_buttonSignal);
        m_signalBufferSize = sizeof(m_buttonSignal);
    }

    virtual ILNodeType type() const
    {
        return NT_WINDOW_INTERFACE_IBOX_PUSH_BUTTON;
    }

    bool init(ILInterfaceGBox *parent, const char *name,
              bool waitForResponse = false)
    {
        bool result = (!m_enabled) && parent && parent->enabled() && name;

        if(result)
        {
            construct(parent);

            if(!(result = sendPacket(INTERFACE_NEW_PUSH_BUTTON,
            name, waitForResponse)))
            {
                deconstruct();
            }
        }

        return result;
    }

    // Properties /////////////////////////////////////////////////////////////

    bool setName(const char *name,
                 bool waitForResponse = false)
    {
        return sendPacket(INTERFACE_SET_PUSH_BUTTON_NAME,
        name, waitForResponse);
    }

    bool getName(char *name,
                 uint16 size)
    {
        return receivePacket(INTERFACE_GET_PUSH_BUTTON_NAME,
        name, size);
    }

    bool wasPressed()
    {
        bool temp = m_wasPressed; m_wasPressed = false; return temp;
    }

    bool wasReleased()
    {
        bool temp = m_wasReleased; m_wasReleased = false; return temp;
    }

    bool wasClicked()
    {
        bool temp = m_wasClicked; m_wasClicked = false; return temp;
    }

protected:

    virtual void signalBufferCallBack()
    {
        switch(m_buttonSignal = fromLittleEndian(m_buttonSignal))
        {
            case BS_PRESSED: m_wasPressed = true; break;
            case BS_RELEASED: m_wasReleased = true; break;
            case BS_CLICKED: m_wasClicked = true; break;
            default: break;
        }
    }

private:

    ILInterfacePushButton(const ILInterfacePushButton &);
    ILInterfacePushButton &operator=(const ILInterfacePushButton &);

    int32 m_buttonSignal;

    bool m_wasPressed; bool m_wasReleased; bool m_wasClicked;
};

#endif // ILINTERFACEPUSHBUTTON_H

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
