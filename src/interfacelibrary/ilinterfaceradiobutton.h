/***************************************************************************//**
* @file
* Interface Library Interface Radio Button
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

#ifndef ILINTERFACERADIOBUTTON_H
#define ILINTERFACERADIOBUTTON_H

#include "ilinterfacegroupbox.h"

class ILInterfaceRadioButton : public ILInterfaceIBox
{

public:

    ILInterfaceRadioButton() : ILInterfaceIBox()
    {
        m_checked = bool();

        m_checkedChanged = false;
        m_signalBuffer = reinterpret_cast<uint8 *>(&m_checked);
        m_signalBufferSize = sizeof(m_checked);
    }

    virtual ILNodeType type() const
    {
        return NT_WINDOW_INTERFACE_IBOX_RADIO_BUTTON;
    }

    bool init(ILInterfaceGBox *parent, const char *name,
              bool waitForResponse = false)
    {
        bool result = (!m_enabled) && parent && parent->enabled() && name;

        if(result)
        {
            construct(parent);

            if(!(result = sendPacket(INTERFACE_NEW_RADIO_BUTTON,
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
        return sendPacket(INTERFACE_SET_RADIO_BUTTON_NAME,
        name, waitForResponse);
    }

    bool getName(char *name,
                 uint16 size)
    {
        return receivePacket(INTERFACE_GET_RADIO_BUTTON_NAME,
        name, size);
    }

    bool setChecked(bool checked,
                    bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(INTERFACE_SET_RADIO_BUTTON_CHECKED,
        int32(checked), waitForResponse)))
        {
            m_checked = checked;
        }

        return result;
    }

    bool getChecked(bool bypassCache = false,
                    bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(INTERFACE_GET_RADIO_BUTTON_CHECKED,
            &response)))
            {
               m_checked = bool(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_checked : bool();
    }

    bool checkedChanged()
    {
        bool temp = m_checkedChanged; m_checkedChanged = false; return temp;
    }

protected:

    virtual void signalBufferCallBack()
    {
        m_checked = fromLittleEndian(m_checked); m_checkedChanged = true;
    }

private:

    ILInterfaceRadioButton(const ILInterfaceRadioButton &);
    ILInterfaceRadioButton &operator=(const ILInterfaceRadioButton &);

    int32 m_checked;

    bool m_checkedChanged;
};

#endif // ILINTERFACERADIOBUTTON_H

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
