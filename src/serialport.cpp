/***************************************************************************//**
* @file
* Serial Port
*
* @version @n 1.0
* @date @n 4/3/2014
*
* @author @n Kwabena W. Agyeman
* @copyright @n (c) 2014 Kwabena W. Agyeman
* @n All rights reserved - Please see the end of the file for the terms of use
*
* @par Update History:
* @n v1.0 - Original release - 4/3/2014
*******************************************************************************/

#include "serialport.h"

const char *SerialPort::m_demoPortNameList[] =
{
    DemoPort::staticMetaObject.className()
};

const int SerialPort::m_demoPortNameListSize =
sizeof(SerialPort::m_demoPortNameList) / sizeof(const char *);

SerialPort::SerialPort(QWidget *widget,
QSettings *settings, QObject *parent) : QObject(parent)
{
    qRegisterMetaType< QPair<QString, QString> >
    ("QPair<QString, QString>");

    qRegisterMetaType< QList< QPair<QString, QString> > >
    ("QList< QPair<QString, QString> >");

    m_port = NULL;

    m_widget = widget;
    m_settings = settings;

    m_demoPortList = availableDemoPorts();
    m_serialPortList = availableSerialPorts();

    // Demo Port List Scanner /////////////////////////////////////////////////

    m_demoPortListTimer = new QTimer(this);

    connect(m_demoPortListTimer, SIGNAL(timeout()),
            this, SLOT(scanDemoPorts()));

    m_demoPortListTimer->start(DEMO_PORT_LIST_SCAN_RATE);

    // Serial Port List Scanner ///////////////////////////////////////////////

    m_serialPortListTimer = new QTimer(this);

    connect(m_serialPortListTimer, SIGNAL(timeout()),
            this, SLOT(scanSerialPorts()));

    m_serialPortListTimer->start(SERIAL_PORT_LIST_SCAN_RATE);
}

SerialPort::~SerialPort()
{
    if(m_port)
    {
        m_port->close(); delete m_port;
    }
}

void SerialPort::setPort(QIODevice *port)
{
    if(m_port != port)
    {
        if(m_port)
        {
            m_port->close(); delete m_port;
        }

        if(port)
        {
            if(port->parent() != this)
            {
                port->setParent(this);
            }

            if(!getOverride(port))
            {
                attachOverride(port);
            }

            connect(port, SIGNAL(aboutToClose()),
                    port, SLOT(deleteLater()));

            connect(port, SIGNAL(aboutToClose()),
                    this, SIGNAL(portAboutToClose()));

            connect(port, SIGNAL(destroyed()),
                    this, SIGNAL(portDestroyedOrChanged()));
        }

        QSettings settings(m_settings ? m_settings->fileName() : QString(),
        m_settings ? m_settings->format() : QSettings::Format());

        settings.beginGroup(SERIAL_PORT_KEY_GROUP);
        settings.setValue(SERIAL_PORT_KEY_PORT_OPEN, getName(port));

        m_port = port;

        emit portDestroyedOrChanged();
    }
}

QIODevice *SerialPort::getPort() const
{
    return m_port;
}

QString SerialPort::getPortName() const
{
    return getName(getPort());
}

QIODeviceSettingsOverride *SerialPort::getOverride() const
{
    return getOverride(getPort());
}

bool SerialPort::isDemoPort() const
{
    return isDemoPort(getPort());
}

bool SerialPort::isSerialPort() const
{
    return isSerialPort(getPort());
}

QString SerialPort::getLastPort() const
{
    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_PORT_KEY_GROUP);
    return settings.value(SERIAL_PORT_KEY_PORT_OPEN).toString();
}

bool SerialPort::getLastPortWasSet() const
{
    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_PORT_KEY_GROUP);
    return settings.contains(SERIAL_PORT_KEY_PORT_OPEN);
}

void SerialPort::setLastPortWasSet()
{
    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_PORT_KEY_GROUP);
    return settings.setValue(SERIAL_PORT_KEY_PORT_OPEN, getLastPort());
}

void SerialPort::setWidget(QWidget *widget)
{
    m_widget = widget;
}

QWidget *SerialPort::getWidget() const
{
    return m_widget;
}

void SerialPort::setSettings(QSettings *settings)
{
    m_settings = settings;
}

QSettings *SerialPort::getSettings() const
{
    return m_settings;
}

void SerialPort::setSerialMake(SerialMake *instance)
{
    if(m_serialMake != instance)
    {
        if(m_serialMake)
        {
            SerialMake *serialMake = m_serialMake; m_serialMake = NULL;

            serialMake->setSerialPort(NULL);
        }

        if(instance)
        {
            m_serialMake = instance;

            m_serialMake->setSerialPort(this);
        }
    }
}

SerialMake *SerialPort::getSerialMake() const
{
    return m_serialMake;
}

void SerialPort::setDemoPortList(const QList< QPair<QString, QString> >
                                 &portList)
{
    if(portList != m_demoPortList)
    {
        bool added = portList.size() > m_demoPortList.size();
        bool removed = portList.size() < m_demoPortList.size();

        m_demoPortList = portList;

        QList<SerialDevice *> list = findChildren<SerialDevice *>();

        foreach(SerialDevice *port, list)
        {
            if(!portInDemoPortList(getName(port)))
            {
                port->close(); delete port;
            }
        }

        emit demoPortListChanged();

        if(added)
        {
            emit demoPortAdded();
        }
        else if(removed)
        {
            emit demoPortRemoved();
        }
    }
}

QList< QPair<QString, QString> > SerialPort::getDemoPortList() const
{
    return m_demoPortList;
}

bool SerialPort::portInDemoPortList(const QString &portName) const
{
    bool found = false;

    QList< QPair<QString, QString> >::const_iterator i =
    m_demoPortList.constBegin();

    QList< QPair<QString, QString> >::const_iterator j =
    m_demoPortList.constEnd();

    for(; i != j; i++)
    {
        if(portName == i->second)
        {
            found = true; break;
        }
    }

    return found;
}

void SerialPort::setSerialPortList(const QList< QPair<QString, QString> >
                                   &portList)
{
    if(portList != m_serialPortList)
    {
        bool added = portList.size() > m_serialPortList.size();
        bool removed = portList.size() < m_serialPortList.size();

        m_serialPortList = portList;

        QList<QSerialPort *> list = findChildren<QSerialPort *>();

        foreach(QSerialPort *port, list)
        {
            if(!portInSerialPortList(getName(port)))
            {
                port->close(); delete port;
            }
        }

        emit serialPortListChanged();

        if(added)
        {
            emit serialPortAdded();
        }
        else if(removed)
        {
            emit serialPortRemoved();
        }
    }
}

QList< QPair<QString, QString> > SerialPort::getSerialPortList() const
{
    return m_serialPortList;
}

bool SerialPort::portInSerialPortList(const QString &portName) const
{
    bool found = false;

    QList< QPair<QString, QString> >::const_iterator i =
    m_serialPortList.constBegin();

    QList< QPair<QString, QString> >::const_iterator j =
    m_serialPortList.constEnd();

    for(; i != j; i++)
    {
        if(portName == i->second)
        {
            found = true; break;
        }
    }

    return found;
}

QList< QPair<QString, QString> > SerialPort::availableDemoPorts()
{
    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_PORT_KEY_GROUP);
    settings.beginGroup(SERIAL_PORT_KEY_PORT_LIST);

    QMap<QString, QPair<QString, QString> > list;

    for(int i = 0; i < m_demoPortNameListSize; i++)
    {
        QString portName = m_demoPortNameList[i];
        QString prettyName;

        // Can't use getPrettyName() as it requires the port to exist first.

        settings.beginGroup(portName);
        prettyName = settings.value(SERIAL_PORT_KEY_PRETTY_NAME,
        prettyDemoPortName(portName)).toString();
        settings.endGroup();

        list.insertMulti(prettyName,
        QPair<QString, QString>(prettyName, portName));
    }

    return list.values();
}

QList< QPair<QString, QString> > SerialPort::availableSerialPorts()
{
    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_PORT_KEY_GROUP);
    settings.beginGroup(SERIAL_PORT_KEY_PORT_LIST);

    QMap<QString, QPair<QString, QString> > list;

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QString portName = info.portName();
        QString prettyName;

        // Can't use getPrettyName() as it requires the port to exist first.

        settings.beginGroup(portName);
        prettyName = settings.value(SERIAL_PORT_KEY_PRETTY_NAME,
        prettySerialPortName(portName)).toString();
        settings.endGroup();

        list.insertMulti(prettyName,
        QPair<QString, QString>(prettyName, portName));
    }

    return list.values();
}

QIODevice *SerialPort::openPort(const QString &portName, bool silent)
{
    if(portName.isEmpty())
    {
        setPort(NULL); return NULL;
    }

    // Check Demo Ports

    for(int i = 0; i < m_demoPortNameListSize; i++)
    {
        if(portName == m_demoPortNameList[i])
        {
            SerialDevice *port;

            if(portName == DemoPort::staticMetaObject.className())
            {
                attachOverride(port = new DemoPort(this));
            }
            else
            {
                if(!silent) { QMessageBox::critical(m_widget, tr("Open Port"),
                tr("Demo port not found")); } setPort(NULL); return NULL;
            }

            if(port->open(QIODevice::ReadWrite))
            {
                setPort(port); return port;
            }

            QString errorString = port->errorString(); port->close();
            if(!silent) { QMessageBox::critical(m_widget, tr("Open Port"),
            errorString); } delete port; setPort(NULL); return NULL;
        }
    }

    // Check Serial Ports

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if(portName == info.portName())
        {
            QSerialPort *port;

            attachOverride(port = new QSerialPort(portName, this));

            if(!silent)
            {
                askSetPrettyNameWithDialog(portName);
                askSetMakeFileWithDialog(portName);
            }

            if(setBaudRate(port, DEF_BAUD_RATE)
            && port->setDataBits(QSerialPort::Data8)
            && port->setParity(QSerialPort::NoParity)
            && port->setStopBits(QSerialPort::OneStop)
            && port->setFlowControl(QSerialPort::NoFlowControl)
            && port->setDataTerminalReady(true)
            && port->setRequestToSend(true)
            && port->open(QIODevice::ReadWrite)
            && port->setBreakEnabled(true))
            {
                QThread::msleep(SERIAL_PORT_OPEN_RESET_WAIT);

                if((!port->setBreakEnabled(false))
                || (!port->setRequestToSend(false))
                || (!port->setDataTerminalReady(false)))
                {
                    QString errorString = port->errorString(); port->close();
                    if(!silent){QMessageBox::critical(m_widget,tr("Open Port"),
                    errorString); } delete port; setPort(NULL); return NULL;
                }

                QThread::msleep(SERIAL_PORT_OPEN_CLEAR_WAIT);

                if((!port->flush())
                || (!port->clear()))
                {
                    QString errorString = port->errorString(); port->close();
                    if(!silent){QMessageBox::critical(m_widget,tr("Open Port"),
                    errorString); } delete port; setPort(NULL); return NULL;
                }

                setPort(port); return port;
            }

            QString errorString = port->errorString(); port->close();
            if(!silent) { QMessageBox::critical(m_widget, tr("Open Port"),
            errorString); } delete port; setPort(NULL); return NULL;
        }
    }

    if(!silent) { QMessageBox::critical(m_widget, tr("Open Port"),
    tr("Serial port not found")); } setPort(NULL); return NULL;
}

void SerialPort::askSetPrettyNameWithDialog(const QString &portName)
{
    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_PORT_KEY_GROUP);
    settings.beginGroup(SERIAL_PORT_KEY_PORT_LIST);
    settings.beginGroup(portName);

    if(!settings.contains(SERIAL_PORT_KEY_PRETTY_NAME))
    {
        QMessageBox mb(QMessageBox::Question, tr("Change Name"),
        tr("Would you like to change this port's name?"),
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
        m_widget);

        mb.setDefaultButton(QMessageBox::Yes);
        mb.setEscapeButton(QMessageBox::Cancel);

        QIODevice *port = getPort(portName);

        if(port)
        {
            connect(port, SIGNAL(aboutToClose()), &mb, SLOT(reject()));
        }

        int result = mb.exec();

        if(result == QMessageBox::Yes)
        {
            setPrettyNameWithDialog(portName);
        }
        else if(result == QMessageBox::No)
        {
            // Don't ask again...

            settings.setValue(SERIAL_PORT_KEY_PRETTY_NAME,
                              getPrettyName(portName));
        }
    }
}

void SerialPort::askSetMakeFileWithDialog(const QString &portName)
{
    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_PORT_KEY_GROUP);
    settings.beginGroup(SERIAL_PORT_KEY_PORT_LIST);
    settings.beginGroup(portName);

    if(!settings.contains(SERIAL_PORT_KEY_MAKE_FILE))
    {
        QMessageBox mb(QMessageBox::Question, tr("Change Type"),
        tr("Would you like to change this port's type?"),
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
        m_widget);

        mb.setDefaultButton(QMessageBox::Yes);
        mb.setEscapeButton(QMessageBox::Cancel);

        QIODevice *port = getPort(portName);

        if(port)
        {
            connect(port, SIGNAL(aboutToClose()), &mb, SLOT(reject()));
        }

        int result = mb.exec();

        if(result == QMessageBox::Yes)
        {
            setMakeFileWithDialog(portName);
        }
        else if(result == QMessageBox::No)
        {
            // Don't ask again...

            settings.setValue(SERIAL_PORT_KEY_MAKE_FILE,
                              getMakeFile(portName));
        }
    }
}

void SerialPort::setPrettyNameWithDialog(const QString &portName)
{
    if(portName.isEmpty())
    {
        QMessageBox::critical(m_widget, tr("Change Name"),
        tr("Invalid Port")); return;
    }

    QInputDialog id(m_widget, Qt::MSWindowsFixedSizeDialogHint |
    Qt::WindowTitleHint | Qt::WindowSystemMenuHint |
    Qt::WindowCloseButtonHint);

    id.setWindowTitle(tr("Change Name"));
    id.setInputMode(QInputDialog::TextInput);
    id.setLabelText(tr("Please enter a new name"));
    id.setTextEchoMode(QLineEdit::Normal);
    id.setTextValue(getPrettyName(portName));

    QIODevice *port = getPort(portName);

    if(port)
    {
        connect(port, SIGNAL(aboutToClose()), &id, SLOT(reject()));
    }

    if(id.exec() == QDialog::Accepted)
    {
        if(!id.textValue().isEmpty())
        {
            setPrettyName(portName, id.textValue());
        }
        else
        {
            QMessageBox::critical(m_widget, tr("Change Name"),
            tr("Name cannot be empty")); return;
        }
    }
}

void SerialPort::setMakeFileWithDialog(const QString &portName)
{
    if(portName.isEmpty())
    {
        QMessageBox::critical(m_widget, tr("Change Type"),
        tr("Invalid Port")); return;
    }

    // Begin SerialMake Override //////////////////////////////////////////////

    if(m_serialMake)
    {
        m_serialMake->setMakeFileWithDialogWithPort(portName, this); return;
    }

    // End SerialMake Override ////////////////////////////////////////////////

    QInputDialog id(m_widget, Qt::MSWindowsFixedSizeDialogHint |
    Qt::WindowTitleHint | Qt::WindowSystemMenuHint |
    Qt::WindowCloseButtonHint);

    id.setWindowTitle(tr("Change Type"));
    id.setInputMode(QInputDialog::TextInput);
    id.setLabelText(tr("Please enter a new type"));
    id.setTextEchoMode(QLineEdit::Normal);
    id.setTextValue(getMakeFile(portName));

    QIODevice *port = getPort(portName);

    if(port)
    {
        connect(port, SIGNAL(aboutToClose()), &id, SLOT(reject()));
    }

    if(id.exec() == QDialog::Accepted)
    {
        if(!id.textValue().isEmpty())
        {
            setMakeFile(portName, id.textValue());
        }
        else
        {
            QMessageBox::critical(m_widget, tr("Change Type"),
            tr("Type cannot be empty")); return;
        }
    }
}

void SerialPort::setPrettyName(const QString &portName,
                               const QString &prettyName)
{
    if(portName.isEmpty())
    {
        return;
    }

    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_PORT_KEY_GROUP);
    settings.beginGroup(SERIAL_PORT_KEY_PORT_LIST);
    settings.beginGroup(portName);

    settings.setValue(SERIAL_PORT_KEY_PRETTY_NAME, prettyName);

    scanDemoPorts();
    scanSerialPorts();

    if(portName == getPortName())
    {
        emit portDestroyedOrChanged();
    }
}

QString SerialPort::getPrettyName(const QString &portName) const
{
    if(portName.isEmpty())
    {
        return QString();
    }

    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_PORT_KEY_GROUP);
    settings.beginGroup(SERIAL_PORT_KEY_PORT_LIST);
    settings.beginGroup(portName);

    QString result = settings.value(SERIAL_PORT_KEY_PRETTY_NAME).toString();

    if(!settings.contains(SERIAL_PORT_KEY_PRETTY_NAME))
    {
        if(qobject_cast<SerialDevice *>(getPort(portName)))
        {
            result = prettyDemoPortName(portName);
        }
        else if(qobject_cast<QSerialPort *>(getPort(portName)))
        {
            result = prettySerialPortName(portName);
        }
    }

    return result;
}

void SerialPort::setMakeFile(const QString &portName,
                             const QString &makeFile)
{
    if(portName.isEmpty())
    {
        return;
    }

    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_PORT_KEY_GROUP);
    settings.beginGroup(SERIAL_PORT_KEY_PORT_LIST);
    settings.beginGroup(portName);

    settings.setValue(SERIAL_PORT_KEY_MAKE_FILE,
    QDir::fromNativeSeparators(QDir::cleanPath(makeFile)));

    if(portName == getPortName())
    {
        emit portDestroyedOrChanged();
    }
}

QString SerialPort::getMakeFile(const QString &portName) const
{
    if(portName.isEmpty())
    {
        return QString();
    }

    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_PORT_KEY_GROUP);
    settings.beginGroup(SERIAL_PORT_KEY_PORT_LIST);
    settings.beginGroup(portName);

    QString result = QDir::fromNativeSeparators(QDir::cleanPath(
    settings.value(SERIAL_PORT_KEY_MAKE_FILE).toString()));

    if(!settings.contains(SERIAL_PORT_KEY_MAKE_FILE))
    {
        if(qobject_cast<SerialDevice *>(getPort(portName)))
        {
            result = result; // UNUSED //
        }
        else if(qobject_cast<QSerialPort *>(getPort(portName)))
        {
            result = result; // UNUSED //
        }
    }

    return result;
}

void SerialPort::setOverrideBaudRate(const QString &portName,
                                     const QString &baudRate)
{
    if(portName.isEmpty())
    {
        return;
    }

    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_PORT_KEY_GROUP);
    settings.beginGroup(SERIAL_PORT_KEY_PORT_LIST);
    settings.beginGroup(portName);

    settings.setValue(SERIAL_PORT_KEY_BAUD_RATE, baudRate);

    QIODeviceSettingsOverride *override = getOverride(portName);

    if(override)
    {
        override->m_baudRateOverride = baudRate;
    }

    QIODevice *port = getPort(override);

    setBaudRate(port, getBaudRate(port));

    if(portName == getPortName())
    {
        emit portDestroyedOrChanged();
    }
}

QString SerialPort::getOverrideBaudRate(const QString &portName) const
{
    if(portName.isEmpty())
    {
        return QString();
    }

    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_PORT_KEY_GROUP);
    settings.beginGroup(SERIAL_PORT_KEY_PORT_LIST);
    settings.beginGroup(portName);

    return settings.value(SERIAL_PORT_KEY_BAUD_RATE).toString();
}

void SerialPort::setOverrideLatencyTimer(const QString &portName,
                                         const QString &latencyTimer)
{
    if(portName.isEmpty())
    {
        return;
    }

    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_PORT_KEY_GROUP);
    settings.beginGroup(SERIAL_PORT_KEY_PORT_LIST);
    settings.beginGroup(portName);

    settings.setValue(SERIAL_PORT_KEY_LATENCY_TIMER, latencyTimer);

    QIODeviceSettingsOverride *override = getOverride(portName);

    if(override)
    {
        override->m_latencyTimerOverride = latencyTimer;
    }

    QIODevice *port = getPort(override);

    setLatencyTimer(port, getLatencyTimer(port));

    if(portName == getPortName())
    {
        emit portDestroyedOrChanged();
    }
}

QString SerialPort::getOverrideLatencyTimer(const QString &portName) const
{
    if(portName.isEmpty())
    {
        return QString();
    }

    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_PORT_KEY_GROUP);
    settings.beginGroup(SERIAL_PORT_KEY_PORT_LIST);
    settings.beginGroup(portName);

    return settings.value(SERIAL_PORT_KEY_LATENCY_TIMER).toString();
}

void SerialPort::setOverrideTransferTime(const QString &portName,
                                         const QString &readTime,
                                         const QString &writeTime)
{
    if(portName.isEmpty())
    {
        return;
    }

    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_PORT_KEY_GROUP);
    settings.beginGroup(SERIAL_PORT_KEY_PORT_LIST);
    settings.beginGroup(portName);

    settings.setValue(SERIAL_PORT_KEY_READ_TRANSFER_TIME, readTime);
    settings.setValue(SERIAL_PORT_KEY_WRITE_TRANSFER_TIME, writeTime);

    QIODeviceSettingsOverride *override = getOverride(portName);

    if(override)
    {
        override->m_readTransferTimeOverride = readTime;
        override->m_writeTransferTimeOverride = writeTime;
    }

    QIODevice *port = getPort(override);
    QPair<int, int> time = getTransferTime(port);

    setTransferTime(port, time.first, time.second);

    if(portName == getPortName())
    {
        emit portDestroyedOrChanged();
    }
}

QPair<QString, QString> SerialPort::getOverrideTransferTime(const QString
                                                            &portName) const
{
    if(portName.isEmpty())
    {
        return QPair<QString, QString>(QString(), QString());
    }

    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_PORT_KEY_GROUP);
    settings.beginGroup(SERIAL_PORT_KEY_PORT_LIST);
    settings.beginGroup(portName);

    QPair<QString, QString> pair;

    pair.first=settings.value(SERIAL_PORT_KEY_READ_TRANSFER_TIME).toString();
    pair.second=settings.value(SERIAL_PORT_KEY_WRITE_TRANSFER_TIME).toString();

    return pair;
}

void SerialPort::setOverrideTransferSize(const QString &portName,
                                         const QString &readSize,
                                         const QString &writeSize)
{
    if(portName.isEmpty())
    {
        return;
    }

    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_PORT_KEY_GROUP);
    settings.beginGroup(SERIAL_PORT_KEY_PORT_LIST);
    settings.beginGroup(portName);

    settings.setValue(SERIAL_PORT_KEY_READ_TRANSFER_SIZE, readSize);
    settings.setValue(SERIAL_PORT_KEY_WRITE_TRANSFER_SIZE, writeSize);

    QIODeviceSettingsOverride *override = getOverride(portName);

    if(override)
    {
        override->m_readTransferSizeOverride = readSize;
        override->m_writeTransferSizeOverride = writeSize;
    }

    QIODevice *port = getPort(override);
    QPair<int, int> size = getTransferSize(port);

    setTransferSize(port, size.first, size.second);

    if(portName == getPortName())
    {
        emit portDestroyedOrChanged();
    }
}

QPair<QString, QString> SerialPort::getOverrideTransferSize(const QString
                                                            &portName) const
{
    if(portName.isEmpty())
    {
        return QPair<QString, QString>(QString(), QString());
    }

    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_PORT_KEY_GROUP);
    settings.beginGroup(SERIAL_PORT_KEY_PORT_LIST);
    settings.beginGroup(portName);

    QPair<QString, QString> pair;

    pair.first=settings.value(SERIAL_PORT_KEY_READ_TRANSFER_SIZE).toString();
    pair.second=settings.value(SERIAL_PORT_KEY_WRITE_TRANSFER_SIZE).toString();

    return pair;
}

bool SerialPort::clearAll(QIODevice *port)
{
    if(qobject_cast<QSerialPort *>(port))
    {
        return static_cast<QSerialPort *>(port)->clear();
    }

    return port;
}

bool SerialPort::flushAll(QIODevice *port)
{
    if(qobject_cast<QSerialPort *>(port))
    {
        return static_cast<QSerialPort *>(port)->flush();
    }

    return port;
}

void SerialPort::connectResetEvent(QIODevice *port,
                                   QObject *receiver,
                                   const char *slot)
{
    if(receiver && slot)
    {
        if(qobject_cast<SerialDevice *>(port))
        {
            connect(static_cast<SerialDevice *>(port),
            SIGNAL(resetEvent()), receiver, slot);
        }
        else if(qobject_cast<QSerialPort *>(port))
        {
            QSerialPortSignalMapper *mapper =
            new QSerialPortSignalMapper(receiver);

            mapper->connectSender(static_cast<QSerialPort *>(port),
            SIGNAL(error(QSerialPort::SerialPortError)),
            SLOT(map(QSerialPort::SerialPortError)));

            connect(mapper, SIGNAL(mapped()), receiver, slot);
            connect(port, SIGNAL(destroyed()), mapper, SLOT(deleteLater()));
        }
    }
}

void SerialPort::disconnectResetEvent(QIODevice *port,
                                      QObject *receiver,
                                      const char *slot)
{
    if(receiver && slot)
    {
        if(qobject_cast<SerialDevice *>(port))
        {
            disconnect(static_cast<SerialDevice *>(port),
            SIGNAL(resetEvent()), receiver, slot);
        }
        else if(qobject_cast<QSerialPort *>(port))
        {
            QList<QSerialPortSignalMapper *> list =
            receiver->findChildren<QSerialPortSignalMapper *>();

            foreach(QSerialPortSignalMapper *mapper, list)
            {
                if(mapper->getSenderObject() == port)
                {
                    delete mapper; break;
                }
            }
        }
    }
}

void SerialPort::connectResetEvent2(QIODevice *port,
                                    QObject *parent,
                                    const char *slot)
{
    Q_UNUSED(port); Q_UNUSED(parent); Q_UNUSED(slot);
}

void SerialPort::disconnectResetEvent2(QIODevice *port,
                                       QObject *parent,
                                       const char *slot)
{
    Q_UNUSED(port); Q_UNUSED(parent); Q_UNUSED(slot);
}

bool SerialPort::setBaudRate(QIODevice *port,
                             int baudRate)
{
    QIODeviceSettingsOverride *override = getOverride(port);

    if(override)
    {
        if(!override->m_baudRateOverride.isEmpty())
        {
            baudRate = override->m_baudRateOverride.toInt();
        }
    }

    if(qobject_cast<QSerialPort *>(port))
    {
        return static_cast<QSerialPort *>(port)->setBaudRate(baudRate);
    }

    return port;
}

int SerialPort::getBaudRate(QIODevice *port)
{
    if(qobject_cast<QSerialPort *>(port))
    {
        return static_cast<QSerialPort *>(port)->baudRate();
    }

    return int();
}

bool SerialPort::setLatencyTimer(QIODevice *port,
                                 int latencyTimer)
{
    QIODeviceSettingsOverride *override = getOverride(port);

    if(override)
    {
        if(!override->m_latencyTimerOverride.isEmpty())
        {
            latencyTimer = override->m_latencyTimerOverride.toInt();
        }
    }

    Q_UNUSED(latencyTimer);

    return port;
}

int SerialPort::getLatencyTimer(QIODevice *port)
{
    Q_UNUSED(port);

    return int();
}

bool SerialPort::setTransferTime(QIODevice *port,
                                 int readTime,
                                 int writeTime)
{
    QIODeviceSettingsOverride *override = getOverride(port);

    if(override)
    {
        if(!override->m_readTransferTimeOverride.isEmpty())
        {
            readTime = override->m_readTransferTimeOverride.toInt();
        }

        if(!override->m_writeTransferTimeOverride.isEmpty())
        {
            writeTime = override->m_writeTransferTimeOverride.toInt();
        }
    }

    Q_UNUSED(readTime); Q_UNUSED(writeTime);

    return port;
}

QPair<int, int> SerialPort::getTransferTime(QIODevice *port)
{
    Q_UNUSED(port);

    return QPair<int, int>(int(), int());
}

bool SerialPort::setTransferSize(QIODevice *port,
                                 int readSize,
                                 int writeSize)
{
    QIODeviceSettingsOverride *override = getOverride(port);

    if(override)
    {
        if(!override->m_readTransferSizeOverride.isEmpty())
        {
            readSize = override->m_readTransferSizeOverride.toInt();
        }

        if(!override->m_writeTransferSizeOverride.isEmpty())
        {
            writeSize = override->m_writeTransferSizeOverride.toInt();
        }
    }

    Q_UNUSED(readSize); Q_UNUSED(writeSize);

    return port;
}

QPair<int, int> SerialPort::getTransferSize(QIODevice *port)
{
    Q_UNUSED(port);

    return QPair<int, int>(int(), int());
}

bool SerialPort::isDemoPort(QIODevice *port)
{
    return qobject_cast<SerialDevice *>(port);
}

bool SerialPort::isSerialPort(QIODevice *port)
{
    return qobject_cast<QSerialPort *>(port);
}

void SerialPort::scanDemoPorts()
{
    DemoPortScanner *scanner = new DemoPortScanner(this);
    QThread *thread = new QThread;

    scanner->moveToThread(thread);

    connect(thread, SIGNAL(started()), scanner, SLOT(scan()));

    connect(scanner, SIGNAL(availablePorts(QList<QPair<QString,QString> >)),
            this, SLOT(scanDemoPorts2(QList<QPair<QString,QString> >)));

    connect(scanner, SIGNAL(availablePorts(QList<QPair<QString,QString> >)),
            thread, SLOT(quit()));

    connect(scanner, SIGNAL(availablePorts(QList<QPair<QString,QString> >)),
            scanner, SLOT(deleteLater()));

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}

void SerialPort::scanDemoPorts2(QList< QPair<QString, QString> > list)
{
    setDemoPortList(list);
}

void SerialPort::scanSerialPorts()
{
    SerialPortScanner *scanner = new SerialPortScanner(this);
    QThread *thread = new QThread;

    scanner->moveToThread(thread);

    connect(thread, SIGNAL(started()), scanner, SLOT(scan()));

    connect(scanner, SIGNAL(availablePorts(QList<QPair<QString,QString> >)),
            this, SLOT(scanSerialPorts2(QList<QPair<QString,QString> >)));

    connect(scanner, SIGNAL(availablePorts(QList<QPair<QString,QString> >)),
            thread, SLOT(quit()));

    connect(scanner, SIGNAL(availablePorts(QList<QPair<QString,QString> >)),
            scanner, SLOT(deleteLater()));

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}

void SerialPort::scanSerialPorts2(QList< QPair<QString, QString> > list)
{
    setSerialPortList(list);
}

void SerialPort::attachOverride(QIODevice *port)
{
    QSettings settings(m_settings ? m_settings->fileName() : QString(),
    m_settings ? m_settings->format() : QSettings::Format());

    settings.beginGroup(SERIAL_PORT_KEY_GROUP);
    settings.beginGroup(SERIAL_PORT_KEY_PORT_LIST);
    settings.beginGroup(getName(port));

    QIODeviceSettingsOverride *override = new QIODeviceSettingsOverride(port);

    override->m_baudRateOverride = settings.value(
    SERIAL_PORT_KEY_BAUD_RATE).toString();

    override->m_latencyTimerOverride = settings.value(
    SERIAL_PORT_KEY_LATENCY_TIMER).toString();

    override->m_readTransferTimeOverride = settings.value(
    SERIAL_PORT_KEY_READ_TRANSFER_TIME).toString();

    override->m_writeTransferTimeOverride = settings.value(
    SERIAL_PORT_KEY_WRITE_TRANSFER_TIME).toString();

    override->m_readTransferSizeOverride = settings.value(
    SERIAL_PORT_KEY_READ_TRANSFER_SIZE).toString();

    override->m_writeTransferSizeOverride = settings.value(
    SERIAL_PORT_KEY_WRITE_TRANSFER_SIZE).toString();
}

QString SerialPort::prettyDemoPortName(const QString &portName)
{
    if(portName.isEmpty())
    {
        return QString();
    }

    return QString(portName).replace(QRegularExpression(QString("%1|%2|%3").
    arg("(?<=[A-Z])(?=[A-Z][a-z])").
    arg("(?<=[^A-Z])(?=[A-Z])").
    arg("(?<=[A-Za-z])(?=[^A-Za-z])")),
    " "); // by: polygenelubricants
}

QString SerialPort::prettySerialPortName(const QString &portName)
{
    if(portName.isEmpty())
    {
        return QString();
    }

    QSerialPortInfo info(portName);

    return info.manufacturer() +
    " - " + info.description() +
    " - " + info.portName() /* +
    " - " + info.systemLocation()*/;
}

QIODevice *SerialPort::getPort(const QString &name) const
{
    if(!name.isEmpty())
    {
        QList<QIODevice *> list = findChildren<QIODevice *>();

        foreach(QIODevice *port, list)
        {
            if(getName(port) == name)
            {
                return port;
            }
        }
    }

    return NULL;
}

QString SerialPort::getName(QIODevice *port)
{
    if(port)
    {
        if(qobject_cast<SerialDevice *>(port))
        {
            return static_cast<SerialDevice *>(port)->metaObject()->className();
        }
        else if(qobject_cast<QSerialPort *>(port))
        {
            return static_cast<QSerialPort *>(port)->portName();
        }
    }

    return QString();
}

QIODeviceSettingsOverride *SerialPort::getOverride(const QString &name) const
{
    if(!name.isEmpty())
    {
        return getOverride(getPort(name));
    }

    return NULL;
}

QString SerialPort::getName(QIODeviceSettingsOverride *override)
{
    if(override)
    {
        return getName(getPort(override));
    }

    return QString();
}

QIODeviceSettingsOverride *SerialPort::getOverride(QIODevice *port)
{
    if(port)
    {
        return port->findChild<QIODeviceSettingsOverride *>();
    }

    return NULL;
}

QIODevice *SerialPort::getPort(QIODeviceSettingsOverride *override)
{
    if(override)
    {
        return qobject_cast<QIODevice *>(override->parent());
    }

    return NULL;
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
