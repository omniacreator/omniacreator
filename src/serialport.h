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

#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QtCore>
#include <QtNetwork>
#include <QtSerialPort>
#include <QtWidgets>

#include "projectdefines.h"
#include "serialmake.h"

#include "demoport.h"

#define DEMO_PORT_LIST_SCAN_RATE (1 * 1000)
#define SERIAL_PORT_LIST_SCAN_RATE (1 * 1000)

#define SERIAL_PORT_OPEN_RESET_WAIT (1 * 100)
#define SERIAL_PORT_OPEN_CLEAR_WAIT (1 * 100)

class SerialMake;

class QSerialPortSignalMapper : public QObject
{
    Q_OBJECT

public:

    explicit QSerialPortSignalMapper(QObject *parent) : QObject(parent)
    {
        // UNUSED //
    }

    virtual ~QSerialPortSignalMapper()
    {
        // UNUSED //
    }

    void setSenderObject(const QObject *sender)
    {
        m_sender = sender;
    }

    const QObject *getSenderObject() const
    {
        return m_sender;
    }

    QMetaObject::Connection connectSender(const QObject *sender,
                                          const char *signal,
                                          const char *method)
    {
        return connect(m_sender = sender, signal, method);
    }

public slots:

    void map(QSerialPort::SerialPortError error)
    {
        if(error == QSerialPort::BreakConditionError)
        {
            emit mapped();
        }
    }

signals:

    void mapped();

private:

    Q_DISABLE_COPY(QSerialPortSignalMapper)

    QPointer<const QObject> m_sender;
};

class QIODeviceSettingsOverride : public QObject
{
    Q_OBJECT

public:

    explicit QIODeviceSettingsOverride(QObject *parent) : QObject(parent)
    {
        // UNUSED //
    }

    virtual ~QIODeviceSettingsOverride()
    {
        // UNUSED //
    }

    QString m_baudRateOverride;
    QString m_latencyTimerOverride;
    QString m_readTransferTimeOverride;
    QString m_writeTransferTimeOverride;
    QString m_readTransferSizeOverride;
    QString m_writeTransferSizeOverride;

private:

    Q_DISABLE_COPY(QIODeviceSettingsOverride)
};

class SerialPort : public QObject
{
    Q_OBJECT

public:

    explicit SerialPort(QWidget *widget = NULL,
                        QSettings *settings = NULL, QObject *parent = NULL);

    virtual ~SerialPort();

    void setPort(QIODevice *port);
    QIODevice *getPort() const;

    QString getPortName() const;
    QIODeviceSettingsOverride *getOverride() const;

    bool isDemoPort() const;
    bool isSerialPort() const;

    QString getLastPort() const;
    bool getLastPortWasSet() const;
    void setLastPortWasSet();

    void setWidget(QWidget *widget);
    QWidget *getWidget() const;

    void setSettings(QSettings *settings);
    QSettings *getSettings() const;

    void setSerialMake(SerialMake *instance);
    SerialMake *getSerialMake() const;

    void setDemoPortList(const QList< QPair<QString, QString> > &portList);
    QList< QPair<QString, QString> > getDemoPortList() const;
    bool portInDemoPortList(const QString &portName) const;

    void setSerialPortList(const QList< QPair<QString, QString> > &portList);
    QList< QPair<QString, QString> > getSerialPortList() const;
    bool portInSerialPortList(const QString &portName) const;

    QList< QPair<QString, QString> > availableDemoPorts();
    QList< QPair<QString, QString> > availableSerialPorts();

    QIODevice *openPort(const QString &portName, bool silent);

    void askSetPrettyNameWithDialog(const QString &portName);
    void askSetMakeFileWithDialog(const QString &portName);

    void setPrettyNameWithDialog(const QString &portName);
    void setMakeFileWithDialog(const QString &portName);

    void setPrettyName(const QString &portName, const QString &prettyName);
    QString getPrettyName(const QString &portName) const;

    void setMakeFile(const QString &portName, const QString &makeFile);
    QString getMakeFile(const QString &portName) const;

    // Overrides //////////////////////////////////////////////////////////////

    void setOverrideBaudRate(const QString &portName,
                             const QString &baudRate);

    QString getOverrideBaudRate(const QString &portName) const;

    void setOverrideLatencyTimer(const QString &portName,
                                 const QString &latencyTimer);

    QString getOverrideLatencyTimer(const QString &portName) const;

    void setOverrideTransferTime(const QString &portName,
                                 const QString &readTime,
                                 const QString &writeTime);

    QPair<QString, QString> getOverrideTransferTime(const QString
                                                    &portName) const;

    void setOverrideTransferSize(const QString &portName,
                                 const QString &readSize,
                                 const QString &writeSize);

    QPair<QString, QString> getOverrideTransferSize(const QString
                                                    &portName) const;

    // Actions ////////////////////////////////////////////////////////////////

    static bool clearAll(QIODevice *port);
    static bool flushAll(QIODevice *port);

    // Connects ///////////////////////////////////////////////////////////////

    static void connectResetEvent(QIODevice *port,
                                  QObject *receiver,
                                  const char *slot);

    static void disconnectResetEvent(QIODevice *port,
                                     QObject *receiver,
                                     const char *slot);

    static void connectResetEvent2(QIODevice *port,
                                   QObject *receiver,
                                   const char *slot);

    static void disconnectResetEvent2(QIODevice *port,
                                      QObject *receiver,
                                      const char *slot);

    // Settings ///////////////////////////////////////////////////////////////

    static bool setBaudRate(QIODevice *port,
                            int baudRate);

    static int getBaudRate(QIODevice *port);

    static bool setLatencyTimer(QIODevice *port,
                                int latencyTimer);

    static int getLatencyTimer(QIODevice *port);

    static bool setTransferTime(QIODevice *port,
                                int readTime,
                                int writeTime);

    static QPair<int, int> getTransferTime(QIODevice *port);

    static bool setTransferSize(QIODevice *port,
                                int readSize,
                                int writeSize);

    static QPair<int, int> getTransferSize(QIODevice *port);

    static bool isDemoPort(QIODevice *port);
    static bool isSerialPort(QIODevice *port);

private slots:

    void scanDemoPorts();
    void scanDemoPorts2(QList< QPair<QString, QString> > list);
    void scanSerialPorts();
    void scanSerialPorts2(QList< QPair<QString, QString> > list);

signals:

    void demoPortListChanged();
    void serialPortListChanged();

    void demoPortAdded();
    void serialPortAdded();

    void demoPortRemoved();
    void serialPortRemoved();

    void portAboutToClose();
    void portDestroyedOrChanged();

private:

    Q_DISABLE_COPY(SerialPort)

    friend class SerialMake;

    void attachOverride(QIODevice *port);

    static QString prettyDemoPortName(const QString &portName);
    static QString prettySerialPortName(const QString &portName);

    QIODevice *getPort(const QString &name) const;
    static QString getName(QIODevice *port);

    QIODeviceSettingsOverride *getOverride(const QString &name) const;
    static QString getName(QIODeviceSettingsOverride *override);

    static QIODeviceSettingsOverride *getOverride(QIODevice *port);
    static QIODevice *getPort(QIODeviceSettingsOverride *override);

    static const char *m_demoPortNameList[];
    static const int m_demoPortNameListSize;

    QPointer<QIODevice> m_port;
    QPointer<QWidget> m_widget;
    QPointer<QSettings> m_settings;
    QPointer<SerialMake> m_serialMake;

    QList< QPair<QString, QString> > m_demoPortList;
    QList< QPair<QString, QString> > m_serialPortList;

    QTimer *m_demoPortListTimer;
    QTimer *m_serialPortListTimer;
};

class DemoPortScanner : public QObject
{
    Q_OBJECT

public:

    explicit DemoPortScanner(SerialPort *port)
    {
        m_port = port;
    }

public slots:

    void scan()
    {
        emit availablePorts(m_port->availableDemoPorts());
    }

signals:

    void availablePorts(QList< QPair<QString, QString> > list);

private:

    SerialPort *m_port;
};

class SerialPortScanner : public QObject
{
    Q_OBJECT

public:

    explicit SerialPortScanner(SerialPort *port)
    {
        m_port = port;
    }

public slots:

    void scan()
    {
        emit availablePorts(m_port->availableSerialPorts());
    }

signals:

    void availablePorts(QList< QPair<QString, QString> > list);

private:

    SerialPort *m_port;
};

#endif // SERIALPORT_H

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
