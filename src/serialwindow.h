/***************************************************************************//**
* @file
* Serial Window
*
* @version @n 1.0
* @date @n 1/28/2014
*
* @author @n Kwabena W. Agyeman
* @copyright @n (c) 2014 Kwabena W. Agyeman
* @n All rights reserved - Please see the end of the file for the terms of use
*
* @par Update History:
* @n v1.0 - Original release - 1/28/2014
*******************************************************************************/

#ifndef SERIALWINDOW_H
#define SERIALWINDOW_H

#include <QtCore>
#include <QtWidgets>

#include "ilshared.h"
#include "projectdefines.h"

#ifdef Q_OS_WIN
    #define BY_NAME "USERNAME"
#else
    #define BY_NAME "USER"
#endif

#define ONE_BYTE 0x1
#define ONE_KILOBYTE (ONE_BYTE * 0x400)
#define ONE_MEGABYTE (ONE_KILOBYTE * 0x400)
#define ONE_GIGABYTE (ONE_MEGABYTE * 0x400)

#define UPDATE_WAIT (1000 / 50) // 50 hz

class SerialWindow : public QDockWidget
{
    Q_OBJECT

public:

    SerialWindow(const QString &title = QString(),
                 int windowHandle = int(),
                 QSettings *settings = NULL,
                 QWidget *parent = NULL);

    SerialWindow(const QString &title = QString(),
                 QSettings *settings = NULL,
                 QWidget *parent = NULL);

    virtual ~SerialWindow();

    void restoreState();

    void setWindowTitle(const QString &title); // Hide QWidget setWindowTitle()
    QString windowTitle() const; // Hide QWidget windowTitle()

    void setWindowMessage(const QString &message);
    QString getWindowMessage() const;

    void setWindowHandle(int windowHandle);
    int getWindowHandle() const;

    void setSettings(QSettings *settings);
    QSettings *getSettings() const;

    void setWasShown(bool wasShown);
    bool getWasShown() const;

    virtual bool importExportEnabled() const = 0;

    virtual QString keyGroup() const = 0;
    virtual QString keyState() const = 0;
    virtual QString keyGeometry() const = 0;

    QString settingsFileName() const;
    QSettings::Format settingsFormat() const;

    // Start Deprecated ///////////////////////////////////////////////////////

    bool addDockWidget(QDockWidget *dockWidget);

    bool addDockWidget(Qt::DockWidgetArea area,
                       QDockWidget *dockWidget);

    bool addDockWidget(Qt::DockWidgetArea area,
                       CornerArea corner,
                       QDockWidget *dockWidget);

    // End Deprecated /////////////////////////////////////////////////////////

public slots:

    virtual void importState(const QString &fileName = QString());
    virtual void exportState(const QString &fileName = QString());

signals:

    void errorMessage(const QString &text);

protected:

    virtual bool event(QEvent *event);

    virtual void showEvent(QShowEvent *event);
    virtual void closeEvent(QCloseEvent *event);

private:

    Q_DISABLE_COPY(SerialWindow)

    QPointer<QLabel> m_windowMessage;

    int m_windowHandle; QPointer<QSettings> m_settings; bool m_wasShown;
};

#endif // SERIALWINDOW_H

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
