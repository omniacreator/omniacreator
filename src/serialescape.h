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

#ifndef SERIALESCAPE_H
#define SERIALESCAPE_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "serialport.h"
#include "projectdefines.h"

#include "serialwindow.h"
#include "serialterminal.h"
#include "serialoscilloscope.h"
#include "serialtable.h"
#include "serialtree.h"
#include "serialgraphics.h"
#include "serialinterface.h"

#define DEF_POLL_WAIT (1000 / 100)
#define MAX_POLL_WAIT (1000 / 100)

#define DEGLITCH_WAIT 10 // 10 ms

class SerialEscape : public QObject
{
    Q_OBJECT

public:

    static int transferTimeHint(int baudRate);
    static int transferSizeHint(int baudRate);

    explicit SerialEscape(QIODevice *port = NULL,
                          QWidget *widget = NULL,
                          QSettings *settings = NULL,
                          QObject *parent = NULL);

    virtual ~SerialEscape();

    void setEnabled(bool enabled);
    bool getEnabled() const;

    void setPort(QIODevice *port);
    QIODevice *getPort() const;

    void setWidget(QWidget *widget);
    QWidget *getWidget() const;

    void setSettings(QSettings *settings);
    QSettings *getSettings() const;

    SerialTerminal *serialTerminal() const;
    QList<SerialWindow *> serialWindows() const;

    void setOpenProcessEnabled(bool enable);
    bool getOpenProcessEnabled() const;

    void setOpenUrlEnabled(bool enable);
    bool getOpenUrlEnabled() const;

public slots:

    void openJSON(const QString &file = QString());

    void removeWidget(QAction *action = NULL);
    void removeAllWidgets(QAction *action = NULL);

    void userReset();

private slots:

    void windowDestroyed(QObject *object);

    void resetEvent(); // modem status changed (for ftdi)
    void resetEvent2(); // line status changed (for ftdi)

    void readData();
    void sendData(const QByteArray &bytes, QWidget *parent);

    void writeList(const QByteArray &bytes);
    void writeData(const QByteArray &bytes);
    void writeData(char byte);
    void writeTask();

    void sendVariantPacket(SerialFunction function,
                           const QVariant &value);

    void sendVariantPacket(SerialFunction function,
                           int handle0,
                           const QVariant &value);

    void sendVariantPacket(SerialFunction function,
                           int handle0,
                           int handle1,
                           const QVariant &value);

    void sendVariantPacket(SerialFunction function,
                           int handle0,
                           int handle1,
                           int handle2,
                           const QVariant &value);

signals:

    void errorMessage(const QString &text);

    void cancelTask();

private:

    enum SerialResponse { SR_OK, SR_INT, SR_FLOAT, SR_STRING, SR_NONE };

    void readLayer0(const QByteArray &bytes);
    void readLayer1(const QByteArray &bytes);

    void parseData(SerialFunction function, const QByteArray &data);
    bool parseData(SerialFunction function, const QByteArray &data,
                   SerialResponse *response, QVariant *result);

    ///////////////////////////////////////////////////////////////////////////

    SerialWindow *newTerminalWindow(int windowHandle,
                                    const QString &name,
                                    bool autoShow = true);

    SerialWindow *newOscilloscopeWindow(int windowHandle,
                                        const QString &name,
                                        bool autoShow = true);

    SerialWindow *newTableWindow(int windowHandle,
                                 const QString &name,
                                 bool autoShow = true);

    SerialWindow *newTreeWindow(int windowHandle,
                                const QString &name,
                                bool autoShow = true);

    SerialWindow *newGraphicsWindow(int windowHandle,
                                    const QString &name,
                                    bool autoShow = true);

    SerialWindow *newInterfaceWindow(int windowHandle,
                                     const QString &name,
                                     bool autoShow = true);

    bool dockWindow(QWidget *widget, SerialWindow *window);

    ///////////////////////////////////////////////////////////////////////////

    void sendStringPacket(SerialFunction function,
                          const QByteArray &value);

    void sendStringPacket(SerialFunction function,
                          int handle0,
                          const QByteArray &value);

    void sendStringPacket(SerialFunction function,
                          int handle0,
                          int handle1,
                          const QByteArray &value);

    void sendStringPacket(SerialFunction function,
                          int handle0,
                          int handle1,
                          int handle2,
                          const QByteArray &value);

    ///////////////////////////////////////////////////////////////////////////

    void sendFloatPacket(SerialFunction function,
                         float value);

    void sendFloatPacket(SerialFunction function,
                         int handle0,
                         float value);

    void sendFloatPacket(SerialFunction function,
                         int handle0,
                         int handle1,
                         float value);

    void sendFloatPacket(SerialFunction function,
                         int handle0,
                         int handle1,
                         int handle2,
                         float value);

    ///////////////////////////////////////////////////////////////////////////

    void sendIntPacket(SerialFunction function,
                       int value);

    void sendIntPacket(SerialFunction function,
                       int handle0,
                       int value);

    void sendIntPacket(SerialFunction function,
                       int handle0,
                       int handle1,
                       int value);

    void sendIntPacket(SerialFunction function,
                       int handle0,
                       int handle1,
                       int handle2,
                       int value);

    ///////////////////////////////////////////////////////////////////////////

    void sendLayer1(SerialFunction function, const QByteArray &bytes);
    void sendLayer0(const QByteArray &bytes);

    ///////////////////////////////////////////////////////////////////////////

    template <class T>
    T *windowCast(const QString &FName, int window);

    SerialWindow *windowExists(const QString &FName, int window);
    bool windowDoesNotExist(const QString &FName, int window);

    quint16 qChecksum2(const char *data, uint len);

    inline QDataStream &streamIn(QDataStream &stream, bool &in);
    inline QDataStream &streamIn(QDataStream &stream, qint8 &in);
    inline QDataStream &streamIn(QDataStream &stream, quint8 &in);
    inline QDataStream &streamIn(QDataStream &stream, qint16 &in);
    inline QDataStream &streamIn(QDataStream &stream, quint16 &in);
    inline QDataStream &streamIn(QDataStream &stream, qint32 &in);
    inline QDataStream &streamIn(QDataStream &stream, quint32 &in);
    inline QDataStream &streamIn(QDataStream &stream, qint64 &in);
    inline QDataStream &streamIn(QDataStream &stream, quint64 &in);
    inline QDataStream &streamIn(QDataStream &stream, float &in);
    inline QDataStream &streamIn(QDataStream &stream, double &in);
    inline QDataStream &streamIn(QDataStream &stream, char *&in);
    QDataStream &streamIn(QDataStream &stream, QByteArray &in);
    QDataStream &streamIn(QDataStream &stream, QString &in);

    ///////////////////////////////////////////////////////////////////////////

    bool m_enabled; QPointer<QIODevice> m_port;
    QPointer<QWidget> m_widget; QPointer<QSettings> m_settings;

    bool m_cancelTask; QMutex m_readRecurseLock, m_sendRecurseLock;

    QByteArray m_writeQueue; int m_writeCount;
    int m_writeCredits; QList<QByteArray> m_writeList;

    int m_maxCredits; int m_maxLayer0DataSize;

    ///////////////////////////////////////////////////////////////////////////

    enum
    {
        L0_WAIT_FOR_ESCAPE,
        L0_WAIT_FOR_NULL,
        L0_WAIT_FOR_PACKET_SIZE_0,
        L0_WAIT_FOR_PACKET_SIZE_1,
        L0_WAIT_FOR_PACKET_DATA,
        L0_WAIT_FOR_CHECKSUM_0,
        L0_WAIT_FOR_CHECKSUM_1,
        L0_ERROR_STATE
    }
    m_L0StateMachine; int m_L0Size, m_L0Checksum; QByteArray m_L0Data;

    ///////////////////////////////////////////////////////////////////////////

    enum
    {
        L1_WAIT_FOR_PACKET_FUNCTION_0,
        L1_WAIT_FOR_PACKET_FUNCTION_1,
        L1_WAIT_FOR_PACKET_SIZE_0,
        L1_WAIT_FOR_PACKET_SIZE_1,
        L1_WAIT_FOR_PACKET_DATA,
        L1_ERROR_STATE
    }
    m_L1StateMachine; int m_L1Function, m_L1Size; QByteArray m_L1Data;

    ///////////////////////////////////////////////////////////////////////////

    bool m_openProcessEn, m_openUrlEn, m_checksumMode; int m_baudRate;

    QPointer<SerialTerminal> m_terminal; QMap<int, SerialWindow *> m_windows;
};

#endif // SERIALESCAPE_H

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
