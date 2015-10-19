// by: Kwabena W. Agyeman

#include "main.h"
#include "il.h"
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtSerialPort>
#include <QtNetwork>
#define ICON_PATH ":media/icons/omniacreator-icon/omniacreator.png"
#define SPLASH_PATH ":media/splash/omniacreator-splash/splash-small.png"

QIODevice *getSerialPort(QWidget *parent)
{
    QMap<QString, QString> map;

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        map.insert(info.manufacturer() + " - " +
        info.description() + " - " +
        info.portName(),
        info.portName());
    }

    if(!map.isEmpty())
    {
        QSettings settings; settings.beginGroup(MAINWINDOW_KEY_GROUP);

        QList<QString> list = map.keys(); int index =
        list.indexOf(settings.value(MAINWINDOW_LAST_PORT).toString());
        int selection = (index != -1) ? index : +0;

        bool ok; QString item = QInputDialog::getItem(parent,
        QApplication::applicationName(),
        QObject::tr("Please select a serial port:"),
        list, selection, false, &ok,
        Qt::MSWindowsFixedSizeDialogHint |
        Qt::WindowTitleHint |
        Qt::WindowSystemMenuHint |
        Qt::WindowCloseButtonHint);

        if(ok)
        {
            settings.setValue(MAINWINDOW_LAST_PORT, item);
            QSerialPort *port = new QSerialPort(map.value(item));

            if(port->setBaudRate(DEF_BAUD_RATE)
            && port->setDataBits(QSerialPort::Data8)
            && port->setFlowControl(QSerialPort::NoFlowControl)
            && port->setParity(QSerialPort::NoParity)
            && port->setStopBits(QSerialPort::OneStop)
            && port->setDataTerminalReady(true)
            && port->setRequestToSend(true)
            && port->open(QIODevice::ReadWrite)
            && port->setBreakEnabled(true))
            {
                QThread::msleep(100);

                if((!port->setBreakEnabled(false))
                || (!port->setRequestToSend(false))
                || (!port->setDataTerminalReady(false)))
                {
                    QString string = port->errorString(); delete port;
                    QMessageBox::critical(parent,
                    QApplication::applicationName(),
                    string); return NULL;
                }

                QThread::msleep(100);

                if(!port->clear())
                {
                    QString string = port->errorString(); delete port;
                    QMessageBox::critical(parent,
                    QApplication::applicationName(),
                    string); return NULL;
                }

                return port;
            }

            QString string = port->errorString(); delete port;
            QMessageBox::critical(parent,
            QApplication::applicationName(),
            string);
        }
    }
    else
    {
        QMessageBox::critical(parent,
        QApplication::applicationName(),
        QObject::tr("No serial ports were found!"));
    }

    return NULL;
}

QIODevice *getTCPPort(QWidget *parent)
{
    QSettings settings; settings.beginGroup(MAINWINDOW_KEY_GROUP);
    QString string = settings.value(MAINWINDOW_LAST_TCP_PORT,
    "xxx.xxx.xxx.xxx:xxxx").toString();

    bool ok; QUrl item = QUrl::fromUserInput(QInputDialog::getText(parent,
    QApplication::applicationName(),
    QObject::tr("Enter the TCP address and port (in URL format):"),
    QLineEdit::Normal, string, &ok,
    Qt::MSWindowsFixedSizeDialogHint |
    Qt::WindowTitleHint |
    Qt::WindowSystemMenuHint |
    Qt::WindowCloseButtonHint));

    if(ok)
    {
        settings.setValue(MAINWINDOW_LAST_TCP_PORT, item);
        QTcpSocket *port = new QTcpSocket();
        port->connectToHost(item.host(), item.port());
        return port;
    }

    return NULL;
}

QIODevice *getUDPPort(QWidget *parent)
{
    QSettings settings; settings.beginGroup(MAINWINDOW_KEY_GROUP);
    QString string = settings.value(MAINWINDOW_LAST_UDP_PORT,
    "xxx.xxx.xxx.xxx:xxxx").toString();

    bool ok; QUrl item = QUrl::fromUserInput(QInputDialog::getText(parent,
    QApplication::applicationName(),
    QObject::tr("Enter the UDP address and port (in URL format):"),
    QLineEdit::Normal, string, &ok,
    Qt::MSWindowsFixedSizeDialogHint |
    Qt::WindowTitleHint |
    Qt::WindowSystemMenuHint |
    Qt::WindowCloseButtonHint));

    if(ok)
    {
        settings.setValue(MAINWINDOW_LAST_UDP_PORT, item);
        QUdpSocket *port = new QUdpSocket();
        port->connectToHost(item.host(), item.port());
        return port;
    }

    return NULL;
}

QIODevice *openSerialPort(QWidget *parent)
{
    QMessageBox box(QMessageBox::Question,
    QApplication::applicationName(),
    QObject::tr("Which type of serial port would you like to open?"),
    QMessageBox::Close, parent,
    Qt::MSWindowsFixedSizeDialogHint |
    Qt::WindowTitleHint |
    Qt::WindowSystemMenuHint |
    Qt::WindowCloseButtonHint);

    QPushButton *button0 = box.addButton(QObject::tr("Serial Port"),
    QMessageBox::AcceptRole);
    QPushButton *button1 = box.addButton(QObject::tr("TCP Port"),
    QMessageBox::AcceptRole);
    QPushButton *button2 = box.addButton(QObject::tr("UDP Port"),
    QMessageBox::AcceptRole);

    box.setDefaultButton(button0);
    box.exec();

    if(box.clickedButton() == button0)
    {
        return getSerialPort(parent);
    }
    else if(box.clickedButton() == button1)
    {
        return getTCPPort(parent);
    }
    else if(box.clickedButton() == button2)
    {
        return getUDPPort(parent);
    }

    return NULL;
}

void exportCInterfaceLibrary(QWidget *parent)
{
    QString string = QFileDialog::getExistingDirectory(parent,
    QObject::tr("Choose a directory to save the library C++ files to:"),
    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

    if(!string.isEmpty())
    {
        QFile::copy(":build/InterfaceLibrary.h",
        QDir::fromNativeSeparators(QDir::cleanPath(string +
        QDir::separator() + "InterfaceLibrary.h")));
    }
}

void exportSPINInterfaceLibrary(QWidget *parent)
{
    QString string = QFileDialog::getExistingDirectory(parent,
    QObject::tr("Choose a directory to save the library SPIN files to:"),
    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

    if(!string.isEmpty())
    {
        QFile::copy(":interfacelibrary/ilcore.spin",
        QDir::fromNativeSeparators(QDir::cleanPath(string +
        QDir::separator() + "ilcore.spin")));

        QFile::copy(":interfacelibrary/ilfullduplexserialdriver.spin",
        QDir::fromNativeSeparators(QDir::cleanPath(string +
        QDir::separator() + "ilfullduplexserialdriver.spin")));

        QFile::copy(":interfacelibrary/iloscilloscope.spin",
        QDir::fromNativeSeparators(QDir::cleanPath(string +
        QDir::separator() + "iloscilloscope.spin")));

        QFile::copy(":interfacelibrary/iloscilloscopebar.spin",
        QDir::fromNativeSeparators(QDir::cleanPath(string +
        QDir::separator() + "iloscilloscopebar.spin")));

        QFile::copy(":interfacelibrary/iloscilloscopebox.spin",
        QDir::fromNativeSeparators(QDir::cleanPath(string +
        QDir::separator() + "iloscilloscopebox.spin")));

        QFile::copy(":interfacelibrary/iloscilloscopecurve.spin",
        QDir::fromNativeSeparators(QDir::cleanPath(string +
        QDir::separator() + "iloscilloscopecurve.spin")));

        QFile::copy(":interfacelibrary/iloscilloscopegraph.spin",
        QDir::fromNativeSeparators(QDir::cleanPath(string +
        QDir::separator() + "iloscilloscopegraph.spin")));

        QFile::copy(":interfacelibrary/ilterminal.spin",
        QDir::fromNativeSeparators(QDir::cleanPath(string +
        QDir::separator() + "ilterminal.spin")));
    }
}

void exportInterfaceLibrary(QWidget *parent)
{
    QMessageBox box(QMessageBox::Question,
    QApplication::applicationName(),
    QObject::tr("Which library version would you like to export?"),
    QMessageBox::Close, parent,
    Qt::MSWindowsFixedSizeDialogHint |
    Qt::WindowTitleHint |
    Qt::WindowSystemMenuHint |
    Qt::WindowCloseButtonHint);

    QPushButton *button0 = box.addButton(QObject::tr("C++ Interface Library"),
    QMessageBox::AcceptRole);
    QPushButton *button1 = box.addButton(QObject::tr("SPIN Interface Library"),
    QMessageBox::AcceptRole);

    box.setDefaultButton(button0);
    box.exec();

    if(box.clickedButton() == button0)
    {
        exportCInterfaceLibrary(parent);
    }
    else if(box.clickedButton() == button1)
    {
        exportSPINInterfaceLibrary(parent);
    }
}

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    QApplication::setOrganizationName
    (QStringLiteral(PROJECT_VENDOR_STR).replace('_', ' '));
    QApplication::setOrganizationDomain
    (QStringLiteral(PROJECT_DOMAIN_NAME_STR).replace('_', ' '));
    QApplication::setApplicationName
    (QStringLiteral(PROJECT_FULL_NAME_STR).replace('_', ' '));
    QApplication::setApplicationVersion
    (QStringLiteral(PROJECT_VERSION_STR).replace('_', ' '));

    QApplication::setWindowIcon(QIcon(QStringLiteral(ICON_PATH)));
    QSplashScreen splash(QPixmap(QStringLiteral(SPLASH_PATH)));
    splash.show();

    QMessageBox box(QMessageBox::Question,
    QApplication::applicationName(),
    QObject::tr("What do you want to do?"),
    QMessageBox::Close, &splash,
    Qt::MSWindowsFixedSizeDialogHint |
    Qt::WindowTitleHint |
    Qt::WindowSystemMenuHint |
    Qt::WindowCloseButtonHint);

    QPushButton *button0=box.addButton(QObject::tr("Open Serial Port"),
    QMessageBox::AcceptRole);
    QPushButton *button1=box.addButton(QObject::tr("Export Interface Library"),
    QMessageBox::AcceptRole);

    box.setDefaultButton(button0);
    box.exec();

    if(box.clickedButton() == button0)
    {
        QIODevice *device = openSerialPort(&splash);

        if(device)
        {
            QMainWindow window; window.show();
            splash.finish(&window);
            return application.exec();
        }
    }
    else if(box.clickedButton() == button1)
    {
        exportInterfaceLibrary(&splash);
    }

    return EXIT_SUCCESS;
}
