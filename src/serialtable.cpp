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

#include "serialtable.h"
#include "ui_serialtable.h"

SerialTable::SerialTable(const QString &title,
QSettings *settings, QWidget *parent) :
SerialWindow(title, settings, parent), m_ui(new Ui::SerialTable)
{
    m_ui->setupUi(this); m_ui->statusBar->hide();

    m_findString = QString();

    m_ui->tableWidget->horizontalHeader()->setSectionsMovable(false);
    m_ui->tableWidget->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeToContents);

    m_ui->tableWidget->verticalHeader()->setSectionsMovable(false);
    m_ui->tableWidget->verticalHeader()->setSectionResizeMode(
    QHeaderView::ResizeToContents);

    connect(m_ui->actionFind_Item, SIGNAL(triggered()),
            this, SLOT(findItem()));

    connect(m_ui->actionOpen_Table, SIGNAL(triggered()),
            this, SLOT(importState()));

    connect(m_ui->actionSave_Table, SIGNAL(triggered()),
            this, SLOT(exportState()));

    connect(m_ui->actionExport_CSV, SIGNAL(triggered()),
            this, SLOT(exportToCSV()));

    connect(m_ui->actionClose_Window, SIGNAL(triggered()),
            this, SLOT(close()));

    connect(m_ui->actionGeneral_Help, SIGNAL(triggered()),
            this, SLOT(generalHelp()));

    connect(m_ui->actionTable_Help, SIGNAL(triggered()),
            this, SLOT(tableHelp()));

    restoreState();
}

SerialTable::~SerialTable()
{
    delete m_ui;
}

bool SerialTable::importExportEnabled() const
{
    return true;
}

QString SerialTable::keyGroup() const
{
    return SERIAL_TABLE_KEY_GROUP;
}

QString SerialTable::keyState() const
{
    return SERIAL_TABLE_KEY_STATE;
}

QString SerialTable::keyGeometry() const
{
    return SERIAL_TABLE_KEY_GEOMETRY;
}

bool SerialTable::newRow(int row, const QString &name)
{
    bool ok = rowHeaderDoesNotExist(__FUNCTION__, row);

    if(ok)
    {
        QTableWidgetItem *item = new QTableWidgetItem(name);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QMap<int, QTableWidgetItem *>::iterator
        i = m_rowHeaders.insert(row, item), j = m_rowHeaders.begin();
        int index = std::distance(j, i);

        m_ui->tableWidget->insertRow(index);
        m_ui->tableWidget->setVerticalHeaderItem(index, item);
        item->setData(Qt::UserRole, index);
    }
    else // Silently handle error...
    {
        return setRowName(row, name);
    }

    return ok;
}

bool SerialTable::setRowName(int row, const QString &name)
{
    QTableWidgetItem *item = rowHeaderExists(__FUNCTION__, row);

    if(item)
    {
        item->setText(name);
    }

    return item;
}

QString SerialTable::getRowName(int row, bool *ok)
{
    QTableWidgetItem *item = rowHeaderExists(__FUNCTION__, row);

    if((*ok = item))
    {
        return item->text();
    }

    return QString();
}

bool SerialTable::deleteRow(int row)
{
    QTableWidgetItem *item = rowHeaderExists(__FUNCTION__, row);

    if(item)
    {
        m_ui->tableWidget->removeRow(item->data(Qt::UserRole).toInt());
        m_rowHeaders.remove(row);
    }

    return item;
}

void SerialTable::deleteRows()
{
    foreach(int row, m_rowHeaders.keys())
    {
        deleteRow(row);
    }
}

bool SerialTable::newColumn(int column, const QString &name)
{
    bool ok = columnHeaderDoesNotExist(__FUNCTION__, column);

    if(ok)
    {
        QTableWidgetItem *item = new QTableWidgetItem(name);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QMap<int, QTableWidgetItem *>::iterator
        i = m_columnHeaders.insert(column, item), j = m_columnHeaders.begin();
        int index = std::distance(j, i);

        m_ui->tableWidget->insertColumn(index);
        m_ui->tableWidget->setHorizontalHeaderItem(index, item);
        item->setData(Qt::UserRole, index);
    }
    else // Silently handle error...
    {
        return setColumnName(column, name);
    }

    return ok;
}

bool SerialTable::setColumnName(int column, const QString &name)
{
    QTableWidgetItem *item = columnHeaderExists(__FUNCTION__, column);

    if(item)
    {
        item->setText(name);
    }

    return item;
}

QString SerialTable::getColumnName(int column, bool *ok)
{
    QTableWidgetItem *item = columnHeaderExists(__FUNCTION__, column);

    if((*ok = item))
    {
        return item->text();
    }

    return QString();
}

bool SerialTable::deleteColumn(int column)
{
    QTableWidgetItem *item = columnHeaderExists(__FUNCTION__, column);

    if(item)
    {
        m_ui->tableWidget->removeColumn(item->data(Qt::UserRole).toInt());
        m_columnHeaders.remove(column);
    }

    return item;
}

void SerialTable::deleteColumns()
{
    foreach(int column, m_columnHeaders.keys())
    {
        deleteColumn(column);
    }
}

bool SerialTable::newItem(int row, int column, const QString &text)
{
    int r, c; bool ok = itemDoesNotExist(__FUNCTION__, row, column, &r, &c);

    if(ok)
    {
        QTableWidgetItem *item = new QTableWidgetItem(text);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        m_ui->tableWidget->setItem(r, c, item);
    }
    else // Silently handle error...
    {
        return setItemText(row, column, text);
    }

    return ok;
}

bool SerialTable::setItemText(int row, int column, const QString &text)
{
    QTableWidgetItem *item = itemExists(__FUNCTION__, row, column);

    if(item)
    {
        item->setText(text);
    }

    return item;
}

QString SerialTable::getItemText(int row, int column, bool *ok)
{
    QTableWidgetItem *item = itemExists(__FUNCTION__, row, column);

    if((*ok = item))
    {
        return item->text();
    }

    return QString();
}

bool SerialTable::deleteItem(int row, int column)
{
    QTableWidgetItem *item = itemExists(__FUNCTION__, row, column);

    if(item)
    {
        delete item;
    }

    return item;
}

void SerialTable::deleteItems()
{
    m_ui->tableWidget->clearContents();
}

bool SerialTable::deleteItemsInRow(int row)
{
    QTableWidgetItem *item = rowHeaderExists(__FUNCTION__, row);

    if(item)
    {
        foreach(int column, m_columnHeaders.keys())
        {
            deleteItem(row, column);
        }
    }

    return item;
}

bool SerialTable::deleteItemsInColumn(int column)
{
    QTableWidgetItem *item = columnHeaderExists(__FUNCTION__, column);

    if(item)
    {
        foreach(int row, m_rowHeaders.keys())
        {
            deleteItem(row, column);
        }
    }

    return item;
}

void SerialTable::clearAll()
{
    deleteItems(); deleteColumns(); deleteRows();
}

void SerialTable::findItem()
{
    bool ok;

    QString findString = QInputDialog::getText(this, tr("Find Item"),
    tr("Item text"), QLineEdit::Normal, m_findString, &ok,
    Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint |
    Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);

    if(ok)
    {
        QList<QTableWidgetItem *> list = m_ui->tableWidget->findItems(
        findString, Qt::MatchContains | Qt::MatchFixedString |
        Qt::MatchCaseSensitive | Qt::MatchWrap | Qt::MatchRecursive);

        if(!list.isEmpty())
        {
            int item = list.indexOf(m_ui->tableWidget->currentItem());

            if(item != -1)
            {
                item = (item + 1) % list.size();

                m_ui->tableWidget->scrollToItem(list.at(item));
                m_ui->tableWidget->setCurrentItem(list.at(item));
            }
            else
            {
                m_ui->tableWidget->scrollToItem(list.first());
                m_ui->tableWidget->setCurrentItem(list.first());
            }

            m_findString = findString;
        }
        else
        {
            QMessageBox::information(this, tr("Find Item"),
            tr("Item \"%L1\" not found").arg(findString));
        }
    }
}

void SerialTable::importState(const QString &fileName)
{
    QSettings settings(settingsFileName(), settingsFormat());

    settings.beginGroup(keyGroup());
    settings.beginGroup(windowTitle());

    QString openFile = settings.value(SERIAL_TABLE_KEY_OPEN_FILE,
                                      QDir::homePath()).toString();

    QString temp = fileName.isEmpty() ? QFileDialog::getOpenFileName(this,
    tr("Open Table"), openFile, tr("JSON Files (*.json)")) : fileName;

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

                if(object.value("type").toString() == "table")
                {
                    clearAll();

                    setWindowTitle(object.value("title").toString());

                    // Read row headers
                    {
                        QJsonArray rowHeaders =
                        object.value("rowHeaders").toArray();

                        for(int i = 0, j = rowHeaders.size(); i < j; i++)
                        {
                            QJsonObject rowHeader =
                            rowHeaders.at(i).toObject();

                            newRow(
                            rowHeader.value("rowNumber").toDouble(),
                            rowHeader.value("rowName").toString());
                        }
                    }

                    // Read column headers
                    {
                        QJsonArray columnHeaders =
                        object.value("columnHeaders").toArray();

                        for(int i = 0, j = columnHeaders.size(); i < j; i++)
                        {
                            QJsonObject columnHeader =
                            columnHeaders.at(i).toObject();

                            newColumn(
                            columnHeader.value("columnNumber").toDouble(),
                            columnHeader.value("columnName").toString());
                        }
                    }

                    QJsonArray rowArray = object.value("table").toArray();

                    for(int i = 0, j = rowArray.size(); i < j; i++)
                    {
                        QJsonArray columnArray = rowArray.at(i).toArray();

                        for(int k = 0, l = columnArray.size(); k < l; k++)
                        {
                            QJsonObject item = columnArray.at(k).toObject();

                            newItem(item.value("row").toDouble(),
                                    item.value("column").toDouble(),
                                    item.value("text").toString());
                        }
                    }

                    QFileInfo fileInfo(temp);

                    settings.setValue(SERIAL_TABLE_KEY_OPEN_FILE,
                    fileInfo.canonicalFilePath());
                }
                else
                {
                    QMessageBox::critical(this, tr("Open Table Error"),
                    tr("Incompatible JSON file"));
                }
            }
            else
            {
                QMessageBox::critical(this, tr("Open Table Error"),
                parseError.errorString());
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Open Table Error"),
            file.errorString());
        }
    }
}

void SerialTable::exportState(const QString &fileName)
{
    QSettings settings(settingsFileName(), settingsFormat());

    settings.beginGroup(keyGroup());
    settings.beginGroup(windowTitle());

    QString saveFile = settings.value(SERIAL_TABLE_KEY_SAVE_FILE,
                                      QDir::homePath()).toString();

    QString temp = fileName.isEmpty() ? QFileDialog::getSaveFileName(this,
    tr("Save Table"), saveFile, tr("JSON Files (*.json)")) : fileName;

    if(!temp.isEmpty())
    {
        QFile file(temp);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QJsonObject object;

            object.insert("type", QString("table"));
            object.insert("title", windowTitle());

            // Write row headers
            {
                QJsonArray rowHeaders;

                foreach(int row, m_rowHeaders.keys())
                {
                    QJsonObject rowHeader;

                    rowHeader.insert("rowNumber", row);
                    rowHeader.insert("rowName",
                    m_rowHeaders.value(row)->text());

                    rowHeaders.append(rowHeader);
                }

                object.insert("rowHeaders", rowHeaders);
            }

            // Write column headers
            {
                QJsonArray columnHeaders;

                foreach(int column, m_columnHeaders.keys())
                {
                    QJsonObject columnHeader;

                    columnHeader.insert("columnNumber", column);
                    columnHeader.insert("columnName",
                    m_columnHeaders.value(column)->text());

                    columnHeaders.append(columnHeader);
                }

                object.insert("columnHeaders", columnHeaders);
            }

            QJsonArray rowArray;

            foreach(int row, m_rowHeaders.keys())
            {
                QJsonArray columnArray;

                foreach(int column, m_columnHeaders.keys())
                {
                    int r =
                    m_rowHeaders.value(row)->data(Qt::UserRole).toInt();

                    int c =
                    m_columnHeaders.value(column)->data(Qt::UserRole).toInt();

                    QTableWidgetItem *item = m_ui->tableWidget->item(r, c);

                    if(item)
                    {
                        QJsonObject temp;

                        temp.insert("row", row);
                        temp.insert("column", column);
                        temp.insert("text", item->text());

                        columnArray.append(temp);
                    }
                }

                rowArray.append(columnArray);
            }

            object.insert("table", rowArray);

            QByteArray json = QJsonDocument(object).toJson();

            if(file.write(json) == json.size())
            {
                QFileInfo fileInfo(temp);

                settings.setValue(SERIAL_TABLE_KEY_SAVE_FILE,
                fileInfo.canonicalFilePath());
            }
            else
            {
                QMessageBox::critical(this, tr("Save Table Error"),
                file.errorString());
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Save Table Error"),
            file.errorString());
        }
    }
}

void SerialTable::exportToCSV(const QString &fileName)
{
    QSettings settings(settingsFileName(), settingsFormat());

    settings.beginGroup(keyGroup());
    settings.beginGroup(windowTitle());

    QString saveFile = settings.value(SERIAL_TABLE_KEY_SAVE_FILE_CSV,
                                      QDir::homePath()).toString();

    QString temp = fileName.isEmpty() ? QFileDialog::getSaveFileName(this,
    tr("Export Table"), saveFile, tr("CSV Files (*.csv)")) : fileName;

    if(!temp.isEmpty())
    {
        QByteArray data;

        switch(QMessageBox::question(this, tr("Export Table"),
        tr("Include Column Header Line?"), QMessageBox::Yes |
        QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes))
        {
            case QMessageBox::Yes:
            {
                QByteArray line;

                foreach(int column, m_columnHeaders.keys())
                {
                    line.append("\"" + m_columnHeaders.value(column)->text() +
                                "\",");
                }

                line.chop(sizeof(','));
                data.append(line + '\n');

                break;
            }

            case QMessageBox::No:
            {
                break;
            }

            default: return;
        }

        QFile file(temp);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QByteArray rData;

            foreach(int row, m_rowHeaders.keys())
            {
                QByteArray cData;

                foreach(int column, m_columnHeaders.keys())
                {
                    int r =
                    m_rowHeaders.value(row)->data(Qt::UserRole).toInt();

                    int c =
                    m_columnHeaders.value(column)->data(Qt::UserRole).toInt();

                    QTableWidgetItem *item = m_ui->tableWidget->item(r, c);

                    cData.append("\"" + (item?item->text():QString()) + "\",");
                }

                cData.chop(sizeof(','));
                rData.append(cData + '\n');
            }

            data.append(rData);

            if(file.write(data) == data.size())
            {
                QFileInfo fileInfo(temp);

                settings.setValue(SERIAL_TABLE_KEY_SAVE_FILE_CSV,
                fileInfo.canonicalFilePath());
            }
            else
            {
                QMessageBox::critical(this, tr("Export Table Error"),
                file.errorString());
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Export Table Error"),
            file.errorString());
        }
    }
}

void SerialTable::generalHelp()
{
    if(!QDesktopServices::openUrl(QUrl("http://" PROJECT_DOMAIN_NAME_STR "/"
    "help/general/")))
    {
        QMessageBox::critical(this, tr("Open General Help Error"),
        tr("Unable to open the URL to the General Help page"));
    }
}

void SerialTable::tableHelp()
{
    if(!QDesktopServices::openUrl(QUrl("http://" PROJECT_DOMAIN_NAME_STR "/"
    "help/widgets/table/")))
    {
        QMessageBox::critical(this, tr("Open Table Help Error"),
        tr("Unable to open the URL to the Table Help page"));
    }
}

QTableWidgetItem *SerialTable::itemExists(const QString &FName,
                                          int row,
                                          int column)
{
    QTableWidgetItem *item = NULL;

    QTableWidgetItem *rowItem = rowHeaderExists(FName, row);

    if(rowItem)
    {
        QTableWidgetItem *columnItem = columnHeaderExists(FName, column);

        if(columnItem)
        {
            int rowIndex = rowItem->data(Qt::UserRole).toInt();
            int columnIndex = columnItem->data(Qt::UserRole).toInt();

            item = m_ui->tableWidget->item(rowIndex, columnIndex);

            if(!item)
            {
                emit errorMessage(QString(metaObject()->className()) +
                "::" + FName + "[" + windowTitle() + "][" +
                rowItem->text() + "][" + columnItem->text() + "] -> " +
                tr("Item at (row == %L1) and (column == %L2) does not exist").
                arg(row).arg(column));
            }
        }
    }

    return item;
}

bool SerialTable::itemDoesNotExist(const QString &FName,
                                   int row,
                                   int column,
                                   int *rowIndex,
                                   int *columnIndex)
{
    Q_UNUSED(FName); // Silently handle error...

    bool ok = false;

    QTableWidgetItem *rowItem = rowHeaderExists(FName, row);

    if(rowItem)
    {
        QTableWidgetItem *columnItem = columnHeaderExists(FName, column);

        if(columnItem)
        {
            *rowIndex = rowItem->data(Qt::UserRole).toInt();
            *columnIndex = columnItem->data(Qt::UserRole).toInt();

            ok = !m_ui->tableWidget->item(*rowIndex, *columnIndex);

            if(!ok)
            {
                /* emit errorMessage(QString(metaObject()->className()) +
                "::" + FName + "[" + windowTitle() + "][" +
                rowItem->text() + "][" + columnItem->text() + "] -> " +
                tr("Item at (row == %L1) and (column == %L2) already exists").
                arg(row).arg(column)); */
            }
        }
    }

    return ok;
}

QTableWidgetItem *SerialTable::rowHeaderExists(const QString &FName,
                                               int row)
{
    QTableWidgetItem *item = m_rowHeaders.value(row);

    if(!item)
    {
        emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "] -> " +
        tr("Row %L1 does not exist").arg(row));
    }

    return item;
}

QTableWidgetItem *SerialTable::columnHeaderExists(const QString &FName,
                                                  int column)
{
    QTableWidgetItem *item = m_columnHeaders.value(column);

    if(!item)
    {
        emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "] -> " +
        tr("Column %L1 does not exist").arg(column));
    }

    return item;
}

bool SerialTable::rowHeaderDoesNotExist(const QString &FName,
                                        int row)
{
    Q_UNUSED(FName); // Silently handle error...

    bool ok = !m_rowHeaders.contains(row);

    if(!ok)
    {
        /* emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "] -> " +
        tr("Row %L1 already exists").arg(row)); */
    }

    return ok;
}

bool SerialTable::columnHeaderDoesNotExist(const QString &FName,
                                           int column)
{
    Q_UNUSED(FName); // Silently handle error...

    bool ok = !m_columnHeaders.contains(column);

    if(!ok)
    {
        /* emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "] -> " +
        tr("Column %L1 already exists").arg(column)); */
    }

    return ok;
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
