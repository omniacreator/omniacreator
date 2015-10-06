/***************************************************************************//**
* @file
* Interface Library Interface Bin Spin Box
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

#ifndef ILINTERFACEBINSPINBOX_H
#define ILINTERFACEBINSPINBOX_H

#include "ilinterfacegroupbox.h"

class ILInterfaceBinSpinBox : public ILInterfaceIBox
{

public:

    ILInterfaceBinSpinBox() : ILInterfaceIBox()
    {
        m_singleStep = int32(); m_pageStep = int32();
        m_minimum = int32(); m_maximum = int32();
        m_value = int32();

        m_valueChanged = false;
        m_signalBuffer = reinterpret_cast<uint8 *>(&m_value);
        m_signalBufferSize = sizeof(m_value);
    }

    virtual ILNodeType type() const
    {
        return NT_WINDOW_INTERFACE_IBOX_BIN_SPIN_BOX;
    }

    bool init(ILInterfaceGBox *parent, const char *name,
              bool waitForResponse = false)
    {
        bool result = (!m_enabled) && parent && parent->enabled() && name;

        if(result)
        {
            construct(parent);

            if(!(result = sendPacket(INTERFACE_NEW_BIN_SPIN_BOX,
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
        return sendPacket(INTERFACE_RESET_BIN_SPIN_BOX,
        waitForResponse);
    }

    bool setName(const char *name,
                 bool waitForResponse = false)
    {
        return sendPacket(INTERFACE_SET_BIN_SPIN_BOX_NAME,
        name, waitForResponse);
    }

    bool getName(char *name,
                 uint16 size)
    {
        return receivePacket(INTERFACE_GET_BIN_SPIN_BOX_NAME,
        name, size);
    }

    bool setPrefix(const char *prefix,
                   bool waitForResponse = false)
    {
        return sendPacket(INTERFACE_SET_BIN_SPIN_BOX_PREFIX,
        prefix, waitForResponse);
    }

    bool getPrefix(char *prefix,
                   uint16 size)
    {
        return receivePacket(INTERFACE_GET_BIN_SPIN_BOX_PREFIX,
        prefix, size);
    }

    bool setSuffix(const char *suffix,
                   bool waitForResponse = false)
    {
        return sendPacket(INTERFACE_SET_BIN_SPIN_BOX_SUFFIX,
        suffix, waitForResponse);
    }

    bool getSuffix(char *suffix,
                   uint16 size)
    {
        return receivePacket(INTERFACE_GET_BIN_SPIN_BOX_SUFFIX,
        suffix, size);
    }

    bool setSingleStep(int32 singleStep,
                       bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(INTERFACE_SET_BIN_SPIN_BOX_SINGLE_STEP,
        int32(singleStep), waitForResponse)))
        {
            m_singleStep = singleStep;
        }

        return result;
    }

    int32 getSingleStep(bool bypassCache = false,
                        bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(INTERFACE_GET_BIN_SPIN_BOX_SINGLE_STEP,
            &response)))
            {
               m_singleStep = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_singleStep : int32();
    }

    bool setPageStep(int32 pageStep,
                     bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(INTERFACE_SET_BIN_SPIN_BOX_PAGE_STEP,
        int32(pageStep), waitForResponse)))
        {
            m_pageStep = pageStep;
        }

        return result;
    }

    int32 getPageStep(bool bypassCache = false,
                      bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(INTERFACE_GET_BIN_SPIN_BOX_PAGE_STEP,
            &response)))
            {
               m_pageStep = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_pageStep : int32();
    }

    bool setMinimum(int32 minimum,
                    bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(INTERFACE_SET_BIN_SPIN_BOX_MINIMUM,
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
            receivePacket(INTERFACE_GET_BIN_SPIN_BOX_MINIMUM,
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
        sendPacket(INTERFACE_SET_BIN_SPIN_BOX_MAXIMUM,
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
            receivePacket(INTERFACE_GET_BIN_SPIN_BOX_MAXIMUM,
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
        sendPacket(INTERFACE_SET_BIN_SPIN_BOX_VALUE,
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
            receivePacket(INTERFACE_GET_BIN_SPIN_BOX_VALUE,
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

    bool setSpecialValueText(const char *specialValueText,
                             bool waitForResponse = false)
    {
        return sendPacket(INTERFACE_SET_BIN_SPIN_BOX_SPECIAL_VALUE_TEXT,
        specialValueText, waitForResponse);
    }

    bool getSpecialValueText(char *specialValueText,
                             uint16 size)
    {
        return receivePacket(INTERFACE_GET_BIN_SPIN_BOX_SPECIAL_VALUE_TEXT,
        specialValueText, size);
    }

    bool valueChanged()
    {
        bool temp = m_valueChanged; m_valueChanged = false; return temp;
    }

protected:

    virtual void signalBufferCallBack()
    {
        m_value = fromLittleEndian(m_value); m_valueChanged = true;
    }

private:

    ILInterfaceBinSpinBox(const ILInterfaceBinSpinBox &);
    ILInterfaceBinSpinBox &operator=(const ILInterfaceBinSpinBox &);

    int32 m_singleStep; int32 m_pageStep;
    int32 m_minimum; int32 m_maximum;
    int32 m_value;

    bool m_valueChanged;
};

#endif // ILINTERFACEBINSPINBOX_H

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
