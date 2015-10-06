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

#include "serialterminal.h"
#include "ui_serialterminal.h"

SerialTerminal::SerialTerminal(const QString &title,
QSettings *settings, QWidget *parent) :
SerialWindow(title, settings, parent), m_ui(new Ui::SerialTerminal)
{
    m_ui->setupUi(this); m_ui->statusBar->hide();

    m_ui->rx->setMaximumBlockCount(TERMINAL_MAX_LINE_SIZE);

    m_textCursor = m_ui->rx->textCursor(); m_brushColor = Qt::black;

    m_stateMachine = ASCII; m_shiftReg = QByteArray(); getTabWidth();

    connect(m_ui->tx, SIGNAL(returnPressed()),
            this, SLOT(returnPressed()));

    connect(m_ui->actionClear_Screen, SIGNAL(triggered()),
            this, SLOT(clearScreen()));

    connect(m_ui->actionSend_Text_File, SIGNAL(triggered()),
            this, SLOT(sendTextFile()));

    connect(m_ui->actionSave_Text_File, SIGNAL(triggered()),
            this, SLOT(saveTextFile()));

    connect(m_ui->actionClose_Window, SIGNAL(triggered()),
            this, SLOT(close()));

    connect(m_ui->actionGeneral_Help, SIGNAL(triggered()),
            this, SLOT(generalHelp()));

    connect(m_ui->actionTerminal_Help, SIGNAL(triggered()),
            this, SLOT(terminalHelp()));

    restoreState();
}

SerialTerminal::~SerialTerminal()
{
    delete m_ui;
}

bool SerialTerminal::importExportEnabled() const
{
    return false;
}

QString SerialTerminal::keyGroup() const
{
    return SERIAL_TERMINAL_KEY_GROUP;
}

QString SerialTerminal::keyState() const
{
    return SERIAL_TERMINAL_KEY_STATE;
}

QString SerialTerminal::keyGeometry() const
{
    return SERIAL_TERMINAL_KEY_GEOMETRY;
}

void SerialTerminal::setTabWidth(int tabWidth)
{
    QSettings settings(settingsFileName(), settingsFormat());

    settings.beginGroup(keyGroup());
    settings.beginGroup(windowTitle());

    settings.setValue(SERIAL_TERMINAL_KEY_TAB_WIDTH, m_tabWidth = tabWidth);
}

int SerialTerminal::getTabWidth()
{
    QSettings settings(settingsFileName(), settingsFormat());

    settings.beginGroup(keyGroup());
    settings.beginGroup(windowTitle());

    return m_tabWidth =
    settings.value(SERIAL_TERMINAL_KEY_TAB_WIDTH, TERMINAL_TAB_WIDTH).toInt();
}

void SerialTerminal::receive(const QByteArray &bytes)
{
    QByteArray buffer; int column = m_textCursor.columnNumber();

    for(int i = 0, j = bytes.size(); i < j; i++)
    {
        if((m_stateMachine == UTF8) && ((bytes.at(i) & 0xC0) != 0x80))
        {
            emit errorMessage(QString(metaObject()->className()) +
            "::" + __FUNCTION__ + "[" + windowTitle() + "] -> " +
            tr("Received invalid UTF8 character 0x%L1").
            arg(QString(m_shiftReg.toHex())));

            m_stateMachine = ASCII;
        }

        if((m_stateMachine == EXIT_0) && ((bytes.at(i) & 0xFF) != 0x00))
        {
            emit errorMessage(QString(metaObject()->className()) +
            "::" + __FUNCTION__ + "[" + windowTitle() + "] -> " +
            tr("Received invalid exit sequence 0x%L1").
            arg(QString(m_shiftReg.toHex())));

            m_stateMachine = ASCII;
        }

        switch(m_stateMachine)
        {
            case ASCII:
            {
                if(((bytes.at(i) & 0xE0) == 0xC0)
                || ((bytes.at(i) & 0xF0) == 0xE0)
                || ((bytes.at(i) & 0xF8) == 0xF0)
                || ((bytes.at(i) & 0xFC) == 0xF8)
                || ((bytes.at(i) & 0xFE) == 0xFC)) // UTF8
                {
                    m_stateMachine = UTF8;

                    m_shiftReg.clear();
                }
                else if(!(bytes.at(i) & 128)) // ASCII
                {
                    switch(bytes.at(i))
                    {
                        case CC_NULL: break;

                        case CC_HOME_CURSOR:
                        {
                            insertText(buffer); buffer.clear();

                            m_textCursor.movePosition(QTextCursor::Start);
                            m_ui->rx->setTextCursor(m_textCursor);

                            column = m_textCursor.columnNumber(); break;
                        }

                        case CC_END_CURSOR:
                        {
                            insertText(buffer); buffer.clear();

                            m_textCursor.movePosition(QTextCursor::End);
                            m_ui->rx->setTextCursor(m_textCursor);

                            column = m_textCursor.columnNumber(); break;
                        }

                        case CC_MOVE_CURSOR_LEFT:
                        {
                            insertText(buffer); buffer.clear();

                            m_textCursor.movePosition(QTextCursor::Left);
                            m_ui->rx->setTextCursor(m_textCursor);

                            column = m_textCursor.columnNumber(); break;
                        }

                        case CC_MOVE_CURSOR_RIGHT:
                        {
                            insertText(buffer); buffer.clear();

                            m_textCursor.movePosition(QTextCursor::Right);
                            m_ui->rx->setTextCursor(m_textCursor);

                            column = m_textCursor.columnNumber(); break;
                        }

                        case CC_MOVE_CURSOR_UP:
                        {
                            insertText(buffer); buffer.clear();

                            m_textCursor.movePosition(QTextCursor::Up);
                            m_ui->rx->setTextCursor(m_textCursor);

                            column = m_textCursor.columnNumber(); break;
                        }

                        case CC_MOVE_CURSOR_DOWN:
                        {
                            insertText(buffer); buffer.clear();

                            m_textCursor.movePosition(QTextCursor::Down);
                            m_ui->rx->setTextCursor(m_textCursor);

                            column = m_textCursor.columnNumber(); break;
                        }

                        case CC_BEEP_SPEAKER:
                        {
                            insertText(buffer); buffer.clear();

                            QApplication::beep(); break;
                        }

                        case CC_BACKSPACE:
                        {
                            insertText(buffer); buffer.clear();

                            m_textCursor.deletePreviousChar();
                            m_ui->rx->setTextCursor(m_textCursor);

                            column = m_textCursor.columnNumber(); break;
                        }

                        case CC_TAB:
                        {
                            for(int k=m_tabWidth-(column%m_tabWidth); k>0; k--)
                            {
                                buffer.append(' '); column += 1;
                            }

                            break;
                        }

                        case CC_LINE_FEED:
                        {
                            if(m_shiftReg.isEmpty()
                            || (!m_shiftReg.endsWith('\r')))
                            {
                                buffer.append('\n'); column = 0;
                            }

                            break;
                        }

                        case CC_CLEAR_TO_END_OF_LINE:
                        {
                            insertText(buffer); buffer.clear();

                            m_textCursor.movePosition(
                            QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
                            m_textCursor.removeSelectedText();
                            m_ui->rx->setTextCursor(m_textCursor);

                            column = m_textCursor.columnNumber(); break;
                        }

                        case CC_CLEAR_LINES_BELOW:
                        {
                            insertText(buffer); buffer.clear();

                            m_textCursor.movePosition(
                            QTextCursor::End, QTextCursor::KeepAnchor);
                            m_textCursor.removeSelectedText();
                            m_ui->rx->setTextCursor(m_textCursor);

                            column = m_textCursor.columnNumber(); break;
                        }

                        case CC_CARRIAGE_RETURN:
                        {
                            buffer.append('\n'); column = 0; break;
                        }

                        case CC_CLEAR_TO_START_OF_LINE:
                        {
                            insertText(buffer); buffer.clear();

                            m_textCursor.movePosition(
                            QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
                            m_textCursor.removeSelectedText();
                            m_ui->rx->setTextCursor(m_textCursor);

                            column = m_textCursor.columnNumber(); break;
                        }

                        case CC_CLEAR_LINES_ABOVE:
                        {
                            insertText(buffer); buffer.clear();

                            m_textCursor.movePosition(
                            QTextCursor::Start, QTextCursor::KeepAnchor);
                            m_textCursor.removeSelectedText();
                            m_ui->rx->setTextCursor(m_textCursor);

                            column = m_textCursor.columnNumber(); break;
                        }

                        case CC_CLEAR_SCREEN:
                        {
                            insertText(buffer); buffer.clear();

                            m_ui->rx->clear();
                            m_textCursor = m_ui->rx->textCursor();

                            column = m_textCursor.columnNumber(); break;
                        }

                        case CC_XON: break;

                        case CC_SET_FOREGROUND_TO_GRAY:
                        {
                            insertText(buffer); buffer.clear();

                            m_brushColor = Qt::gray; break;
                        }

                        case CC_XOFF: break;

                        case CC_SET_FOREGROUND_TO_BLACK:
                        {
                            insertText(buffer); buffer.clear();

                            m_brushColor = Qt::black; break;
                        }

                        case CC_SET_FOREGROUND_TO_BLUE:
                        {
                            insertText(buffer); buffer.clear();

                            m_brushColor = Qt::blue; break;
                        }

                        case CC_SET_FOREGROUND_TO_RED:
                        {
                            insertText(buffer); buffer.clear();

                            m_brushColor = Qt::red; break;
                        }

                        case CC_SET_FOREGROUND_TO_GREEN:
                        {
                            insertText(buffer); buffer.clear();

                            m_brushColor = Qt::green; break;
                        }

                        case CC_SET_FOREGROUND_TO_MAGENTA:
                        {
                            insertText(buffer); buffer.clear();

                            m_brushColor = Qt::magenta; break;
                        }

                        case CC_SET_FOREGROUND_TO_CYAN:
                        {
                            insertText(buffer); buffer.clear();

                            m_brushColor = Qt::cyan; break;
                        }

                        case CC_SET_FOREGROUND_TO_YELLOW:
                        {
                            insertText(buffer); buffer.clear();

                            m_brushColor = Qt::yellow; break;
                        }

                        case CC_ESCAPE: break;

                        case CC_MOVE_TO_START_OF_LINE:
                        {
                            insertText(buffer); buffer.clear();

                            m_textCursor.movePosition(
                            QTextCursor::StartOfLine);
                            m_ui->rx->setTextCursor(m_textCursor);

                            column = m_textCursor.columnNumber(); break;
                        }

                        case CC_MOVE_TO_END_OF_LINE:
                        {
                            insertText(buffer); buffer.clear();

                            m_textCursor.movePosition(
                            QTextCursor::EndOfLine);
                            m_ui->rx->setTextCursor(m_textCursor);

                            column = m_textCursor.columnNumber(); break;
                        }

                        case CC_MOVE_TO_PREVIOUS_START_OF_LINE:
                        {
                            insertText(buffer); buffer.clear();

                            m_textCursor.movePosition(
                            QTextCursor::PreviousBlock);
                            m_ui->rx->setTextCursor(m_textCursor);

                            column = m_textCursor.columnNumber(); break;
                        }

                        case CC_MOVE_TO_NEXT_START_OF_LINE:
                        {
                            insertText(buffer); buffer.clear();

                            m_textCursor.movePosition(
                            QTextCursor::NextBlock);
                            m_ui->rx->setTextCursor(m_textCursor);

                            column = m_textCursor.columnNumber(); break;
                        }

                        case CC_DELETE:
                        {
                            insertText(buffer); buffer.clear();

                            m_textCursor.deleteChar();
                            m_ui->rx->setTextCursor(m_textCursor);

                            column = m_textCursor.columnNumber(); break;
                        }

                        default:
                        {
                            buffer.append(bytes.at(i)); column += 1; break;
                        }
                    }
                }
                else if((bytes.at(i) & 0xFF) == 0xFF)
                {
                    m_stateMachine = EXIT_0;

                    m_shiftReg.clear();
                }
                else
                {
                    QByteArray buff = QByteArray().append(bytes.at(i));

                    emit errorMessage(QString(metaObject()->className()) +
                    "::" + __FUNCTION__ + "[" + windowTitle() + "] -> " +
                     tr("Received invalid ASCII character 0x%L1").
                    arg(QString(buff.toHex())));
                }

                break;
            }

            case UTF8:
            {
                if((((m_shiftReg.at(0)&0xE0)==0xC0)&&(m_shiftReg.size()>=1))
                || (((m_shiftReg.at(0)&0xF0)==0xE0)&&(m_shiftReg.size()>=2))
                || (((m_shiftReg.at(0)&0xF8)==0xF0)&&(m_shiftReg.size()>=3))
                || (((m_shiftReg.at(0)&0xFC)==0xF8)&&(m_shiftReg.size()>=4))
                || (((m_shiftReg.at(0)&0xFE)==0xFC)&&(m_shiftReg.size()>=5)))
                {
                    buffer.append(m_shiftReg + bytes.at(i)); column += 1;

                    m_stateMachine = ASCII;
                }

                break;
            }

            case EXIT_0:
            {
                m_stateMachine = EXIT_1;

                break;
            }

            case EXIT_1:
            {
                if(bytes.at(i))
                {
                    QByteArray buff = QByteArray().append(bytes.at(i));

                    emit errorMessage(QString(metaObject()->className()) +
                    "::" + __FUNCTION__ + "[" + windowTitle() + "] -> " +
                    tr("Received exit failure status 0x%L1").
                    arg(QString(buff.toHex())));
                }

                break;
            }
        }

        if(m_shiftReg.size() >= TERMINAL_SHIFT_REG)
        {
            for(int k = 1; k <= TERMINAL_SHIFT_REG; k++)
            {
                m_shiftReg[k - 1] =
                (k == TERMINAL_SHIFT_REG) ? bytes.at(i) : m_shiftReg.at(k);
            }
        }
        else
        {
            m_shiftReg.append(bytes.at(i));
        }
    }

    insertText(buffer); buffer.clear();
}

void SerialTerminal::userReset()
{
    clearScreen(); m_brushColor = Qt::black;
    m_stateMachine = ASCII; m_shiftReg = QByteArray();
}

void SerialTerminal::returnPressed()
{
    QString text = m_ui->tx->text() + "\r\n";
    emit transmit(text.toUtf8(), this);

    QBrush temp = m_brushColor; m_brushColor = Qt::black; insertText(text);
    m_brushColor = temp; m_ui->tx->clear();
}

void SerialTerminal::clearScreen()
{
    m_ui->rx->clear();
    m_textCursor = m_ui->rx->textCursor();
}

void SerialTerminal::sendTextFile()
{
    QSettings settings(settingsFileName(), settingsFormat());

    settings.beginGroup(keyGroup());
    settings.beginGroup(windowTitle());

    QString openFile = settings.value(SERIAL_TERMINAL_KEY_OPEN_FILE,
                                      QDir::homePath()).toString();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Send Text File"),
    openFile, tr("Text Files (*.txt);;All Files (*)"));

    if(!fileName.isEmpty())
    {
        QFileInfo fileInfo(fileName);

        if(fileInfo.size() <= TERMINAL_MAX_FILE_SIZE)
        {
            QFile file(fileName);

            if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QByteArray data = file.readAll();

                data.replace(CC_XON, "");
                data.replace(CC_XOFF, "");
                data.replace(CC_ESCAPE, "");

                emit transmit(data, this);

                settings.setValue(SERIAL_TERMINAL_KEY_OPEN_FILE,
                fileInfo.canonicalFilePath());
            }
            else
            {
                QMessageBox::critical(this, tr("Send Text File Error"),
                file.errorString());
            }
        }
        else
        {
            QString title = tr("Send Text File Error"); QString text;

            if(TERMINAL_MAX_FILE_SIZE >= ONE_GIGABYTE)
            {
                text = tr("The selected file size (%L1 GB(s)) is greater "
                "than the maximum supported file size of %L2 GB(s)").
                arg(double(fileInfo.size()) / ONE_GIGABYTE).
                arg(double(TERMINAL_MAX_FILE_SIZE) / ONE_GIGABYTE);
            }
            else if(TERMINAL_MAX_FILE_SIZE >= ONE_MEGABYTE)
            {
                text = tr("The selected file size (%L1 MB(s)) is greater "
                "than the maximum supported file size of %L2 MB(s)").
                arg(double(fileInfo.size()) / ONE_MEGABYTE).
                arg(double(TERMINAL_MAX_FILE_SIZE) / ONE_MEGABYTE);
            }
            else if(TERMINAL_MAX_FILE_SIZE >= ONE_KILOBYTE)
            {
                text = tr("The selected file size (%L1 KB(s)) is greater "
                "than the maximum supported file size of %L2 KB(s)").
                arg(double(fileInfo.size()) / ONE_KILOBYTE).
                arg(double(TERMINAL_MAX_FILE_SIZE) / ONE_KILOBYTE);
            }
            else // if(TERMINAL_MAX_FILE_SIZE >= ONE_BYTE)
            {
                text = tr("The selected file size (%L1 byte(s)) is greater "
                "than the maximum supported file size of %L2 byte(s)").
                arg(double(fileInfo.size()) / ONE_BYTE).
                arg(double(TERMINAL_MAX_FILE_SIZE) / ONE_BYTE);
            }

            QMessageBox::critical(this, title, text);
        }
    }
}

void SerialTerminal::saveTextFile()
{
    QSettings settings(settingsFileName(), settingsFormat());

    settings.beginGroup(keyGroup());
    settings.beginGroup(windowTitle());

    QString saveFile = settings.value(SERIAL_TERMINAL_KEY_SAVE_FILE,
                                      QDir::homePath()).toString();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Text File"),
    saveFile, tr("Text Files (*.txt);;All Files (*)"));

    if(!fileName.isEmpty())
    {
        QFile file(fileName);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QByteArray utf8 = m_ui->rx->toPlainText().toUtf8();

            if(file.write(utf8) == utf8.size())
            {
                QFileInfo fileInfo(fileName);

                settings.setValue(SERIAL_TERMINAL_KEY_SAVE_FILE,
                fileInfo.canonicalFilePath());
            }
            else
            {
                QMessageBox::critical(this, tr("Save Text File Error"),
                file.errorString());
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Save Text File Error"),
            file.errorString());
        }
    }
}

void SerialTerminal::generalHelp()
{
    if(!QDesktopServices::openUrl(QUrl("http://" PROJECT_DOMAIN_NAME_STR "/"
    "help/general/")))
    {
        QMessageBox::critical(this, tr("Open General Help Error"),
        tr("Unable to open the URL to the General Help page"));
    }
}

void SerialTerminal::terminalHelp()
{
    if(!QDesktopServices::openUrl(QUrl("http://" PROJECT_DOMAIN_NAME_STR "/"
    "help/widgets/terminal/")))
    {
        QMessageBox::critical(this, tr("Open Terminal Help Error"),
        tr("Unable to open the URL to the Terminal Help page"));
    }
}

void SerialTerminal::insertText(const QString &text)
{
    if(!text.isEmpty())
    {
        QTextCharFormat format = m_textCursor.charFormat();
        format.setForeground(m_brushColor);
        m_textCursor.setCharFormat(format);

        m_textCursor.insertText(text);
        m_ui->rx->setTextCursor(m_textCursor);
    }
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
