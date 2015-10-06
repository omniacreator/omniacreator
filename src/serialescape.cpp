/***************************************************************************//**
* @file
* Serial Escape
*
* @version @n 1.0
* @date @n 2/6/2014
*
* @author @n Kwabena W. Agyeman
* @copyright @n (c) 2014 Kwabena W. Agyeman
* @n All rights reserved - Please see the end of the file for the terms of use
*
* @par Update History:
* @n v1.0 - Original release - 2/6/2014
*******************************************************************************/

/***************************************************************************//**
* @file
* @par Documentation Available:
* @n http://www.ftdichip.com/
*    Support/Documents/AppNotes/AN232B-03_D2XXDataThroughput.pdf
* @n http://www.ftdichip.com/
*    Support/Documents/AppNotes/AN232B-04_DataLatencyFlow.pdf
*
* SerialEscape was designed for the blocking FTDI D2XX serial port QFtdi.
* Please do not remove any unnecessary code for a non-blocking serial port
* like QSerialPort, QTCPSocket, QUDPSocket, QSSLSocket, or QBluetoothSocket.
*******************************************************************************/

#include "serialescape.h"

int SerialEscape::transferTimeHint(int baudRate)
{
    double bytesPerSecond = (double(baudRate) / 10.0);
    double transfersPerSecond = (bytesPerSecond / transferSizeHint(baudRate));
    double msecsPerTransfer = (1000.0 / transfersPerSecond);

    return qMin(qCeil(msecsPerTransfer), MAX_POLL_WAIT);
}

int SerialEscape::transferSizeHint(int baudRate)
{
    double bytesPerSecond = (double(baudRate) / 10.0);
    double packetsPerSecond = (bytesPerSecond / 62.0);
    double transfersPerUpdate = (packetsPerSecond / (1000.0 / UPDATE_WAIT));

    return (qCeil(transfersPerUpdate) * 64.0);
}

SerialEscape::SerialEscape(QIODevice *port, QWidget *widget,
QSettings *settings, QObject *parent) : QObject(parent)
{
    m_enabled = true; m_port = port; m_widget = widget; m_settings = settings;

    m_writeQueue = QByteArray(); m_writeCount = int();
    m_writeCredits = DEF_CREDITS; m_writeList = QList<QByteArray>();

    m_maxCredits = MAX_CREDITS; m_maxLayer0DataSize = MAX_LAYER_0_DATA_SIZE;

    m_cancelTask = false;

    m_L0StateMachine = L0_WAIT_FOR_ESCAPE;
    m_L0Size = int(); m_L0Checksum = int(); m_L0Data = QByteArray();

    m_L1StateMachine = L1_WAIT_FOR_PACKET_FUNCTION_0;
    m_L1Function = int(); m_L1Size = int(); m_L1Data = QByteArray();

    ///////////////////////////////////////////////////////////////////////////

    QSettings mySettings(settings ? settings->fileName() : QString(),
                         settings ? settings->format() : QSettings::Format());

    mySettings.beginGroup(SERIAL_ESCAPE_KEY_GROUP);

    m_openProcessEn =
    mySettings.value(SERIAL_ESCAPE_KEY_OPEN_PROCESS_EN).toBool();

    m_openUrlEn =
    mySettings.value(SERIAL_ESCAPE_KEY_OPEN_URL_EN).toBool();

    ///////////////////////////////////////////////////////////////////////////

    m_checksumMode = false; m_baudRate = DEF_BAUD_RATE;

    ///////////////////////////////////////////////////////////////////////////

    m_terminal = new SerialTerminal(tr("Serial Terminal"), m_settings);
    m_terminal->setAttribute(Qt::WA_DeleteOnClose, false);
    m_terminal->setAttribute(Qt::WA_QuitOnClose, false);
    m_terminal->setWindowHandle(m_terminal->getWindowHandle());

    connect(m_terminal, SIGNAL(transmit(QByteArray,QWidget*)),
            this, SLOT(sendData(QByteArray,QWidget*)));

    connect(m_terminal, SIGNAL(errorMessage(QString)),
            this, SIGNAL(errorMessage(QString)));

    m_terminal->hide(); dockWindow(m_widget, m_terminal);

    m_windows = QMap<int, SerialWindow *>();

    ///////////////////////////////////////////////////////////////////////////

    if(m_port)
    {
        connect(m_port, SIGNAL(aboutToClose()),
                this, SLOT(userReset()));

        SerialPort::connectResetEvent(m_port,
                this, SLOT(resetEvent()));

        SerialPort::connectResetEvent2(m_port,
                this, SLOT(resetEvent2()));

        connect(m_port, SIGNAL(readyRead()),
                this, SLOT(readData()));
    }

    ///////////////////////////////////////////////////////////////////////////

    QTimer *rTimer = new QTimer(this);

    connect(rTimer, SIGNAL(timeout()),
            this, SLOT(readData()));

    rTimer->start(DEF_POLL_WAIT);

    ///////////////////////////////////////////////////////////////////////////

    QTimer *wTimer = new QTimer(this);

    connect(wTimer, SIGNAL(timeout()),
            this, SLOT(writeTask()));

    wTimer->start(DEF_POLL_WAIT);
}

SerialEscape::~SerialEscape()
{
    if(m_terminal)
    {
        m_terminal->setAttribute(Qt::WA_DeleteOnClose, true);
        m_terminal->close(); delete m_terminal; // delete faster
    }

    foreach(SerialWindow *window, m_windows.values())
    {
        window->setAttribute(Qt::WA_DeleteOnClose, true);
        window->close(); delete window; // delete faster
    }
}

void SerialEscape::setEnabled(bool enabled)
{
    if(m_enabled != enabled)
    {
        userReset();
    }

    m_enabled = enabled;
}

bool SerialEscape::getEnabled() const
{
    return m_enabled;
}

void SerialEscape::setPort(QIODevice *port)
{
    if(m_port != port)
    {
        userReset();
    }

    if(m_port)
    {
        disconnect(m_port, SIGNAL(aboutToClose()),
                   this, SLOT(userReset()));

        SerialPort::disconnectResetEvent(m_port,
                   this, SLOT(resetEvent()));

        SerialPort::disconnectResetEvent2(m_port,
                   this, SLOT(resetEvent2()));

        disconnect(m_port, SIGNAL(readyRead()),
                   this, SLOT(readData()));
    }

    m_port = port;

    if(m_port)
    {
        connect(m_port, SIGNAL(aboutToClose()),
                this, SLOT(userReset()));

        SerialPort::connectResetEvent(m_port,
                this, SLOT(resetEvent()));

        SerialPort::connectResetEvent2(m_port,
                this, SLOT(resetEvent2()));

        connect(m_port, SIGNAL(readyRead()),
                this, SLOT(readData()));
    }
}

QIODevice *SerialEscape::getPort() const
{
    return m_port;
}

void SerialEscape::setWidget(QWidget *widget)
{
    m_widget = widget;
}

QWidget *SerialEscape::getWidget() const
{
    return m_widget;
}

void SerialEscape::setSettings(QSettings *settings)
{
    m_settings = settings;

    if(m_terminal)
    {
        m_terminal->setSettings(m_settings);
    }

    foreach(SerialWindow *window, m_windows.values())
    {
        window->setSettings(m_settings);
    }
}

QSettings *SerialEscape::getSettings() const
{
    return m_settings;
}

SerialTerminal *SerialEscape::serialTerminal() const
{
    return m_terminal;
}

QList<SerialWindow *> SerialEscape::serialWindows() const
{
    return m_windows.values();
}

void SerialEscape::setOpenProcessEnabled(bool enable)
{
    m_openProcessEn = enable;

    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_ESCAPE_KEY_GROUP);

    settings.setValue(SERIAL_ESCAPE_KEY_OPEN_PROCESS_EN, m_openProcessEn);
}

bool SerialEscape::getOpenProcessEnabled() const
{
    return m_openProcessEn;
}

void SerialEscape::setOpenUrlEnabled(bool enable)
{
    m_openUrlEn = enable;

    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_ESCAPE_KEY_GROUP);

    settings.setValue(SERIAL_ESCAPE_KEY_OPEN_URL_EN, m_openUrlEn);
}

bool SerialEscape::getOpenUrlEnabled() const
{
    return m_openUrlEn;
}

void SerialEscape::openJSON(const QString &file)
{
    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_ESCAPE_JSON_KEY_GROUP);

    QString openFile = settings.value(SERIAL_ESCAPE_JSON_KEY_FILE,
                                      QDir::homePath()).toString();

    QString temp = file.isEmpty() ? QFileDialog::getOpenFileName(m_widget,
    tr("Import Widget State"), openFile, tr("JSON Files (*.json)")) : file;

    if(!temp.isEmpty())
    {
        QFile file(temp);

        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QJsonParseError parseError; QJsonDocument json =
            QJsonDocument::fromJson(file.readAll(), &parseError);

            if(parseError.error == QJsonParseError::NoError)
            {
                QJsonObject object = json.object();

                SerialWindow *window = NULL;

                if(object.value("type").toString() == "oscilloscope")
                {
                    window = new SerialOscilloscope(
                    object.value("title").toString(),
                    m_settings);
                }
                else if(object.value("type").toString() == "table")
                {
                    window = new SerialTable(
                    object.value("title").toString(),
                    m_settings);
                }
                else if(object.value("type").toString() == "tree")
                {
                    window = new SerialTree(
                    object.value("title").toString(),
                    m_settings);
                }
                else if(object.value("type").toString() == "graphics")
                {
                    window = new SerialGraphics(
                    object.value("title").toString(),
                    m_settings);
                }

                if(window)
                {
                    window->setAttribute(Qt::WA_QuitOnClose, false);
                    window->importState(temp);
                    window->show();

                    QFileInfo fileInfo(temp);

                    settings.setValue(SERIAL_ESCAPE_JSON_KEY_FILE,
                    fileInfo.canonicalFilePath());
                }
                else
                {
                    QMessageBox::critical(m_widget,
                    tr("Import Widget State Error"),
                    tr("Incompatible JSON file"));
                }
            }
            else
            {
                QMessageBox::critical(m_widget,
                tr("Import Widget State Error"),
                parseError.errorString());
            }
        }
        else
        {
            QMessageBox::critical(m_widget,
            tr("Import Widget State Error"),
            file.errorString());
        }
    }
}

void SerialEscape::removeWidget(QAction *action)
{
    if(action)
    {
        SerialWindow *window = qvariant_cast<SerialWindow *>(action->data());

        if(serialWindows().contains(window))
        {
            QMessageBox messageBox(QMessageBox::Warning, tr("Remove Widget"),
            tr("Are you sure you want to do this?"), QMessageBox::Ok |
            QMessageBox::Cancel, m_widget);

            messageBox.setDefaultButton(QMessageBox::Ok);
            messageBox.setEscapeButton(QMessageBox::Cancel);

            connect(window, SIGNAL(destroyed()), &messageBox, SLOT(reject()));

            if(messageBox.exec() == QMessageBox::Ok)
            {
                window->setAttribute(Qt::WA_DeleteOnClose, true);
                window->close(); delete window; // delete faster
            }
        }
    }
}

void SerialEscape::removeAllWidgets(QAction *action)
{
    Q_UNUSED(action);

    QMessageBox messageBox(QMessageBox::Warning, tr("Remove All Widgets"),
    tr("Are you sure you want to do this?"), QMessageBox::Ok |
    QMessageBox::Cancel, m_widget);

    messageBox.setDefaultButton(QMessageBox::Ok);
    messageBox.setEscapeButton(QMessageBox::Cancel);

    if(messageBox.exec() == QMessageBox::Ok)
    {
        foreach(SerialWindow *window, m_windows.values())
        {
            window->setAttribute(Qt::WA_DeleteOnClose, true);
            window->close(); delete window; // delete faster
        }
    }
}

void SerialEscape::windowDestroyed(QObject *object)
{
    if(object)
    {
        m_windows.remove(m_windows.key(static_cast<SerialWindow *>(object)));
    }
}

void SerialEscape::userReset()
{
    m_writeQueue = QByteArray(); m_writeCount = int();
    m_writeCredits = DEF_CREDITS; m_writeList = QList<QByteArray>();

    m_maxCredits = MAX_CREDITS; m_maxLayer0DataSize = MAX_LAYER_0_DATA_SIZE;

    m_cancelTask = true;

    m_L0StateMachine = L0_WAIT_FOR_ESCAPE;
    m_L0Size = int(); m_L0Checksum = int(); m_L0Data = QByteArray();

    m_L1StateMachine = L1_WAIT_FOR_PACKET_FUNCTION_0;
    m_L1Function = int(); m_L1Size = int(); m_L1Data = QByteArray();

    m_checksumMode = false; m_baudRate = DEF_BAUD_RATE;

    emit cancelTask();
}

void SerialEscape::resetEvent()
{
    if(m_enabled && m_port && m_port->isOpen())
    {
        int time = transferTimeHint(DEF_BAUD_RATE);
        int size = transferSizeHint(DEF_BAUD_RATE);

        if((!SerialPort::setBaudRate(m_port, DEF_BAUD_RATE))
        || (!SerialPort::setTransferTime(m_port, time, time))
        || (!SerialPort::setTransferSize(m_port, size, size))
        || (!SerialPort::clearAll(m_port)))
        {
            emit errorMessage(QString(metaObject()->className()) +
            "::" + __FUNCTION__ + " -> " + m_port->errorString());
        }

        userReset();
    }
}

void SerialEscape::resetEvent2()
{
    if(m_enabled && m_port && m_port->isOpen())
    {
        int time = transferTimeHint(DEF_BAUD_RATE);
        int size = transferSizeHint(DEF_BAUD_RATE);

        if((!SerialPort::setBaudRate(m_port, DEF_BAUD_RATE))
        || (!SerialPort::setTransferTime(m_port, time, time))
        || (!SerialPort::setTransferSize(m_port, size, size))
        || (!SerialPort::clearAll(m_port)))
        {
            emit errorMessage(QString(metaObject()->className()) +
            "::" + __FUNCTION__ + " -> " + m_port->errorString());
        }

        userReset();
    }
}

void SerialEscape::readData()
{
    if(m_enabled && m_port && m_port->isOpen() && m_readRecurseLock.tryLock())
    {
        m_cancelTask = false;

        ///////////////////////////////////////////////////////////////////////

        readLayer0(m_port->readAll()); writeTask();

        ///////////////////////////////////////////////////////////////////////

        m_readRecurseLock.unlock();
    }
}

void SerialEscape::sendData(const QByteArray &bytes, QWidget *parent)
{
    if(m_enabled && m_port && m_port->isOpen() && m_sendRecurseLock.tryLock())
    {
        m_cancelTask = false;

        ///////////////////////////////////////////////////////////////////////

        int position = m_writeQueue.size();
        int size = position + bytes.size();

        QProgressDialog dialog(parent, Qt::MSWindowsFixedSizeDialogHint |
        Qt::WindowTitleHint | Qt::WindowSystemMenuHint |
        Qt::WindowCloseButtonHint);

        connect(this, SIGNAL(cancelTask()),
                &dialog, SLOT(cancel()));

        dialog.setWindowTitle(parent?parent->windowTitle():tr("Serial Port"));
        dialog.setWindowModality(Qt::ApplicationModal);
        dialog.setLabelText(tr("Sending Data..."));
        dialog.setRange(0, size); dialog.setValue(0);

        writeData(bytes);

        for(m_writeCount = 0; m_writeCount < size;)
        {
            writeTask();

            dialog.setValue(m_writeCount);

            if(dialog.wasCanceled())
            {
                m_writeQueue.remove(qMax(position - m_writeCount, 0),
                qMin((size - m_writeCount), bytes.size()));

                break;
            }
        }

        ///////////////////////////////////////////////////////////////////////

        m_sendRecurseLock.unlock();
    }
}

void SerialEscape::writeList(const QByteArray &bytes)
{
    if(m_enabled && m_port && m_port->isOpen())
    {
        m_writeList.append(bytes);
    }
}

void SerialEscape::writeData(const QByteArray &bytes)
{
    if(m_enabled && m_port && m_port->isOpen())
    {
        m_writeQueue.append(bytes);
    }
}

void SerialEscape::writeData(char byte)
{
    if(m_enabled && m_port && m_port->isOpen())
    {
        m_writeQueue.append(byte);
    }
}

void SerialEscape::writeTask()
{
    if(m_enabled && m_port && m_port->isOpen())
    {
        while((!m_writeList.isEmpty()) && (m_writeCredits > 0))
        {
            m_writeQueue.append(m_writeList.takeFirst()); m_writeCredits -= 1;
        }

        int bytesWritten = m_port->write(m_writeQueue);

        if(bytesWritten > 0)
        {
            m_writeQueue.remove(0, bytesWritten); m_writeCount += bytesWritten;
        }
    }
}

void SerialEscape::sendVariantPacket(SerialFunction function,
                                     const QVariant &value)
{
    if(m_enabled && m_port && m_port->isOpen() && m_sendRecurseLock.tryLock())
    {
        m_cancelTask = false;

        ///////////////////////////////////////////////////////////////////////

        int diff = qMax(m_writeList.size() - DTE_MAX_QUEUE_DEPTH, 0);

        QProgressDialog dialog(m_widget, Qt::MSWindowsFixedSizeDialogHint |
        Qt::WindowTitleHint | Qt::WindowSystemMenuHint |
        Qt::WindowCloseButtonHint);

        connect(this, SIGNAL(cancelTask()),
                &dialog, SLOT(cancel()));

        dialog.setWindowTitle(tr("Serial Port"));
        dialog.setWindowModality(Qt::ApplicationModal);
        dialog.setLabelText(tr("Sending Packets..."));
        dialog.setRange(0, diff); dialog.setValue(0);

        while(m_writeList.size() > DTE_MAX_QUEUE_DEPTH)
        {
            writeTask();

            dialog.setValue(diff - (m_writeList.size() - DTE_MAX_QUEUE_DEPTH));

            if(dialog.wasCanceled())
            {
                m_sendRecurseLock.unlock(); return;
            }
        }

        ///////////////////////////////////////////////////////////////////////

        switch(function)
        {
            case INTERFACE_PUSH_BUTTON_SIGNAL:
            {
                sendIntPacket(function,
                value.toInt()); break;
            }

            case INTERFACE_RADIO_BUTTON_SIGNAL:
            {
                sendIntPacket(function,
                value.toInt()); break;
            }

            case INTERFACE_CHECK_BOX_SIGNAL:
            {
                sendIntPacket(function,
                value.toInt()); break;
            }

            case INTERFACE_LINE_EDIT_SIGNAL:
            {
                sendStringPacket(function,
                value.toString().toUtf8()); break;
            }

            case INTERFACE_INT_SPIN_BOX_SIGNAL:
            {
                sendIntPacket(function,
                value.toInt()); break;
            }

            case INTERFACE_BIN_SPIN_BOX_SIGNAL:
            {
                sendIntPacket(function,
                value.toInt()); break;
            }

            case INTERFACE_HEX_SPIN_BOX_SIGNAL:
            {
                sendIntPacket(function,
                value.toInt()); break;
            }

            case INTERFACE_DOUBLE_SPIN_BOX_SIGNAL:
            {
                sendFloatPacket(function,
                value.toFloat()); break;
            }

            case INTERFACE_TIME_EDIT_SIGNAL:
            {
                sendStringPacket(function,
                value.toString().toUtf8()); break;
            }

            case INTERFACE_DATE_EDIT_SIGNAL:
            {
                sendStringPacket(function,
                value.toString().toUtf8()); break;
            }

            case INTERFACE_DATE_TIME_EDIT_SIGNAL:
            {
                sendStringPacket(function,
                value.toString().toUtf8()); break;
            }

            case INTERFACE_SLIDER_SIGNAL:
            {
                sendIntPacket(function,
                value.toInt()); break;
            }

            default:
            {
                qDebug() <<
                Q_FUNC_INFO <<
                "no case statement for function:" <<
                function; break;
            }
        }

        m_sendRecurseLock.unlock();
    }
}

void SerialEscape::sendVariantPacket(SerialFunction function,
                                     int handle0,
                                     const QVariant &value)
{
    if(m_enabled && m_port && m_port->isOpen() && m_sendRecurseLock.tryLock())
    {
        m_cancelTask = false;

        ///////////////////////////////////////////////////////////////////////

        int diff = qMax(m_writeList.size() - DTE_MAX_QUEUE_DEPTH, 0);

        QProgressDialog dialog(m_widget, Qt::MSWindowsFixedSizeDialogHint |
        Qt::WindowTitleHint | Qt::WindowSystemMenuHint |
        Qt::WindowCloseButtonHint);

        connect(this, SIGNAL(cancelTask()),
                &dialog, SLOT(cancel()));

        dialog.setWindowTitle(tr("Serial Port"));
        dialog.setWindowModality(Qt::ApplicationModal);
        dialog.setLabelText(tr("Sending Packets..."));
        dialog.setRange(0, diff); dialog.setValue(0);

        while(m_writeList.size() > DTE_MAX_QUEUE_DEPTH)
        {
            writeTask();

            dialog.setValue(diff - (m_writeList.size() - DTE_MAX_QUEUE_DEPTH));

            if(dialog.wasCanceled())
            {
                m_sendRecurseLock.unlock(); return;
            }
        }

        ///////////////////////////////////////////////////////////////////////

        switch(function)
        {
            case INTERFACE_PUSH_BUTTON_SIGNAL:
            {
                sendIntPacket(function, handle0,
                value.toInt()); break;
            }

            case INTERFACE_RADIO_BUTTON_SIGNAL:
            {
                sendIntPacket(function, handle0,
                value.toInt()); break;
            }

            case INTERFACE_CHECK_BOX_SIGNAL:
            {
                sendIntPacket(function, handle0,
                value.toInt()); break;
            }

            case INTERFACE_LINE_EDIT_SIGNAL:
            {
                sendStringPacket(function, handle0,
                value.toString().toUtf8()); break;
            }

            case INTERFACE_INT_SPIN_BOX_SIGNAL:
            {
                sendIntPacket(function, handle0,
                value.toInt()); break;
            }

            case INTERFACE_BIN_SPIN_BOX_SIGNAL:
            {
                sendIntPacket(function, handle0,
                value.toInt()); break;
            }

            case INTERFACE_HEX_SPIN_BOX_SIGNAL:
            {
                sendIntPacket(function, handle0,
                value.toInt()); break;
            }

            case INTERFACE_DOUBLE_SPIN_BOX_SIGNAL:
            {
                sendFloatPacket(function, handle0,
                value.toFloat()); break;
            }

            case INTERFACE_TIME_EDIT_SIGNAL:
            {
                sendStringPacket(function, handle0,
                value.toString().toUtf8()); break;
            }

            case INTERFACE_DATE_EDIT_SIGNAL:
            {
                sendStringPacket(function, handle0,
                value.toString().toUtf8()); break;
            }

            case INTERFACE_DATE_TIME_EDIT_SIGNAL:
            {
                sendStringPacket(function, handle0,
                value.toString().toUtf8()); break;
            }

            case INTERFACE_SLIDER_SIGNAL:
            {
                sendIntPacket(function, handle0,
                value.toInt()); break;
            }

            default:
            {
                qDebug() <<
                Q_FUNC_INFO <<
                "no case statement for function:" <<
                function; break;
            }
        }

        m_sendRecurseLock.unlock();
    }
}

void SerialEscape::sendVariantPacket(SerialFunction function,
                                     int handle0,
                                     int handle1,
                                     const QVariant &value)
{
    if(m_enabled && m_port && m_port->isOpen() && m_sendRecurseLock.tryLock())
    {
        m_cancelTask = false;

        ///////////////////////////////////////////////////////////////////////

        int diff = qMax(m_writeList.size() - DTE_MAX_QUEUE_DEPTH, 0);

        QProgressDialog dialog(m_widget, Qt::MSWindowsFixedSizeDialogHint |
        Qt::WindowTitleHint | Qt::WindowSystemMenuHint |
        Qt::WindowCloseButtonHint);

        connect(this, SIGNAL(cancelTask()),
                &dialog, SLOT(cancel()));

        dialog.setWindowTitle(tr("Serial Port"));
        dialog.setWindowModality(Qt::ApplicationModal);
        dialog.setLabelText(tr("Sending Packets..."));
        dialog.setRange(0, diff); dialog.setValue(0);

        while(m_writeList.size() > DTE_MAX_QUEUE_DEPTH)
        {
            writeTask();

            dialog.setValue(diff - (m_writeList.size() - DTE_MAX_QUEUE_DEPTH));

            if(dialog.wasCanceled())
            {
                m_sendRecurseLock.unlock(); return;
            }
        }

        ///////////////////////////////////////////////////////////////////////

        switch(function)
        {
            case INTERFACE_PUSH_BUTTON_SIGNAL:
            {
                sendIntPacket(function, handle0, handle1,
                value.toInt()); break;
            }

            case INTERFACE_RADIO_BUTTON_SIGNAL:
            {
                sendIntPacket(function, handle0, handle1,
                value.toInt()); break;
            }

            case INTERFACE_CHECK_BOX_SIGNAL:
            {
                sendIntPacket(function, handle0, handle1,
                value.toInt()); break;
            }

            case INTERFACE_LINE_EDIT_SIGNAL:
            {
                sendStringPacket(function, handle0, handle1,
                value.toString().toUtf8()); break;
            }

            case INTERFACE_INT_SPIN_BOX_SIGNAL:
            {
                sendIntPacket(function, handle0, handle1,
                value.toInt()); break;
            }

            case INTERFACE_BIN_SPIN_BOX_SIGNAL:
            {
                sendIntPacket(function, handle0, handle1,
                value.toInt()); break;
            }

            case INTERFACE_HEX_SPIN_BOX_SIGNAL:
            {
                sendIntPacket(function, handle0, handle1,
                value.toInt()); break;
            }

            case INTERFACE_DOUBLE_SPIN_BOX_SIGNAL:
            {
                sendFloatPacket(function, handle0, handle1,
                value.toFloat()); break;
            }

            case INTERFACE_TIME_EDIT_SIGNAL:
            {
                sendStringPacket(function, handle0, handle1,
                value.toString().toUtf8()); break;
            }

            case INTERFACE_DATE_EDIT_SIGNAL:
            {
                sendStringPacket(function, handle0, handle1,
                value.toString().toUtf8()); break;
            }

            case INTERFACE_DATE_TIME_EDIT_SIGNAL:
            {
                sendStringPacket(function, handle0, handle1,
                value.toString().toUtf8()); break;
            }

            case INTERFACE_SLIDER_SIGNAL:
            {
                sendIntPacket(function, handle0, handle1,
                value.toInt()); break;
            }

            default:
            {
                qDebug() <<
                Q_FUNC_INFO <<
                "no case statement for function:" <<
                function; break;
            }
        }

        m_sendRecurseLock.unlock();
    }
}

void SerialEscape::sendVariantPacket(SerialFunction function,
                                     int handle0,
                                     int handle1,
                                     int handle2,
                                     const QVariant &value)
{
    if(m_enabled && m_port && m_port->isOpen() && m_sendRecurseLock.tryLock())
    {
        m_cancelTask = false;

        ///////////////////////////////////////////////////////////////////////

        int diff = qMax(m_writeList.size() - DTE_MAX_QUEUE_DEPTH, 0);

        QProgressDialog dialog(m_widget, Qt::MSWindowsFixedSizeDialogHint |
        Qt::WindowTitleHint | Qt::WindowSystemMenuHint |
        Qt::WindowCloseButtonHint);

        connect(this, SIGNAL(cancelTask()),
                &dialog, SLOT(cancel()));

        dialog.setWindowTitle(tr("Serial Port"));
        dialog.setWindowModality(Qt::ApplicationModal);
        dialog.setLabelText(tr("Sending Packets..."));
        dialog.setRange(0, diff); dialog.setValue(0);

        while(m_writeList.size() > DTE_MAX_QUEUE_DEPTH)
        {
            writeTask();

            dialog.setValue(diff - (m_writeList.size() - DTE_MAX_QUEUE_DEPTH));

            if(dialog.wasCanceled())
            {
                m_sendRecurseLock.unlock(); return;
            }
        }

        ///////////////////////////////////////////////////////////////////////

        switch(function)
        {
            case INTERFACE_PUSH_BUTTON_SIGNAL:
            {
                sendIntPacket(function, handle0, handle1, handle2,
                value.toInt()); break;
            }

            case INTERFACE_RADIO_BUTTON_SIGNAL:
            {
                sendIntPacket(function, handle0, handle1, handle2,
                value.toInt()); break;
            }

            case INTERFACE_CHECK_BOX_SIGNAL:
            {
                sendIntPacket(function, handle0, handle1, handle2,
                value.toInt()); break;
            }

            case INTERFACE_LINE_EDIT_SIGNAL:
            {
                sendStringPacket(function, handle0, handle1, handle2,
                value.toString().toUtf8()); break;
            }

            case INTERFACE_INT_SPIN_BOX_SIGNAL:
            {
                sendIntPacket(function, handle0, handle1, handle2,
                value.toInt()); break;
            }

            case INTERFACE_BIN_SPIN_BOX_SIGNAL:
            {
                sendIntPacket(function, handle0, handle1, handle2,
                value.toInt()); break;
            }

            case INTERFACE_HEX_SPIN_BOX_SIGNAL:
            {
                sendIntPacket(function, handle0, handle1, handle2,
                value.toInt()); break;
            }

            case INTERFACE_DOUBLE_SPIN_BOX_SIGNAL:
            {
                sendFloatPacket(function, handle0, handle1, handle2,
                value.toFloat()); break;
            }

            case INTERFACE_TIME_EDIT_SIGNAL:
            {
                sendStringPacket(function, handle0, handle1, handle2,
                value.toString().toUtf8()); break;
            }

            case INTERFACE_DATE_EDIT_SIGNAL:
            {
                sendStringPacket(function, handle0, handle1, handle2,
                value.toString().toUtf8()); break;
            }

            case INTERFACE_DATE_TIME_EDIT_SIGNAL:
            {
                sendStringPacket(function, handle0, handle1, handle2,
                value.toString().toUtf8()); break;
            }

            case INTERFACE_SLIDER_SIGNAL:
            {
                sendIntPacket(function, handle0, handle1, handle2,
                value.toInt()); break;
            }

            default:
            {
                qDebug() <<
                Q_FUNC_INFO <<
                "no case statement for function:" <<
                function; break;
            }
        }

        m_sendRecurseLock.unlock();
    }
}

void SerialEscape::readLayer0(const QByteArray &bytes)
{
    QByteArray buffer;

    for(int i = 0, j = bytes.size(); i < j; i++)
    {
        switch(m_L0StateMachine)
        {
            case L0_WAIT_FOR_ESCAPE:
            {
                switch(bytes.at(i))
                {
                    case CC_XON:
                    {
                        if(m_terminal)
                        {
                            m_terminal->receive(buffer); buffer.clear();
                        }

                        m_writeCredits =
                        qMin(m_writeCredits + 1, m_maxCredits);

                        break;
                    }

                    case CC_XOFF:
                    {
                        if(m_terminal)
                        {
                            m_terminal->receive(buffer); buffer.clear();
                        }

                        m_writeCredits =
                        qMin(m_writeCredits + 1, m_maxCredits);

                        break;
                    }

                    case CC_ESCAPE:
                    {
                        if(m_terminal)
                        {
                            m_terminal->receive(buffer); buffer.clear();
                        }

                        m_L0Data.append(bytes.at(i));

                        m_L0StateMachine = L0_WAIT_FOR_NULL;

                        break;
                    }

                    default:
                    {
                        buffer.append(bytes.at(i)); break;
                    }
                }

                break;
            }

            case L0_WAIT_FOR_NULL:
            {
                m_L0Data.append(bytes.at(i));

                m_L0StateMachine = L0_WAIT_FOR_PACKET_SIZE_0;

                break;
            }

            case L0_WAIT_FOR_PACKET_SIZE_0:
            {
                m_L0Data.append(bytes.at(i));

                m_L0Size = static_cast<quint8>(bytes.at(i));

                if(m_L0Size > MAX_LAYER_0_DATA_SIZE)
                {
                    emit errorMessage(QString(metaObject()->className()) +
                    "::" + __FUNCTION__ + " -> " + tr("Layer 0 packet size "
                    "(%L1) is out of the valid range between [0 : %L2] - "
                    "waiting for reset sequence...").
                    arg(m_L0Size).arg(MAX_LAYER_0_DATA_SIZE));

                    writeData(CC_XOFF);

                    m_L0Data.clear(); m_L0Size = int();

                    m_L0StateMachine = L0_ERROR_STATE;
                }
                else
                {
                    m_L0StateMachine = L0_WAIT_FOR_PACKET_SIZE_1;
                }

                break;
            }

            case L0_WAIT_FOR_PACKET_SIZE_1:
            {
                m_L0Data.append(bytes.at(i));

                quint8 invertedL0Size = ~bytes.at(i);

                if(m_L0Size != invertedL0Size)
                {
                    emit errorMessage(QString(metaObject()->className()) +
                    "::" + __FUNCTION__ + " -> " + tr("Layer 0 packet size "
                    "(%L1) != inverted packet size (%L2) - "
                    "waiting for reset sequence...").
                    arg(m_L0Size).arg(invertedL0Size));

                    writeData(CC_XOFF);

                    m_L0Data.clear(); m_L0Size = int();

                    m_L0StateMachine = L0_ERROR_STATE;
                }
                else
                {
                    m_L0StateMachine = L0_WAIT_FOR_PACKET_DATA;

                    if(!m_L0Size)
                    {
                        m_L0StateMachine = L0_WAIT_FOR_CHECKSUM_0;
                    }
                }

                break;
            }

            case L0_WAIT_FOR_PACKET_DATA:
            {
                m_L0Data.append(bytes.at(i)); m_L0Size -= 1;

                if(!m_L0Size)
                {
                    m_L0StateMachine = L0_WAIT_FOR_CHECKSUM_0;
                }

                break;
            }

            case L0_WAIT_FOR_CHECKSUM_0:
            {
                m_L0Checksum = bytes.at(i);

                m_L0StateMachine = L0_WAIT_FOR_CHECKSUM_1;

                break;
            }

            case L0_WAIT_FOR_CHECKSUM_1:
            {
                union { quint16 q; char c[sizeof(q)]; } u;

                u.c[0] = m_L0Checksum;
                u.c[1] = bytes.at(i);

                m_L0Checksum = qFromLittleEndian(u.q);

                quint16 expectedL0Checksum = m_checksumMode
                ? qChecksum2(m_L0Data.constData(), m_L0Data.size())
                : qChecksum(m_L0Data.constData(), m_L0Data.size());

                if(m_L0Checksum == expectedL0Checksum)
                {
                    writeData(CC_XON);

                    readLayer1(m_L0Data.remove(0, LAYER_0_HEAD_SIZE));

                    if(m_cancelTask)
                    {
                        return;
                    }

                    m_L0Data.clear(); m_L0Checksum = int();

                    m_L0StateMachine = L0_WAIT_FOR_ESCAPE;
                }
                else
                {
                    emit errorMessage(QString(metaObject()->className()) +
                    "::" + __FUNCTION__ + " -> " + tr("Layer 0 checksum "
                    "(%L1) != expected checksum (%L2) - "
                    "waiting for reset sequence...").
                    arg(m_L0Checksum).arg(expectedL0Checksum));

                    writeData(CC_XOFF);

                    m_L0Data.clear(); m_L0Checksum = int();

                    m_L0StateMachine = L0_ERROR_STATE;
                }

                break;
            }

            case L0_ERROR_STATE:
            {
                break; // wait here until reset
            }
        }
    }

    if(m_terminal)
    {
        m_terminal->receive(buffer); buffer.clear();
    }
}

void SerialEscape::readLayer1(const QByteArray &bytes)
{
    for(int i = 0, j = bytes.size(); i < j; i++)
    {
        switch(m_L1StateMachine)
        {
            case L1_WAIT_FOR_PACKET_FUNCTION_0:
            {
                m_L1Function = bytes.at(i);

                m_L1StateMachine = L1_WAIT_FOR_PACKET_FUNCTION_1;

                break;
            }

            case L1_WAIT_FOR_PACKET_FUNCTION_1:
            {
                union { quint16 q; char c[sizeof(q)]; } u;

                u.c[0] = m_L1Function;
                u.c[1] = bytes.at(i);

                m_L1Function = qFromLittleEndian(u.q);

                m_L1StateMachine = L1_WAIT_FOR_PACKET_SIZE_0;

                break;
            }

            case L1_WAIT_FOR_PACKET_SIZE_0:
            {
                m_L1Size = bytes.at(i);

                m_L1StateMachine = L1_WAIT_FOR_PACKET_SIZE_1;

                break;
            }

            case L1_WAIT_FOR_PACKET_SIZE_1:
            {
                union { quint16 q; char c[sizeof(q)]; } u;

                u.c[0] = m_L1Size;
                u.c[1] = bytes.at(i);

                m_L1Size = qFromLittleEndian(u.q);

                if(m_L1Size > MAX_LAYER_1_DATA_SIZE)
                {
                    emit errorMessage(QString(metaObject()->className()) +
                    "::" + __FUNCTION__ + " -> " + tr("Layer 1 packet size "
                    "(%L1) is out of the valid range between [0 : %L2] - "
                    "waiting for reset sequence...").
                    arg(m_L1Size).arg(MAX_LAYER_1_DATA_SIZE));

                    sendStringPacket(ESCAPE_NAK, QByteArray());
                    sendStringPacket(ESCAPE_RESET_DCE, QByteArray());

                    m_L1Function = int(); m_L1Size = int();

                    m_L1StateMachine = L1_ERROR_STATE;
                }
                else
                {
                    m_L1StateMachine = L1_WAIT_FOR_PACKET_DATA;

                    if(!m_L1Size)
                    {
                        parseData(static_cast<SerialFunction>(m_L1Function),
                                  m_L1Data);

                        if(m_cancelTask)
                        {
                            return;
                        }

                        m_L1Data.clear(); m_L1Function = int();

                        m_L1StateMachine = L1_WAIT_FOR_PACKET_FUNCTION_0;
                    }
                }

                break;
            }

            case L1_WAIT_FOR_PACKET_DATA:
            {
                m_L1Data.append(bytes.at(i)); m_L1Size -= 1;

                if(!m_L1Size)
                {
                    parseData(static_cast<SerialFunction>(m_L1Function),
                              m_L1Data);

                    if(m_cancelTask)
                    {
                        return;
                    }

                    m_L1Data.clear(); m_L1Function = int();

                    m_L1StateMachine = L1_WAIT_FOR_PACKET_FUNCTION_0;
                }

                break;
            }

            case L1_ERROR_STATE:
            {
                break; // wait here until reset
            }
        }
    }
}

void SerialEscape::parseData(SerialFunction function, const QByteArray &data)
{
    SerialResponse response = SR_OK; QVariant result = QByteArray();

    if(parseData(function, data, &response, &result))
    {
        switch(response)
        {
            case SR_OK:
            {
                sendLayer1(ESCAPE_ACK, QByteArray()); break;
            }

            case SR_INT:
            {
                sendIntPacket(ESCAPE_ACK, result.toInt()); break;
            }

            case SR_FLOAT:
            {
                sendFloatPacket(ESCAPE_ACK, result.toFloat()); break;
            }

            case SR_STRING:
            {
                sendStringPacket(ESCAPE_ACK, result.toByteArray()); break;
            }

            case SR_NONE:
            {
                break;
            }

            default: break;
        }
    }
    else
    {
        sendLayer1(ESCAPE_NAK, QByteArray());
    }
}

bool SerialEscape::parseData(SerialFunction function, const QByteArray &data,
                             SerialResponse *response, QVariant *result)
{
    QDataStream reader(data);

    reader.setFloatingPointPrecision(QDataStream::SinglePrecision);
    reader.setByteOrder(QDataStream::LittleEndian);

    switch(function)
    {
        case ESCAPE_NAK:
        {
            *response = SR_NONE; return true;
        }

        case ESCAPE_ACK:
        {
            *response = SR_NONE; return true;
        }

        case ESCAPE_RESET_DCE:
        {
            return true;
        }

        case ESCAPE_RESET_DTE:
        {
            resetEvent();

            return true;
        }

        case ESCAPE_GET_VERSION_INT:
        {
            *response = SR_INT;

            int version = int();
            int multiplier = 100 * 100 * 100;

            QStringList list = QString(PROJECT_VERSION_STR).split('.');

            QList<QString>::const_iterator i = list.constBegin();
            QList<QString>::const_iterator j = list.constEnd();

            for(; i != j; i++)
            {
                version += i->toInt() * multiplier; multiplier /= 100;
            }

            *result = version;

            return true;
        }

        case ESCAPE_GET_VERSION_STR:
        {
            *response = SR_STRING;
            *result = QByteArray(PROJECT_VERSION_STR);

            return true;
        }

        case ESCAPE_OPEN_PROCESS:
        {
            if(m_openProcessEn)
            {
                QByteArray string; streamIn(reader, string);

                return QProcess::startDetached(string);
            }
            else
            {
                emit errorMessage(QString(metaObject()->className()) +
                "::" + __FUNCTION__ + " -> " +
                tr("OpenProcess has been disabled for your security."));
            }

            return false;
        }

        case ESCAPE_OPEN_URL:
        {
            if(m_openUrlEn)
            {
                QByteArray string; streamIn(reader, string);

                return QDesktopServices::openUrl(QUrl::fromUserInput(string));
            }
            else
            {
                emit errorMessage(QString(metaObject()->className()) +
                "::" + __FUNCTION__ + " -> " +
                tr("OpenUrl has been disabled for your security."));
            }

            return false;
        }

        case ESCAPE_SET_CHECKSUM_MODE:
        {
            qint32 checksumMode; streamIn(reader, checksumMode);

            m_checksumMode = checksumMode;

            return true;
        }

        case ESCAPE_GET_CHECKSUM_MODE:
        {
            *response = SR_INT; *result = m_checksumMode; return true;
        }

        case ESCAPE_SET_MAX_CREDITS:
        {
            qint32 maxCredits; streamIn(reader, maxCredits);

            if((1 <= maxCredits)
            && (maxCredits <= MAX_CREDITS))
            {
                m_writeCredits = qMin(m_writeCredits, m_maxCredits=maxCredits);

                return true;
            }
            else
            {
                emit errorMessage(QString(metaObject()->className()) +
                "::" + __FUNCTION__ + " -> " +
                tr("maxCredits (%L1) is out of the valid range between "
                "[%L2 : %L3]").arg(maxCredits).
                arg(1).arg(MAX_CREDITS));

                return false;
            }
        }

        case ESCAPE_GET_MAX_CREDITS:
        {
            *response = SR_INT; *result = m_maxCredits; return true;
        }

        case ESCAPE_SET_MAX_LAYER_0_DATA_SIZE:
        {
            qint32 maxLayer0DataSize; streamIn(reader, maxLayer0DataSize);

            if((1 <= maxLayer0DataSize)
            && (maxLayer0DataSize <= MAX_LAYER_0_DATA_SIZE))
            {
                m_maxLayer0DataSize = maxLayer0DataSize;

                return true;
            }
            else
            {
                emit errorMessage(QString(metaObject()->className()) +
                "::" + __FUNCTION__ + " -> " +
                tr("maxLayer0DataSize (%L1) is out of the valid range between "
                "[%L2 : %L3]").arg(maxLayer0DataSize).
                arg(1).arg(MAX_LAYER_0_DATA_SIZE));

                return false;
            }
        }

        case ESCAPE_GET_MAX_LAYER_0_DATA_SIZE:
        {
            *response = SR_INT; *result = m_maxLayer0DataSize; return true;
        }

        case ESCAPE_SET_BAUD_RATE:
        {
            qint32 baudRate; streamIn(reader, baudRate);

            if((MIN_BAUD_RATE <= baudRate) && (baudRate <= MAX_BAUD_RATE))
            {
                m_baudRate = baudRate;

                QThread::msleep(DEGLITCH_WAIT); ///////////////////////////////

                int time = transferTimeHint(m_baudRate);
                int size = transferSizeHint(m_baudRate);

                if((!SerialPort::setBaudRate(m_port, m_baudRate))
                || (!SerialPort::setTransferTime(m_port, time, time))
                || (!SerialPort::setTransferSize(m_port, size, size)))
                {
                    emit errorMessage(QString(metaObject()->className()) +
                    "::" + __FUNCTION__ + " -> " + m_port->errorString());
                }

                QThread::msleep(DEGLITCH_WAIT); ///////////////////////////////

                return true;
            }
            else
            {
                emit errorMessage(QString(metaObject()->className()) +
                "::" + __FUNCTION__ + " -> " +
                tr("The baudrate (%L1) is out of the valid range between "
                "[%L2 : %L3]").arg(baudRate).
                arg(MIN_BAUD_RATE).arg(MAX_BAUD_RATE));

                return false;
            }
        }

        case ESCAPE_GET_BAUD_RATE:
        {
            *response = SR_INT; *result = m_baudRate; return true;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Input Dialog Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case ESCAPE_SHOW_COLOR_DIALOG:
        {
            QByteArray title; streamIn(reader, title);

            QColorDialog dialog(m_widget);

            dialog.setWindowTitle(title);
            dialog.setOption(QColorDialog::ShowAlphaChannel);

            connect(this, SIGNAL(cancelTask()),
                    &dialog, SLOT(reject()));

            bool accepted = (dialog.exec() == QDialog::Accepted);

            *result = dialog.selectedColor().toRgb();

            if(m_cancelTask)
            {
                *response = SR_NONE;

                return true;
            }
            else
            {
                *response = SR_INT;

                return accepted;
            }
        }

        case ESCAPE_SHOW_COLOR_DIALOG_ON_WINDOW:
        {
            quint8 targetWindow; streamIn(reader, targetWindow);

            SerialWindow *myTargetWindow = windowCast
            <SerialWindow>(__FUNCTION__, targetWindow);

            if(myTargetWindow)
            {
                QByteArray title; streamIn(reader, title);

                QColorDialog dialog(myTargetWindow);

                dialog.setWindowTitle(title);
                dialog.setOption(QColorDialog::ShowAlphaChannel);

                connect(this, SIGNAL(cancelTask()),
                        &dialog, SLOT(reject()));

                bool accepted = (dialog.exec() == QDialog::Accepted);

                *result = dialog.selectedColor().toRgb();

                if(m_cancelTask)
                {
                    *response = SR_NONE;

                    return true;
                }
                else
                {
                    *response = SR_INT;

                    return accepted;
                }
            }

            return myTargetWindow;
        }

        case ESCAPE_SHOW_FOLDER_DIALOG:
        {
            QByteArray title; streamIn(reader, title);
            QByteArray value; streamIn(reader, value);

            QFileDialog dialog(m_widget, title, value);

            dialog.setFileMode(QFileDialog::Directory);
            dialog.setOption(QFileDialog::ShowDirsOnly);

            connect(this, SIGNAL(cancelTask()),
                    &dialog, SLOT(reject()));

            bool accepted = (dialog.exec() == QDialog::Accepted);

            QStringList list = dialog.selectedFiles();

            if(!list.isEmpty())
            {
                *result = list.first().toUtf8();
            }

            if(m_cancelTask)
            {
                *response = SR_NONE;

                return true;
            }
            else
            {
                *response = SR_STRING;

                return accepted;
            }
        }

        case ESCAPE_SHOW_FOLDER_DIALOG_ON_WINDOW:
        {
            quint8 targetWindow; streamIn(reader, targetWindow);

            SerialWindow *myTargetWindow = windowCast
            <SerialWindow>(__FUNCTION__, targetWindow);

            if(myTargetWindow)
            {
                QByteArray title; streamIn(reader, title);
                QByteArray value; streamIn(reader, value);

                QFileDialog dialog(myTargetWindow, title, value);

                dialog.setFileMode(QFileDialog::Directory);
                dialog.setOption(QFileDialog::ShowDirsOnly);

                connect(this, SIGNAL(cancelTask()),
                        &dialog, SLOT(reject()));

                bool accepted = (dialog.exec() == QDialog::Accepted);

                QStringList list = dialog.selectedFiles();

                if(!list.isEmpty())
                {
                    *result = list.first().toUtf8();
                }

                if(m_cancelTask)
                {
                    *response = SR_NONE;

                    return true;
                }
                else
                {
                    *response = SR_STRING;

                    return accepted;
                }
            }

            return myTargetWindow;
        }

        case ESCAPE_SHOW_OPEN_FILE_DIALOG:
        {
            QByteArray title; streamIn(reader, title);
            QByteArray value; streamIn(reader, value);
            QByteArray filter; streamIn(reader, filter);

            QFileDialog dialog(m_widget, title, value, filter);

            dialog.setFileMode(QFileDialog::ExistingFile);

            connect(this, SIGNAL(cancelTask()),
                    &dialog, SLOT(reject()));

            bool accepted = (dialog.exec() == QDialog::Accepted);

            QStringList list = dialog.selectedFiles();

            if(!list.isEmpty())
            {
                *result = list.first().toUtf8();
            }

            if(m_cancelTask)
            {
                *response = SR_NONE;

                return true;
            }
            else
            {
                *response = SR_STRING;

                return accepted;
            }
        }

        case ESCAPE_SHOW_OPEN_FILE_DIALOG_ON_WINDOW:
        {
            quint8 targetWindow; streamIn(reader, targetWindow);

            SerialWindow *myTargetWindow = windowCast
            <SerialWindow>(__FUNCTION__, targetWindow);

            if(myTargetWindow)
            {
                QByteArray title; streamIn(reader, title);
                QByteArray value; streamIn(reader, value);
                QByteArray filter; streamIn(reader, filter);

                QFileDialog dialog(myTargetWindow, title, value, filter);

                dialog.setFileMode(QFileDialog::ExistingFile);

                connect(this, SIGNAL(cancelTask()),
                        &dialog, SLOT(reject()));

                bool accepted = (dialog.exec() == QDialog::Accepted);

                QStringList list = dialog.selectedFiles();

                if(!list.isEmpty())
                {
                    *result = list.first().toUtf8();
                }

                if(m_cancelTask)
                {
                    *response = SR_NONE;

                    return true;
                }
                else
                {
                    *response = SR_STRING;

                    return accepted;
                }
            }

            return myTargetWindow;
        }

        case ESCAPE_SHOW_SAVE_FILE_DIALOG:
        {
            QByteArray title; streamIn(reader, title);
            QByteArray value; streamIn(reader, value);
            QByteArray filter; streamIn(reader, filter);

            QFileDialog dialog(m_widget, title, value, filter);

            dialog.setFileMode(QFileDialog::AnyFile);

            connect(this, SIGNAL(cancelTask()),
                    &dialog, SLOT(reject()));

            bool accepted = (dialog.exec() == QDialog::Accepted);

            QStringList list = dialog.selectedFiles();

            if(!list.isEmpty())
            {
                *result = list.first().toUtf8();
            }

            if(m_cancelTask)
            {
                *response = SR_NONE;

                return true;
            }
            else
            {
                *response = SR_STRING;

                return accepted;
            }
        }

        case ESCAPE_SHOW_SAVE_FILE_DIALOG_ON_WINDOW:
        {
            quint8 targetWindow; streamIn(reader, targetWindow);

            SerialWindow *myTargetWindow = windowCast
            <SerialWindow>(__FUNCTION__, targetWindow);

            if(myTargetWindow)
            {
                QByteArray title; streamIn(reader, title);
                QByteArray value; streamIn(reader, value);
                QByteArray filter; streamIn(reader, filter);

                QFileDialog dialog(myTargetWindow, title, value, filter);

                dialog.setFileMode(QFileDialog::AnyFile);

                connect(this, SIGNAL(cancelTask()),
                        &dialog, SLOT(reject()));

                bool accepted = (dialog.exec() == QDialog::Accepted);

                QStringList list = dialog.selectedFiles();

                if(!list.isEmpty())
                {
                    *result = list.first().toUtf8();
                }

                if(m_cancelTask)
                {
                    *response = SR_NONE;

                    return true;
                }
                else
                {
                    *response = SR_STRING;

                    return accepted;
                }
            }

            return myTargetWindow;
        }

        case ESCAPE_SHOW_FONT_DIALOG:
        {
            QByteArray title; streamIn(reader, title);

            QFontDialog dialog(m_widget);

            dialog.setWindowTitle(title);
            dialog.setOption(QFontDialog::ScalableFonts);
            dialog.setOption(QFontDialog::NonScalableFonts);
            dialog.setOption(QFontDialog::MonospacedFonts);
            dialog.setOption(QFontDialog::ProportionalFonts);

            connect(this, SIGNAL(cancelTask()),
                    &dialog, SLOT(reject()));

            bool accepted = (dialog.exec() == QDialog::Accepted);

            *result = dialog.selectedFont().toString().toUtf8();

            if(m_cancelTask)
            {
                *response = SR_NONE;

                return true;
            }
            else
            {
                *response = SR_STRING;

                return accepted;
            }
        }

        case ESCAPE_SHOW_FONT_DIALOG_ON_WINDOW:
        {
            quint8 targetWindow; streamIn(reader, targetWindow);

            SerialWindow *myTargetWindow = windowCast
            <SerialWindow>(__FUNCTION__, targetWindow);

            if(myTargetWindow)
            {
                QByteArray title; streamIn(reader, title);

                QFontDialog dialog(myTargetWindow);

                dialog.setWindowTitle(title);
                dialog.setOption(QFontDialog::ScalableFonts);
                dialog.setOption(QFontDialog::NonScalableFonts);
                dialog.setOption(QFontDialog::MonospacedFonts);
                dialog.setOption(QFontDialog::ProportionalFonts);

                connect(this, SIGNAL(cancelTask()),
                        &dialog, SLOT(reject()));

                bool accepted = (dialog.exec() == QDialog::Accepted);

                *result = dialog.selectedFont().toString().toUtf8();

                if(m_cancelTask)
                {
                    *response = SR_NONE;

                    return true;
                }
                else
                {
                    *response = SR_STRING;

                    return accepted;
                }
            }

            return myTargetWindow;
        }

        case ESCAPE_SHOW_DOUBLE_DIALOG:
        {
            QByteArray title; streamIn(reader, title);
            QByteArray label; streamIn(reader, label);
            float min; streamIn(reader, min);
            float max; streamIn(reader, max);
            float value; streamIn(reader, value);
            float decimals; streamIn(reader, decimals);

            QInputDialog dialog(m_widget,
            Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint |
            Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);

            dialog.setWindowTitle(title);
            dialog.setLabelText(label);
            dialog.setDoubleRange(min, max);
            dialog.setDoubleValue(value);
            dialog.setDoubleDecimals(decimals);
            dialog.setInputMode(QInputDialog::DoubleInput);

            connect(this, SIGNAL(cancelTask()),
                    &dialog, SLOT(reject()));

            bool accepted = (dialog.exec() == QDialog::Accepted);

            *result = dialog.doubleValue();

            if(m_cancelTask)
            {
                *response = SR_NONE;

                return true;
            }
            else
            {
                *response = SR_FLOAT;

                return accepted;
            }
        }

        case ESCAPE_SHOW_DOUBLE_DIALOG_ON_WINDOW:
        {
            quint8 targetWindow; streamIn(reader, targetWindow);

            SerialWindow *myTargetWindow = windowCast
            <SerialWindow>(__FUNCTION__, targetWindow);

            if(myTargetWindow)
            {
                QByteArray title; streamIn(reader, title);
                QByteArray label; streamIn(reader, label);
                float min; streamIn(reader, min);
                float max; streamIn(reader, max);
                float value; streamIn(reader, value);
                float decimals; streamIn(reader, decimals);

                QInputDialog dialog(myTargetWindow,
                Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint |
                Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);

                dialog.setWindowTitle(title);
                dialog.setLabelText(label);
                dialog.setDoubleRange(min, max);
                dialog.setDoubleValue(value);
                dialog.setDoubleDecimals(decimals);
                dialog.setInputMode(QInputDialog::DoubleInput);

                connect(this, SIGNAL(cancelTask()),
                        &dialog, SLOT(reject()));

                bool accepted = (dialog.exec() == QDialog::Accepted);

                *result = dialog.doubleValue();

                if(m_cancelTask)
                {
                    *response = SR_NONE;

                    return true;
                }
                else
                {
                    *response = SR_FLOAT;

                    return accepted;
                }
            }

            return myTargetWindow;
        }

        case ESCAPE_SHOW_INT_DIALOG:
        {
            QByteArray title; streamIn(reader, title);
            QByteArray label; streamIn(reader, label);
            qint32 min; streamIn(reader, min);
            qint32 max; streamIn(reader, max);
            qint32 value; streamIn(reader, value);
            qint32 step; streamIn(reader, step);

            QInputDialog dialog(m_widget,
            Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint |
            Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);

            dialog.setWindowTitle(title);
            dialog.setLabelText(label);
            dialog.setIntRange(min, max);
            dialog.setIntValue(value);
            dialog.setIntStep(step);
            dialog.setInputMode(QInputDialog::IntInput);

            connect(this, SIGNAL(cancelTask()),
                    &dialog, SLOT(reject()));

            bool accepted = (dialog.exec() == QDialog::Accepted);

            *result = dialog.intValue();

            if(m_cancelTask)
            {
                *response = SR_NONE;

                return true;
            }
            else
            {
                *response = SR_INT;

                return accepted;
            }
        }

        case ESCAPE_SHOW_INT_DIALOG_ON_WINDOW:
        {
            quint8 targetWindow; streamIn(reader, targetWindow);

            SerialWindow *myTargetWindow = windowCast
            <SerialWindow>(__FUNCTION__, targetWindow);

            if(myTargetWindow)
            {
                QByteArray title; streamIn(reader, title);
                QByteArray label; streamIn(reader, label);
                qint32 min; streamIn(reader, min);
                qint32 max; streamIn(reader, max);
                qint32 value; streamIn(reader, value);
                qint32 step; streamIn(reader, step);

                QInputDialog dialog(myTargetWindow,
                Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint |
                Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);

                dialog.setWindowTitle(title);
                dialog.setLabelText(label);
                dialog.setIntRange(min, max);
                dialog.setIntValue(value);
                dialog.setIntStep(step);

                dialog.setInputMode(QInputDialog::IntInput);

                connect(this, SIGNAL(cancelTask()),
                        &dialog, SLOT(reject()));

                bool accepted = (dialog.exec() == QDialog::Accepted);

                *result = dialog.intValue();

                if(m_cancelTask)
                {
                    *response = SR_NONE;

                    return true;
                }
                else
                {
                    *response = SR_INT;

                    return accepted;
                }
            }

            return myTargetWindow;
        }

        case ESCAPE_SHOW_TEXT_DIALOG:
        {
            QByteArray title; streamIn(reader, title);
            QByteArray label; streamIn(reader, label);
            QByteArray text; streamIn(reader, text);
            qint32 mode; streamIn(reader, mode);
            qint32 inputMethodHints; streamIn(reader, inputMethodHints);

            QInputDialog dialog(m_widget,
            Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint |
            Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);

            dialog.setWindowTitle(title);
            dialog.setLabelText(label);
            dialog.setTextValue(text);
            dialog.setTextEchoMode(
            static_cast<QLineEdit::EchoMode>(mode));
            dialog.setInputMethodHints(
            static_cast<Qt::InputMethodHints>(inputMethodHints));
            dialog.setInputMode(QInputDialog::TextInput);

            connect(this, SIGNAL(cancelTask()),
                    &dialog, SLOT(reject()));

            bool accepted = (dialog.exec() == QDialog::Accepted);

            *result = dialog.textValue().toUtf8();

            if(m_cancelTask)
            {
                *response = SR_NONE;

                return true;
            }
            else
            {
                *response = SR_STRING;

                return accepted;
            }
        }

        case ESCAPE_SHOW_TEXT_DIALOG_ON_WINDOW:
        {
            quint8 targetWindow; streamIn(reader, targetWindow);

            SerialWindow *myTargetWindow = windowCast
            <SerialWindow>(__FUNCTION__, targetWindow);

            if(myTargetWindow)
            {
                QByteArray title; streamIn(reader, title);
                QByteArray label; streamIn(reader, label);
                QByteArray text; streamIn(reader, text);
                qint32 mode; streamIn(reader, mode);
                qint32 inputMethodHints; streamIn(reader, inputMethodHints);

                QInputDialog dialog(myTargetWindow,
                Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint |
                Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);

                dialog.setWindowTitle(title);
                dialog.setLabelText(label);
                dialog.setTextValue(text);
                dialog.setTextEchoMode(
                static_cast<QLineEdit::EchoMode>(mode));
                dialog.setInputMethodHints(
                static_cast<Qt::InputMethodHints>(inputMethodHints));
                dialog.setInputMode(QInputDialog::TextInput);

                connect(this, SIGNAL(cancelTask()),
                        &dialog, SLOT(reject()));

                bool accepted = (dialog.exec() == QDialog::Accepted);

                *result = dialog.textValue().toUtf8();

                if(m_cancelTask)
                {
                    *response = SR_NONE;

                    return true;
                }
                else
                {
                    *response = SR_STRING;

                    return accepted;
                }
            }

            return myTargetWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Output Dialog Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case ESCAPE_SHOW_CRITICAL_DIALOG:
        {
            QByteArray title; streamIn(reader, title);
            QByteArray text; streamIn(reader, text);

            QMessageBox messageBox(QMessageBox::Critical, title, text,
            QMessageBox::Ok, m_widget);

            messageBox.setDefaultButton(QMessageBox::Ok);

            connect(this, SIGNAL(cancelTask()),
                    &messageBox, SLOT(reject()));

            *result = messageBox.exec();

            if(m_cancelTask)
            {
                *response = SR_NONE;
            }
            else
            {
                *response = SR_INT;
            }

            return true;
        }

        case ESCAPE_SHOW_CRITICAL_DIALOG_ON_WINDOW:
        {
            quint8 targetWindow; streamIn(reader, targetWindow);

            SerialWindow *myTargetWindow = windowCast
            <SerialWindow>(__FUNCTION__, targetWindow);

            if(myTargetWindow)
            {
                QByteArray title; streamIn(reader, title);
                QByteArray text; streamIn(reader, text);

                QMessageBox messageBox(QMessageBox::Critical, title, text,
                QMessageBox::Ok,
                myTargetWindow);

                messageBox.setDefaultButton(QMessageBox::Ok);

                connect(this, SIGNAL(cancelTask()),
                        &messageBox, SLOT(reject()));

                *result = messageBox.exec();

                if(m_cancelTask)
                {
                    *response = SR_NONE;
                }
                else
                {
                    *response = SR_INT;
                }

                return true;
            }

            return myTargetWindow;
        }

        case ESCAPE_SHOW_INFORMATION_DIALOG:
        {
            QByteArray title; streamIn(reader, title);
            QByteArray text; streamIn(reader, text);

            QMessageBox messageBox(QMessageBox::Information, title, text,
            QMessageBox::Ok, m_widget);

            messageBox.setDefaultButton(QMessageBox::Ok);

            connect(this, SIGNAL(cancelTask()),
                    &messageBox, SLOT(reject()));

            *result = messageBox.exec();

            if(m_cancelTask)
            {
                *response = SR_NONE;
            }
            else
            {
                *response = SR_INT;
            }

            return true;
        }

        case ESCAPE_SHOW_INFORMATION_DIALOG_ON_WINDOW:
        {
            quint8 targetWindow; streamIn(reader, targetWindow);

            SerialWindow *myTargetWindow = windowCast
            <SerialWindow>(__FUNCTION__, targetWindow);

            if(myTargetWindow)
            {
                QByteArray title; streamIn(reader, title);
                QByteArray text; streamIn(reader, text);

                QMessageBox messageBox(QMessageBox::Information, title, text,
                QMessageBox::Ok,
                myTargetWindow);

                messageBox.setDefaultButton(QMessageBox::Ok);

                connect(this, SIGNAL(cancelTask()),
                        &messageBox, SLOT(reject()));

                *result = messageBox.exec();

                if(m_cancelTask)
                {
                    *response = SR_NONE;
                }
                else
                {
                    *response = SR_INT;
                }

                return true;
            }

            return myTargetWindow;
        }

        case ESCAPE_SHOW_QUESTION_DIALOG:
        {
            QByteArray title; streamIn(reader, title);
            QByteArray text; streamIn(reader, text);

            QMessageBox messageBox(QMessageBox::Question, title, text,
            QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, m_widget);

            messageBox.setDefaultButton(QMessageBox::Cancel);

            connect(this, SIGNAL(cancelTask()),
                    &messageBox, SLOT(reject()));

            *result = messageBox.exec();

            if(m_cancelTask)
            {
                *response = SR_NONE;
            }
            else
            {
                *response = SR_INT;
            }

            return true;
        }

        case ESCAPE_SHOW_QUESTION_DIALOG_ON_WINDOW:
        {
            quint8 targetWindow; streamIn(reader, targetWindow);

            SerialWindow *myTargetWindow = windowCast
            <SerialWindow>(__FUNCTION__, targetWindow);

            if(myTargetWindow)
            {
                QByteArray title; streamIn(reader, title);
                QByteArray text; streamIn(reader, text);

                QMessageBox messageBox(QMessageBox::Question, title, text,
                QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel,
                myTargetWindow);

                messageBox.setDefaultButton(QMessageBox::Cancel);

                connect(this, SIGNAL(cancelTask()),
                        &messageBox, SLOT(reject()));

                *result = messageBox.exec();

                if(m_cancelTask)
                {
                    *response = SR_NONE;
                }
                else
                {
                    *response = SR_INT;
                }

                return true;
            }

            return myTargetWindow;
        }

        case ESCAPE_SHOW_WARNING_DIALOG:
        {
            QByteArray title; streamIn(reader, title);
            QByteArray text; streamIn(reader, text);

            QMessageBox messageBox(QMessageBox::Warning, title, text,
            QMessageBox::Ok, m_widget);

            messageBox.setDefaultButton(QMessageBox::Ok);

            connect(this, SIGNAL(cancelTask()),
                    &messageBox, SLOT(reject()));

            *result = messageBox.exec();

            if(m_cancelTask)
            {
                *response = SR_NONE;
            }
            else
            {
                *response = SR_INT;
            }

            return true;
        }

        case ESCAPE_SHOW_WARNING_DIALOG_ON_WINDOW:
        {
            quint8 targetWindow; streamIn(reader, targetWindow);

            SerialWindow *myTargetWindow = windowCast
            <SerialWindow>(__FUNCTION__, targetWindow);

            if(myTargetWindow)
            {
                QByteArray title; streamIn(reader, title);
                QByteArray text; streamIn(reader, text);

                QMessageBox messageBox(QMessageBox::Warning, title, text,
                QMessageBox::Ok,
                myTargetWindow);

                messageBox.setDefaultButton(QMessageBox::Ok);

                connect(this, SIGNAL(cancelTask()),
                        &messageBox, SLOT(reject()));

                *result = messageBox.exec();

                if(m_cancelTask)
                {
                    *response = SR_NONE;
                }
                else
                {
                    *response = SR_INT;
                }

                return true;
            }

            return myTargetWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Window Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case WINDOW_CLOSE_WINDOW:
        {
            quint8 window; streamIn(reader, window);

            SerialWindow *myWindow = windowCast
            <SerialWindow>(__FUNCTION__, window);

            if(myWindow)
            {
                myWindow->close();
            }

            return myWindow;
        }

        case WINDOW_CLOSE_WINDOWS:
        {
            foreach(SerialWindow *window, m_windows.values())
            {
                window->close();
            }

            return true;
        }

        case WINDOW_DELETE_WINDOW:
        {
            quint8 window; streamIn(reader, window);

            SerialWindow *myWindow = windowCast
            <SerialWindow>(__FUNCTION__, window);

            if(myWindow)
            {
                myWindow->setAttribute(Qt::WA_DeleteOnClose, true);
                myWindow->close(); delete myWindow; // delete faster
            }

            return myWindow;
        }

        case WINDOW_DELETE_WINDOWS:
        {
            foreach(SerialWindow *window, m_windows.values())
            {
                window->setAttribute(Qt::WA_DeleteOnClose, true);
                window->close(); delete window; // delete faster
            }

            return true;
        }

        case WINDOW_SET_WINDOW_TITLE:
        {
            quint8 window; streamIn(reader, window);

            SerialWindow *myWindow = windowCast
            <SerialWindow>(__FUNCTION__, window);

            if(myWindow)
            {
                QByteArray title; streamIn(reader, title);

                myWindow->setWindowTitle(title);
            }

            return myWindow;
        }

        case WINDOW_GET_WINDOW_TITLE:
        {
            quint8 window; streamIn(reader, window);

            SerialWindow *myWindow = windowCast
            <SerialWindow>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_STRING; *result =
                myWindow->windowTitle().toUtf8();
            }

            return myWindow;
        }

        case WINDOW_SET_WINDOW_MESSAGE:
        {
            quint8 window; streamIn(reader, window);

            SerialWindow *myWindow = windowCast
            <SerialWindow>(__FUNCTION__, window);

            if(myWindow)
            {
                QByteArray message; streamIn(reader, message);

                myWindow->setWindowMessage(message);
            }

            return myWindow;
        }

        case WINDOW_GET_WINDOW_MESSAGE:
        {
            quint8 window; streamIn(reader, window);

            SerialWindow *myWindow = windowCast
            <SerialWindow>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_STRING; *result =
                myWindow->getWindowMessage().toUtf8();
            }

            return myWindow;
        }

        case WINDOW_SET_WINDOW_VISIBLE:
        {
            quint8 window; streamIn(reader, window);

            SerialWindow *myWindow = windowCast
            <SerialWindow>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 show; streamIn(reader, show);

                if(show)
                {
                    myWindow->show();
                }
                else
                {
                    myWindow->hide();
                }
            }

            return myWindow;
        }

        case WINDOW_GET_WINDOW_VISIBLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                !myWindow->isHidden());
            }

            return myWindow;
        }

        case WINDOW_SET_WINDOW_MINIMIZED:
        {
            quint8 window; streamIn(reader, window);

            SerialWindow *myWindow = windowCast
            <SerialWindow>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 show; streamIn(reader, show);

                if(show)
                {
                    myWindow->showMinimized();
                }
                else
                {
                    myWindow->showNormal();
                }
            }

            return myWindow;
        }

        case WINDOW_GET_WINDOW_MINIMIZED:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->isMinimized());
            }

            return myWindow;
        }

        case WINDOW_SET_WINDOW_MAXIMIZED:
        {
            quint8 window; streamIn(reader, window);

            SerialWindow *myWindow = windowCast
            <SerialWindow>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 show; streamIn(reader, show);

                if(show)
                {
                    myWindow->showMaximized();
                }
                else
                {
                    myWindow->showNormal();
                }
            }

            return myWindow;
        }

        case WINDOW_GET_WINDOW_MAXIMIZED:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->isMaximized());
            }

            return myWindow;
        }

        case WINDOW_SET_WINDOW_FULL_SCREEN:
        {
            quint8 window; streamIn(reader, window);

            SerialWindow *myWindow = windowCast
            <SerialWindow>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 show; streamIn(reader, show);

                if(show)
                {
                    myWindow->showFullScreen();
                }
                else
                {
                    myWindow->showNormal();
                }
            }

            return myWindow;
        }

        case WINDOW_GET_WINDOW_FULL_SCREEN:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->isFullScreen());
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Terminal Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case TERMINAL_NEW_UNDOCKED_WINDOW:
        {
            quint8 window; streamIn(reader, window);
            QByteArray name; streamIn(reader, name);

            return newTerminalWindow(window, name);
        }

        case TERMINAL_NEW_DOCKED_WINDOW:
        {
            quint8 window; streamIn(reader, window);
            quint8 targetWindow; streamIn(reader, targetWindow);

            SerialWindow *myTargetWindow = windowCast
            <SerialWindow>(__FUNCTION__, targetWindow);

            if(myTargetWindow)
            {
                QByteArray name; streamIn(reader, name);

                SerialWindow *myWindow =
                newTerminalWindow(window, name, false);

                if(myWindow && (!myWindow->parent())
                && (!dockWindow(myTargetWindow->widget(), myWindow)))
                {
                    myWindow->setFloating(true);
                    myWindow->show();
                }

                return myWindow;
            }

            return myTargetWindow;
        }

        case TERMINAL_NEW_MAIN_DOCKED_WINDOW:
        {
            quint8 window; streamIn(reader, window);
            QByteArray name; streamIn(reader, name);

            SerialWindow *myWindow =
            newTerminalWindow(window, name, false);

            if(myWindow && (!myWindow->parent())
            && (!dockWindow(m_widget, myWindow)))
            {
                myWindow->setFloating(true);
                myWindow->show();
            }

            return myWindow;
        }

        case TERMINAL_PRINT:
        {
            quint8 window; streamIn(reader, window);

            SerialTerminal *myWindow = windowCast
            <SerialTerminal>(__FUNCTION__, window);

            if(myWindow)
            {
                QByteArray bytes; streamIn(reader, bytes);

                myWindow->receive(bytes);
            }

            return myWindow;
        }

        case TERMINAL_PRINTLN:
        {
            quint8 window; streamIn(reader, window);

            SerialTerminal *myWindow = windowCast
            <SerialTerminal>(__FUNCTION__, window);

            if(myWindow)
            {
                QByteArray bytes; streamIn(reader, bytes);

                myWindow->receive(bytes + '\n');
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Oscilloscope Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case OSCILLOSCOPE_NEW_UNDOCKED_WINDOW:
        {
            quint8 window; streamIn(reader, window);
            QByteArray name; streamIn(reader, name);

            return newOscilloscopeWindow(window, name);
        }

        case OSCILLOSCOPE_NEW_DOCKED_WINDOW:
        {
            quint8 window; streamIn(reader, window);
            quint8 targetWindow; streamIn(reader, targetWindow);

            SerialWindow *myTargetWindow = windowCast
            <SerialWindow>(__FUNCTION__, targetWindow);

            if(myTargetWindow)
            {
                QByteArray name; streamIn(reader, name);

                SerialWindow *myWindow =
                newOscilloscopeWindow(window, name, false);

                if(myWindow && (!myWindow->parent())
                && (!dockWindow(myTargetWindow->widget(), myWindow)))
                {
                    myWindow->setFloating(true);
                    myWindow->show();
                }

                return myWindow;
            }

            return myTargetWindow;
        }

        case OSCILLOSCOPE_NEW_MAIN_DOCKED_WINDOW:
        {
            quint8 window; streamIn(reader, window);
            QByteArray name; streamIn(reader, name);

            SerialWindow *myWindow =
            newOscilloscopeWindow(window, name, false);

            if(myWindow && (!myWindow->parent())
            && (!dockWindow(m_widget, myWindow)))
            {
                myWindow->setFloating(true);
                myWindow->show();
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_DATA:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);

                return myWindow->removeData(plottable);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_DATAS:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                myWindow->removeDatas();
            }

            return myWindow;
        }

        case OSCILLOSCOPE_DELETE_PLOT:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);

                return myWindow->deletePlot(plottable);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_DELETE_PLOTS:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                myWindow->deletePlots();
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BACKGROUND_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 rgba; streamIn(reader, rgba);

                myWindow->setBackgroundColor(rgba);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BACKGROUND_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBackgroundColor());
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Oscilloscope XAxis Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case OSCILLOSCOPE_SET_XAXIS_SCALE_TYPE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 scaleType; streamIn(reader, scaleType);

                return myWindow->setXAxisScaleType(
                static_cast<QCPAxis::ScaleType>(scaleType));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_XAXIS_SCALE_TYPE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getXAxisScaleType());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_XAXIS_SCALE_LOG_BASE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 base; streamIn(reader, base);

                return myWindow->setXAxisScaleLogBase(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_XAXIS_SCALE_LOG_BASE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getXAxisScaleLogBase());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_XAXIS_SCALE_LOG_BASE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                float base; streamIn(reader, base);

                return myWindow->setXAxisScaleLogBase(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_XAXIS_SCALE_LOG_BASE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getXAxisScaleLogBase());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_XAXIS_RANGE_AUTO_SCALE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 on; streamIn(reader, on);

                myWindow->setXAxisRangeAutoScale(on);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_XAXIS_RANGE_AUTO_SCALE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getXAxisRangeAutoScale());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_XAXIS_RANGE_LOWER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 base; streamIn(reader, base);

                return myWindow->setXAxisRangeLower(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_XAXIS_RANGE_LOWER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getXAxisRangeLower());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_XAXIS_RANGE_LOWER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                float base; streamIn(reader, base);

                return myWindow->setXAxisRangeLower(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_XAXIS_RANGE_LOWER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getXAxisRangeLower());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_XAXIS_RANGE_UPPER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 base; streamIn(reader, base);

                return myWindow->setXAxisRangeUpper(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_XAXIS_RANGE_UPPER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getXAxisRangeUpper());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_XAXIS_RANGE_UPPER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                float base; streamIn(reader, base);

                return myWindow->setXAxisRangeUpper(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_XAXIS_RANGE_UPPER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getXAxisRangeUpper());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_XAXIS_RANGE_REVERSED:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 reversed; streamIn(reader, reversed);

                myWindow->setXAxisRangeReversed(reversed);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_XAXIS_RANGE_REVERSED:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getXAxisRangeReversed());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_XAXIS_LABEL:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                QByteArray name; streamIn(reader, name);

                myWindow->setXAxisLabel(name);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_XAXIS_LABEL:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_STRING; *result =
                myWindow->getXAxisLabel().toUtf8();
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Oscilloscope YAxis Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case OSCILLOSCOPE_SET_YAXIS_SCALE_TYPE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 scaleType; streamIn(reader, scaleType);

                return myWindow->setYAxisScaleType(
                static_cast<QCPAxis::ScaleType>(scaleType));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_YAXIS_SCALE_TYPE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getYAxisScaleType());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_YAXIS_SCALE_LOG_BASE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 base; streamIn(reader, base);

                return myWindow->setYAxisScaleLogBase(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_YAXIS_SCALE_LOG_BASE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getYAxisScaleLogBase());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_YAXIS_SCALE_LOG_BASE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                float base; streamIn(reader, base);

                return myWindow->setYAxisScaleLogBase(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_YAXIS_SCALE_LOG_BASE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getYAxisScaleLogBase());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_YAXIS_RANGE_LOWER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 base; streamIn(reader, base);

                return myWindow->setYAxisRangeLower(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_YAXIS_RANGE_LOWER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getYAxisRangeLower());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_YAXIS_RANGE_LOWER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                float base; streamIn(reader, base);

                return myWindow->setYAxisRangeLower(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_YAXIS_RANGE_LOWER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getYAxisRangeLower());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_YAXIS_RANGE_AUTO_SCALE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 on; streamIn(reader, on);

                myWindow->setYAxisRangeAutoScale(on);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_YAXIS_RANGE_AUTO_SCALE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getYAxisRangeAutoScale());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_YAXIS_RANGE_UPPER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 base; streamIn(reader, base);

                return myWindow->setYAxisRangeUpper(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_YAXIS_RANGE_UPPER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getYAxisRangeUpper());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_YAXIS_RANGE_UPPER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                float base; streamIn(reader, base);

                return myWindow->setYAxisRangeUpper(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_YAXIS_RANGE_UPPER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getYAxisRangeUpper());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_YAXIS_RANGE_REVERSED:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 reversed; streamIn(reader, reversed);

                myWindow->setYAxisRangeReversed(reversed);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_YAXIS_RANGE_REVERSED:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getYAxisRangeReversed());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_YAXIS_LABEL:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                QByteArray name; streamIn(reader, name);

                myWindow->setYAxisLabel(name);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_YAXIS_LABEL:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_STRING; *result =
                myWindow->getYAxisLabel().toUtf8();
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Oscilloscope FFTXAxis Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case OSCILLOSCOPE_SET_FFTXAXIS_SCALE_TYPE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 scaleType; streamIn(reader, scaleType);

                return myWindow->setFFTXAxisScaleType(
                static_cast<QCPAxis::ScaleType>(scaleType));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_FFTXAXIS_SCALE_TYPE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getFFTXAxisScaleType());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_FFTXAXIS_SCALE_LOG_BASE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 base; streamIn(reader, base);

                return myWindow->setFFTXAxisScaleLogBase(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_FFTXAXIS_SCALE_LOG_BASE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getFFTXAxisScaleLogBase());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_FFTXAXIS_SCALE_LOG_BASE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                float base; streamIn(reader, base);

                return myWindow->setFFTXAxisScaleLogBase(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_FFTXAXIS_SCALE_LOG_BASE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getFFTXAxisScaleLogBase());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_FFTXAXIS_RANGE_AUTO_SCALE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 on; streamIn(reader, on);

                myWindow->setFFTXAxisRangeAutoScale(on);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_FFTXAXIS_RANGE_AUTO_SCALE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getFFTXAxisRangeAutoScale());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_FFTXAXIS_RANGE_LOWER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 base; streamIn(reader, base);

                return myWindow->setFFTXAxisRangeLower(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_FFTXAXIS_RANGE_LOWER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getFFTXAxisRangeLower());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_FFTXAXIS_RANGE_LOWER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                float base; streamIn(reader, base);

                return myWindow->setFFTXAxisRangeLower(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_FFTXAXIS_RANGE_LOWER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getFFTXAxisRangeLower());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_FFTXAXIS_RANGE_UPPER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 base; streamIn(reader, base);

                return myWindow->setFFTXAxisRangeUpper(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_FFTXAXIS_RANGE_UPPER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getFFTXAxisRangeUpper());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_FFTXAXIS_RANGE_UPPER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                float base; streamIn(reader, base);

                return myWindow->setFFTXAxisRangeUpper(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_FFTXAXIS_RANGE_UPPER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getFFTXAxisRangeUpper());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_FFTXAXIS_RANGE_REVERSED:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 reversed; streamIn(reader, reversed);

                myWindow->setFFTXAxisRangeReversed(reversed);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_FFTXAXIS_RANGE_REVERSED:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getFFTXAxisRangeReversed());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_FFTXAXIS_LABEL:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                QByteArray name; streamIn(reader, name);

                myWindow->setFFTXAxisLabel(name);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_FFTXAXIS_LABEL:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_STRING; *result =
                myWindow->getFFTXAxisLabel().toUtf8();
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Oscilloscope FFTYAxis Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case OSCILLOSCOPE_SET_FFTYAXIS_SCALE_TYPE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 scaleType; streamIn(reader, scaleType);

                return myWindow->setFFTYAxisScaleType(
                static_cast<QCPAxis::ScaleType>(scaleType));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_FFTYAXIS_SCALE_TYPE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getFFTYAxisScaleType());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_FFTYAXIS_SCALE_LOG_BASE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 base; streamIn(reader, base);

                return myWindow->setFFTYAxisScaleLogBase(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_FFTYAXIS_SCALE_LOG_BASE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getFFTYAxisScaleLogBase());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_FFTYAXIS_SCALE_LOG_BASE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                float base; streamIn(reader, base);

                return myWindow->setFFTYAxisScaleLogBase(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_FFTYAXIS_SCALE_LOG_BASE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getFFTYAxisScaleLogBase());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_FFTYAXIS_RANGE_AUTO_SCALE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 on; streamIn(reader, on);

                myWindow->setFFTYAxisRangeAutoScale(on);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_FFTYAXIS_RANGE_AUTO_SCALE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getFFTYAxisRangeAutoScale());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_FFTYAXIS_RANGE_LOWER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 base; streamIn(reader, base);

                return myWindow->setFFTYAxisRangeLower(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_FFTYAXIS_RANGE_LOWER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getFFTYAxisRangeLower());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_FFTYAXIS_RANGE_LOWER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                float base; streamIn(reader, base);

                return myWindow->setFFTYAxisRangeLower(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_FFTYAXIS_RANGE_LOWER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getFFTYAxisRangeLower());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_FFTYAXIS_RANGE_UPPER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 base; streamIn(reader, base);

                return myWindow->setFFTYAxisRangeUpper(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_FFTYAXIS_RANGE_UPPER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getFFTYAxisRangeUpper());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_FFTYAXIS_RANGE_UPPER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                float base; streamIn(reader, base);

                return myWindow->setFFTYAxisRangeUpper(base);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_FFTYAXIS_RANGE_UPPER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getFFTYAxisRangeUpper());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_FFTYAXIS_RANGE_REVERSED:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 reversed; streamIn(reader, reversed);

                myWindow->setFFTYAxisRangeReversed(reversed);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_FFTYAXIS_RANGE_REVERSED:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getFFTYAxisRangeReversed());
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_FFTYAXIS_LABEL:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                QByteArray name; streamIn(reader, name);

                myWindow->setFFTYAxisLabel(name);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_FFTYAXIS_LABEL:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_STRING; *result =
                myWindow->getFFTYAxisLabel().toUtf8();
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Oscilloscope Bar Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case OSCILLOSCOPE_NEW_BAR:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                QByteArray name; streamIn(reader, name);

                return myWindow->newBar(plottable, name);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BAR_DATA:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);

                return myWindow->removeBarData(plottable);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_BAR_DATA_0:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 key; streamIn(reader, key);
                qint32 value; streamIn(reader, value);

                return myWindow->addBarData(plottable, key, value);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_BAR_DATA_0_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 key; streamIn(reader, key);
                    qint32 value; streamIn(reader, value);

                    myWindow->addBarData(plottable, key, value);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_BAR_DATA_0_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float key; streamIn(reader, key);
                float value; streamIn(reader, value);

                return myWindow->addBarData(plottable, key, value);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_BAR_DATA_0_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float key; streamIn(reader, key);
                    float value; streamIn(reader, value);

                    myWindow->addBarData(plottable, key, value);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_BAR_DATA_1:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 value; streamIn(reader, value);

                return myWindow->addBarData(plottable, value);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_BAR_DATA_1_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 value; streamIn(reader, value);

                    myWindow->addBarData(plottable, value);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_BAR_DATA_1_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float value; streamIn(reader, value);

                return myWindow->addBarData(plottable, value);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_BAR_DATA_1_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float value; streamIn(reader, value);

                    myWindow->addBarData(plottable, value);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BAR_DATA_BEFORE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 key; streamIn(reader, key);

                return myWindow->removeBarDataBefore(plottable, key);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BAR_DATA_BEFORE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float key; streamIn(reader, key);

                return myWindow->removeBarDataBefore(plottable, key);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BAR_DATA_AFTER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 key; streamIn(reader, key);

                return myWindow->removeBarDateAfter(plottable, key);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BAR_DATA_AFTER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float key; streamIn(reader, key);

                return myWindow->removeBarDateAfter(plottable, key);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BAR_DATA_BETWEEN:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 fKey; streamIn(reader, fKey);
                qint32 tKey; streamIn(reader, tKey);

                return myWindow->removeBarDataBetween(plottable, fKey, tKey);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BAR_DATA_BETWEEN_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 fKey; streamIn(reader, fKey);
                    qint32 tKey; streamIn(reader, tKey);

                    myWindow->removeBarDataBetween(plottable, fKey, tKey);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BAR_DATA_BETWEEN_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float fKey; streamIn(reader, fKey);
                float tKey; streamIn(reader, tKey);

                return myWindow->removeBarDataBetween(plottable, fKey, tKey);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BAR_DATA_BETWEEN_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float fKey; streamIn(reader, fKey);
                    float tKey; streamIn(reader, tKey);

                    myWindow->removeBarDataBetween(plottable, fKey, tKey);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BAR_DATUM:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 key; streamIn(reader, key);

                return myWindow->removeBarDataBetween(plottable, key, key);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BAR_DATUM_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 key; streamIn(reader, key);

                    myWindow->removeBarDataBetween(plottable, key, key);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BAR_DATUM_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float key; streamIn(reader, key);

                return myWindow->removeBarDataBetween(plottable, key, key);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BAR_DATUM_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float key; streamIn(reader, key);

                    myWindow->removeBarDataBetween(plottable, key, key);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BAR_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                QByteArray name; streamIn(reader, name);

                return myWindow->setBarName(plottable, name);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BAR_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_STRING; *result =
                myWindow->getBarName(plottable, &ok).toUtf8();

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BAR_COORDINATE_SYSTEM:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 coordinateSystem; streamIn(reader, coordinateSystem);

                return myWindow->setBarCoordinateSystem(plottable,
                static_cast<CoordinateSystem>(coordinateSystem));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BAR_COORDINATE_SYSTEM:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBarCoordinateSystem(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BAR_ANGLE_UNITS:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 angleUnits; streamIn(reader, angleUnits);

                return myWindow->setBarAngleUnits(plottable,
                static_cast<AngleUnits>(angleUnits));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BAR_ANGLE_UNITS:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBarAngleUnits(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BAR_LINE_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 rgba; streamIn(reader, rgba);

                return myWindow->setBarLineColor(plottable, rgba);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BAR_LINE_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBarLineColor(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BAR_LINE_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 lineStyle; streamIn(reader, lineStyle);

                return myWindow->setBarLineStyle(plottable,
                static_cast<Qt::PenStyle>(lineStyle));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BAR_LINE_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBarLineStyle(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BAR_FILL_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 rgba; streamIn(reader, rgba);

                return myWindow->setBarFillColor(plottable, rgba);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BAR_FILL_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBarFillColor(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BAR_FILL_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 fillStyle; streamIn(reader, fillStyle);

                return myWindow->setBarFillStyle(plottable,
                static_cast<Qt::BrushStyle>(fillStyle));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BAR_FILL_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBarFillStyle(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BAR_DELTA_MODE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 deltaMode; streamIn(reader, deltaMode);

                return myWindow->setBarDeltaMode(plottable, deltaMode);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BAR_DELTA_MODE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBarDeltaMode(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BAR_SIZE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 size; streamIn(reader, size);

                return myWindow->setBarSize(plottable, size);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BAR_SIZE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBarSize(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BAR_SAMPLE_RATE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 sampleRate; streamIn(reader, sampleRate);

                return myWindow->setBarSampleRate(plottable, sampleRate);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BAR_SAMPLE_RATE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBarSampleRate(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BAR_SAMPLE_RATE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float sampleRate; streamIn(reader, sampleRate);

                return myWindow->setBarSampleRate(plottable, sampleRate);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BAR_SAMPLE_RATE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getBarSampleRate(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BAR_SCALER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 scaler; streamIn(reader, scaler);

                return myWindow->setBarScaler(plottable, scaler);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BAR_SCALER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBarScaler(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BAR_SCALER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float scaler; streamIn(reader, scaler);

                return myWindow->setBarScaler(plottable, scaler);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BAR_SCALER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getBarScaler(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BAR_WIDTH:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 width; streamIn(reader, width);

                return myWindow->setBarWidth(plottable, width);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BAR_WIDTH:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBarWidth(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BAR_WIDTH_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float width; streamIn(reader, width);

                return myWindow->setBarWidth(plottable, width);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BAR_WIDTH_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getBarWidth(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BAR_SCATTER_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 scatterShape; streamIn(reader, scatterShape);

                return myWindow->setBarScatterStyle(plottable,
                static_cast<QCPScatterStyle::ScatterShape>(scatterShape));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BAR_SCATTER_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBarScatterStyle(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BAR_BELOW:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 targetPlottable; streamIn(reader, targetPlottable);

                return myWindow->setBarBelow(plottable, targetPlottable);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BAR_BELOW:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBarBelow(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BAR_ABOVE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 targetPlottable; streamIn(reader, targetPlottable);

                return myWindow->setBarAbove(plottable, targetPlottable);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BAR_ABOVE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBarAbove(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Oscilloscope Curve Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case OSCILLOSCOPE_NEW_CURVE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                QByteArray name; streamIn(reader, name);

                return myWindow->newCurve(plottable, name);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_CURVE_DATA:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);

                return myWindow->removeCurveData(plottable);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_CURVE_DATA_0:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 index; streamIn(reader, index);
                qint32 key; streamIn(reader, key);
                qint32 value; streamIn(reader, value);

                return myWindow->addCurveData(plottable, index, key, value);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_CURVE_DATA_0_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 index; streamIn(reader, index);
                    qint32 key; streamIn(reader, key);
                    qint32 value; streamIn(reader, value);

                    myWindow->addCurveData(plottable, index, key, value);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_CURVE_DATA_0_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float index; streamIn(reader, index);
                float key; streamIn(reader, key);
                float value; streamIn(reader, value);

                return myWindow->addCurveData(plottable, index, key, value);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_CURVE_DATA_0_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float index; streamIn(reader, index);
                    float key; streamIn(reader, key);
                    float value; streamIn(reader, value);

                    myWindow->addCurveData(plottable, index, key, value);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_CURVE_DATA_1:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 key; streamIn(reader, key);
                qint32 value; streamIn(reader, value);

                return myWindow->addCurveData(plottable, key, value);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_CURVE_DATA_1_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 key; streamIn(reader, key);
                    qint32 value; streamIn(reader, value);

                    myWindow->addCurveData(plottable, key, value);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_CURVE_DATA_1_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float key; streamIn(reader, key);
                float value; streamIn(reader, value);

                return myWindow->addCurveData(plottable, key, value);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_CURVE_DATA_1_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float key; streamIn(reader, key);
                    float value; streamIn(reader, value);

                    myWindow->addCurveData(plottable, key, value);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_CURVE_DATA_BEFORE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 index; streamIn(reader, index);

                return myWindow->removeCurveDataBefore(plottable, index);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_CURVE_DATA_BEFORE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float index; streamIn(reader, index);

                return myWindow->removeCurveDataBefore(plottable, index);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_CURVE_DATA_AFTER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 index; streamIn(reader, index);

                return myWindow->removeCurveDateAfter(plottable, index);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_CURVE_DATA_AFTER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float index; streamIn(reader, index);

                return myWindow->removeCurveDateAfter(plottable, index);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_CURVE_DATA_BETWEEN:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 fIndex; streamIn(reader, fIndex);
                qint32 tIndex; streamIn(reader, tIndex);

                return myWindow->removeCurveDataBetween(plottable,
                                                        fIndex, tIndex);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_CURVE_DATA_BETWEEN_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 fIndex; streamIn(reader, fIndex);
                    qint32 tIndex; streamIn(reader, tIndex);

                    myWindow->removeCurveDataBetween(plottable,
                                                     fIndex, tIndex);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_CURVE_DATA_BETWEEN_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float fIndex; streamIn(reader, fIndex);
                float tIndex; streamIn(reader, tIndex);

                return myWindow->removeCurveDataBetween(plottable,
                                                        fIndex, tIndex);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_CURVE_DATA_BETWEEN_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float fIndex; streamIn(reader, fIndex);
                    float tIndex; streamIn(reader, tIndex);

                    myWindow->removeCurveDataBetween(plottable,
                                                     fIndex, tIndex);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_CURVE_DATUM:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 index; streamIn(reader, index);

                return myWindow->removeCurveDataBetween(plottable,
                                                        index, index);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_CURVE_DATUM_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 index; streamIn(reader, index);

                    myWindow->removeCurveDataBetween(plottable,
                                                     index, index);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_CURVE_DATUM_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float index; streamIn(reader, index);

                return myWindow->removeCurveDataBetween(plottable,
                                                        index, index);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_CURVE_DATUM_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float index; streamIn(reader, index);

                    myWindow->removeCurveDataBetween(plottable,
                                                     index, index);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_CURVE_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                QByteArray name; streamIn(reader, name);

                return myWindow->setCurveName(plottable, name);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_CURVE_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_STRING; *result =
                myWindow->getCurveName(plottable, &ok).toUtf8();

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_CURVE_COORDINATE_SYSTEM:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 coordinateSystem; streamIn(reader, coordinateSystem);

                return myWindow->setCurveCoordinateSystem(plottable,
                static_cast<CoordinateSystem>(coordinateSystem));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_CURVE_COORDINATE_SYSTEM:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getCurveCoordinateSystem(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_CURVE_ANGLE_UNITS:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 angleUnits; streamIn(reader, angleUnits);

                return myWindow->setCurveAngleUnits(plottable,
                static_cast<AngleUnits>(angleUnits));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_CURVE_ANGLE_UNITS:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getCurveAngleUnits(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_CURVE_LINE_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 rgba; streamIn(reader, rgba);

                return myWindow->setCurveLineColor(plottable, rgba);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_CURVE_LINE_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getCurveLineColor(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_CURVE_LINE_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 lineStyle; streamIn(reader, lineStyle);

                return myWindow->setCurveLineStyle(plottable,
                static_cast<Qt::PenStyle>(lineStyle));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_CURVE_LINE_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getCurveLineStyle(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_CURVE_FILL_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 rgba; streamIn(reader, rgba);

                return myWindow->setCurveFillColor(plottable, rgba);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_CURVE_FILL_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getCurveFillColor(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_CURVE_FILL_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 fillStyle; streamIn(reader, fillStyle);

                return myWindow->setCurveFillStyle(plottable,
                static_cast<Qt::BrushStyle>(fillStyle));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_CURVE_FILL_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getCurveFillStyle(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_CURVE_DELTA_MODE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 deltaMode; streamIn(reader, deltaMode);

                return myWindow->setCurveDeltaMode(plottable, deltaMode);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_CURVE_DELTA_MODE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getCurveDeltaMode(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_CURVE_SIZE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 size; streamIn(reader, size);

                return myWindow->setCurveSize(plottable, size);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_CURVE_SIZE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getCurveSize(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_CURVE_SAMPLE_RATE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 sampleRate; streamIn(reader, sampleRate);

                return myWindow->setCurveSampleRate(plottable, sampleRate);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_CURVE_SAMPLE_RATE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getCurveSampleRate(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_CURVE_SAMPLE_RATE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float sampleRate; streamIn(reader, sampleRate);

                return myWindow->setCurveSampleRate(plottable, sampleRate);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_CURVE_SAMPLE_RATE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getCurveSampleRate(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_CURVE_SCALER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 scaler; streamIn(reader, scaler);

                return myWindow->setCurveScaler(plottable, scaler);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_CURVE_SCALER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getCurveScaler(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_CURVE_SCALER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float scaler; streamIn(reader, scaler);

                return myWindow->setCurveScaler(plottable, scaler);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_CURVE_SCALER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getCurveScaler(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_CURVE_FFT:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 option; streamIn(reader, option);

                return myWindow->setCurveFFT(plottable,
                static_cast<CurveFFT>(option));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_CURVE_FFT:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getCurveFFT(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_CURVE_FFT_WINDOW:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 option; streamIn(reader, option);

                return myWindow->setCurveFFTWindowFunction(plottable,
                static_cast<CurveFFTWF>(option));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_CURVE_FFT_WINDOW:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getCurveFFTWindowFunction(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_CURVE_SCATTER_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 scatterShape; streamIn(reader, scatterShape);

                return myWindow->setCurveScatterStyle(plottable,
                static_cast<QCPScatterStyle::ScatterShape>(scatterShape));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_CURVE_SCATTER_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getCurveScatterStyle(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_CURVE_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 style; streamIn(reader, style);

                return myWindow->setCurveStyle(plottable,
                static_cast<QCPCurve::LineStyle>(style));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_CURVE_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getCurveStyle(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_CURVE_CHANNEL_FILL_CURVE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 targetPlottable; streamIn(reader, targetPlottable);

                return myWindow->setCurveChannelFillCurve(plottable,
                targetPlottable);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_CURVE_CHANNEL_FILL_CURVE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getCurveChannelFillCurve(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Oscilloscope Graph Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case OSCILLOSCOPE_NEW_GRAPH:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                QByteArray name; streamIn(reader, name);

                return myWindow->newGraph(plottable, name);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_GRAPH_DATA:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);

                return myWindow->removeGraphData(plottable);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_0:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 key; streamIn(reader, key);
                qint32 value; streamIn(reader, value);

                return myWindow->addGraphData(plottable, key, value);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_0_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 key; streamIn(reader, key);
                    qint32 value; streamIn(reader, value);

                    myWindow->addGraphData(plottable, key, value);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_0_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float key; streamIn(reader, key);
                float value; streamIn(reader, value);

                return myWindow->addGraphData(plottable, key, value);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_0_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float key; streamIn(reader, key);
                    float value; streamIn(reader, value);

                    myWindow->addGraphData(plottable, key, value);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_1:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 value; streamIn(reader, value);

                return myWindow->addGraphData(plottable, value);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_1_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 value; streamIn(reader, value);

                    myWindow->addGraphData(plottable, value);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_1_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float value; streamIn(reader, value);

                return myWindow->addGraphData(plottable, value);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_1_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float value; streamIn(reader, value);

                    myWindow->addGraphData(plottable, value);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_0:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 key; streamIn(reader, key);
                qint32 value; streamIn(reader, value);
                qint32 keyError; streamIn(reader, keyError);
                qint32 valueError; streamIn(reader, valueError);

                return myWindow->addGraphDataWithErrors(plottable,
                key, value, keyError, valueError);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_0_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 key; streamIn(reader, key);
                    qint32 value; streamIn(reader, value);
                    qint32 keyError; streamIn(reader, keyError);
                    qint32 valueError; streamIn(reader, valueError);

                    myWindow->addGraphDataWithErrors(plottable,
                    key, value, keyError, valueError);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_0_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float key; streamIn(reader, key);
                float value; streamIn(reader, value);
                float keyError; streamIn(reader, keyError);
                float valueError; streamIn(reader, valueError);

                return myWindow->addGraphDataWithErrors(plottable,
                key, value, keyError, valueError);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_0_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float key; streamIn(reader, key);
                    float value; streamIn(reader, value);
                    float keyError; streamIn(reader, keyError);
                    float valueError; streamIn(reader, valueError);

                    myWindow->addGraphDataWithErrors(plottable,
                    key, value, keyError, valueError);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_1:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 value; streamIn(reader, value);
                qint32 keyError; streamIn(reader, keyError);
                qint32 valueError; streamIn(reader, valueError);

                return myWindow->addGraphDataWithErrors(plottable,
                value, keyError, valueError);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_1_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 value; streamIn(reader, value);
                    qint32 keyError; streamIn(reader, keyError);
                    qint32 valueError; streamIn(reader, valueError);

                    myWindow->addGraphDataWithErrors(plottable,
                    value, keyError, valueError);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_1_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float value; streamIn(reader, value);
                float keyError; streamIn(reader, keyError);
                float valueError; streamIn(reader, valueError);

                return myWindow->addGraphDataWithErrors(plottable,
                value, keyError, valueError);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_1_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float value; streamIn(reader, value);
                    float keyError; streamIn(reader, keyError);
                    float valueError; streamIn(reader, valueError);

                    myWindow->addGraphDataWithErrors(plottable,
                    value, keyError, valueError);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_2:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 key; streamIn(reader, key);
                qint32 value; streamIn(reader, value);
                qint32 keyErrorMinus; streamIn(reader, keyErrorMinus);
                qint32 keyErrorPlus; streamIn(reader, keyErrorPlus);
                qint32 valueErrorMinus; streamIn(reader, valueErrorMinus);
                qint32 valueErrorPlus; streamIn(reader, valueErrorPlus);

                return myWindow->addGraphDataWithErrors(plottable,
                key, value, keyErrorMinus, keyErrorPlus,
                valueErrorMinus, valueErrorPlus);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_2_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 key; streamIn(reader, key);
                    qint32 value; streamIn(reader, value);
                    qint32 keyErrorMinus; streamIn(reader, keyErrorMinus);
                    qint32 keyErrorPlus; streamIn(reader, keyErrorPlus);
                    qint32 valueErrorMinus; streamIn(reader, valueErrorMinus);
                    qint32 valueErrorPlus; streamIn(reader, valueErrorPlus);

                    myWindow->addGraphDataWithErrors(plottable,
                    key, value, keyErrorMinus, keyErrorPlus,
                    valueErrorMinus, valueErrorPlus);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_2_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float key; streamIn(reader, key);
                float value; streamIn(reader, value);
                float keyErrorMinus; streamIn(reader, keyErrorMinus);
                float keyErrorPlus; streamIn(reader, keyErrorPlus);
                float valueErrorMinus; streamIn(reader, valueErrorMinus);
                float valueErrorPlus; streamIn(reader, valueErrorPlus);

                return myWindow->addGraphDataWithErrors(plottable,
                key, value, keyErrorMinus, keyErrorPlus,
                valueErrorMinus, valueErrorPlus);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_2_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float key; streamIn(reader, key);
                    float value; streamIn(reader, value);
                    float keyErrorMinus; streamIn(reader, keyErrorMinus);
                    float keyErrorPlus; streamIn(reader, keyErrorPlus);
                    float valueErrorMinus; streamIn(reader, valueErrorMinus);
                    float valueErrorPlus; streamIn(reader, valueErrorPlus);

                    myWindow->addGraphDataWithErrors(plottable,
                    key, value, keyErrorMinus, keyErrorPlus,
                    valueErrorMinus, valueErrorPlus);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_3:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 value; streamIn(reader, value);
                qint32 keyErrorMinus; streamIn(reader, keyErrorMinus);
                qint32 keyErrorPlus; streamIn(reader, keyErrorPlus);
                qint32 valueErrorMinus; streamIn(reader, valueErrorMinus);
                qint32 valueErrorPlus; streamIn(reader, valueErrorPlus);

                return myWindow->addGraphDataWithErrors(plottable,
                value, keyErrorMinus, keyErrorPlus,
                valueErrorMinus, valueErrorPlus);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_3_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 value; streamIn(reader, value);
                    qint32 keyErrorMinus; streamIn(reader, keyErrorMinus);
                    qint32 keyErrorPlus; streamIn(reader, keyErrorPlus);
                    qint32 valueErrorMinus; streamIn(reader, valueErrorMinus);
                    qint32 valueErrorPlus; streamIn(reader, valueErrorPlus);

                    myWindow->addGraphDataWithErrors(plottable,
                    value, keyErrorMinus, keyErrorPlus,
                    valueErrorMinus, valueErrorPlus);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_3_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float value; streamIn(reader, value);
                float keyErrorMinus; streamIn(reader, keyErrorMinus);
                float keyErrorPlus; streamIn(reader, keyErrorPlus);
                float valueErrorMinus; streamIn(reader, valueErrorMinus);
                float valueErrorPlus; streamIn(reader, valueErrorPlus);

                return myWindow->addGraphDataWithErrors(plottable,
                value, keyErrorMinus, keyErrorPlus,
                valueErrorMinus, valueErrorPlus);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_3_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float value; streamIn(reader, value);
                    float keyErrorMinus; streamIn(reader, keyErrorMinus);
                    float keyErrorPlus; streamIn(reader, keyErrorPlus);
                    float valueErrorMinus; streamIn(reader, valueErrorMinus);
                    float valueErrorPlus; streamIn(reader, valueErrorPlus);

                    myWindow->addGraphDataWithErrors(plottable,
                    value, keyErrorMinus, keyErrorPlus,
                    valueErrorMinus, valueErrorPlus);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_GRAPH_DATA_BEFORE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 key; streamIn(reader, key);

                return myWindow->removeGraphDataBefore(plottable, key);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_GRAPH_DATA_BEFORE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float key; streamIn(reader, key);

                return myWindow->removeGraphDataBefore(plottable, key);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_GRAPH_DATA_AFTER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 key; streamIn(reader, key);

                return myWindow->removeGraphDateAfter(plottable, key);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_GRAPH_DATA_AFTER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float key; streamIn(reader, key);

                return myWindow->removeGraphDateAfter(plottable, key);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_GRAPH_DATA_BETWEEN:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 fKey; streamIn(reader, fKey);
                qint32 tKey; streamIn(reader, tKey);

                return myWindow->removeGraphDataBetween(plottable, fKey, tKey);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_GRAPH_DATA_BETWEEN_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 fKey; streamIn(reader, fKey);
                    qint32 tKey; streamIn(reader, tKey);

                    myWindow->removeGraphDataBetween(plottable, fKey, tKey);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_GRAPH_DATA_BETWEEN_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float fKey; streamIn(reader, fKey);
                float tKey; streamIn(reader, tKey);

                return myWindow->removeGraphDataBetween(plottable, fKey, tKey);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_GRAPH_DATA_BETWEEN_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float fKey; streamIn(reader, fKey);
                    float tKey; streamIn(reader, tKey);

                    myWindow->removeGraphDataBetween(plottable, fKey, tKey);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_GRAPH_DATUM:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 key; streamIn(reader, key);

                return myWindow->removeGraphDataBetween(plottable, key, key);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_GRAPH_DATUM_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 key; streamIn(reader, key);

                    myWindow->removeGraphDataBetween(plottable, key, key);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_GRAPH_DATUM_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float key; streamIn(reader, key);

                return myWindow->removeGraphDataBetween(plottable, key, key);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_GRAPH_DATUM_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float key; streamIn(reader, key);

                    myWindow->removeGraphDataBetween(plottable, key, key);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_GRAPH_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                QByteArray name; streamIn(reader, name);

                return myWindow->setGraphName(plottable, name);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_GRAPH_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_STRING; *result =
                myWindow->getGraphName(plottable, &ok).toUtf8();

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_GRAPH_COORDINATE_SYSTEM:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 coordinateSystem; streamIn(reader, coordinateSystem);

                return myWindow->setGraphCoordinateSystem(plottable,
                static_cast<CoordinateSystem>(coordinateSystem));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_GRAPH_COORDINATE_SYSTEM:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getGraphCoordinateSystem(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_GRAPH_ANGLE_UNITS:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 angleUnits; streamIn(reader, angleUnits);

                return myWindow->setGraphAngleUnits(plottable,
                static_cast<AngleUnits>(angleUnits));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_GRAPH_ANGLE_UNITS:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getGraphAngleUnits(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_GRAPH_LINE_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 rgba; streamIn(reader, rgba);

                return myWindow->setGraphLineColor(plottable, rgba);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_GRAPH_LINE_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getGraphLineColor(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_GRAPH_LINE_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 lineStyle; streamIn(reader, lineStyle);

                return myWindow->setGraphLineStyle(plottable,
                static_cast<Qt::PenStyle>(lineStyle));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_GRAPH_LINE_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getGraphLineStyle(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_GRAPH_FILL_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 rgba; streamIn(reader, rgba);

                return myWindow->setGraphFillColor(plottable, rgba);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_GRAPH_FILL_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getGraphFillColor(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_GRAPH_FILL_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 fillStyle; streamIn(reader, fillStyle);

                return myWindow->setGraphFillStyle(plottable,
                static_cast<Qt::BrushStyle>(fillStyle));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_GRAPH_FILL_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getGraphFillStyle(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_GRAPH_DELTA_MODE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 deltaMode; streamIn(reader, deltaMode);

                return myWindow->setGraphDeltaMode(plottable, deltaMode);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_GRAPH_DELTA_MODE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getGraphDeltaMode(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_GRAPH_SIZE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 size; streamIn(reader, size);

                return myWindow->setGraphSize(plottable, size);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_GRAPH_SIZE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getGraphSize(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_GRAPH_SAMPLE_RATE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 sampleRate; streamIn(reader, sampleRate);

                return myWindow->setGraphSampleRate(plottable, sampleRate);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_GRAPH_SAMPLE_RATE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getGraphSampleRate(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_GRAPH_SAMPLE_RATE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float sampleRate; streamIn(reader, sampleRate);

                return myWindow->setGraphSampleRate(plottable, sampleRate);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_GRAPH_SAMPLE_RATE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getGraphSampleRate(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_GRAPH_SCALER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 scaler; streamIn(reader, scaler);

                return myWindow->setGraphScaler(plottable, scaler);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_GRAPH_SCALER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getGraphScaler(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_GRAPH_SCALER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float scaler; streamIn(reader, scaler);

                return myWindow->setGraphScaler(plottable, scaler);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_GRAPH_SCALER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getGraphScaler(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_GRAPH_FFT:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 option; streamIn(reader, option);

                return myWindow->setGraphFFT(plottable,
                static_cast<GraphFFT>(option));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_GRAPH_FFT:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getGraphFFT(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_GRAPH_FFT_WINDOW:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 option; streamIn(reader, option);

                return myWindow->setGraphFFTWindowFunction(plottable,
                static_cast<GraphFFTWF>(option));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_GRAPH_FFT_WINDOW:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getGraphFFTWindowFunction(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_GRAPH_SCATTER_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 scatterShape; streamIn(reader, scatterShape);

                return myWindow->setGraphScatterStyle(plottable,
                static_cast<QCPScatterStyle::ScatterShape>(scatterShape));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_GRAPH_SCATTER_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getGraphScatterStyle(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_GRAPH_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 style; streamIn(reader, style);

                return myWindow->setGraphStyle(plottable,
                static_cast<QCPGraph::LineStyle>(style));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_GRAPH_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getGraphStyle(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_GRAPH_CHANNEL_FILL_GRAPH:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 targetPlottable; streamIn(reader, targetPlottable);

                return myWindow->setGraphChannelFillGraph(plottable,
                targetPlottable);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_GRAPH_CHANNEL_FILL_GRAPH:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getGraphChannelFillGraph(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_GRAPH_KEY_ERRORS:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 keyErrors; streamIn(reader, keyErrors);

                return myWindow->setGraphKeyErrors(plottable, keyErrors);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_GRAPH_KEY_ERRORS:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getGraphKeyErrors(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_GRAPH_VALUE_ERRORS:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 valueErrors; streamIn(reader, valueErrors);

                return myWindow->setGraphValueErrors(plottable, valueErrors);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_GRAPH_VALUE_ERRORS:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getGraphValueErrors(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Oscilloscope Box Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case OSCILLOSCOPE_NEW_BOX:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                QByteArray name; streamIn(reader, name);

                return myWindow->newBox(plottable, name);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BOX_DATA:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);

                return myWindow->removeBoxData(plottable);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_BOX_DATA_0:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 index; streamIn(reader, index);
                qint32 value; streamIn(reader, value);

                return myWindow->addBoxData(plottable, index, value);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_BOX_DATA_0_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 index; streamIn(reader, index);
                    qint32 value; streamIn(reader, value);

                    myWindow->addBoxData(plottable, index, value);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_BOX_DATA_0_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float index; streamIn(reader, index);
                float value; streamIn(reader, value);

                return myWindow->addBoxData(plottable, index, value);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_BOX_DATA_0_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float index; streamIn(reader, index);
                    float value; streamIn(reader, value);

                    myWindow->addBoxData(plottable, index, value);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_BOX_DATA_1:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 value; streamIn(reader, value);

                return myWindow->addBoxData(plottable, value);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_BOX_DATA_1_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 value; streamIn(reader, value);

                    myWindow->addBoxData(plottable, value);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_BOX_DATA_1_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float value; streamIn(reader, value);

                return myWindow->addBoxData(plottable, value);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_ADD_BOX_DATA_1_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float value; streamIn(reader, value);

                    myWindow->addBoxData(plottable, value);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BOX_DATA_BEFORE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 index; streamIn(reader, index);

                return myWindow->removeBoxDataBefore(plottable, index);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BOX_DATA_BEFORE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float index; streamIn(reader, index);

                return myWindow->removeBoxDataBefore(plottable, index);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BOX_DATA_AFTER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 index; streamIn(reader, index);

                return myWindow->removeBoxDateAfter(plottable, index);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BOX_DATA_AFTER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float index; streamIn(reader, index);

                return myWindow->removeBoxDateAfter(plottable, index);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BOX_DATA_BETWEEN:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 fIndex; streamIn(reader, fIndex);
                qint32 tIndex; streamIn(reader, tIndex);

                return myWindow->removeBoxDataBetween(plottable,
                                                      fIndex, tIndex);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BOX_DATA_BETWEEN_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 fIndex; streamIn(reader, fIndex);
                    qint32 tIndex; streamIn(reader, tIndex);

                    myWindow->removeBoxDataBetween(plottable,
                                                   fIndex, tIndex);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BOX_DATA_BETWEEN_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float fIndex; streamIn(reader, fIndex);
                float tIndex; streamIn(reader, tIndex);

                return myWindow->removeBoxDataBetween(plottable,
                                                      fIndex, tIndex);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BOX_DATA_BETWEEN_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float fIndex; streamIn(reader, fIndex);
                    float tIndex; streamIn(reader, tIndex);

                    myWindow->removeBoxDataBetween(plottable,
                                                   fIndex, tIndex);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BOX_DATUM:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 index; streamIn(reader, index);

                return myWindow->removeBoxDataBetween(plottable,
                                                      index, index);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BOX_DATUM_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 index; streamIn(reader, index);

                    myWindow->removeBoxDataBetween(plottable,
                                                   index, index);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BOX_DATUM_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float index; streamIn(reader, index);

                return myWindow->removeBoxDataBetween(plottable,
                                                      index, index);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_REMOVE_BOX_DATUM_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float index; streamIn(reader, index);

                    myWindow->removeBoxDataBetween(plottable,
                                                   index, index);
                }
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                QByteArray name; streamIn(reader, name);

                return myWindow->setBoxName(plottable, name);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_STRING; *result =
                myWindow->getBoxName(plottable, &ok).toUtf8();

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_COORDINATE_SYSTEM:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 coordinateSystem; streamIn(reader, coordinateSystem);

                return myWindow->setBoxCoordinateSystem(plottable,
                static_cast<CoordinateSystem>(coordinateSystem));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_COORDINATE_SYSTEM:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBoxCoordinateSystem(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_ANGLE_UNITS:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 angleUnits; streamIn(reader, angleUnits);

                return myWindow->setBoxAngleUnits(plottable,
                static_cast<AngleUnits>(angleUnits));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_ANGLE_UNITS:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBoxAngleUnits(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_LINE_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 rgba; streamIn(reader, rgba);

                return myWindow->setBoxLineColor(plottable, rgba);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_LINE_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBoxLineColor(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_LINE_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 lineStyle; streamIn(reader, lineStyle);

                return myWindow->setBoxLineStyle(plottable,
                static_cast<Qt::PenStyle>(lineStyle));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_LINE_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBoxLineStyle(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_FILL_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 rgba; streamIn(reader, rgba);

                return myWindow->setBoxFillColor(plottable, rgba);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_FILL_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBoxFillColor(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_FILL_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 fillStyle; streamIn(reader, fillStyle);

                return myWindow->setBoxFillStyle(plottable,
                static_cast<Qt::BrushStyle>(fillStyle));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_FILL_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBoxFillStyle(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_DELTA_MODE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 deltaMode; streamIn(reader, deltaMode);

                return myWindow->setBoxDeltaMode(plottable, deltaMode);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_DELTA_MODE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBoxDeltaMode(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_SIZE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 size; streamIn(reader, size);

                return myWindow->setBoxSize(plottable, size);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_SIZE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBoxSize(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_SAMPLE_RATE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 sampleRate; streamIn(reader, sampleRate);

                return myWindow->setBoxSampleRate(plottable, sampleRate);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_SAMPLE_RATE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBoxSampleRate(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_SAMPLE_RATE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float sampleRate; streamIn(reader, sampleRate);

                return myWindow->setBoxSampleRate(plottable, sampleRate);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_SAMPLE_RATE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getBoxSampleRate(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_SCALER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 scaler; streamIn(reader, scaler);

                return myWindow->setBoxScaler(plottable, scaler);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_SCALER:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBoxScaler(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_SCALER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float scaler; streamIn(reader, scaler);

                return myWindow->setBoxScaler(plottable, scaler);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_SCALER_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getBoxScaler(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_WIDTH:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 width; streamIn(reader, width);

                return myWindow->setBoxWidth(plottable, width);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_WIDTH:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBoxWidth(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_WIDTH_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float width; streamIn(reader, width);

                return myWindow->setBoxWidth(plottable, width);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_WIDTH_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<float>(
                myWindow->getBoxWidth(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_SCATTER_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 scatterShape; streamIn(reader, scatterShape);

                return myWindow->setBoxScatterStyle(plottable,
                static_cast<QCPScatterStyle::ScatterShape>(scatterShape));
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_SCATTER_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBoxScatterStyle(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_KEY:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 key; streamIn(reader, key);

                return myWindow->setBoxKey(plottable, key);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_KEY:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBoxKey(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_KEY_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float key; streamIn(reader, key);

                return myWindow->setBoxKey(plottable, key);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_KEY_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getBoxKey(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_MINIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 minimum; streamIn(reader, minimum);

                return myWindow->setBoxMinimum(plottable, minimum);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_MINIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBoxMinimum(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_MINIMUM_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float minimum; streamIn(reader, minimum);

                return myWindow->setBoxMinimum(plottable, minimum);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_MINIMUM_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getBoxMinimum(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_LOWER_QUARTILE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 lowerQuartile; streamIn(reader, lowerQuartile);

                return myWindow->setBoxLowerQuartile(plottable, lowerQuartile);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_LOWER_QUARTILE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBoxLowerQuartile(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_LOWER_QUARTILE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float lowerQuartile; streamIn(reader, lowerQuartile);

                return myWindow->setBoxLowerQuartile(plottable, lowerQuartile);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_LOWER_QUARTILE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getBoxLowerQuartile(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_MEDIAN:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 median; streamIn(reader, median);

                return myWindow->setBoxMedian(plottable, median);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_MEDIAN:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBoxMedian(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_MEDIAN_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float median; streamIn(reader, median);

                return myWindow->setBoxMedian(plottable, median);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_MEDIAN_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getBoxMedian(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_UPPER_QUARTILE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 upperQuartile; streamIn(reader, upperQuartile);

                return myWindow->setBoxUpperQuartile(plottable, upperQuartile);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_UPPER_QUARTILE:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getBoxUpperQuartile(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_UPPER_QUARTILE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float upperQuartile; streamIn(reader, upperQuartile);

                return myWindow->setBoxUpperQuartile(plottable, upperQuartile);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_UPPER_QUARTILE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getBoxUpperQuartile(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_MAXIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                qint32 maximum; streamIn(reader, maximum);

                return myWindow->setBoxMaximum(plottable, maximum);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_MAXIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBoxMaximum(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

        case OSCILLOSCOPE_SET_BOX_MAXIMUM_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 plottable; streamIn(reader, plottable);
                float maximum; streamIn(reader, maximum);

                return myWindow->setBoxMaximum(plottable, maximum);
            }

            return myWindow;
        }

        case OSCILLOSCOPE_GET_BOX_MAXIMUM_F:
        {
            quint8 window; streamIn(reader, window);

            SerialOscilloscope *myWindow = windowCast
            <SerialOscilloscope>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 plottable; streamIn(reader, plottable);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getBoxMaximum(plottable, &ok));

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Table Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case TABLE_NEW_UNDOCKED_WINDOW:
        {
            quint8 window; streamIn(reader, window);
            QByteArray name; streamIn(reader, name);

            return newTableWindow(window, name);
        }

        case TABLE_NEW_DOCKED_WINDOW:
        {
            quint8 window; streamIn(reader, window);
            quint8 targetWindow; streamIn(reader, targetWindow);

            SerialWindow *myTargetWindow = windowCast
            <SerialWindow>(__FUNCTION__, targetWindow);

            if(myTargetWindow)
            {
                QByteArray name; streamIn(reader, name);

                SerialWindow *myWindow =
                newTableWindow(window, name, false);

                if(myWindow && (!myWindow->parent())
                && (!dockWindow(myTargetWindow->widget(), myWindow)))
                {
                    myWindow->setFloating(true);
                    myWindow->show();
                }

                return myWindow;
            }

            return myTargetWindow;
        }

        case TABLE_NEW_MAIN_DOCKED_WINDOW:
        {
            quint8 window; streamIn(reader, window);
            QByteArray name; streamIn(reader, name);

            SerialWindow *myWindow =
            newTableWindow(window, name, false);

            if(myWindow && (!myWindow->parent())
            && (!dockWindow(m_widget, myWindow)))
            {
                myWindow->setFloating(true);
                myWindow->show();
            }

            return myWindow;
        }

        case TABLE_CLEAR_ALL:
        {
            quint8 window; streamIn(reader, window);

            SerialTable *myWindow = windowCast
            <SerialTable>(__FUNCTION__, window);

            if(myWindow)
            {
                myWindow->clearAll();
            }

            return myWindow;
        }

        case TABLE_NEW_ITEM:
        {
            quint8 window; streamIn(reader, window);

            SerialTable *myWindow = windowCast
            <SerialTable>(__FUNCTION__, window);

            if(myWindow)
            {
                quint16 row; streamIn(reader, row);
                quint16 column; streamIn(reader, column);
                QByteArray text; streamIn(reader, text);

                return myWindow->newItem(row, column, text);
            }

            return myWindow;
        }

        case TABLE_DELETE_ITEM:
        {
            quint8 window; streamIn(reader, window);

            SerialTable *myWindow = windowCast
            <SerialTable>(__FUNCTION__, window);

            if(myWindow)
            {
                quint16 row; streamIn(reader, row);
                quint16 column; streamIn(reader, column);

                return myWindow->deleteItem(row, column);
            }

            return myWindow;
        }

        case TABLE_DELETE_ITEMS:
        {
            quint8 window; streamIn(reader, window);

            SerialTable *myWindow = windowCast
            <SerialTable>(__FUNCTION__, window);

            if(myWindow)
            {
                myWindow->deleteItems();
            }

            return myWindow;
        }

        case TABLE_DELETE_ITEMS_IN_ROW:
        {
            quint8 window; streamIn(reader, window);

            SerialTable *myWindow = windowCast
            <SerialTable>(__FUNCTION__, window);

            if(myWindow)
            {
                quint16 row; streamIn(reader, row);

                return myWindow->deleteItemsInRow(row);
            }

            return myWindow;
        }

        case TABLE_DELETE_ITEMS_IN_COLUMN:
        {
            quint8 window; streamIn(reader, window);

            SerialTable *myWindow = windowCast
            <SerialTable>(__FUNCTION__, window);

            if(myWindow)
            {
                quint16 column; streamIn(reader, column);

                return myWindow->deleteItemsInColumn(column);
            }

            return myWindow;
        }

        case TABLE_SET_ITEM_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialTable *myWindow = windowCast
            <SerialTable>(__FUNCTION__, window);

            if(myWindow)
            {
                quint16 row; streamIn(reader, row);
                quint16 column; streamIn(reader, column);
                QByteArray text; streamIn(reader, text);

                return myWindow->setItemText(row, column, text);
            }

            return myWindow;
        }

        case TABLE_GET_ITEM_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialTable *myWindow = windowCast
            <SerialTable>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint16 row; streamIn(reader, row);
                quint16 column; streamIn(reader, column);

                *response = SR_STRING; *result =
                myWindow->getItemText(row, column, &ok).toUtf8();

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Table Row Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case TABLE_NEW_ROW:
        {
            quint8 window; streamIn(reader, window);

            SerialTable *myWindow = windowCast
            <SerialTable>(__FUNCTION__, window);

            if(myWindow)
            {
                quint16 row; streamIn(reader, row);
                QByteArray name; streamIn(reader, name);

                return myWindow->newRow(row, name);
            }

            return myWindow;
        }

        case TABLE_DELETE_ROW:
        {
            quint8 window; streamIn(reader, window);

            SerialTable *myWindow = windowCast
            <SerialTable>(__FUNCTION__, window);

            if(myWindow)
            {
                quint16 row; streamIn(reader, row);

                return myWindow->deleteRow(row);
            }

            return myWindow;
        }

        case TABLE_DELETE_ROWS:
        {
            quint8 window; streamIn(reader, window);

            SerialTable *myWindow = windowCast
            <SerialTable>(__FUNCTION__, window);

            if(myWindow)
            {
                myWindow->deleteRows();
            }

            return myWindow;
        }

        case TABLE_SET_ROW_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialTable *myWindow = windowCast
            <SerialTable>(__FUNCTION__, window);

            if(myWindow)
            {
                quint16 row; streamIn(reader, row);
                QByteArray name; streamIn(reader, name);

                return myWindow->setRowName(row, name);
            }

            return myWindow;
        }

        case TABLE_GET_ROW_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialTable *myWindow = windowCast
            <SerialTable>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint16 row; streamIn(reader, row);

                *response = SR_STRING; *result =
                myWindow->getRowName(row, &ok).toUtf8();

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Table Column Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case TABLE_NEW_COLUMN:
        {
            quint8 window; streamIn(reader, window);

            SerialTable *myWindow = windowCast
            <SerialTable>(__FUNCTION__, window);

            if(myWindow)
            {
                quint16 column; streamIn(reader, column);
                QByteArray name; streamIn(reader, name);

                return myWindow->newColumn(column, name);
            }

            return myWindow;
        }

        case TABLE_DELETE_COLUMN:
        {
            quint8 window; streamIn(reader, window);

            SerialTable *myWindow = windowCast
            <SerialTable>(__FUNCTION__, window);

            if(myWindow)
            {
                quint16 column; streamIn(reader, column);

                return myWindow->deleteColumn(column);
            }

            return myWindow;
        }

        case TABLE_DELETE_COLUMNS:
        {
            quint8 window; streamIn(reader, window);

            SerialTable *myWindow = windowCast
            <SerialTable>(__FUNCTION__, window);

            if(myWindow)
            {
                myWindow->deleteColumns();
            }

            return myWindow;
        }

        case TABLE_SET_COLUMN_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialTable *myWindow = windowCast
            <SerialTable>(__FUNCTION__, window);

            if(myWindow)
            {
                quint16 column; streamIn(reader, column);
                QByteArray name; streamIn(reader, name);

                return myWindow->setColumnName(column, name);
            }

            return myWindow;
        }

        case TABLE_GET_COLUMN_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialTable *myWindow = windowCast
            <SerialTable>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint16 column; streamIn(reader, column);

                *response = SR_STRING; *result =
                myWindow->getColumnName(column, &ok).toUtf8();

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Tree Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case TREE_NEW_UNDOCKED_WINDOW:
        {
            quint8 window; streamIn(reader, window);
            QByteArray name; streamIn(reader, name);

            return newTreeWindow(window, name);
        }

        case TREE_NEW_DOCKED_WINDOW:
        {
            quint8 window; streamIn(reader, window);
            quint8 targetWindow; streamIn(reader, targetWindow);

            SerialWindow *myTargetWindow = windowCast
            <SerialWindow>(__FUNCTION__, targetWindow);

            if(myTargetWindow)
            {
                QByteArray name; streamIn(reader, name);

                SerialWindow *myWindow =
                newTreeWindow(window, name, false);

                if(myWindow && (!myWindow->parent())
                && (!dockWindow(myTargetWindow->widget(), myWindow)))
                {
                    myWindow->setFloating(true);
                    myWindow->show();
                }

                return myWindow;
            }

            return myTargetWindow;
        }

        case TREE_NEW_MAIN_DOCKED_WINDOW:
        {
            quint8 window; streamIn(reader, window);
            QByteArray name; streamIn(reader, name);

            SerialWindow *myWindow =
            newTreeWindow(window, name, false);

            if(myWindow && (!myWindow->parent())
            && (!dockWindow(m_widget, myWindow)))
            {
                myWindow->setFloating(true);
                myWindow->show();
            }

            return myWindow;
        }

        case TREE_CLEAR_ALL:
        {
            quint8 window; streamIn(reader, window);

            SerialTree *myWindow = windowCast
            <SerialTree>(__FUNCTION__, window);

            if(myWindow)
            {
                myWindow->deleteItems();
            }

            return myWindow;
        }

        case TREE_NEW_ITEM:
        {
            quint8 window; streamIn(reader, window);

            SerialTree *myWindow = windowCast
            <SerialTree>(__FUNCTION__, window);

            if(myWindow)
            {
                QByteArray key; streamIn(reader, key);
                QByteArray value; streamIn(reader, value);

                return myWindow->newItem(key, value);
            }

            return myWindow;
        }

        case TREE_DELETE_ITEM:
        {
            quint8 window; streamIn(reader, window);

            SerialTree *myWindow = windowCast
            <SerialTree>(__FUNCTION__, window);

            if(myWindow)
            {
                QByteArray key; streamIn(reader, key);

                return myWindow->deleteItem(key);
            }

            return myWindow;
        }

        case TREE_DELETE_ITEMS:
        {
            quint8 window; streamIn(reader, window);

            SerialTree *myWindow = windowCast
            <SerialTree>(__FUNCTION__, window);

            if(myWindow)
            {
                myWindow->deleteItems();
            }

            return myWindow;
        }

        case TREE_SET_ITEM_VALUE:
        {
            quint8 window; streamIn(reader, window);

            SerialTree *myWindow = windowCast
            <SerialTree>(__FUNCTION__, window);

            if(myWindow)
            {
                QByteArray key; streamIn(reader, key);
                QByteArray value; streamIn(reader, value);

                return myWindow->setItemValue(key, value);
            }

            return myWindow;
        }

        case TREE_GET_ITEM_VALUE:
        {
            quint8 window; streamIn(reader, window);

            SerialTree *myWindow = windowCast
            <SerialTree>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                QByteArray key; streamIn(reader, key);

                *response = SR_STRING; *result =
                myWindow->getItemValue(key, &ok).toUtf8();

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Graphics Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case GRAPHICS_NEW_UNDOCKED_WINDOW:
        {
            quint8 window; streamIn(reader, window);
            QByteArray name; streamIn(reader, name);

            return newGraphicsWindow(window, name);
        }

        case GRAPHICS_NEW_DOCKED_WINDOW:
        {
            quint8 window; streamIn(reader, window);
            quint8 targetWindow; streamIn(reader, targetWindow);

            SerialWindow *myTargetWindow = windowCast
            <SerialWindow>(__FUNCTION__, targetWindow);

            if(myTargetWindow)
            {
                QByteArray name; streamIn(reader, name);

                SerialWindow *myWindow =
                newGraphicsWindow(window, name, false);

                if(myWindow && (!myWindow->parent())
                && (!dockWindow(myTargetWindow->widget(), myWindow)))
                {
                    myWindow->setFloating(true);
                    myWindow->show();
                }

                return myWindow;
            }

            return myTargetWindow;
        }

        case GRAPHICS_NEW_MAIN_DOCKED_WINDOW:
        {
            quint8 window; streamIn(reader, window);
            QByteArray name; streamIn(reader, name);

            SerialWindow *myWindow =
            newGraphicsWindow(window, name, false);

            if(myWindow && (!myWindow->parent())
            && (!dockWindow(m_widget, myWindow)))
            {
                myWindow->setFloating(true);
                myWindow->show();
            }

            return myWindow;
        }

        case GRAPHICS_CLEAR_ALL_0:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 x1; streamIn(reader, x1);
                qint32 y1; streamIn(reader, y1);
                qint32 x2; streamIn(reader, x2);
                qint32 y2; streamIn(reader, y2);

                return myWindow->clearAll(x1, y1, x2, y2);
            }

            return myWindow;
        }

        case GRAPHICS_CLEAR_ALL_0_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 x1; streamIn(reader, x1);
                    qint32 y1; streamIn(reader, y1);
                    qint32 x2; streamIn(reader, x2);
                    qint32 y2; streamIn(reader, y2);

                    myWindow->clearAll(x1, y1, x2, y2);
                }
            }

            return myWindow;
        }

        case GRAPHICS_CLEAR_ALL_0_F:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                float x1; streamIn(reader, x1);
                float y1; streamIn(reader, y1);
                float x2; streamIn(reader, x2);
                float y2; streamIn(reader, y2);

                return myWindow->clearAll(x1, y1, x2, y2);
            }

            return myWindow;
        }

        case GRAPHICS_CLEAR_ALL_0_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float x1; streamIn(reader, x1);
                    float y1; streamIn(reader, y1);
                    float x2; streamIn(reader, x2);
                    float y2; streamIn(reader, y2);

                    myWindow->clearAll(x1, y1, x2, y2);
                }
            }

            return myWindow;
        }

        case GRAPHICS_CLEAR_ALL_1:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                myWindow->clearAll();
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 x; streamIn(reader, x);
                qint32 y; streamIn(reader, y);
                QByteArray text; streamIn(reader, text);

                return myWindow->drawText(x, y, text);
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_TEXT_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 x; streamIn(reader, x);
                    qint32 y; streamIn(reader, y);
                    QByteArray text; streamIn(reader, text);

                    myWindow->drawText(x, y, text);
                }
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_TEXT_F:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                float x; streamIn(reader, x);
                float y; streamIn(reader, y);
                QByteArray text; streamIn(reader, text);

                return myWindow->drawText(x, y, text);
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_TEXT_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float x; streamIn(reader, x);
                    float y; streamIn(reader, y);
                    QByteArray text; streamIn(reader, text);

                    myWindow->drawText(x, y, text);
                }
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_LINE:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 x1; streamIn(reader, x1);
                qint32 y1; streamIn(reader, y1);
                qint32 x2; streamIn(reader, x2);
                qint32 y2; streamIn(reader, y2);

                return myWindow->drawLine(x1, y1, x2, y2);
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_LINE_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 x1; streamIn(reader, x1);
                    qint32 y1; streamIn(reader, y1);
                    qint32 x2; streamIn(reader, x2);
                    qint32 y2; streamIn(reader, y2);

                    myWindow->drawLine(x1, y1, x2, y2);
                }
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_LINE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                float x1; streamIn(reader, x1);
                float y1; streamIn(reader, y1);
                float x2; streamIn(reader, x2);
                float y2; streamIn(reader, y2);

                return myWindow->drawLine(x1, y1, x2, y2);
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_LINE_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float x1; streamIn(reader, x1);
                    float y1; streamIn(reader, y1);
                    float x2; streamIn(reader, x2);
                    float y2; streamIn(reader, y2);

                    myWindow->drawLine(x1, y1, x2, y2);
                }
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_QUADRATIC_LINE:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 x1; streamIn(reader, x1);
                qint32 y1; streamIn(reader, y1);
                qint32 x2; streamIn(reader, x2);
                qint32 y2; streamIn(reader, y2);
                qint32 x3; streamIn(reader, x3);
                qint32 y3; streamIn(reader, y3);

                return myWindow->drawQuadraticLine(x1, y1, x2, y2, x3, y3);
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_QUADRATIC_LINE_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 x1; streamIn(reader, x1);
                    qint32 y1; streamIn(reader, y1);
                    qint32 x2; streamIn(reader, x2);
                    qint32 y2; streamIn(reader, y2);
                    qint32 x3; streamIn(reader, x3);
                    qint32 y3; streamIn(reader, y3);

                    myWindow->drawQuadraticLine(x1, y1, x2, y2, x3, y3);
                }
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_QUADRATIC_LINE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                float x1; streamIn(reader, x1);
                float y1; streamIn(reader, y1);
                float x2; streamIn(reader, x2);
                float y2; streamIn(reader, y2);
                float x3; streamIn(reader, x3);
                float y3; streamIn(reader, y3);

                return myWindow->drawQuadraticLine(x1, y1, x2, y2, x3, y3);
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_QUADRATIC_LINE_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float x1; streamIn(reader, x1);
                    float y1; streamIn(reader, y1);
                    float x2; streamIn(reader, x2);
                    float y2; streamIn(reader, y2);
                    float x3; streamIn(reader, x3);
                    float y3; streamIn(reader, y3);

                    myWindow->drawQuadraticLine(x1, y1, x2, y2, x3, y3);
                }
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_CUBIC_LINE:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 x1; streamIn(reader, x1);
                qint32 y1; streamIn(reader, y1);
                qint32 x2; streamIn(reader, x2);
                qint32 y2; streamIn(reader, y2);
                qint32 x3; streamIn(reader, x3);
                qint32 y3; streamIn(reader, y3);
                qint32 x4; streamIn(reader, x4);
                qint32 y4; streamIn(reader, y4);

                return myWindow->drawCubicLine(x1,y1,x2,y2,x3,y3,x4,y4);
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_CUBIC_LINE_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 x1; streamIn(reader, x1);
                    qint32 y1; streamIn(reader, y1);
                    qint32 x2; streamIn(reader, x2);
                    qint32 y2; streamIn(reader, y2);
                    qint32 x3; streamIn(reader, x3);
                    qint32 y3; streamIn(reader, y3);
                    qint32 x4; streamIn(reader, x4);
                    qint32 y4; streamIn(reader, y4);

                    myWindow->drawCubicLine(x1,y1,x2,y2,x3,y3,x4,y4);
                }
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_CUBIC_LINE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                float x1; streamIn(reader, x1);
                float y1; streamIn(reader, y1);
                float x2; streamIn(reader, x2);
                float y2; streamIn(reader, y2);
                float x3; streamIn(reader, x3);
                float y3; streamIn(reader, y3);
                float x4; streamIn(reader, x4);
                float y4; streamIn(reader, y4);

                return myWindow->drawCubicLine(x1,y1,x2,y2,x3,y3,x4,y4);
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_CUBIC_LINE_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float x1; streamIn(reader, x1);
                    float y1; streamIn(reader, y1);
                    float x2; streamIn(reader, x2);
                    float y2; streamIn(reader, y2);
                    float x3; streamIn(reader, x3);
                    float y3; streamIn(reader, y3);
                    float x4; streamIn(reader, x4);
                    float y4; streamIn(reader, y4);

                    myWindow->drawCubicLine(x1,y1,x2,y2,x3,y3,x4,y4);
                }
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_ARC:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 x1; streamIn(reader, x1);
                qint32 y1; streamIn(reader, y1);
                qint32 x2; streamIn(reader, x2);
                qint32 y2; streamIn(reader, y2);
                qint32 sA; streamIn(reader, sA);
                qint32 eA; streamIn(reader, eA);

                return myWindow->drawArc(x1, y1, x2, y2, sA, eA);
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_ARC_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 x1; streamIn(reader, x1);
                    qint32 y1; streamIn(reader, y1);
                    qint32 x2; streamIn(reader, x2);
                    qint32 y2; streamIn(reader, y2);
                    qint32 sA; streamIn(reader, sA);
                    qint32 eA; streamIn(reader, eA);

                    myWindow->drawArc(x1, y1, x2, y2, sA, eA);
                }
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_ARC_F:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                float x1; streamIn(reader, x1);
                float y1; streamIn(reader, y1);
                float x2; streamIn(reader, x2);
                float y2; streamIn(reader, y2);
                float sA; streamIn(reader, sA);
                float eA; streamIn(reader, eA);

                return myWindow->drawArc(x1, y1, x2, y2, sA, eA);
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_ARC_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float x1; streamIn(reader, x1);
                    float y1; streamIn(reader, y1);
                    float x2; streamIn(reader, x2);
                    float y2; streamIn(reader, y2);
                    float sA; streamIn(reader, sA);
                    float eA; streamIn(reader, eA);

                    myWindow->drawArc(x1, y1, x2, y2, sA, eA);
                }
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_CHORD:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 x1; streamIn(reader, x1);
                qint32 y1; streamIn(reader, y1);
                qint32 x2; streamIn(reader, x2);
                qint32 y2; streamIn(reader, y2);
                qint32 sA; streamIn(reader, sA);
                qint32 eA; streamIn(reader, eA);

                return myWindow->drawChord(x1, y1, x2, y2, sA, eA);
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_CHORD_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 x1; streamIn(reader, x1);
                    qint32 y1; streamIn(reader, y1);
                    qint32 x2; streamIn(reader, x2);
                    qint32 y2; streamIn(reader, y2);
                    qint32 sA; streamIn(reader, sA);
                    qint32 eA; streamIn(reader, eA);

                    myWindow->drawChord(x1, y1, x2, y2, sA, eA);
                }
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_CHORD_F:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                float x1; streamIn(reader, x1);
                float y1; streamIn(reader, y1);
                float x2; streamIn(reader, x2);
                float y2; streamIn(reader, y2);
                float sA; streamIn(reader, sA);
                float eA; streamIn(reader, eA);

                return myWindow->drawChord(x1, y1, x2, y2, sA, eA);
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_CHORD_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float x1; streamIn(reader, x1);
                    float y1; streamIn(reader, y1);
                    float x2; streamIn(reader, x2);
                    float y2; streamIn(reader, y2);
                    float sA; streamIn(reader, sA);
                    float eA; streamIn(reader, eA);

                    myWindow->drawChord(x1, y1, x2, y2, sA, eA);
                }
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_TRIANGLE:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 x1; streamIn(reader, x1);
                qint32 y1; streamIn(reader, y1);
                qint32 x2; streamIn(reader, x2);
                qint32 y2; streamIn(reader, y2);
                qint32 x3; streamIn(reader, x3);
                qint32 y3; streamIn(reader, y3);

                return myWindow->drawTriangle(x1, y1, x2, y2, x3, y3);
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_TRIANGLE_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 x1; streamIn(reader, x1);
                    qint32 y1; streamIn(reader, y1);
                    qint32 x2; streamIn(reader, x2);
                    qint32 y2; streamIn(reader, y2);
                    qint32 x3; streamIn(reader, x3);
                    qint32 y3; streamIn(reader, y3);

                    myWindow->drawTriangle(x1, y1, x2, y2, x3, y3);
                }
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_TRIANGLE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                float x1; streamIn(reader, x1);
                float y1; streamIn(reader, y1);
                float x2; streamIn(reader, x2);
                float y2; streamIn(reader, y2);
                float x3; streamIn(reader, x3);
                float y3; streamIn(reader, y3);

                return myWindow->drawTriangle(x1, y1, x2, y2, x3, y3);
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_TRIANGLE_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float x1; streamIn(reader, x1);
                    float y1; streamIn(reader, y1);
                    float x2; streamIn(reader, x2);
                    float y2; streamIn(reader, y2);
                    float x3; streamIn(reader, x3);
                    float y3; streamIn(reader, y3);

                    myWindow->drawTriangle(x1, y1, x2, y2, x3, y3);
                }
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_QUADRILATERAL:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 x1; streamIn(reader, x1);
                qint32 y1; streamIn(reader, y1);
                qint32 x2; streamIn(reader, x2);
                qint32 y2; streamIn(reader, y2);
                qint32 x3; streamIn(reader, x3);
                qint32 y3; streamIn(reader, y3);
                qint32 x4; streamIn(reader, x4);
                qint32 y4; streamIn(reader, y4);

                return myWindow->drawQuadrilateral(x1,y1,x2,y2,x3,y3,x4,y4);
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_QUADRILATERAL_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 x1; streamIn(reader, x1);
                    qint32 y1; streamIn(reader, y1);
                    qint32 x2; streamIn(reader, x2);
                    qint32 y2; streamIn(reader, y2);
                    qint32 x3; streamIn(reader, x3);
                    qint32 y3; streamIn(reader, y3);
                    qint32 x4; streamIn(reader, x4);
                    qint32 y4; streamIn(reader, y4);

                    myWindow->drawQuadrilateral(x1,y1,x2,y2,x3,y3,x4,y4);
                }
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_QUADRILATERAL_F:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                float x1; streamIn(reader, x1);
                float y1; streamIn(reader, y1);
                float x2; streamIn(reader, x2);
                float y2; streamIn(reader, y2);
                float x3; streamIn(reader, x3);
                float y3; streamIn(reader, y3);
                float x4; streamIn(reader, x4);
                float y4; streamIn(reader, y4);

                return myWindow->drawQuadrilateral(x1,y1,x2,y2,x3,y3,x4,y4);
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_QUADRILATERAL_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float x1; streamIn(reader, x1);
                    float y1; streamIn(reader, y1);
                    float x2; streamIn(reader, x2);
                    float y2; streamIn(reader, y2);
                    float x3; streamIn(reader, x3);
                    float y3; streamIn(reader, y3);
                    float x4; streamIn(reader, x4);
                    float y4; streamIn(reader, y4);

                    myWindow->drawQuadrilateral(x1,y1,x2,y2,x3,y3,x4,y4);
                }
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_RECTANGLE:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 x1; streamIn(reader, x1);
                qint32 y1; streamIn(reader, y1);
                qint32 x2; streamIn(reader, x2);
                qint32 y2; streamIn(reader, y2);
                qint32 xR; streamIn(reader, xR);
                qint32 yR; streamIn(reader, yR);

                return myWindow->drawRectangle(x1, y1, x2, y2, xR, yR);
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_RECTANGLE_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 x1; streamIn(reader, x1);
                    qint32 y1; streamIn(reader, y1);
                    qint32 x2; streamIn(reader, x2);
                    qint32 y2; streamIn(reader, y2);
                    qint32 xR; streamIn(reader, xR);
                    qint32 yR; streamIn(reader, yR);

                    myWindow->drawRectangle(x1, y1, x2, y2, xR, yR);
                }
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_RECTANGLE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                float x1; streamIn(reader, x1);
                float y1; streamIn(reader, y1);
                float x2; streamIn(reader, x2);
                float y2; streamIn(reader, y2);
                float xR; streamIn(reader, xR);
                float yR; streamIn(reader, yR);

                return myWindow->drawRectangle(x1, y1, x2, y2, xR, yR);
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_RECTANGLE_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float x1; streamIn(reader, x1);
                    float y1; streamIn(reader, y1);
                    float x2; streamIn(reader, x2);
                    float y2; streamIn(reader, y2);
                    float xR; streamIn(reader, xR);
                    float yR; streamIn(reader, yR);

                    myWindow->drawRectangle(x1, y1, x2, y2, xR, yR);
                }
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_ELLIPSE:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 x1; streamIn(reader, x1);
                qint32 y1; streamIn(reader, y1);
                qint32 x2; streamIn(reader, x2);
                qint32 y2; streamIn(reader, y2);
                qint32 sA; streamIn(reader, sA);
                qint32 eA; streamIn(reader, eA);

                return myWindow->drawEllipse(x1, y1, x2, y2, sA, eA);
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_ELLIPSE_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    qint32 x1; streamIn(reader, x1);
                    qint32 y1; streamIn(reader, y1);
                    qint32 x2; streamIn(reader, x2);
                    qint32 y2; streamIn(reader, y2);
                    qint32 sA; streamIn(reader, sA);
                    qint32 eA; streamIn(reader, eA);

                    myWindow->drawEllipse(x1, y1, x2, y2, sA, eA);
                }
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_ELLIPSE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                float x1; streamIn(reader, x1);
                float y1; streamIn(reader, y1);
                float x2; streamIn(reader, x2);
                float y2; streamIn(reader, y2);
                float sA; streamIn(reader, sA);
                float eA; streamIn(reader, eA);

                return myWindow->drawEllipse(x1, y1, x2, y2, sA, eA);
            }

            return myWindow;
        }

        case GRAPHICS_DRAW_ELLIPSE_F_A:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 size; streamIn(reader, size);

                for(int i = 0; i <= size; i++)
                {
                    float x1; streamIn(reader, x1);
                    float y1; streamIn(reader, y1);
                    float x2; streamIn(reader, x2);
                    float y2; streamIn(reader, y2);
                    float sA; streamIn(reader, sA);
                    float eA; streamIn(reader, eA);

                    myWindow->drawEllipse(x1, y1, x2, y2, sA, eA);
                }
            }

            return myWindow;
        }

        case GRAPHICS_SET_BACKGROUND_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 rgba; streamIn(reader, rgba);

                myWindow->setBackgroundColor(rgba);
            }

            return myWindow;
        }

        case GRAPHICS_GET_BACKGROUND_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBackgroundColor());
            }

            return myWindow;
        }

        case GRAPHICS_SET_COORDINATE_SYSTEM:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 coordinateSystem; streamIn(reader, coordinateSystem);

                return myWindow->setCoordinateSystem(
                static_cast<CoordinateSystem>(coordinateSystem));
            }

            return myWindow;
        }

        case GRAPHICS_GET_COORDINATE_SYSTEM:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getCoordinateSystem());
            }

            return myWindow;
        }

        case GRAPHICS_SET_ANGLE_UNITS:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 angleUnits; streamIn(reader, angleUnits);

                return myWindow->setAngleUnits(
                static_cast<AngleUnits>(angleUnits));
            }

            return myWindow;
        }

        case GRAPHICS_GET_ANGLE_UNITS:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getAngleUnits());
            }

            return myWindow;
        }

        case GRAPHICS_SET_LINE_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 rgba; streamIn(reader, rgba);

                myWindow->setLineColor(rgba);
            }

            return myWindow;
        }

        case GRAPHICS_GET_LINE_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getLineColor());
            }

            return myWindow;
        }

        case GRAPHICS_SET_LINE_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 lineStyle; streamIn(reader, lineStyle);

                return myWindow->setLineStyle(
                static_cast<Qt::PenStyle>(lineStyle));
            }

            return myWindow;
        }

        case GRAPHICS_GET_LINE_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getLineStyle());
            }

            return myWindow;
        }

        case GRAPHICS_SET_FILL_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 rgba; streamIn(reader, rgba);

                myWindow->setFillColor(rgba);
            }

            return myWindow;
        }

        case GRAPHICS_GET_FILL_COLOR:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getFillColor());
            }

            return myWindow;
        }

        case GRAPHICS_SET_FILL_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 fillStyle; streamIn(reader, fillStyle);

                return myWindow->setFillStyle(
                static_cast<Qt::BrushStyle>(fillStyle));
            }

            return myWindow;
        }

        case GRAPHICS_GET_FILL_STYLE:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getFillStyle());
            }

            return myWindow;
        }

        case GRAPHICS_SET_ROTATION:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 rotation; streamIn(reader, rotation);

                return myWindow->setRotation(rotation);
            }

            return myWindow;
        }

        case GRAPHICS_GET_ROTATION:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getRotation());
            }

            return myWindow;
        }

        case GRAPHICS_SET_ROTATION_F:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                float rotation; streamIn(reader, rotation);

                return myWindow->setRotation(rotation);
            }

            return myWindow;
        }

        case GRAPHICS_GET_ROTATION_F:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getRotation());
            }

            return myWindow;
        }

        case GRAPHICS_SET_SCALE:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                qint32 scale; streamIn(reader, scale);

                return myWindow->setScale(scale);
            }

            return myWindow;
        }

        case GRAPHICS_GET_SCALE:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getScale());
            }

            return myWindow;
        }

        case GRAPHICS_SET_SCALE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                float scale; streamIn(reader, scale);

                return myWindow->setScale(scale);
            }

            return myWindow;
        }

        case GRAPHICS_GET_SCALE_F:
        {
            quint8 window; streamIn(reader, window);

            SerialGraphics *myWindow = windowCast
            <SerialGraphics>(__FUNCTION__, window);

            if(myWindow)
            {
                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getScale());
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Interface Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case INTERFACE_NEW_UNDOCKED_WINDOW:
        {
            quint8 window; streamIn(reader, window);
            QByteArray name; streamIn(reader, name);

            return newInterfaceWindow(window, name);
        }

        case INTERFACE_NEW_DOCKED_WINDOW:
        {
            quint8 window; streamIn(reader, window);
            quint8 targetWindow; streamIn(reader, targetWindow);

            SerialWindow *myTargetWindow = windowCast
            <SerialWindow>(__FUNCTION__, targetWindow);

            if(myTargetWindow)
            {
                QByteArray name; streamIn(reader, name);

                SerialWindow *myWindow =
                newInterfaceWindow(window, name, false);

                if(myWindow && (!myWindow->parent())
                && (!dockWindow(myTargetWindow->widget(), myWindow)))
                {
                    myWindow->setFloating(true);
                    myWindow->show();
                }

                return myWindow;
            }

            return myTargetWindow;
        }

        case INTERFACE_NEW_MAIN_DOCKED_WINDOW:
        {
            quint8 window; streamIn(reader, window);
            QByteArray name; streamIn(reader, name);

            SerialWindow *myWindow =
            newInterfaceWindow(window, name, false);

            if(myWindow && (!myWindow->parent())
            && (!dockWindow(m_widget, myWindow)))
            {
                myWindow->setFloating(true);
                myWindow->show();
            }

            return myWindow;
        }

        case INTERFACE_DELETE_INTERFACE_BOX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                return myWindow->deleteInterfaceBox(gBox, iBox);
            }

            return myWindow;
        }

        case INTERFACE_DELETE_ALL_INTERFACE_BOXES_0:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);

                return myWindow->deleteAllInterfaceBoxes(gBox);
            }

            return myWindow;
        }

        case INTERFACE_DELETE_ALL_INTERFACE_BOXES_1:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                myWindow->deleteAllInterfaceBoxes();
            }

            return myWindow;
        }

        case INTERFACE_DELETE_GROUP_BOX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);

                return myWindow->deleteGroupBox(gBox);
            }

            return myWindow;
        }

        case INTERFACE_DELETE_ALL_GROUP_BOXES:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                myWindow->deleteAllGroupBoxes();
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Interface Group Box Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case INTERFACE_NEW_GROUP_BOX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->newGroupBox(gBox, name);
            }

            return myWindow;
        }

        case INTERFACE_SET_GROUP_BOX_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->setGroupBoxName(gBox, name);
            }

            return myWindow;
        }

        case INTERFACE_GET_GROUP_BOX_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);

                *response = SR_STRING; *result =
                myWindow->getGroupBoxName(gBox, &ok);

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Interface Push Button Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case INTERFACE_NEW_PUSH_BUTTON:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->newPushButton(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_PUSH_BUTTON_SIGNAL:
        {
            return true;
        }

        case INTERFACE_SET_PUSH_BUTTON_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->setPushButtonName(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_GET_PUSH_BUTTON_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getPushButtonName(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Interface Radio Button Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case INTERFACE_NEW_RADIO_BUTTON:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->newRadioButton(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_RADIO_BUTTON_SIGNAL:
        {
            return true;
        }

        case INTERFACE_SET_RADIO_BUTTON_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->setRadioButtonName(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_GET_RADIO_BUTTON_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getRadioButtonName(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_RADIO_BUTTON_CHECKED:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 checked; streamIn(reader, checked);

                return myWindow->setRadioButtonChecked(gBox, iBox, checked);
            }

            return myWindow;
        }

        case INTERFACE_GET_RADIO_BUTTON_CHECKED:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getRadioButtonChecked(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Interface Check Box Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case INTERFACE_NEW_CHECK_BOX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->newCheckBox(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_CHECK_BOX_SIGNAL:
        {
            return true;
        }

        case INTERFACE_SET_CHECK_BOX_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->setCheckBoxName(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_GET_CHECK_BOX_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getCheckBoxName(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_CHECK_BOX_CHECKED:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 checked; streamIn(reader, checked);

                return myWindow->setCheckBoxChecked(gBox, iBox, checked);
            }

            return myWindow;
        }

        case INTERFACE_GET_CHECK_BOX_CHECKED:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getCheckBoxChecked(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Interface Line Edit Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case INTERFACE_NEW_LINE_EDIT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->newLineEdit(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_RESET_LINE_EDIT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                return myWindow->setLineEditText(gBox, iBox, QString());
            }

            return myWindow;
        }

        case INTERFACE_LINE_EDIT_SIGNAL:
        {
            return true;
        }

        case INTERFACE_SET_LINE_EDIT_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->setLineEditName(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_GET_LINE_EDIT_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getLineEditName(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_LINE_EDIT_ECHO_MODE:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 echoMode; streamIn(reader, echoMode);

                return myWindow->setLineEditEchoMode(gBox, iBox,
                static_cast<QLineEdit::EchoMode>(echoMode));
            }

            return myWindow;
        }

        case INTERFACE_GET_LINE_EDIT_ECHO_MODE:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getLineEditEchoMode(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_LINE_EDIT_INPUT_MASK:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray inputMask; streamIn(reader, inputMask);

                return myWindow->setLineEditInputMask(gBox, iBox, inputMask);
            }

            return myWindow;
        }

        case INTERFACE_GET_LINE_EDIT_INPUT_MASK:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getLineEditInputMask(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_LINE_EDIT_PLACEHOLDER_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray placeholderText; streamIn(reader, placeholderText);

                return myWindow->setLineEditPlaceholderText(gBox, iBox,
                placeholderText);
            }

            return myWindow;
        }

        case INTERFACE_GET_LINE_EDIT_PLACEHOLDER_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getLineEditPlaceholderText(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_LINE_EDIT_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray text; streamIn(reader, text);

                return myWindow->setLineEditText(gBox, iBox, text);
            }

            return myWindow;
        }

        case INTERFACE_GET_LINE_EDIT_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getLineEditText(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Interface Int Spin Box Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case INTERFACE_NEW_INT_SPIN_BOX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->newIntSpinBox(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_RESET_INT_SPIN_BOX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                return myWindow->resetIntSpinBox(gBox, iBox);
            }

            return myWindow;
        }

        case INTERFACE_INT_SPIN_BOX_SIGNAL:
        {
            return true;
        }

        case INTERFACE_SET_INT_SPIN_BOX_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->setIntSpinBoxName(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_GET_INT_SPIN_BOX_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getIntSpinBoxName(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_INT_SPIN_BOX_PREFIX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray prefix; streamIn(reader, prefix);

                return myWindow->setIntSpinBoxPrefix(gBox, iBox, prefix);
            }

            return myWindow;
        }

        case INTERFACE_GET_INT_SPIN_BOX_PREFIX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getIntSpinBoxPrefix(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_INT_SPIN_BOX_SUFFIX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray suffix; streamIn(reader, suffix);

                return myWindow->setIntSpinBoxSuffix(gBox, iBox, suffix);
            }

            return myWindow;
        }

        case INTERFACE_GET_INT_SPIN_BOX_SUFFIX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getIntSpinBoxSuffix(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_INT_SPIN_BOX_SINGLE_STEP:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 singleStep; streamIn(reader, singleStep);

                return myWindow->setIntSpinBoxSingleStep(gBox, iBox,
                                                         singleStep);
            }

            return myWindow;
        }

        case INTERFACE_GET_INT_SPIN_BOX_SINGLE_STEP:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getIntSpinBoxSingleStep(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_INT_SPIN_BOX_PAGE_STEP:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 pageStep; streamIn(reader, pageStep);

                return myWindow->setIntSpinBoxPageStep(gBox, iBox,
                                                       pageStep);
            }

            return myWindow;
        }

        case INTERFACE_GET_INT_SPIN_BOX_PAGE_STEP:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getIntSpinBoxPageStep(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_INT_SPIN_BOX_MINIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 minimum; streamIn(reader, minimum);

                return myWindow->setIntSpinBoxMinimum(gBox, iBox, minimum);
            }

            return myWindow;
        }

        case INTERFACE_GET_INT_SPIN_BOX_MINIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getIntSpinBoxMinimum(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_INT_SPIN_BOX_MAXIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 maximum; streamIn(reader, maximum);

                return myWindow->setIntSpinBoxMaximum(gBox, iBox, maximum);
            }

            return myWindow;
        }

        case INTERFACE_GET_INT_SPIN_BOX_MAXIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getIntSpinBoxMaximum(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_INT_SPIN_BOX_VALUE:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 value; streamIn(reader, value);

                return myWindow->setIntSpinBoxValue(gBox, iBox, value);
            }

            return myWindow;
        }

        case INTERFACE_GET_INT_SPIN_BOX_VALUE:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getIntSpinBoxValue(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_INT_SPIN_BOX_SPECIAL_VALUE_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray text; streamIn(reader, text);

                return myWindow->setIntSpinBoxSpecialValueText(gBox, iBox,
                                                               text);
            }

            return myWindow;
        }

        case INTERFACE_GET_INT_SPIN_BOX_SPECIAL_VALUE_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getIntSpinBoxSpecialValueText(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Interface Bin Spin Box Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case INTERFACE_NEW_BIN_SPIN_BOX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->newBinSpinBox(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_RESET_BIN_SPIN_BOX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                return myWindow->resetBinSpinBox(gBox, iBox);
            }

            return myWindow;
        }

        case INTERFACE_BIN_SPIN_BOX_SIGNAL:
        {
            return true;
        }

        case INTERFACE_SET_BIN_SPIN_BOX_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->setBinSpinBoxName(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_GET_BIN_SPIN_BOX_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getBinSpinBoxName(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_BIN_SPIN_BOX_PREFIX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray prefix; streamIn(reader, prefix);

                return myWindow->setBinSpinBoxPrefix(gBox, iBox, prefix);
            }

            return myWindow;
        }

        case INTERFACE_GET_BIN_SPIN_BOX_PREFIX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getBinSpinBoxPrefix(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_BIN_SPIN_BOX_SUFFIX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray suffix; streamIn(reader, suffix);

                return myWindow->setBinSpinBoxSuffix(gBox, iBox, suffix);
            }

            return myWindow;
        }

        case INTERFACE_GET_BIN_SPIN_BOX_SUFFIX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getBinSpinBoxSuffix(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_BIN_SPIN_BOX_SINGLE_STEP:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 singleStep; streamIn(reader, singleStep);

                return myWindow->setBinSpinBoxSingleStep(gBox, iBox,
                                                         singleStep);
            }

            return myWindow;
        }

        case INTERFACE_GET_BIN_SPIN_BOX_SINGLE_STEP:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBinSpinBoxSingleStep(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_BIN_SPIN_BOX_PAGE_STEP:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 pageStep; streamIn(reader, pageStep);

                return myWindow->setBinSpinBoxPageStep(gBox, iBox,
                                                       pageStep);
            }

            return myWindow;
        }

        case INTERFACE_GET_BIN_SPIN_BOX_PAGE_STEP:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBinSpinBoxPageStep(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_BIN_SPIN_BOX_MINIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 minimum; streamIn(reader, minimum);

                return myWindow->setBinSpinBoxMinimum(gBox, iBox, minimum);
            }

            return myWindow;
        }

        case INTERFACE_GET_BIN_SPIN_BOX_MINIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBinSpinBoxMinimum(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_BIN_SPIN_BOX_MAXIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 maximum; streamIn(reader, maximum);

                return myWindow->setBinSpinBoxMaximum(gBox, iBox, maximum);
            }

            return myWindow;
        }

        case INTERFACE_GET_BIN_SPIN_BOX_MAXIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBinSpinBoxMaximum(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_BIN_SPIN_BOX_VALUE:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 value; streamIn(reader, value);

                return myWindow->setBinSpinBoxValue(gBox, iBox, value);
            }

            return myWindow;
        }

        case INTERFACE_GET_BIN_SPIN_BOX_VALUE:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getBinSpinBoxValue(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_BIN_SPIN_BOX_SPECIAL_VALUE_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray text; streamIn(reader, text);

                return myWindow->setBinSpinBoxSpecialValueText(gBox, iBox,
                                                               text);
            }

            return myWindow;
        }

        case INTERFACE_GET_BIN_SPIN_BOX_SPECIAL_VALUE_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getBinSpinBoxSpecialValueText(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Interface Hex Spin Box Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case INTERFACE_NEW_HEX_SPIN_BOX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->newHexSpinBox(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_RESET_HEX_SPIN_BOX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                return myWindow->resetHexSpinBox(gBox, iBox);
            }

            return myWindow;
        }

        case INTERFACE_HEX_SPIN_BOX_SIGNAL:
        {
            return true;
        }

        case INTERFACE_SET_HEX_SPIN_BOX_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->setHexSpinBoxName(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_GET_HEX_SPIN_BOX_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getHexSpinBoxName(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_HEX_SPIN_BOX_PREFIX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray prefix; streamIn(reader, prefix);

                return myWindow->setHexSpinBoxPrefix(gBox, iBox, prefix);
            }

            return myWindow;
        }

        case INTERFACE_GET_HEX_SPIN_BOX_PREFIX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getHexSpinBoxPrefix(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_HEX_SPIN_BOX_SUFFIX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray suffix; streamIn(reader, suffix);

                return myWindow->setHexSpinBoxSuffix(gBox, iBox, suffix);
            }

            return myWindow;
        }

        case INTERFACE_GET_HEX_SPIN_BOX_SUFFIX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getHexSpinBoxSuffix(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_HEX_SPIN_BOX_SINGLE_STEP:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 singleStep; streamIn(reader, singleStep);

                return myWindow->setHexSpinBoxSingleStep(gBox, iBox,
                                                         singleStep);
            }

            return myWindow;
        }

        case INTERFACE_GET_HEX_SPIN_BOX_SINGLE_STEP:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getHexSpinBoxSingleStep(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_HEX_SPIN_BOX_PAGE_STEP:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 pageStep; streamIn(reader, pageStep);

                return myWindow->setHexSpinBoxPageStep(gBox, iBox,
                                                       pageStep);
            }

            return myWindow;
        }

        case INTERFACE_GET_HEX_SPIN_BOX_PAGE_STEP:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getHexSpinBoxPageStep(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_HEX_SPIN_BOX_MINIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 minimum; streamIn(reader, minimum);

                return myWindow->setHexSpinBoxMinimum(gBox, iBox, minimum);
            }

            return myWindow;
        }

        case INTERFACE_GET_HEX_SPIN_BOX_MINIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getHexSpinBoxMinimum(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_HEX_SPIN_BOX_MAXIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 maximum; streamIn(reader, maximum);

                return myWindow->setHexSpinBoxMaximum(gBox, iBox, maximum);
            }

            return myWindow;
        }

        case INTERFACE_GET_HEX_SPIN_BOX_MAXIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getHexSpinBoxMaximum(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_HEX_SPIN_BOX_VALUE:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 value; streamIn(reader, value);

                return myWindow->setHexSpinBoxValue(gBox, iBox, value);
            }

            return myWindow;
        }

        case INTERFACE_GET_HEX_SPIN_BOX_VALUE:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getHexSpinBoxValue(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_HEX_SPIN_BOX_SPECIAL_VALUE_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray text; streamIn(reader, text);

                return myWindow->setHexSpinBoxSpecialValueText(gBox, iBox,
                                                               text);
            }

            return myWindow;
        }

        case INTERFACE_GET_HEX_SPIN_BOX_SPECIAL_VALUE_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getHexSpinBoxSpecialValueText(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Interface Double Spin Box Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case INTERFACE_NEW_DOUBLE_SPIN_BOX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->newDoubleSpinBox(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_RESET_DOUBLE_SPIN_BOX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                return myWindow->resetDoubleSpinBox(gBox, iBox);
            }

            return myWindow;
        }

        case INTERFACE_DOUBLE_SPIN_BOX_SIGNAL:
        {
            return true;
        }

        case INTERFACE_SET_DOUBLE_SPIN_BOX_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->setDoubleSpinBoxName(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_GET_DOUBLE_SPIN_BOX_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getDoubleSpinBoxName(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_DOUBLE_SPIN_BOX_PREFIX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray prefix; streamIn(reader, prefix);

                return myWindow->setDoubleSpinBoxPrefix(gBox, iBox, prefix);
            }

            return myWindow;
        }

        case INTERFACE_GET_DOUBLE_SPIN_BOX_PREFIX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getDoubleSpinBoxPrefix(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_DOUBLE_SPIN_BOX_SUFFIX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray suffix; streamIn(reader, suffix);

                return myWindow->setDoubleSpinBoxSuffix(gBox, iBox, suffix);
            }

            return myWindow;
        }

        case INTERFACE_GET_DOUBLE_SPIN_BOX_SUFFIX:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getDoubleSpinBoxSuffix(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_DOUBLE_SPIN_BOX_SINGLE_STEP:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                float singleStep; streamIn(reader, singleStep);

                return myWindow->setDoubleSpinBoxSingleStep(gBox, iBox,
                                                            singleStep);
            }

            return myWindow;
        }

        case INTERFACE_GET_DOUBLE_SPIN_BOX_SINGLE_STEP:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getDoubleSpinBoxSingleStep(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_DOUBLE_SPIN_BOX_PAGE_STEP:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                float pageStep; streamIn(reader, pageStep);

                return myWindow->setDoubleSpinBoxPageStep(gBox, iBox,
                                                          pageStep);
            }

            return myWindow;
        }

        case INTERFACE_GET_DOUBLE_SPIN_BOX_PAGE_STEP:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getDoubleSpinBoxPageStep(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_DOUBLE_SPIN_BOX_MINIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                float minimum; streamIn(reader, minimum);

                return myWindow->setDoubleSpinBoxMinimum(gBox, iBox, minimum);
            }

            return myWindow;
        }

        case INTERFACE_GET_DOUBLE_SPIN_BOX_MINIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getDoubleSpinBoxMinimum(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_DOUBLE_SPIN_BOX_MAXIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                float maximum; streamIn(reader, maximum);

                return myWindow->setDoubleSpinBoxMaximum(gBox, iBox, maximum);
            }

            return myWindow;
        }

        case INTERFACE_GET_DOUBLE_SPIN_BOX_MAXIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getDoubleSpinBoxMaximum(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_DOUBLE_SPIN_BOX_VALUE:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                float value; streamIn(reader, value);

                return myWindow->setDoubleSpinBoxValue(gBox, iBox, value);
            }

            return myWindow;
        }

        case INTERFACE_GET_DOUBLE_SPIN_BOX_VALUE:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_FLOAT; *result = static_cast<float>(
                myWindow->getDoubleSpinBoxValue(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_DOUBLE_SPIN_BOX_SPECIAL_VALUE_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray text; streamIn(reader, text);

                return myWindow->setDoubleSpinBoxSpecialValueText(gBox, iBox,
                                                                  text);
            }

            return myWindow;
        }

        case INTERFACE_GET_DOUBLE_SPIN_BOX_SPECIAL_VALUE_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getDoubleSpinBoxSpecialValueText(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Interface Time Edit Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case INTERFACE_NEW_TIME_EDIT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->newTimeEdit(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_RESET_TIME_EDIT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                return myWindow->resetTimeEdit(gBox, iBox);
            }

            return myWindow;
        }

        case INTERFACE_TIME_EDIT_SIGNAL:
        {
            return true;
        }

        case INTERFACE_SET_TIME_EDIT_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->setTimeEditName(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_GET_TIME_EDIT_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getTimeEditName(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_TIME_EDIT_MINIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray minimum; streamIn(reader, minimum);

                return myWindow->setTimeEditMinimum(gBox, iBox, minimum);
            }

            return myWindow;
        }

        case INTERFACE_GET_TIME_EDIT_MINIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getTimeEditMinimum(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_TIME_EDIT_MAXIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray maximum; streamIn(reader, maximum);

                return myWindow->setTimeEditMaximum(gBox, iBox, maximum);
            }

            return myWindow;
        }

        case INTERFACE_GET_TIME_EDIT_MAXIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getTimeEditMaximum(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_TIME_EDIT_VALUE:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray value; streamIn(reader, value);

                return myWindow->setTimeEditValue(gBox, iBox, value);
            }

            return myWindow;
        }

        case INTERFACE_GET_TIME_EDIT_VALUE:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getTimeEditValue(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_TIME_EDIT_SPECIAL_VALUE_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray text; streamIn(reader, text);

                return myWindow->setTimeEditSpecialValueText(gBox, iBox, text);
            }

            return myWindow;
        }

        case INTERFACE_GET_TIME_EDIT_SPECIAL_VALUE_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getTimeEditSpecialValueText(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Interface Date Edit Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case INTERFACE_NEW_DATE_EDIT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->newDateEdit(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_RESET_DATE_EDIT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                return myWindow->resetDateEdit(gBox, iBox);
            }

            return myWindow;
        }

        case INTERFACE_DATE_EDIT_SIGNAL:
        {
            return true;
        }

        case INTERFACE_SET_DATE_EDIT_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->setDateEditName(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_GET_DATE_EDIT_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getDateEditName(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_DATE_EDIT_MINIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray minimum; streamIn(reader, minimum);

                return myWindow->setDateEditMinimum(gBox, iBox, minimum);
            }

            return myWindow;
        }

        case INTERFACE_GET_DATE_EDIT_MINIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getDateEditMinimum(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_DATE_EDIT_MAXIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray maximum; streamIn(reader, maximum);

                return myWindow->setDateEditMaximum(gBox, iBox, maximum);
            }

            return myWindow;
        }

        case INTERFACE_GET_DATE_EDIT_MAXIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getDateEditMaximum(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_DATE_EDIT_VALUE:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray value; streamIn(reader, value);

                return myWindow->setDateEditValue(gBox, iBox, value);
            }

            return myWindow;
        }

        case INTERFACE_GET_DATE_EDIT_VALUE:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getDateEditValue(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_DATE_EDIT_SPECIAL_VALUE_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray text; streamIn(reader, text);

                return myWindow->setDateEditSpecialValueText(gBox, iBox, text);
            }

            return myWindow;
        }

        case INTERFACE_GET_DATE_EDIT_SPECIAL_VALUE_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getDateEditSpecialValueText(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Interface Date Time Edit Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case INTERFACE_NEW_DATE_TIME_EDIT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->newDateTimeEdit(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_RESET_DATE_TIME_EDIT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                return myWindow->resetDateTimeEdit(gBox, iBox);
            }

            return myWindow;
        }

        case INTERFACE_DATE_TIME_EDIT_SIGNAL:
        {
            return true;
        }

        case INTERFACE_SET_DATE_TIME_EDIT_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->setDateTimeEditName(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_GET_DATE_TIME_EDIT_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getDateTimeEditName(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_DATE_TIME_EDIT_MINIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray minimum; streamIn(reader, minimum);

                return myWindow->setDateTimeEditMinimum(gBox, iBox, minimum);
            }

            return myWindow;
        }

        case INTERFACE_GET_DATE_TIME_EDIT_MINIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getDateTimeEditMinimum(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_DATE_TIME_EDIT_MAXIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray maximum; streamIn(reader, maximum);

                return myWindow->setDateTimeEditMaximum(gBox, iBox, maximum);
            }

            return myWindow;
        }

        case INTERFACE_GET_DATE_TIME_EDIT_MAXIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getDateTimeEditMaximum(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_DATE_TIME_EDIT_VALUE:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray value; streamIn(reader, value);

                return myWindow->setDateTimeEditValue(gBox, iBox, value);
            }

            return myWindow;
        }

        case INTERFACE_GET_DATE_TIME_EDIT_VALUE:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getDateTimeEditValue(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_DATE_TIME_EDIT_SPECIAL_VALUE_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gB; streamIn(reader, gB);
                quint8 iB; streamIn(reader, iB);
                QByteArray text; streamIn(reader, text);

                return myWindow->setDateTimeEditSpecialValueText(gB, iB, text);
            }

            return myWindow;
        }

        case INTERFACE_GET_DATE_TIME_EDIT_SPECIAL_VALUE_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gB; streamIn(reader, gB);
                quint8 iB; streamIn(reader, iB);

                *response = SR_STRING; *result =
                myWindow->getDateTimeEditSpecialValueText(gB, iB, &ok);

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Interface Slider Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case INTERFACE_NEW_SLIDER:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->newSlider(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_RESET_SLIDER:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                return myWindow->resetSlider(gBox, iBox);
            }

            return myWindow;
        }

        case INTERFACE_SLIDER_SIGNAL:
        {
            return true;
        }

        case INTERFACE_SET_SLIDER_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->setSliderName(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_GET_SLIDER_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getSliderName(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_SLIDER_SINGLE_STEP:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 singleStep; streamIn(reader, singleStep);

                return myWindow->setSliderSingleStep(gBox, iBox, singleStep);
            }

            return myWindow;
        }

        case INTERFACE_GET_SLIDER_SINGLE_STEP:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getSliderSingleStep(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_SLIDER_PAGE_STEP:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 pageStep; streamIn(reader, pageStep);

                return myWindow->setSliderPageStep(gBox, iBox, pageStep);
            }

            return myWindow;
        }

        case INTERFACE_GET_SLIDER_PAGE_STEP:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getSliderPageStep(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_SLIDER_MINIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 minimum; streamIn(reader, minimum);

                return myWindow->setSliderMinimum(gBox, iBox, minimum);
            }

            return myWindow;
        }

        case INTERFACE_GET_SLIDER_MINIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getSliderMinimum(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_SLIDER_MAXIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 maximum; streamIn(reader, maximum);

                return myWindow->setSliderMaximum(gBox, iBox, maximum);
            }

            return myWindow;
        }

        case INTERFACE_GET_SLIDER_MAXIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getSliderMaximum(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_SLIDER_VALUE:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 value; streamIn(reader, value);

                return myWindow->setSliderValue(gBox, iBox, value);
            }

            return myWindow;
        }

        case INTERFACE_GET_SLIDER_VALUE:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getSliderValue(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Interface Label Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case INTERFACE_NEW_LABEL:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray text; streamIn(reader, text);

                return myWindow->newLabel(gBox, iBox, text);
            }

            return myWindow;
        }

        case INTERFACE_SET_LABEL_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray text; streamIn(reader, text);

                return myWindow->setLabelText(gBox, iBox, text);
            }

            return myWindow;
        }

        case INTERFACE_GET_LABEL_TEXT:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getLabelText(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Interface Progress Bar Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case INTERFACE_NEW_PROGRESS_BAR:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->newProgressBar(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_RESET_PROGRESS_BAR:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                return myWindow->resetProgressBar(gBox, iBox);
            }

            return myWindow;
        }

        case INTERFACE_SET_PROGRESS_BAR_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                QByteArray name; streamIn(reader, name);

                return myWindow->setProgressBarName(gBox, iBox, name);
            }

            return myWindow;
        }

        case INTERFACE_GET_PROGRESS_BAR_NAME:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_STRING; *result =
                myWindow->getProgressBarName(gBox, iBox, &ok);

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_PROGRESS_BAR_MINIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 minimum; streamIn(reader, minimum);

                return myWindow->setProgressBarMinimum(gBox, iBox, minimum);
            }

            return myWindow;
        }

        case INTERFACE_GET_PROGRESS_BAR_MINIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getProgressBarMinimum(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_PROGRESS_BAR_MAXIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 maximum; streamIn(reader, maximum);

                return myWindow->setProgressBarMaximum(gBox, iBox, maximum);
            }

            return myWindow;
        }

        case INTERFACE_GET_PROGRESS_BAR_MAXIMUM:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getProgressBarMaximum(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

        case INTERFACE_SET_PROGRESS_BAR_VALUE:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);
                qint32 value; streamIn(reader, value);

                return myWindow->setProgressBarValue(gBox, iBox, value);
            }

            return myWindow;
        }

        case INTERFACE_GET_PROGRESS_BAR_VALUE:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                bool ok;

                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                *response = SR_INT; *result = static_cast<qint32>(
                myWindow->getProgressBarValue(gBox, iBox, &ok));

                return ok;
            }

            return myWindow;
        }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////    Serial Interface Seperator Base
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

        case INTERFACE_NEW_SEPERATOR:
        {
            quint8 window; streamIn(reader, window);

            SerialInterface *myWindow = windowCast
            <SerialInterface>(__FUNCTION__, window);

            if(myWindow)
            {
                quint8 gBox; streamIn(reader, gBox);
                quint8 iBox; streamIn(reader, iBox);

                return myWindow->newSeperator(gBox, iBox);
            }

            return myWindow;
        }

        default:
        {
            emit errorMessage(QString(metaObject()->className()) +
            "::" + __FUNCTION__ + " -> " +
            tr("The function (%L1) is out of the valid range").arg(function));

            return false;
        }
    }
}

SerialWindow *SerialEscape::newTerminalWindow(int windowHandle,
                                              const QString &name,
                                              bool autoShow)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    SerialTerminal *myWindow = NULL;

    if(!windowDoesNotExist(__FUNCTION__, windowHandle))
    {
        // Silently handle error...

        SerialWindow *tempWindow = m_windows.value(windowHandle);

        if(qobject_cast<SerialTerminal *>(tempWindow))
        {
            tempWindow->setWindowTitle(name); return tempWindow;
        }
        else // Replace object instead of error...
        {
            tempWindow->setAttribute(Qt::WA_DeleteOnClose, true);
            tempWindow->close(); delete tempWindow; // delete faster
        }
    }

    myWindow = new SerialTerminal(name, m_settings);
    myWindow->setAttribute(Qt::WA_DeleteOnClose, false);
    myWindow->setAttribute(Qt::WA_QuitOnClose, false);
    myWindow->setWindowHandle(windowHandle);

    connect(myWindow, SIGNAL(transmit(QByteArray,QWidget*)),
            this, SLOT(sendData(QByteArray,QWidget*)));

    connect(myWindow, SIGNAL(errorMessage(QString)),
            this, SIGNAL(errorMessage(QString)));

    connect(myWindow, SIGNAL(destroyed(QObject*)),
            this, SLOT(windowDestroyed(QObject*)));

    m_windows.insert(windowHandle, myWindow);

    if(autoShow)
    {
        myWindow->show();
    }

    return myWindow;
}

SerialWindow *SerialEscape::newOscilloscopeWindow(int windowHandle,
                                                  const QString &name,
                                                  bool autoShow)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    SerialOscilloscope *myWindow = NULL;

    if(!windowDoesNotExist(__FUNCTION__, windowHandle))
    {
        // Silently handle error...

        SerialWindow *tempWindow = m_windows.value(windowHandle);

        if(qobject_cast<SerialOscilloscope *>(tempWindow))
        {
            tempWindow->setWindowTitle(name); return tempWindow;
        }
        else // Replace object instead of error...
        {
            tempWindow->setAttribute(Qt::WA_DeleteOnClose, true);
            tempWindow->close(); delete tempWindow; // delete faster
        }
    }

    myWindow = new SerialOscilloscope(name, m_settings);
    myWindow->setAttribute(Qt::WA_DeleteOnClose, false);
    myWindow->setAttribute(Qt::WA_QuitOnClose, false);
    myWindow->setWindowHandle(windowHandle);

    connect(myWindow, SIGNAL(errorMessage(QString)),
            this, SIGNAL(errorMessage(QString)));

    connect(myWindow, SIGNAL(destroyed(QObject*)),
            this, SLOT(windowDestroyed(QObject*)));

    m_windows.insert(windowHandle, myWindow);

    if(autoShow)
    {
        myWindow->show();
    }

    return myWindow;
}

SerialWindow *SerialEscape::newTableWindow(int windowHandle,
                                           const QString &name,
                                           bool autoShow)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    SerialTable *myWindow = NULL;

    if(!windowDoesNotExist(__FUNCTION__, windowHandle))
    {
        // Silently handle error...

        SerialWindow *tempWindow = m_windows.value(windowHandle);

        if(qobject_cast<SerialTable *>(tempWindow))
        {
            tempWindow->setWindowTitle(name); return tempWindow;
        }
        else // Replace object instead of error...
        {
            tempWindow->setAttribute(Qt::WA_DeleteOnClose, true);
            tempWindow->close(); delete tempWindow; // delete faster
        }
    }

    myWindow = new SerialTable(name, m_settings);
    myWindow->setAttribute(Qt::WA_DeleteOnClose, false);
    myWindow->setAttribute(Qt::WA_QuitOnClose, false);
    myWindow->setWindowHandle(windowHandle);

    connect(myWindow, SIGNAL(errorMessage(QString)),
            this, SIGNAL(errorMessage(QString)));

    connect(myWindow, SIGNAL(destroyed(QObject*)),
            this, SLOT(windowDestroyed(QObject*)));

    m_windows.insert(windowHandle, myWindow);

    if(autoShow)
    {
        myWindow->show();
    }

    return myWindow;
}

SerialWindow *SerialEscape::newTreeWindow(int windowHandle,
                                          const QString &name,
                                          bool autoShow)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    SerialTree *myWindow = NULL;

    if(!windowDoesNotExist(__FUNCTION__, windowHandle))
    {
        // Silently handle error...

        SerialWindow *tempWindow = m_windows.value(windowHandle);

        if(qobject_cast<SerialTree *>(tempWindow))
        {
            tempWindow->setWindowTitle(name); return tempWindow;
        }
        else // Replace object instead of error...
        {
            tempWindow->setAttribute(Qt::WA_DeleteOnClose, true);
            tempWindow->close(); delete tempWindow; // delete faster
        }
    }

    myWindow = new SerialTree(name, m_settings);
    myWindow->setAttribute(Qt::WA_DeleteOnClose, false);
    myWindow->setAttribute(Qt::WA_QuitOnClose, false);
    myWindow->setWindowHandle(windowHandle);

    connect(myWindow, SIGNAL(errorMessage(QString)),
            this, SIGNAL(errorMessage(QString)));

    connect(myWindow, SIGNAL(destroyed(QObject*)),
            this, SLOT(windowDestroyed(QObject*)));

    m_windows.insert(windowHandle, myWindow);

    if(autoShow)
    {
        myWindow->show();
    }

    return myWindow;
}

SerialWindow *SerialEscape::newGraphicsWindow(int windowHandle,
                                              const QString &name,
                                              bool autoShow)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    SerialGraphics *myWindow = NULL;

    if(!windowDoesNotExist(__FUNCTION__, windowHandle))
    {
        // Silently handle error...

        SerialWindow *tempWindow = m_windows.value(windowHandle);

        if(qobject_cast<SerialGraphics *>(tempWindow))
        {
            tempWindow->setWindowTitle(name); return tempWindow;
        }
        else // Replace object instead of error...
        {
            tempWindow->setAttribute(Qt::WA_DeleteOnClose, true);
            tempWindow->close(); delete tempWindow; // delete faster
        }
    }

    myWindow = new SerialGraphics(name, m_settings);
    myWindow->setAttribute(Qt::WA_DeleteOnClose, false);
    myWindow->setAttribute(Qt::WA_QuitOnClose, false);
    myWindow->setWindowHandle(windowHandle);

    connect(myWindow, SIGNAL(errorMessage(QString)),
            this, SIGNAL(errorMessage(QString)));

    connect(myWindow, SIGNAL(destroyed(QObject*)),
            this, SLOT(windowDestroyed(QObject*)));

    m_windows.insert(windowHandle, myWindow);

    if(autoShow)
    {
        myWindow->show();
    }

    return myWindow;
}

SerialWindow *SerialEscape::newInterfaceWindow(int windowHandle,
                                               const QString &name,
                                               bool autoShow)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    SerialInterface *myWindow = NULL;

    if(!windowDoesNotExist(__FUNCTION__, windowHandle))
    {
        // Silently handle error...

        SerialWindow *tempWindow = m_windows.value(windowHandle);

        if(qobject_cast<SerialInterface *>(tempWindow))
        {
            tempWindow->setWindowTitle(name); return tempWindow;
        }
        else // Replace object instead of error...
        {
            tempWindow->setAttribute(Qt::WA_DeleteOnClose, true);
            tempWindow->close(); delete tempWindow; // delete faster
        }
    }

    myWindow = new SerialInterface(name, m_settings);
    myWindow->setAttribute(Qt::WA_DeleteOnClose, false);
    myWindow->setAttribute(Qt::WA_QuitOnClose, false);
    myWindow->setWindowHandle(windowHandle);

    connect(myWindow,
            SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)),
            this,
            SLOT(sendVariantPacket(SerialFunction,int,int,int,QVariant)));

    connect(myWindow, SIGNAL(errorMessage(QString)),
            this, SIGNAL(errorMessage(QString)));

    connect(myWindow, SIGNAL(destroyed(QObject*)),
            this, SLOT(windowDestroyed(QObject*)));

    m_windows.insert(windowHandle, myWindow);

    if(autoShow)
    {
        myWindow->show();
    }

    return myWindow;
}

bool SerialEscape::dockWindow(QWidget *widget, SerialWindow *window)
{
    bool ok = true;

    QMainWindow *main = qobject_cast<QMainWindow *>(widget);

    if(main)
    {
        if(!(ok = main->restoreDockWidget(window)))
        {
            main->addDockWidget(Qt::RightDockWidgetArea, window);
        }
        // Begin - Always Show Window Override ////////////////////////////////
        else
        {
            window->show();
        }
        // End - Always Show Window Override //////////////////////////////////

        // Begin Action/Shortcut Workaround ///////////////////////////////////

        foreach(QAction *action, window->findChildren<QAction *>())
        {
            action->setShortcut(QKeySequence());
        }

        foreach(QShortcut *shortcut, window->findChildren<QShortcut *>())
        {
            shortcut->setKey(QKeySequence());
        }

        // End Action/Shortcut Workaround /////////////////////////////////////
    }
    else
    {
        window->show();
    }

    return ok;
}

void SerialEscape::sendStringPacket(SerialFunction function,
                                    const QByteArray &value)
{
    QByteArray packet; QDataStream writer(&packet, QIODevice::WriteOnly);
    writer.setFloatingPointPrecision(QDataStream::SinglePrecision);
    writer.setByteOrder(QDataStream::LittleEndian);

    writer.writeRawData(value.constData(), value.size() + sizeof('\0'));

    sendLayer1(function, packet);
}

void SerialEscape::sendStringPacket(SerialFunction function,
                                    int handle0,
                                    const QByteArray &value)
{
    QByteArray packet; QDataStream writer(&packet, QIODevice::WriteOnly);
    writer.setFloatingPointPrecision(QDataStream::SinglePrecision);
    writer.setByteOrder(QDataStream::LittleEndian);

    writer << static_cast<quint8>(handle0);
    writer.writeRawData(value.constData(), value.size() + sizeof('\0'));

    sendLayer1(function, packet);
}

void SerialEscape::sendStringPacket(SerialFunction function,
                                    int handle0,
                                    int handle1,
                                    const QByteArray &value)
{
    QByteArray packet; QDataStream writer(&packet, QIODevice::WriteOnly);
    writer.setFloatingPointPrecision(QDataStream::SinglePrecision);
    writer.setByteOrder(QDataStream::LittleEndian);

    writer << static_cast<quint8>(handle0);
    writer << static_cast<quint8>(handle1);
    writer.writeRawData(value.constData(), value.size() + sizeof('\0'));

    sendLayer1(function, packet);
}

void SerialEscape::sendStringPacket(SerialFunction function,
                                    int handle0,
                                    int handle1,
                                    int handle2,
                                    const QByteArray &value)
{
    QByteArray packet; QDataStream writer(&packet, QIODevice::WriteOnly);
    writer.setFloatingPointPrecision(QDataStream::SinglePrecision);
    writer.setByteOrder(QDataStream::LittleEndian);

    writer << static_cast<quint8>(handle0);
    writer << static_cast<quint8>(handle1);
    writer << static_cast<quint8>(handle2);
    writer.writeRawData(value.constData(), value.size() + sizeof('\0'));

    sendLayer1(function, packet);
}

void SerialEscape::sendFloatPacket(SerialFunction function,
                                   float value)
{
    QByteArray packet; QDataStream writer(&packet, QIODevice::WriteOnly);
    writer.setFloatingPointPrecision(QDataStream::SinglePrecision);
    writer.setByteOrder(QDataStream::LittleEndian);

    writer << static_cast<float>(value);

    sendLayer1(function, packet);
}

void SerialEscape::sendFloatPacket(SerialFunction function,
                                   int handle0,
                                   float value)
{
    QByteArray packet; QDataStream writer(&packet, QIODevice::WriteOnly);
    writer.setFloatingPointPrecision(QDataStream::SinglePrecision);
    writer.setByteOrder(QDataStream::LittleEndian);

    writer << static_cast<quint8>(handle0);
    writer << static_cast<float>(value);

    sendLayer1(function, packet);
}

void SerialEscape::sendFloatPacket(SerialFunction function,
                                   int handle0,
                                   int handle1,
                                   float value)
{
    QByteArray packet; QDataStream writer(&packet, QIODevice::WriteOnly);
    writer.setFloatingPointPrecision(QDataStream::SinglePrecision);
    writer.setByteOrder(QDataStream::LittleEndian);

    writer << static_cast<quint8>(handle0);
    writer << static_cast<quint8>(handle1);
    writer << static_cast<float>(value);

    sendLayer1(function, packet);
}

void SerialEscape::sendFloatPacket(SerialFunction function,
                                   int handle0,
                                   int handle1,
                                   int handle2,
                                   float value)
{
    QByteArray packet; QDataStream writer(&packet, QIODevice::WriteOnly);
    writer.setFloatingPointPrecision(QDataStream::SinglePrecision);
    writer.setByteOrder(QDataStream::LittleEndian);

    writer << static_cast<quint8>(handle0);
    writer << static_cast<quint8>(handle1);
    writer << static_cast<quint8>(handle2);
    writer << static_cast<float>(value);

    sendLayer1(function, packet);
}

void SerialEscape::sendIntPacket(SerialFunction function,
                                 int value)
{
    QByteArray packet; QDataStream writer(&packet, QIODevice::WriteOnly);
    writer.setFloatingPointPrecision(QDataStream::SinglePrecision);
    writer.setByteOrder(QDataStream::LittleEndian);

    writer << static_cast<qint32>(value);

    sendLayer1(function, packet);
}

void SerialEscape::sendIntPacket(SerialFunction function,
                                 int handle0,
                                 int value)
{
    QByteArray packet; QDataStream writer(&packet, QIODevice::WriteOnly);
    writer.setFloatingPointPrecision(QDataStream::SinglePrecision);
    writer.setByteOrder(QDataStream::LittleEndian);

    writer << static_cast<quint8>(handle0);
    writer << static_cast<qint32>(value);

    sendLayer1(function, packet);
}

void SerialEscape::sendIntPacket(SerialFunction function,
                                 int handle0,
                                 int handle1,
                                 int value)
{
    QByteArray packet; QDataStream writer(&packet, QIODevice::WriteOnly);
    writer.setFloatingPointPrecision(QDataStream::SinglePrecision);
    writer.setByteOrder(QDataStream::LittleEndian);

    writer << static_cast<quint8>(handle0);
    writer << static_cast<quint8>(handle1);
    writer << static_cast<qint32>(value);

    sendLayer1(function, packet);
}

void SerialEscape::sendIntPacket(SerialFunction function,
                                 int handle0,
                                 int handle1,
                                 int handle2,
                                 int value)
{
    QByteArray packet; QDataStream writer(&packet, QIODevice::WriteOnly);
    writer.setFloatingPointPrecision(QDataStream::SinglePrecision);
    writer.setByteOrder(QDataStream::LittleEndian);

    writer << static_cast<quint8>(handle0);
    writer << static_cast<quint8>(handle1);
    writer << static_cast<quint8>(handle2);
    writer << static_cast<qint32>(value);

    sendLayer1(function, packet);
}

void SerialEscape::sendLayer1(SerialFunction function, const QByteArray &bytes)
{
    int size = qMin(bytes.size(), MAX_LAYER_1_DATA_SIZE);

    QByteArray packet; QDataStream writer(&packet, QIODevice::WriteOnly);
    writer.setFloatingPointPrecision(QDataStream::SinglePrecision);
    writer.setByteOrder(QDataStream::LittleEndian);

    writer << static_cast<quint16>(function);
    writer << static_cast<quint16>(size);

    writer.writeRawData(bytes.constData(), size);

    sendLayer0(packet);
}

void SerialEscape::sendLayer0(const QByteArray &bytes)
{
    for(int i = 0, j = bytes.size(); i < j;)
    {
        int size = qMin(j - i, m_maxLayer0DataSize);

        QByteArray packet; QDataStream writer(&packet, QIODevice::WriteOnly);
        writer.setFloatingPointPrecision(QDataStream::SinglePrecision);
        writer.setByteOrder(QDataStream::LittleEndian);

        writer << static_cast<quint8>(CC_ESCAPE);
        writer << static_cast<quint8>(CC_NULL);
        writer << static_cast<quint8>(size);
        writer << static_cast<quint8>(~size);

        writer.writeRawData(bytes.constData() + i, size);

        quint16 checksum = m_checksumMode
        ? qChecksum2(packet.constData(), packet.size())
        : qChecksum(packet.constData(), packet.size());

        writer << static_cast<quint16>(checksum);

        writeList(packet);

        i += size;
    }
}

template <class T>
T *SerialEscape::windowCast(const QString &FName, int window)
{
    SerialWindow *aWindow = windowExists(FName, window);

    if(aWindow)
    {
        T *myWindow = qobject_cast<T *>(aWindow);

        if(!myWindow)
        {
            QString instanceClassName = aWindow->metaObject()->className();
            QString templateClassName = T::staticMetaObject.className();

            emit errorMessage(QString(metaObject()->className()) +
            "::" + FName + " -> " +
            tr("Window %L1 type cast error (%L2 != %L3)").
            arg(window).
            arg(instanceClassName).
            arg(templateClassName));
        }

        return myWindow;
    }

    return NULL;
}

SerialWindow *SerialEscape::windowExists(const QString &FName, int window)
{
    SerialWindow *aWindow = m_windows.value(window);

    if(!aWindow)
    {
        emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + " -> " +
        tr("Window %L1 does not exist").
        arg(window));
    }

    return aWindow;
}

bool SerialEscape::windowDoesNotExist(const QString &FName, int window)
{
    Q_UNUSED(FName); // Silently handle error...

    bool ok = !m_windows.contains(window);

    if(!ok)
    {
        /* emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + " -> " +
        tr("Window %L1 already exists").
        arg(window)); */
    }

    return ok;
}

quint16 SerialEscape::qChecksum2(const char *data, uint len)
{
    quint16 checksum = 0;

    for(uint i = 0; i < len; i++)
    {
        checksum += static_cast<quint16>(data[i]);
    }

    return checksum;
}

inline QDataStream &SerialEscape::streamIn(QDataStream &stream, bool &in)
{
    return (stream >> in);
}

inline QDataStream &SerialEscape::streamIn(QDataStream &stream, qint8 &in)
{
    return (stream >> in);
}

inline QDataStream &SerialEscape::streamIn(QDataStream &stream, quint8 &in)
{
    return (stream >> in);
}

inline QDataStream &SerialEscape::streamIn(QDataStream &stream, qint16 &in)
{
    return (stream >> in);
}

inline QDataStream &SerialEscape::streamIn(QDataStream &stream, quint16 &in)
{
    return (stream >> in);
}

inline QDataStream &SerialEscape::streamIn(QDataStream &stream, qint32 &in)
{
    return (stream >> in);
}

inline QDataStream &SerialEscape::streamIn(QDataStream &stream, quint32 &in)
{
    return (stream >> in);
}

inline QDataStream &SerialEscape::streamIn(QDataStream &stream, qint64 &in)
{
    return (stream >> in);
}

inline QDataStream &SerialEscape::streamIn(QDataStream &stream, quint64 &in)
{
    return (stream >> in);
}

inline QDataStream &SerialEscape::streamIn(QDataStream &stream, float &in)
{
    return (stream >> in);
}

inline QDataStream &SerialEscape::streamIn(QDataStream &stream, double &in)
{
    return (stream >> in);
}

inline QDataStream &streamIn(QDataStream &stream, char *&in)
{
    return (stream >> in);
}

QDataStream &SerialEscape::streamIn(QDataStream &stream, QByteArray &in)
{
    QByteArray temp;

    while(!stream.atEnd())
    {
        qint8 c; stream >> c;

        if(c == '\0')
        {
            break;
        }

        temp.append(c);
    }

    in = temp;

    return stream;
}

QDataStream &SerialEscape::streamIn(QDataStream &stream, QString &in)
{
    QByteArray temp;

    while(!stream.atEnd())
    {
        qint8 c; stream >> c;

        if(c == '\0')
        {
            break;
        }

        temp.append(c);
    }

    in = temp;

    return stream;
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
