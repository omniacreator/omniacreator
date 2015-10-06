/***************************************************************************//**
* @file
* Util Column Dialog
*
* @version @n 1.0
* @date @n 6/10/2014
*
* @author @n Kwabena W. Agyeman
* @copyright @n (c) 2014 Kwabena W. Agyeman
* @n All rights reserved - Please see the end of the file for the terms of use
*
* @par Update History:
* @n v1.0 - Original release - 6/10/2014
*******************************************************************************/

// NOTE: Bug https://bugreports.qt-project.org/browse/QTBUG-2329 makes
// QColumnView unusable so QTreeView is used instead...

#include "utilcolumndialog.h"
#include "ui_utilcolumndialog.h"

UtilColumnDialog::UtilColumnDialog(const QString &title,
QWidget *parent) : QDialog(parent), m_ui(new Ui::UtilColumnDialog)
{
    m_ui->setupUi(this); setWindowTitle(title); setObjectName(title);

    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint |
    Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::Dialog);
}

UtilColumnDialog::~UtilColumnDialog()
{
    delete m_ui;
}

void UtilColumnDialog::setModel(QAbstractItemModel *model)
{
    m_ui->columnView->setModel(model);
}

QAbstractItemModel *UtilColumnDialog::getModel() const
{
    return m_ui->columnView->model();
}

void UtilColumnDialog::setCurrentIndex(const QModelIndex &index)
{
    m_ui->columnView->setCurrentIndex(index);
}

QModelIndex UtilColumnDialog::getCurrentIndex() const
{
    return m_ui->columnView->currentIndex();
}

void UtilColumnDialog::setText(const QString &text)
{
    m_ui->descriptionLabel->setText(text);
}

QString UtilColumnDialog::getText() const
{
    return m_ui->descriptionLabel->text();
}

void UtilColumnDialog::selectionChanged()
{
    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setDefault(
    getCurrentIndex().flags() & Qt::ItemIsSelectable);

    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
    getCurrentIndex().flags() & Qt::ItemIsSelectable);
}

void UtilColumnDialog::showEvent(QShowEvent *event)
{
    if(m_ui->columnView->selectionModel())
    {
        connect(m_ui->columnView->selectionModel(),
        SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
        this, SLOT(selectionChanged()));
    }

    m_ui->columnView->setFocus();

    QDialog::showEvent(event);
}

/***************************************************************************//**
* @file
* @par MIT License - TERMS OF USE:
* @n Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"), to
* deal in the Software without restriction, including without limitati on the
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
