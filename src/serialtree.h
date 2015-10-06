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

#ifndef SERIALTREE_H
#define SERIALTREE_H

#include <QtCore>
#include <QtWidgets>

#include "serialwindow.h"

#define TREE_KEY_COLUMN 0
#define TREE_VALUE_COLUMN 1

namespace Ui
{
    class SerialTree;
}

class SerialTree : public SerialWindow
{
    Q_OBJECT

public:

    explicit SerialTree(const QString &title,
                        QSettings *settings = NULL,
                        QWidget *parent = NULL);

    virtual ~SerialTree();

    virtual bool importExportEnabled() const;

    virtual QString keyGroup() const;
    virtual QString keyState() const;
    virtual QString keyGeometry() const;

    bool newItem(const QString &key, const QString &value);
    bool setItemValue(const QString &key, const QString &value);
    QString getItemValue(const QString &key, bool *ok);
    bool deleteItem(const QString &key);
    void deleteItems();

public slots:

    virtual void importState(const QString &fileName = QString());
    virtual void exportState(const QString &fileName = QString());

    void exportToCSV(const QString &fileName = QString());

private slots:

    void findKey();

    void generalHelp();
    void treeHelp();

private:

    Q_DISABLE_COPY(SerialTree)

    QList<QTreeWidgetItem *> itemChildren(QTreeWidgetItem *parent);

    QList< QPair<QString, QString> > json2Item(const QJsonObject &json);
    QJsonObject item2Json(QTreeWidgetItem *parent);

    QTreeWidgetItem *itemExists(const QString &FName,
                                const QString &key);

    bool itemDoesNotExist(const QString &FName,
                          const QString &key,
                          QPair<QTreeWidgetItem *, QStringList> *pair);

    QString m_findString;

    Ui::SerialTree *m_ui;
};

#endif // SERIALTREE_H

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
