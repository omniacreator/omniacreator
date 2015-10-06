/***************************************************************************//**
* @file
* Serial Table
*
* @version @n 1.0
* @date @n 1/9/2014
*
* @author @n Kwabena W. Agyeman
* @copyright @n (c) 2014 Kwabena W. Agyeman
* @n All rights reserved - Please see the end of the file for the terms of use
*
* @par Update History:
* @n v1.0 - Original release - 1/9/2014
*******************************************************************************/

#ifndef SERIALTABLE_H
#define SERIALTABLE_H

#include <QtCore>
#include <QtWidgets>

#include "serialwindow.h"

namespace Ui
{
    class SerialTable;
}

class SerialTable : public SerialWindow
{
    Q_OBJECT

public:

    explicit SerialTable(const QString &title,
                         QSettings *settings = NULL,
                         QWidget *parent = NULL);

    virtual ~SerialTable();

    virtual bool importExportEnabled() const;

    virtual QString keyGroup() const;
    virtual QString keyState() const;
    virtual QString keyGeometry() const;

    bool newRow(int row, const QString &name);
    bool setRowName(int row, const QString &name);
    QString getRowName(int row, bool *ok);
    bool deleteRow(int row);
    void deleteRows();

    bool newColumn(int column, const QString &name);
    bool setColumnName(int column, const QString &name);
    QString getColumnName(int column, bool *ok);
    bool deleteColumn(int column);
    void deleteColumns();

    bool newItem(int row, int column, const QString &text);
    bool setItemText(int row, int column, const QString &text);
    QString getItemText(int row, int column, bool *ok);
    bool deleteItem(int row, int column);
    void deleteItems();

    bool deleteItemsInRow(int row);
    bool deleteItemsInColumn(int column);

    void clearAll();

public slots:

    virtual void importState(const QString &fileName = QString());
    virtual void exportState(const QString &fileName = QString());

    void exportToCSV(const QString &fileName = QString());

private slots:

    void findItem();

    void generalHelp();
    void tableHelp();

private:

    Q_DISABLE_COPY(SerialTable)

    QTableWidgetItem *itemExists(const QString &FName,
                                 int row,
                                 int column);

    bool itemDoesNotExist(const QString &FName,
                          int row,
                          int column,
                          int *rowIndex,
                          int *columnIndex);

    QTableWidgetItem *rowHeaderExists(const QString &FName,
                                      int row);

    QTableWidgetItem *columnHeaderExists(const QString &FName,
                                         int column);

    bool rowHeaderDoesNotExist(const QString &FName,
                               int row);

    bool columnHeaderDoesNotExist(const QString &FName,
                                  int column);

    QMap<int, QTableWidgetItem *> m_rowHeaders, m_columnHeaders;

    QString m_findString;

    Ui::SerialTable *m_ui;
};

#endif // SERIALTABLE_H

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
