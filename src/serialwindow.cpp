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

#include "serialwindow.h"

SerialWindow::SerialWindow(const QString &title, int windowHandle,
QSettings *settings, QWidget *parent) : QDockWidget(parent)
{
    setWindowTitle(title); m_windowMessage = NULL;

    m_windowHandle = windowHandle; m_settings = settings; m_wasShown = false;
}

SerialWindow::SerialWindow(const QString &title,
QSettings *settings, QWidget *parent) : QDockWidget(parent)
{
    setWindowTitle(title); m_windowMessage = NULL;

    m_windowHandle = int(); m_settings = settings; m_wasShown = false;
}

SerialWindow::~SerialWindow()
{
    if(m_windowMessage)
    {
        delete m_windowMessage;
    }
}

void SerialWindow::restoreState()
{
    QSettings mySettings(settingsFileName(), settingsFormat());

    mySettings.beginGroup(keyGroup());
    mySettings.beginGroup(windowTitle());

    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(widget());

    if(mainWindow)
    {
        if(mySettings.contains(keyState()))
        {
            mainWindow->restoreState(mySettings.value(keyState()).
                                     toByteArray());
        }
    }

    if(mySettings.contains(keyGeometry()))
    {
        restoreGeometry(mySettings.value(keyGeometry()).
                        toByteArray());
    }

    if(mainWindow && (!m_windowMessage))
    {
        m_windowMessage = new QLabel;

        m_windowMessage->setOpenExternalLinks(true);
        m_windowMessage->setWordWrap(true);

        mainWindow->statusBar()->addWidget(m_windowMessage);

        connect(m_windowMessage, SIGNAL(destroyed()),
                mainWindow->statusBar(), SLOT(hide()));
    }
}

void SerialWindow::setWindowTitle(const QString &title)
{
    QString temp = QString(title).
    replace('\\', '_').replace('/', '_');

    if(temp.isEmpty())
    {
        temp = "\"\"";
    }

    QDockWidget::setWindowTitle(temp); setObjectName(temp);
}

QString SerialWindow::windowTitle() const
{
    QString temp = QDockWidget::windowTitle().
    replace('\\', '_').replace('/', '_');

    if(temp.isEmpty())
    {
        temp = "\"\"";
    }

    return temp;
}

void SerialWindow::setWindowMessage(const QString &message)
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(widget());

    if(mainWindow)
    {
        mainWindow->statusBar()->setVisible((!message.isEmpty())
                                            && m_windowMessage);
    }

    if(m_windowMessage)
    {
        m_windowMessage->setText(message);
    }
}

QString SerialWindow::getWindowMessage() const
{
    return m_windowMessage ? m_windowMessage->text() : QString();
}

void SerialWindow::setWindowHandle(int windowHandle)
{
    m_windowHandle = windowHandle;
}

int SerialWindow::getWindowHandle() const
{
    return m_windowHandle;
}

void SerialWindow::setSettings(QSettings *settings)
{
    m_settings = settings;
}

QSettings *SerialWindow::getSettings() const
{
    return m_settings;
}

void SerialWindow::setWasShown(bool wasShown)
{
    m_wasShown = wasShown;
}

bool SerialWindow::getWasShown() const
{
    return m_wasShown;
}

QString SerialWindow::settingsFileName() const
{
    return m_settings ? m_settings->fileName() : QString();
}

QSettings::Format SerialWindow::settingsFormat() const
{
    return m_settings ? m_settings->format() : QSettings::Format();
}

bool SerialWindow::addDockWidget(QDockWidget *dockWidget)
{
    return addDockWidget(Qt::RightDockWidgetArea, dockWidget);
}

bool SerialWindow::addDockWidget(Qt::DockWidgetArea area,
                                 QDockWidget *dockWidget)
{
    switch(area)
    {
        case Qt::LeftDockWidgetArea:
        {
            return addDockWidget(area,CA_LEFT_MIDDLE_TOP_BOTTOM,dockWidget);
        }

        case Qt::RightDockWidgetArea:
        {
            return addDockWidget(area,CA_RIGHT_MIDDLE_TOP_BOTTOM,dockWidget);
        }

        case Qt::TopDockWidgetArea:
        {
            return addDockWidget(area,CA_TOP_MIDDLE_LEFT_RIGHT,dockWidget);
        }

        case Qt::BottomDockWidgetArea:
        {
            return addDockWidget(area,CA_BOTTOM_MIDDLE_LEFT_RIGHT,dockWidget);
        }

        default: return false;
    }
}

bool SerialWindow::addDockWidget(Qt::DockWidgetArea area,
                                 CornerArea corner,
                                 QDockWidget *dockWidget)
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(widget());

    if(mainWindow)
    {
        switch(area)
        {
            case Qt::LeftDockWidgetArea:
            {
                switch(corner)
                {
                    case CA_LEFT_MIDDLE:
                    {
                        mainWindow->setCorner(Qt::TopLeftCorner,
                                              Qt::TopDockWidgetArea);

                        mainWindow->setCorner(Qt::BottomLeftCorner,
                                              Qt::BottomDockWidgetArea);

                        mainWindow->addDockWidget(area, dockWidget);

                        return true;
                    }

                    case CA_LEFT_MIDDLE_TOP:
                    {
                        mainWindow->setCorner(Qt::TopLeftCorner,
                                              Qt::LeftDockWidgetArea);

                        mainWindow->setCorner(Qt::BottomLeftCorner,
                                              Qt::BottomDockWidgetArea);

                        mainWindow->addDockWidget(area, dockWidget);

                        return true;
                    }

                    case CA_LEFT_MIDDLE_BOTTOM:
                    {
                        mainWindow->setCorner(Qt::TopLeftCorner,
                                              Qt::TopDockWidgetArea);

                        mainWindow->setCorner(Qt::BottomLeftCorner,
                                              Qt::LeftDockWidgetArea);

                        mainWindow->addDockWidget(area, dockWidget);

                        return true;
                    }

                    case CA_LEFT_MIDDLE_TOP_BOTTOM:
                    {
                        mainWindow->setCorner(Qt::TopLeftCorner,
                                              Qt::LeftDockWidgetArea);

                        mainWindow->setCorner(Qt::BottomLeftCorner,
                                              Qt::LeftDockWidgetArea);

                        mainWindow->addDockWidget(area, dockWidget);

                        return true;
                    }

                    default: return false;
                }

                return false;
            }

            case Qt::RightDockWidgetArea:
            {
                switch(corner)
                {
                    case CA_RIGHT_MIDDLE:
                    {
                        mainWindow->setCorner(Qt::TopRightCorner,
                                              Qt::TopDockWidgetArea);

                        mainWindow->setCorner(Qt::BottomRightCorner,
                                              Qt::BottomDockWidgetArea);

                        mainWindow->addDockWidget(area, dockWidget);

                        return true;
                    }

                    case CA_RIGHT_MIDDLE_TOP:
                    {
                        mainWindow->setCorner(Qt::TopRightCorner,
                                              Qt::RightDockWidgetArea);

                        mainWindow->setCorner(Qt::BottomRightCorner,
                                              Qt::BottomDockWidgetArea);

                        mainWindow->addDockWidget(area, dockWidget);

                        return true;
                    }

                    case CA_RIGHT_MIDDLE_BOTTOM:
                    {
                        mainWindow->setCorner(Qt::TopRightCorner,
                                              Qt::TopDockWidgetArea);

                        mainWindow->setCorner(Qt::BottomRightCorner,
                                              Qt::RightDockWidgetArea);

                        mainWindow->addDockWidget(area, dockWidget);

                        return true;
                    }

                    case CA_RIGHT_MIDDLE_TOP_BOTTOM:
                    {
                        mainWindow->setCorner(Qt::TopRightCorner,
                                              Qt::RightDockWidgetArea);

                        mainWindow->setCorner(Qt::BottomRightCorner,
                                              Qt::RightDockWidgetArea);

                        mainWindow->addDockWidget(area, dockWidget);

                        return true;
                    }

                    default: return false;
                }

                return false;
            }

            case Qt::TopDockWidgetArea:
            {
                switch(corner)
                {
                    case CA_TOP_MIDDLE:
                    {
                        mainWindow->setCorner(Qt::TopLeftCorner,
                                              Qt::LeftDockWidgetArea);

                        mainWindow->setCorner(Qt::TopRightCorner,
                                              Qt::RightDockWidgetArea);

                        mainWindow->addDockWidget(area, dockWidget);

                        return true;
                    }

                    case CA_TOP_MIDDLE_LEFT:
                    {
                        mainWindow->setCorner(Qt::TopLeftCorner,
                                              Qt::TopDockWidgetArea);

                        mainWindow->setCorner(Qt::TopRightCorner,
                                              Qt::RightDockWidgetArea);

                        mainWindow->addDockWidget(area, dockWidget);

                        return true;
                    }

                    case CA_TOP_MIDDLE_RIGHT:
                    {
                        mainWindow->setCorner(Qt::TopLeftCorner,
                                              Qt::LeftDockWidgetArea);

                        mainWindow->setCorner(Qt::TopRightCorner,
                                              Qt::TopDockWidgetArea);

                        mainWindow->addDockWidget(area, dockWidget);

                        return true;
                    }

                    case CA_TOP_MIDDLE_LEFT_RIGHT:
                    {
                        mainWindow->setCorner(Qt::TopLeftCorner,
                                              Qt::TopDockWidgetArea);

                        mainWindow->setCorner(Qt::TopRightCorner,
                                              Qt::TopDockWidgetArea);

                        mainWindow->addDockWidget(area, dockWidget);

                        return true;
                    }

                    default: return false;
                }

                return false;
            }

            case Qt::BottomDockWidgetArea:
            {
                switch(corner)
                {
                    case CA_BOTTOM_MIDDLE:
                    {
                        mainWindow->setCorner(Qt::BottomLeftCorner,
                                              Qt::LeftDockWidgetArea);

                        mainWindow->setCorner(Qt::BottomRightCorner,
                                              Qt::RightDockWidgetArea);

                        mainWindow->addDockWidget(area, dockWidget);

                        return true;
                    }

                    case CA_BOTTOM_MIDDLE_LEFT:
                    {
                        mainWindow->setCorner(Qt::BottomLeftCorner,
                                              Qt::BottomDockWidgetArea);

                        mainWindow->setCorner(Qt::BottomRightCorner,
                                              Qt::RightDockWidgetArea);

                        mainWindow->addDockWidget(area, dockWidget);

                        return true;
                    }

                    case CA_BOTTOM_MIDDLE_RIGHT:
                    {
                        mainWindow->setCorner(Qt::BottomLeftCorner,
                                              Qt::LeftDockWidgetArea);

                        mainWindow->setCorner(Qt::BottomRightCorner,
                                              Qt::BottomDockWidgetArea);

                        mainWindow->addDockWidget(area, dockWidget);

                        return true;
                    }

                    case CA_BOTTOM_MIDDLE_LEFT_RIGHT:
                    {
                        mainWindow->setCorner(Qt::BottomLeftCorner,
                                              Qt::BottomDockWidgetArea);

                        mainWindow->setCorner(Qt::BottomRightCorner,
                                              Qt::BottomDockWidgetArea);

                        mainWindow->addDockWidget(area, dockWidget);

                        return true;
                    }

                    default: return false;
                }

                return false;
            }

            default: return false;
        }
    }

    return mainWindow;
}

void SerialWindow::importState(const QString &fileName)
{
    Q_UNUSED(fileName);
}

void SerialWindow::exportState(const QString &fileName)
{
    Q_UNUSED(fileName);
}

bool SerialWindow::event(QEvent *event)
{
    // Begin Qt Maximize Bug Workaround ///////////////////////////////////

    if((event->type() == QEvent::NonClientAreaMouseButtonDblClick)
    && (!parentWidget()))
    {
        if(isMaximized())
        {
            showNormal();
        }
        else
        {
            showMaximized();
        }

        return true;
    }

    // End Qt Maximize Bug Workaround /////////////////////////////////////

    return QDockWidget::event(event);
}

void SerialWindow::showEvent(QShowEvent *event)
{
    m_wasShown = true;

    QDockWidget::showEvent(event);
}

void SerialWindow::closeEvent(QCloseEvent *event)
{
    foreach(QObject *child, children())
    {
        QDialog *dialog = qobject_cast<QDialog *>(child);

        if(dialog)
        {
            dialog->reject();
        }
    }

    if(m_wasShown)
    {
        QSettings settings(settingsFileName(), settingsFormat());

        settings.beginGroup(keyGroup());
        settings.beginGroup(windowTitle());

        QMainWindow *mainWindow = qobject_cast<QMainWindow *>(widget());

        if(mainWindow)
        {
            settings.setValue(keyState(), mainWindow->saveState());
        }

        settings.setValue(keyGeometry(), saveGeometry());
    }

    QDockWidget::closeEvent(event);
}

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
