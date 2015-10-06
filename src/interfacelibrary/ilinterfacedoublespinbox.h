/***************************************************************************//**
* @file
* Interface Library Interface Double Spin Box
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

#ifndef ILINTERFACEDOUBLESPINBOX_H
#define ILINTERFACEDOUBLESPINBOX_H

#include "ilinterfacegroupbox.h"

class ILInterfaceDoubleSpinBox : public ILInterfaceIBox
{

public:

    ILInterfaceDoubleSpinBox() : ILInterfaceIBox()
    {
        m_singleStep = float32(); m_pageStep = float32();
        m_minimum = float32(); m_maximum = float32();
        m_value = float32();

        m_valueChanged = false;
        m_signalBuffer = reinterpret_cast<uint8 *>(&m_value);
        m_signalBufferSize = sizeof(m_value);
    }

    virtual ILNodeType type() const
    {
        return NT_WINDOW_INTERFACE_IBOX_DOUBLE_SPIN_BOX;
    }

    bool init(ILInterfaceGBox *parent, const char *name,
              bool waitForResponse = false)
    {
        bool result = (!m_enabled) && parent && parent->enabled() && name;

        if(result)
        {
            construct(parent);

            if(!(result = sendPacket(INTERFACE_NEW_DOUBLE_SPIN_BOX,
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
        return sendPacket(INTERFACE_RESET_DOUBLE_SPIN_BOX,
        waitForResponse);
    }

    bool setName(const char *name,
                 bool waitForResponse = false)
    {
        return sendPacket(INTERFACE_SET_DOUBLE_SPIN_BOX_NAME,
        name, waitForResponse);
    }

    bool getName(char *name,
                 uint16 size)
    {
        return receivePacket(INTERFACE_GET_DOUBLE_SPIN_BOX_NAME,
        name, size);
    }

    bool setPrefix(const char *prefix,
                   bool waitForResponse = false)
    {
        return sendPacket(INTERFACE_SET_DOUBLE_SPIN_BOX_PREFIX,
        prefix, waitForResponse);
    }

    bool getPrefix(char *prefix,
                   uint16 size)
    {
        return receivePacket(INTERFACE_GET_DOUBLE_SPIN_BOX_PREFIX,
        prefix, size);
    }

    bool setSuffix(const char *suffix,
                   bool waitForResponse = false)
    {
        return sendPacket(INTERFACE_SET_DOUBLE_SPIN_BOX_SUFFIX,
        suffix, waitForResponse);
    }

    bool getSuffix(char *suffix,
                   uint16 size)
    {
        return receivePacket(INTERFACE_GET_DOUBLE_SPIN_BOX_SUFFIX,
        suffix, size);
    }

    bool setSingleStep(float32 singleStep,
                       bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(INTERFACE_SET_DOUBLE_SPIN_BOX_SINGLE_STEP,
        float32(singleStep), waitForResponse)))
        {
            m_singleStep = singleStep;
        }

        return result;
    }

    float32 getSingleStep(bool bypassCache = false,
                          bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(INTERFACE_GET_DOUBLE_SPIN_BOX_SINGLE_STEP,
            &response)))
            {
               m_singleStep = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_singleStep : float32();
    }

    bool setPageStep(float32 pageStep,
                     bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(INTERFACE_SET_DOUBLE_SPIN_BOX_PAGE_STEP,
        float32(pageStep), waitForResponse)))
        {
            m_pageStep = pageStep;
        }

        return result;
    }

    float32 getPageStep(bool bypassCache = false,
                        bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(INTERFACE_GET_DOUBLE_SPIN_BOX_PAGE_STEP,
            &response)))
            {
               m_pageStep = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_pageStep : float32();
    }

    bool setMinimum(float32 minimum,
                    bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(INTERFACE_SET_DOUBLE_SPIN_BOX_MINIMUM,
        float32(minimum), waitForResponse)))
        {
            m_minimum = minimum;
        }

        return result;
    }

    float32 getMinimum(bool bypassCache = false,
                       bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(INTERFACE_GET_DOUBLE_SPIN_BOX_MINIMUM,
            &response)))
            {
               m_minimum = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_minimum : float32();
    }

    bool setMaximum(float32 maximum,
                    bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(INTERFACE_SET_DOUBLE_SPIN_BOX_MAXIMUM,
        float32(maximum), waitForResponse)))
        {
            m_maximum = maximum;
        }

        return result;
    }

    float32 getMaximum(bool bypassCache = false,
                       bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(INTERFACE_GET_DOUBLE_SPIN_BOX_MAXIMUM,
            &response)))
            {
               m_maximum = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_maximum : float32();
    }

    bool setValue(float32 value,
                  bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(INTERFACE_SET_DOUBLE_SPIN_BOX_VALUE,
        float32(value), waitForResponse)))
        {
            m_value = value;
        }

        return result;
    }

    float32 getValue(bool bypassCache = false,
                     bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(INTERFACE_GET_DOUBLE_SPIN_BOX_VALUE,
            &response)))
            {
               m_value = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_value : float32();
    }

    bool setSpecialValueText(const char *specialValueText,
                             bool waitForResponse = false)
    {
        return sendPacket(INTERFACE_SET_DOUBLE_SPIN_BOX_SPECIAL_VALUE_TEXT,
        specialValueText, waitForResponse);
    }

    bool getSpecialValueText(char *specialValueText,
                             uint16 size)
    {
        return receivePacket(INTERFACE_GET_DOUBLE_SPIN_BOX_SPECIAL_VALUE_TEXT,
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

    ILInterfaceDoubleSpinBox(const ILInterfaceDoubleSpinBox &);
    ILInterfaceDoubleSpinBox &operator=(const ILInterfaceDoubleSpinBox &);

    float32 m_singleStep; float32 m_pageStep;
    float32 m_minimum; float32 m_maximum;
    float32 m_value;

    bool m_valueChanged;
};

#endif // ILINTERFACEDOUBLESPINBOX_H

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
