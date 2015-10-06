/***************************************************************************//**
* @file
* Interface Library Interface Time Edit
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

#ifndef ILINTERFACETIMEEDIT_H
#define ILINTERFACETIMEEDIT_H

#include "ilinterfacegroupbox.h"

class ILInterfaceTimeEdit : public ILInterfaceIBox
{

public:

    ILInterfaceTimeEdit(char *valueChangedStringStorage = 0,
                        uint16 valueChangedStringStorageSize = 0) :
                        ILInterfaceIBox()
    {
        if(valueChangedStringStorage && valueChangedStringStorageSize)
        {
            valueChangedStringStorage[valueChangedStringStorageSize -= 1] = 0;
        }

        m_valueChanged = false;

        m_signalBuffer = reinterpret_cast<uint8 *>(valueChangedStringStorage);
        m_signalBufferSize = valueChangedStringStorageSize;
    }

    virtual ILNodeType type() const
    {
        return NT_WINDOW_INTERFACE_IBOX_TIME_EDIT;
    }

    bool init(ILInterfaceGBox *parent, const char *name,
              bool waitForResponse = false)
    {
        bool result = (!m_enabled) && parent && parent->enabled() && name;

        if(result)
        {
            construct(parent);

            if(!(result = sendPacket(INTERFACE_NEW_TIME_EDIT,
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
        return sendPacket(INTERFACE_RESET_TIME_EDIT,
        waitForResponse);
    }

    bool setName(const char *name,
                 bool waitForResponse = false)
    {
        return sendPacket(INTERFACE_SET_TIME_EDIT_NAME,
        name, waitForResponse);
    }

    bool getName(char *name,
                 uint16 size)
    {
        return receivePacket(INTERFACE_GET_TIME_EDIT_NAME,
        name, size);
    }

    bool setMinimum(const char *minimum,
                    bool waitForResponse = false)
    {
        return sendPacket(INTERFACE_SET_TIME_EDIT_MINIMUM,
        minimum, waitForResponse);
    }

    bool getMinimum(char *minimum,
                    uint16 size)
    {
        return receivePacket(INTERFACE_GET_TIME_EDIT_MINIMUM,
        minimum, size);
    }

    bool setMaximum(const char *maximum,
                    bool waitForResponse = false)
    {
        return sendPacket(INTERFACE_SET_TIME_EDIT_MAXIMUM,
        maximum, waitForResponse);
    }

    bool getMaximum(char *maximum,
                    uint16 size)
    {
        return receivePacket(INTERFACE_GET_TIME_EDIT_MAXIMUM,
        maximum, size);
    }

    bool setValue(const char *value,
                  bool waitForResponse = false)
    {
        return sendPacket(INTERFACE_SET_TIME_EDIT_VALUE,
        value, waitForResponse);
    }

    bool getValue(char *value,
                  uint16 size)
    {
        return receivePacket(INTERFACE_GET_TIME_EDIT_VALUE,
        value, size);
    }

    bool setSpecialValueText(const char *specialValueText,
                             bool waitForResponse = false)
    {
        return sendPacket(INTERFACE_SET_TIME_EDIT_SPECIAL_VALUE_TEXT,
        specialValueText, waitForResponse);
    }

    bool getSpecialValueText(char *specialValueText,
                             uint16 size)
    {
        return receivePacket(INTERFACE_GET_TIME_EDIT_SPECIAL_VALUE_TEXT,
        specialValueText, size);
    }

    bool valueChanged()
    {
        bool temp = m_valueChanged; m_valueChanged = false; return temp;
    }

protected:

    virtual void signalBufferCallBack()
    {
        m_valueChanged = true;
    }

private:

    ILInterfaceTimeEdit(const ILInterfaceTimeEdit &);
    ILInterfaceTimeEdit &operator=(const ILInterfaceTimeEdit &);

    bool m_valueChanged;
};

#endif // ILINTERFACETIMEEDIT_H

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
