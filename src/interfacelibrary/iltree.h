/***************************************************************************//**
* @file
* Interface Library Tree
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

#ifndef ILTREE_H
#define ILTREE_H

#include "ilcore.h"

class ILTree : public ILWindow
{

public:

    ILTree() : ILWindow()
    {
        // UNUSED //
    }

    virtual ILNodeType type() const
    {
        return NT_WINDOW_TREE;
    }

    bool initUndocked(ILCore *parent, const char *name,
                      bool waitForResponse = false)
    {
        bool result = (!m_enabled) && parent && parent->enabled() && name;

        if(result)
        {
            construct(parent);

            if(!(result = sendPacket(TREE_NEW_UNDOCKED_WINDOW,
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

//            if(!(result = sendPacket(TREE_NEW_DOCKED_WINDOW,
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

            if(!(result = sendPacket(TREE_NEW_MAIN_DOCKED_WINDOW,
            name, waitForResponse)))
            {
                deconstruct();
            }
        }

        return result;
    }

    bool clearAll(bool waitForResponse = false)
    {
        return sendPacket(TREE_CLEAR_ALL,
        waitForResponse);
    }

    bool newItem(const char *key, const char *value,
                 bool waitForResponse = false)
    {
        const char *list[] = {key, value};

        return sendPacket(TREE_NEW_ITEM,
        list, sizeof(list) / sizeof(const char *),
        waitForResponse);
    }

//    bool newItem(const char *key, uint8 value, uint16 base = DEC,
//                 bool waitForResponse = false)
//    {
//        return newItem(key, itoa(value, base, false),
//        waitForResponse);
//    }

//    bool newItem(const char *key, int8 value, uint16 base = DEC,
//                 bool waitForResponse = false)
//    {
//        return newItem(key, itoa(value, base, true),
//        waitForResponse);
//    }

//    bool newItem(const char *key, uint16 value, uint16 base = DEC,
//                 bool waitForResponse = false)
//    {
//        return newItem(key, itoa(value, base, false),
//        waitForResponse);
//    }

//    bool newItem(const char *key, int16 value, uint16 base = DEC,
//                 bool waitForResponse = false)
//    {
//        return newItem(key, itoa(value, base, true),
//        waitForResponse);
//    }

//    bool newItem(const char *key, uint32 value, uint16 base = DEC,
//                 bool waitForResponse = false)
//    {
//        return newItem(key, itoa(value, base, false),
//        waitForResponse);
//    }

    bool newItem(const char *key, int32 value, uint16 base = DEC,
                 bool waitForResponse = false)
    {
        return newItem(key, itoa(value, base, true),
        waitForResponse);
    }

    // TODO: Real float support...
    bool newItemF(const char *key, float32 value,
                  bool waitForResponse = false)
    {
        return newItem(key, int32(value), DEC,
        waitForResponse);
    }

    bool deleteItem(const char *key,
                    bool waitForResponse = false)
    {
        return sendPacket(TREE_DELETE_ITEM,
        key, waitForResponse);
    }

    bool deleteItems(bool waitForResponse = false)
    {
        return sendPacket(TREE_DELETE_ITEMS,
        waitForResponse);
    }

    bool setItemValue(const char *key, const char *value,
                      bool waitForResponse = false)
    {
        const char *list[] = {key, value};

        return sendPacket(TREE_SET_ITEM_VALUE,
        list, sizeof(list) / sizeof(const char *),
        waitForResponse);
    }

//    bool setItemValue(const char *key, uint8 value, uint16 base = DEC,
//                      bool waitForResponse = false)
//    {
//        return setItemValue(key, itoa(value, base, false),
//        waitForResponse);
//    }

//    bool setItemValue(const char *key, int8 value, uint16 base = DEC,
//                      bool waitForResponse = false)
//    {
//        return setItemValue(key, itoa(value, base, true),
//        waitForResponse);
//    }

//    bool setItemValue(const char *key, uint16 value, uint16 base = DEC,
//                      bool waitForResponse = false)
//    {
//        return setItemValue(key, itoa(value, base, false),
//        waitForResponse);
//    }

//    bool setItemValue(const char *key, int16 value, uint16 base = DEC,
//                      bool waitForResponse = false)
//    {
//        return setItemValue(key, itoa(value, base, true),
//        waitForResponse);
//    }

//    bool setItemValue(const char *key, uint32 value, uint16 base = DEC,
//                      bool waitForResponse = false)
//    {
//        return setItemValue(key, itoa(value, base, false),
//        waitForResponse);
//    }

    bool setItemValue(const char *key, int32 value, uint16 base = DEC,
                      bool waitForResponse = false)
    {
        return setItemValue(key, itoa(value, base, true),
        waitForResponse);
    }

    // TODO: Real float support...
    bool setItemValueF(const char *key, float32 value,
                       bool waitForResponse = false)
    {
        return setItemValue(key, int32(value), DEC,
        waitForResponse);
    }

    bool getItemValue(const char *key, char *value,
                      uint16 size)
    {
        return receivePacket(TREE_GET_ITEM_VALUE,
        key, value, size);
    }

private:

    ILTree(const ILTree &);
    ILTree &operator=(const ILTree &);
};

#endif // ILTREE_H

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
