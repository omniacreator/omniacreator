/***************************************************************************//**
* @file
* Interface Library Interface
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

#ifndef ILINTERFACE_H
#define ILINTERFACE_H

#include "ilcore.h"

class ILInterfaceGBox : public ILNode
{

public:

    ILInterfaceGBox() : ILNode()
    {
        // UNUSED //
    }

protected:

    virtual uint16 handlesSize() const
    {
        return 2;
    }

    virtual void construct(ILNode *parent)
    {
        if(parent && parent->parent())
        {
            ILNode::construct(parent);

            m_handles[0] = parent->handle();
            m_handles[1] = m_handle;
        }
    }

private:

    ILInterfaceGBox(const ILInterfaceGBox &);
    ILInterfaceGBox &operator=(const ILInterfaceGBox &);

    friend class ILInterface;
};

class ILInterface : public ILWindow
{

public:

    ILInterface() : ILWindow()
    {
        // UNUSED //
    }

    virtual ILNodeType type() const
    {
        return NT_WINDOW_INTERFACE;
    }

    bool initUndocked(ILCore *parent, const char *name,
                      bool waitForResponse = false)
    {
        bool result = (!m_enabled) && parent && parent->enabled() && name;

        if(result)
        {
            construct(parent);

            if(!(result = sendPacket(INTERFACE_NEW_UNDOCKED_WINDOW,
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

//            if(!(result = sendPacket(INTERFACE_NEW_DOCKED_WINDOW,
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

            if(!(result = sendPacket(INTERFACE_NEW_MAIN_DOCKED_WINDOW,
            name, waitForResponse)))
            {
                deconstruct();
            }
        }

        return result;
    }

    bool deinitAllInterfaceBoxes(bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(INTERFACE_DELETE_ALL_INTERFACE_BOXES_1,
        waitForResponse)))
        {
            ILNode *next = m_children;

            while(next)
            {
                static_cast<ILInterfaceGBox *>
                (next)->deconstructChildren();
                next = next->next();
            }
        }

        return result;
    }

    bool deinitGroupBox(ILInterfaceGBox *gbox,
                        bool waitForResponse = false)
    {
        bool result = gbox && gbox->enabled() && (gbox->parent() == this);

        if(result)
        {
            if((result = sendPacket(INTERFACE_DELETE_GROUP_BOX,
            gbox->handle(), waitForResponse)))
            {
                gbox->deconstruct();
            }
        }

        return result;
    }

    bool deinitAllGroupBoxes(bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(INTERFACE_DELETE_ALL_GROUP_BOXES,
        waitForResponse)))
        {
            deconstructChildren();
        }

        return result;
    }

private:

    ILInterface(const ILInterface &);
    ILInterface &operator=(const ILInterface &);
};

#endif // ILINTERFACE_H

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
