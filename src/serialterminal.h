/***************************************************************************//**
* @file
* Serial Terminal
*
* @version @n 1.0
* @date @n 10/17/2013
*
* @author @n Kwabena W. Agyeman
* @copyright @n (c) 2013 Kwabena W. Agyeman
* @n All rights reserved - Please see the end of the file for the terms of use
*
* @par Update History:
* @n v1.0 - Original release - 10/17/2013
*******************************************************************************/

#ifndef SERIALTERMINAL_H
#define SERIALTERMINAL_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "serialwindow.h"

#define TERMINAL_MAX_FILE_SIZE ONE_MEGABYTE
#define TERMINAL_MAX_LINE_SIZE (1000 * 1000)

#define TERMINAL_SHIFT_REG 5 // for utf-8

#define TERMINAL_TAB_WIDTH 8

namespace Ui
{
    class SerialTerminal;
}

class SerialTerminal : public SerialWindow
{
    Q_OBJECT

public:

    explicit SerialTerminal(const QString &title,
                            QSettings *settings = NULL,
                            QWidget *parent = NULL);

    virtual ~SerialTerminal();

    virtual bool importExportEnabled() const;

    virtual QString keyGroup() const;
    virtual QString keyState() const;
    virtual QString keyGeometry() const;

    void setTabWidth(int tabWidth);
    int getTabWidth();

    void receive(const QByteArray &bytes);

public slots:

    void userReset();

private slots:

    void returnPressed();
    void clearScreen();

    void sendTextFile();
    void saveTextFile();

    void generalHelp();
    void terminalHelp();

signals:

    void transmit(const QByteArray &bytes, QWidget *widget);

private:

    Q_DISABLE_COPY(SerialTerminal)

    void insertText(const QString &text);

    QTextCursor m_textCursor; QBrush m_brushColor;

    enum
    {
        ASCII,
        UTF8,
        EXIT_0,
        EXIT_1
    }
    m_stateMachine;

    QByteArray m_shiftReg; int m_tabWidth;

    Ui::SerialTerminal *m_ui;
};

#endif // SERIALTERMINAL_H

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
