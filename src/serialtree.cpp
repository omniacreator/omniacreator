/***************************************************************************//**
* @file
* Serial Tree
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

#include "serialtree.h"
#include "ui_serialtree.h"

SerialTree::SerialTree(const QString &title,
QSettings *settings, QWidget *parent) :
SerialWindow(title, settings, parent), m_ui(new Ui::SerialTree)
{
    m_ui->setupUi(this); m_ui->statusBar->hide();

    m_findString = QString();

    m_ui->treeWidget->header()->setSectionsMovable(false);
    m_ui->treeWidget->header()->setSectionResizeMode(
    QHeaderView::ResizeToContents);

    connect(m_ui->actionFind_Key, SIGNAL(triggered()),
            this, SLOT(findKey()));

    connect(m_ui->actionOpen_Tree, SIGNAL(triggered()),
            this, SLOT(importState()));

    connect(m_ui->actionSave_Tree, SIGNAL(triggered()),
            this, SLOT(exportState()));

    connect(m_ui->actionExport_CSV, SIGNAL(triggered()),
            this, SLOT(exportToCSV()));

    connect(m_ui->actionClose_Window, SIGNAL(triggered()),
            this, SLOT(close()));

    connect(m_ui->actionGeneral_Help, SIGNAL(triggered()),
            this, SLOT(generalHelp()));

    connect(m_ui->actionTree_Help, SIGNAL(triggered()),
            this, SLOT(treeHelp()));

    restoreState();
}

SerialTree::~SerialTree()
{
    delete m_ui;
}

bool SerialTree::importExportEnabled() const
{
    return true;
}

QString SerialTree::keyGroup() const
{
    return SERIAL_TREE_KEY_GROUP;
}

QString SerialTree::keyState() const
{
    return SERIAL_TREE_KEY_STATE;
}

QString SerialTree::keyGeometry() const
{
    return SERIAL_TREE_KEY_GEOMETRY;
}

bool SerialTree::newItem(const QString &key, const QString &value)
{
    QString cleanKey = QDir::fromNativeSeparators(QDir::cleanPath(key));

    QPair<QTreeWidgetItem *, QStringList> pair;

    bool ok = itemDoesNotExist(__FUNCTION__, cleanKey, &pair);

    if(ok)
    {
        foreach(const QString &string, pair.second)
        {
            pair.first = new QTreeWidgetItem(pair.first);
            pair.first->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

            pair.first->setText(TREE_KEY_COLUMN, string);
        }

        pair.first->setText(TREE_VALUE_COLUMN, value);
    }
    else // Silently handle error...
    {
        return setItemValue(cleanKey, value);
    }

    return ok;
}

bool SerialTree::setItemValue(const QString &key, const QString &value)
{
    QString cleanKey = QDir::fromNativeSeparators(QDir::cleanPath(key));

    QTreeWidgetItem *item = itemExists(__FUNCTION__, cleanKey);

    if(item)
    {
        item->setText(TREE_VALUE_COLUMN, value);
    }

    return item;
}

QString SerialTree::getItemValue(const QString &key, bool *ok)
{
    QString cleanKey = QDir::fromNativeSeparators(QDir::cleanPath(key));

    QTreeWidgetItem *item = itemExists(__FUNCTION__, cleanKey);

    if((*ok = item))
    {
        return item->text(TREE_VALUE_COLUMN);
    }

    return QString();
}

bool SerialTree::deleteItem(const QString &key)
{
    QString cleanKey = QDir::fromNativeSeparators(QDir::cleanPath(key));

    QTreeWidgetItem *item = itemExists(__FUNCTION__, cleanKey);

    if(item)
    {
        delete item;
    }

    return item;
}

void SerialTree::deleteItems()
{
    m_ui->treeWidget->clear();
}

void SerialTree::findKey()
{
    bool ok;

    QString findString = QInputDialog::getText(this, tr("Find Key"),
    tr("Key text"), QLineEdit::Normal, m_findString, &ok,
    Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint |
    Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);

    if(ok)
    {
        QList<QTreeWidgetItem *> list = m_ui->treeWidget->findItems(
        findString, Qt::MatchContains | Qt::MatchFixedString |
        Qt::MatchCaseSensitive | Qt::MatchWrap | Qt::MatchRecursive,
        TREE_KEY_COLUMN);

        if(!list.isEmpty())
        {
            int item = list.indexOf(m_ui->treeWidget->currentItem());

            if(item != -1)
            {
                item = (item + 1) % list.size();

                m_ui->treeWidget->scrollToItem(list.at(item));
                m_ui->treeWidget->setCurrentItem(list.at(item));
            }
            else
            {
                m_ui->treeWidget->scrollToItem(list.first());
                m_ui->treeWidget->setCurrentItem(list.first());
            }

            m_findString = findString;
        }
        else
        {
            QMessageBox::information(this, tr("Find Key"),
            tr("Key \"%L1\" not found").arg(findString));
        }
    }
}

void SerialTree::importState(const QString &fileName)
{
    QSettings settings(settingsFileName(), settingsFormat());

    settings.beginGroup(keyGroup());
    settings.beginGroup(windowTitle());

    QString openFile = settings.value(SERIAL_TREE_KEY_OPEN_FILE,
                                      QDir::homePath()).toString();

    QString temp = fileName.isEmpty() ? QFileDialog::getOpenFileName(this,
    tr("Open Tree"), openFile, tr("JSON Files (*.json)")) : fileName;

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

                if(object.value("type").toString() == "tree")
                {
                    deleteItems();

                    setWindowTitle(object.value("title").toString());

                    QList< QPair<QString, QString> > list =
                    json2Item(object.value("tree").toObject());

                    list.removeFirst();

                    QList< QPair<QString, QString> >::const_iterator i =
                    list.constBegin();

                    QList< QPair<QString, QString> >::const_iterator j =
                    list.constEnd();

                    for(; i != j; i++)
                    {
                        newItem(
                        QDir::fromNativeSeparators(QDir::cleanPath(i->first)),
                        i->second);
                    }

                    QFileInfo fileInfo(temp);

                    settings.setValue(SERIAL_TREE_KEY_OPEN_FILE,
                    fileInfo.canonicalFilePath());
                }
                else
                {
                    QMessageBox::critical(this, tr("Open Tree Error"),
                    tr("Incompatible JSON file"));
                }
            }
            else
            {
                QMessageBox::critical(this, tr("Open Tree Error"),
                parseError.errorString());
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Open Tree Error"),
            file.errorString());
        }
    }
}

void SerialTree::exportState(const QString &fileName)
{
    QSettings settings(settingsFileName(), settingsFormat());

    settings.beginGroup(keyGroup());
    settings.beginGroup(windowTitle());

    QString saveFile = settings.value(SERIAL_TREE_KEY_SAVE_FILE,
                                      QDir::homePath()).toString();

    QString temp = fileName.isEmpty() ? QFileDialog::getSaveFileName(this,
    tr("Save Tree"), saveFile, tr("JSON Files (*.json)")) : fileName;

    if(!temp.isEmpty())
    {
        QFile file(temp);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QJsonObject object;

            object.insert("type", QString("tree"));
            object.insert("title", windowTitle());

            object.insert("tree",
            item2Json(m_ui->treeWidget->invisibleRootItem()));

            QByteArray json = QJsonDocument(object).toJson();

            if(file.write(json) == json.size())
            {
                QFileInfo fileInfo(temp);

                settings.setValue(SERIAL_TREE_KEY_SAVE_FILE,
                fileInfo.canonicalFilePath());
            }
            else
            {
                QMessageBox::critical(this, tr("Save Tree Error"),
                file.errorString());
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Save Tree Error"),
            file.errorString());
        }
    }
}

void SerialTree::exportToCSV(const QString &fileName)
{
    QSettings settings(settingsFileName(), settingsFormat());

    settings.beginGroup(keyGroup());
    settings.beginGroup(windowTitle());

    QString saveFile = settings.value(SERIAL_TREE_KEY_SAVE_FILE_CSV,
                                      QDir::homePath()).toString();

    QString temp = fileName.isEmpty() ? QFileDialog::getSaveFileName(this,
    tr("Export Tree"), saveFile, tr("CSV Files (*.csv)")) : fileName;

    if(!temp.isEmpty())
    {
        QByteArray data;

        switch(QMessageBox::question(this, tr("Export Tree"),
        tr("Include Column Header Line?"), QMessageBox::Yes |
        QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes))
        {
            case QMessageBox::Yes:
            {
                data.append("\"key\",\"value\"\n"); break;
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
            QList< QPair<QString, QString> > list =
            json2Item(item2Json(m_ui->treeWidget->invisibleRootItem()));

            list.removeFirst();

            QList< QPair<QString, QString> >::const_iterator i =
            list.constBegin();

            QList< QPair<QString, QString> >::const_iterator j =
            list.constEnd();

            for(; i != j; i++)
            {
                data.append("\"" +
                QDir::fromNativeSeparators(QDir::cleanPath(i->first)) +
                "\",\"" + i->second + "\"\n");
            }

            if(file.write(data) == data.size())
            {
                QFileInfo fileInfo(temp);

                settings.setValue(SERIAL_TREE_KEY_SAVE_FILE_CSV,
                fileInfo.canonicalFilePath());
            }
            else
            {
                QMessageBox::critical(this, tr("Export Tree Error"),
                file.errorString());
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Export Tree Error"),
            file.errorString());
        }
    }
}

void SerialTree::generalHelp()
{
    if(!QDesktopServices::openUrl(QUrl("http://" PROJECT_DOMAIN_NAME_STR "/"
    "help/general/")))
    {
        QMessageBox::critical(this, tr("Open General Help Error"),
        tr("Unable to open the URL to the General Help page"));
    }
}

void SerialTree::treeHelp()
{
    if(!QDesktopServices::openUrl(QUrl("http://" PROJECT_DOMAIN_NAME_STR "/"
    "help/widgets/tree/")))
    {
        QMessageBox::critical(this, tr("Open Tree Help Error"),
        tr("Unable to open the URL to the Tree Help page"));
    }
}

QList<QTreeWidgetItem *> SerialTree::itemChildren(QTreeWidgetItem *parent)
{
    QList<QTreeWidgetItem *> list;

    for(int i = 0, j = parent->childCount(); i < j; i++)
    {
        list.append(parent->child(i));
    }

    return list;
}

QList< QPair<QString, QString> > SerialTree::json2Item(const QJsonObject &json)
{
    QList< QPair<QString, QString> > list;

    QString key = json.value("key").toString();
    QString value = json.value("value").toString();

    list.append(QPair<QString, QString>(key, value));

    QJsonArray children = json.value("children").toArray();

    for(int i = 0, j = children.count(); i < j; i++)
    {
        QList< QPair<QString, QString> > temp =
        json2Item(children.at(i).toObject());

        QList< QPair<QString, QString> >::iterator k = temp.begin();
        QList< QPair<QString, QString> >::iterator l = temp.end();

        for(; k != l; k++)
        {
            k->first.prepend(key + QDir::separator());
        }

        list.append(temp);
    }

    return list;
}

QJsonObject SerialTree::item2Json(QTreeWidgetItem *parent)
{
    QJsonArray children;

    foreach(QTreeWidgetItem *item, itemChildren(parent))
    {
        children.append(item2Json(item));
    }

    QJsonObject object;

    object.insert("key", parent->text(TREE_KEY_COLUMN));
    object.insert("value", parent->text(TREE_VALUE_COLUMN));
    object.insert("children", children);

    return object;
}

QTreeWidgetItem *SerialTree::itemExists(const QString &FName,
                                        const QString &key)
{
    QStringList list = QString(key).simplified().
    replace('\\', '/').split('/', QString::SkipEmptyParts);

    if(!list.isEmpty())
    {
        QTreeWidgetItem *parent = m_ui->treeWidget->invisibleRootItem();

        QString string; for(int i = 0, j = list.size(); i < j; i++)
        {
            bool notFound = true; string = list.at(i);

            foreach(QTreeWidgetItem *item, itemChildren(parent))
            {
                if(item->text(TREE_KEY_COLUMN) == string)
                {
                    parent = item; notFound = false; break;
                }
            }

            if(notFound)
            {
                emit errorMessage(QString(metaObject()->className()) +
                "::" + FName + "[" + windowTitle() + "] -> " +
                tr("Item \"%L1\" does not exist in key \"%L2\"").
                arg(string).arg(key)); return NULL;
            }
        }

        return parent;
    }
    else
    {
        emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "] -> " +
        tr("No items found in key \"%L1\"").
        arg(key)); return NULL;
    }
}

bool SerialTree::itemDoesNotExist(const QString &FName,
                                  const QString &key,
                                  QPair<QTreeWidgetItem *, QStringList> *pair)
{
    Q_UNUSED(FName); // Silently handle error...

    QStringList list = QString(key).simplified().
    replace('\\', '/').split('/', QString::SkipEmptyParts);

    if(!list.isEmpty())
    {
        QTreeWidgetItem *parent = m_ui->treeWidget->invisibleRootItem();

        QString string; for(int i = 0, j = list.size(); i < j; i++)
        {
            bool notFound = true; string = list.at(i);

            foreach(QTreeWidgetItem *item, itemChildren(parent))
            {
                if(item->text(TREE_KEY_COLUMN) == string)
                {
                    parent = item; notFound = false; break;
                }
            }

            if(notFound)
            {
                pair->first = parent; pair->second = list.mid(i); return true;
            }
        }

        /* emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "] -> " +
        tr("Item \"%L1\" already exists in key \"%L2\"").
        arg(string).arg(key)); */ return false;
    }
    else
    {
        /* emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "] -> " +
        tr("No items found in key \"%L1\"").
        arg(key)); */ return false;
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
