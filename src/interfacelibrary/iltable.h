/***************************************************************************//**
* @file
* Interface Library Table
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

#ifndef ILTABLE_H
#define ILTABLE_H

#include "ilcore.h"

class ILTable : public ILWindow
{

public:

    ILTable() : ILWindow()
    {
        // UNUSED //
    }

    virtual ILNodeType type() const
    {
        return NT_WINDOW_TABLE;
    }

    bool initUndocked(ILCore *parent, const char *name,
                      bool waitForResponse = false)
    {
        bool result = (!m_enabled) && parent && parent->enabled() && name;

        if(result)
        {
            construct(parent);

            if(!(result = sendPacket(TABLE_NEW_UNDOCKED_WINDOW,
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

//            if(!(result = sendPacket(TABLE_NEW_DOCKED_WINDOW,
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

            if(!(result = sendPacket(TABLE_NEW_MAIN_DOCKED_WINDOW,
            name, waitForResponse)))
            {
                deconstruct();
            }
        }

        return result;
    }

    bool clearAll(bool waitForResponse = false)
    {
        return sendPacket(TABLE_CLEAR_ALL,
        waitForResponse);
    }

    bool newItem(uint16 row, uint16 column, const char *text,
                 bool waitForResponse = false)
    {
        uint16 list[] = {row, column};

        return sendPacket(TABLE_NEW_ITEM,
        list, sizeof(list) / sizeof(uint16),
        text, waitForResponse);
    }

//    bool newItem(uint16 row, uint16 column, uint8 value, uint16 base = DEC,
//                 bool waitForResponse = false)
//    {
//        return newItem(row, column, itoa(value, base, false),
//        waitForResponse);
//    }

//    bool newItem(uint16 row, uint16 column, int8 value, uint16 base = DEC,
//                 bool waitForResponse = false)
//    {
//        return newItem(row, column, itoa(value, base, true),
//        waitForResponse);
//    }

//    bool newItem(uint16 row, uint16 column, uint16 value, uint16 base = DEC,
//                 bool waitForResponse = false)
//    {
//        return newItem(row, column, itoa(value, base, false),
//        waitForResponse);
//    }

//    bool newItem(uint16 row, uint16 column, int16 value, uint16 base = DEC,
//                 bool waitForResponse = false)
//    {
//        return newItem(row, column, itoa(value, base, true),
//        waitForResponse);
//    }

//    bool newItem(uint16 row, uint16 column, uint32 value, uint16 base = DEC,
//                 bool waitForResponse = false)
//    {
//        return newItem(row, column, itoa(value, base, false),
//        waitForResponse);
//    }

    bool newItem(uint16 row, uint16 column, int32 value, uint16 base = DEC,
                 bool waitForResponse = false)
    {
        return newItem(row, column, itoa(value, base, true),
        waitForResponse);
    }

    // TODO: Real float support...
    bool newItemF(uint16 row, uint16 column, float32 value,
                  bool waitForResponse = false)
    {
        return newItem(row, column, int32(value), DEC,
        waitForResponse);
    }

    bool deleteItem(uint16 row, uint16 column,
                    bool waitForResponse = false)
    {
        uint16 list[] = {row, column};

        return sendPacket(TABLE_DELETE_ITEM,
        list, sizeof(list) / sizeof(uint16),
        waitForResponse);
    }

    bool deleteItems(bool waitForResponse = false)
    {
        return sendPacket(TABLE_DELETE_ITEMS,
        waitForResponse);
    }

    bool deleteItemsInRow(uint16 row,
                          bool waitForResponse = false)
    {
        return sendPacket(TABLE_DELETE_ITEMS_IN_ROW,
        row, waitForResponse);
    }

    bool deleteItemsInColumn(uint16 column,
                             bool waitForResponse = false)
    {
        return sendPacket(TABLE_DELETE_ITEMS_IN_COLUMN,
        column, waitForResponse);
    }

    bool setItemText(uint16 row, uint16 column, const char *text,
                     bool waitForResponse = false)
    {
        uint16 list[] = {row, column};

        return sendPacket(TABLE_SET_ITEM_TEXT,
        list,  sizeof(list) / sizeof(uint16),
        text, waitForResponse);
    }

//    bool setItemText(uint16 row, uint16 column, uint8 value, uint16 base = DEC,
//                     bool waitForResponse = false)
//    {
//        return setItemText(row, column, itoa(value, base, false),
//        waitForResponse);
//    }

//    bool setItemText(uint16 row, uint16 column, int8 value, uint16 base = DEC,
//                     bool waitForResponse = false)
//    {
//        return setItemText(row, column, itoa(value, base, true),
//        waitForResponse);
//    }

//    bool setItemText(uint16 row, uint16 column, uint16 value, uint16 base = DEC,
//                     bool waitForResponse = false)
//    {
//        return setItemText(row, column, itoa(value, base, false),
//        waitForResponse);
//    }

//    bool setItemText(uint16 row, uint16 column, int16 value, uint16 base = DEC,
//                     bool waitForResponse = false)
//    {
//        return setItemText(row, column, itoa(value, base, true),
//        waitForResponse);
//    }

//    bool setItemText(uint16 row, uint16 column, uint32 value, uint16 base = DEC,
//                     bool waitForResponse = false)
//    {
//        return setItemText(row, column, itoa(value, base, false),
//        waitForResponse);
//    }

    bool setItemText(uint16 row, uint16 column, int32 value, uint16 base = DEC,
                     bool waitForResponse = false)
    {
        return setItemText(row, column, itoa(value, base, true),
        waitForResponse);
    }

    // TODO: Real float support...
    bool setItemTextF(uint16 row, uint16 column, float32 value,
                      bool waitForResponse = false)
    {
        return setItemText(row, column, int32(value), DEC,
        waitForResponse);
    }

    bool getItemText(uint16 row, uint16 column, char *text,
                     uint16 size)
    {
        uint16 list[] = {row, column};

        return receivePacket(TABLE_GET_ITEM_TEXT,
        list, sizeof(list) / sizeof(uint16),
        text, size);
    }

    // Row ////////////////////////////////////////////////////////////////////

    bool newRow(uint16 row, const char *name,
                bool waitForResponse = false)
    {
        return sendPacket(TABLE_NEW_ROW,
        row, name, waitForResponse);
    }

//    bool newRow(uint16 row, uint8 value, uint16 base = DEC,
//                bool waitForResponse = false)
//    {
//        return newRow(row, itoa(value, base, false),
//        waitForResponse);
//    }

//    bool newRow(uint16 row, int8 value, uint16 base = DEC,
//                bool waitForResponse = false)
//    {
//        return newRow(row, itoa(value, base, true),
//        waitForResponse);
//    }

//    bool newRow(uint16 row, uint16 value, uint16 base = DEC,
//                bool waitForResponse = false)
//    {
//        return newRow(row, itoa(value, base, false),
//        waitForResponse);
//    }

//    bool newRow(uint16 row, int16 value, uint16 base = DEC,
//                bool waitForResponse = false)
//    {
//        return newRow(row, itoa(value, base, true),
//        waitForResponse);
//    }

//    bool newRow(uint16 row, uint32 value, uint16 base = DEC,
//                bool waitForResponse = false)
//    {
//        return newRow(row, itoa(value, base, false),
//        waitForResponse);
//    }

    bool newRow(uint16 row, int32 value, uint16 base = DEC,
                bool waitForResponse = false)
    {
        return newRow(row, itoa(value, base, true),
        waitForResponse);
    }

    // TODO: Real float support...
    bool newRowF(uint16 row, float32 value,
                 bool waitForResponse = false)
    {
        return newRow(row, int32(value), DEC,
        waitForResponse);
    }

    bool deleteRow(uint16 row,
                   bool waitForResponse = false)
    {
        return sendPacket(TABLE_DELETE_ROW,
        row, waitForResponse);
    }

    bool deleteRows(bool waitForResponse = false)
    {
        return sendPacket(TABLE_DELETE_ROWS,
        waitForResponse);
    }

    bool setRowName(uint16 row, const char *name,
                    bool waitForResponse = false)
    {
        return sendPacket(TABLE_SET_ROW_NAME,
        row, name, waitForResponse);
    }

//    bool setRowName(uint16 row, uint8 value, uint16 base = DEC,
//                    bool waitForResponse = false)
//    {
//        return setRowName(row, itoa(value, base, false),
//        waitForResponse);
//    }

//    bool setRowName(uint16 row, int8 value, uint16 base = DEC,
//                    bool waitForResponse = false)
//    {
//        return setRowName(row, itoa(value, base, true),
//        waitForResponse);
//    }

//    bool setRowName(uint16 row, uint16 value, uint16 base = DEC,
//                    bool waitForResponse = false)
//    {
//        return setRowName(row, itoa(value, base, false),
//        waitForResponse);
//    }

//    bool setRowName(uint16 row, int16 value, uint16 base = DEC,
//                    bool waitForResponse = false)
//    {
//        return setRowName(row, itoa(value, base, true),
//        waitForResponse);
//    }

//    bool setRowName(uint16 row, uint32 value, uint16 base = DEC,
//                    bool waitForResponse = false)
//    {
//        return setRowName(row, itoa(value, base, false),
//        waitForResponse);
//    }

    bool setRowName(uint16 row, int32 value, uint16 base = DEC,
                    bool waitForResponse = false)
    {
        return setRowName(row, itoa(value, base, true),
        waitForResponse);
    }

    // TODO: Real float support...
    bool setRowNameF(uint16 row, float value,
                     bool waitForResponse = false)
    {
        return setRowName(row, int32(value), DEC,
        waitForResponse);
    }

    bool getRowName(uint16 row, char *name,
                    uint16 size)
    {
        return receivePacket(TABLE_GET_ROW_NAME,
        row, name, size);
    }

    // Column /////////////////////////////////////////////////////////////////

    bool newColumn(uint16 column, const char *name,
                   bool waitForResponse = false)
    {
        return sendPacket(TABLE_NEW_COLUMN,
        column, name, waitForResponse);
    }

//    bool newColumn(uint16 column, uint8 value, uint16 base = DEC,
//                   bool waitForResponse = false)
//    {
//        return newColumn(column, itoa(value, base, false),
//        waitForResponse);
//    }

//    bool newColumn(uint16 column, int8 value, uint16 base = DEC,
//                   bool waitForResponse = false)
//    {
//        return newColumn(column, itoa(value, base, true),
//        waitForResponse);
//    }

//    bool newColumn(uint16 column, uint16 value, uint16 base = DEC,
//                   bool waitForResponse = false)
//    {
//        return newColumn(column, itoa(value, base, false),
//        waitForResponse);
//    }

//    bool newColumn(uint16 column, int16 value, uint16 base = DEC,
//                   bool waitForResponse = false)
//    {
//        return newColumn(column, itoa(value, base, true),
//        waitForResponse);
//    }

//    bool newColumn(uint16 column, uint32 value, uint16 base = DEC,
//                   bool waitForResponse = false)
//    {
//        return newColumn(column, itoa(value, base, false),
//        waitForResponse);
//    }

    bool newColumn(uint16 column, int32 value, uint16 base = DEC,
                   bool waitForResponse = false)
    {
        return newColumn(column, itoa(value, base, true),
        waitForResponse);
    }

    // TODO: Real float support...
    bool newColumnF(uint16 column, float32 value,
                    bool waitForResponse = false)
    {
        return newColumn(column, int32(value), DEC,
        waitForResponse);
    }

    bool deleteColumn(uint16 column,
                      bool waitForResponse = false)
    {
        return sendPacket(TABLE_DELETE_COLUMN,
        column, waitForResponse);
    }

    bool deleteColumns(bool waitForResponse = false)
    {
        return sendPacket(TABLE_DELETE_COLUMNS,
        waitForResponse);
    }

    bool setColumnName(uint16 column, const char *name,
                       bool waitForResponse = false)
    {
        return sendPacket(TABLE_SET_COLUMN_NAME,
        column, name, waitForResponse);
    }

//    bool setColumnName(uint16 column, uint8 value, uint16 base = DEC,
//                       bool waitForResponse = false)
//    {
//        return setColumnName(column, itoa(value, base, false),
//        waitForResponse);
//    }

//    bool setColumnName(uint16 column, int8 value, uint16 base = DEC,
//                       bool waitForResponse = false)
//    {
//        return setColumnName(column, itoa(value, base, true),
//        waitForResponse);
//    }

//    bool setColumnName(uint16 column, uint16 value, uint16 base = DEC,
//                       bool waitForResponse = false)
//    {
//        return setColumnName(column, itoa(value, base, false),
//        waitForResponse);
//    }

//    bool setColumnName(uint16 column, int16 value, uint16 base = DEC,
//                       bool waitForResponse = false)
//    {
//        return setColumnName(column, itoa(value, base, true),
//        waitForResponse);
//    }

//    bool setColumnName(uint16 column, uint32 value, uint16 base = DEC,
//                       bool waitForResponse = false)
//    {
//        return setColumnName(column, itoa(value, base, false),
//        waitForResponse);
//    }

    bool setColumnName(uint16 column, int32 value, uint16 base = DEC,
                       bool waitForResponse = false)
    {
        return setColumnName(column, itoa(value, base, true),
        waitForResponse);
    }

    // TODO: Real float support...
    bool setColumnNameF(uint16 column, float32 value,
                        bool waitForResponse = false)
    {
        return setColumnName(column, int32(value), DEC,
        waitForResponse);
    }

    bool getColumnName(uint16 column, char *name,
                       uint16 size)
    {
        return receivePacket(TABLE_GET_COLUMN_NAME,
        column, name, size);
    }

private:

    ILTable(const ILTable &);
    ILTable &operator=(const ILTable &);
};

#endif // ILTABLE_H

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
