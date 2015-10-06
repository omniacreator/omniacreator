/***************************************************************************//**
* @file
* Interface Library Interface Group Box
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

#ifndef ILINTERFACEGROUPBOX_H
#define ILINTERFACEGROUPBOX_H

#include "ilinterface.h"

class ILInterfaceIBox : public ILNode
{

public:

    ILInterfaceIBox() : ILNode()
    {
        // UNUSED //
    }

protected:

    virtual uint16 handlesSize() const
    {
        return 3;
    }

    virtual void construct(ILNode *parent)
    {
        if(parent && parent->parent() && parent->parent()->parent())
        {
            ILNode::construct(parent);

            m_handles[0] = parent->parent()->handle();
            m_handles[1] = parent->handle();
            m_handles[2] = m_handle;
        }
    }

private:

    ILInterfaceIBox(const ILInterfaceIBox &);
    ILInterfaceIBox &operator=(const ILInterfaceIBox &);

    friend class ILInterfaceGroupBox;
};

class ILInterfaceGroupBox : public ILInterfaceGBox
{

public:

    ILInterfaceGroupBox() : ILInterfaceGBox()
    {
        // UNUSED //
    }

    virtual ILNodeType type() const
    {
        return NT_WINDOW_INTERFACE_GBOX_GROUP_BOX;
    }

    bool init(ILInterface *parent, const char *name,
              bool waitForResponse = false)
    {
        bool result = (!m_enabled) && parent && parent->enabled() && name;

        if(result)
        {
            construct(parent);

            if(!(result = sendPacket(INTERFACE_NEW_GROUP_BOX,
            name, waitForResponse)))
            {
                deconstruct();
            }
        }

        return result;
    }

    bool deinitInterfaceBox(ILInterfaceIBox *ibox,
                            bool waitForResponse = false)
    {
        bool result = ibox && ibox->enabled() && (ibox->parent() == this);

        if(result)
        {
            if((result = sendPacket(INTERFACE_DELETE_INTERFACE_BOX,
            ibox->handle(), waitForResponse)))
            {
                ibox->deconstruct();
            }
        }

        return result;
    }

    bool deinitAllInterfaceBoxes(bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(INTERFACE_DELETE_ALL_INTERFACE_BOXES_0,
        waitForResponse)))
        {
            deconstructChildren();
        }

        return result;
    }

    // Properties /////////////////////////////////////////////////////////////

    bool setName(const char *name,
                 bool waitForResponse = false)
    {
        return sendPacket(INTERFACE_SET_GROUP_BOX_NAME,
        name, waitForResponse);
    }

    bool getName(char *name,
                 uint16 size)
    {
        return receivePacket(INTERFACE_GET_GROUP_BOX_NAME,
        name, size);
    }

private:

    ILInterfaceGroupBox(const ILInterfaceGroupBox &);
    ILInterfaceGroupBox &operator=(const ILInterfaceGroupBox &);
};

#endif // ILINTERFACEGROUPBOX_H

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
