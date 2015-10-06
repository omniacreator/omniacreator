/***************************************************************************//**
* @file
* Util Item Picker
*
* @version @n 1.0
* @date @n 10/12/2013
*
* @author @n Kwabena W. Agyeman
* @copyright @n (c) 2013 Kwabena W. Agyeman
* @n All rights reserved - Please see the end of the file for the terms of use
*
* @par Update History:
* @n v1.0 - Original release - 10/12/2013
*******************************************************************************/

#include "utilitempicker.h"
#include "ui_utilitempicker.h"

UtilItemPicker::UtilItemPicker(const QString &title, const QString &label,
const QList< QPair<QString, QVariant> > &itemData, QWidget *parent) :
QDialog(parent), m_ui(new Ui::UtilItemPicker)
{
    m_ui->setupUi(this); setWindowTitle(title); setObjectName(title);

    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint |
    Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::Dialog);

    setLabel(label);
    setItemData(itemData);
}

UtilItemPicker::~UtilItemPicker()
{
    delete m_ui;
}

void UtilItemPicker::setLabel(const QString &label)
{
    m_ui->label->setText(label);
}

QString UtilItemPicker::getLabel() const
{
    return m_ui->label->text();
}

void UtilItemPicker::setItemData(const QList< QPair<QString, QVariant> > &
                                 itemData)
{
    m_ui->edit->clear();

    for(int i = 0, j = itemData.count(); i < j; i++)
    {
        m_ui->edit->addItem(itemData.at(i).first, itemData.at(i).second);
    }
}

QList< QPair<QString, QVariant> > UtilItemPicker::getItemData() const
{
    QList< QPair<QString, QVariant> > list;

    for(int i = 0, j = m_ui->edit->count(); i < j; i++)
    {
        list.append(QPair<QString, QVariant>(m_ui->edit->itemText(i),
                                             m_ui->edit->itemData(i)));
    }

    return list;
}

void UtilItemPicker::setSelectedText(const QString &text)
{
    m_ui->edit->setCurrentIndex(m_ui->edit->findText(text));
}

QString UtilItemPicker::getSelectedText() const
{
    return m_ui->edit->currentText();
}

void UtilItemPicker::setSelectedItem(const QVariant &item)
{
    m_ui->edit->setCurrentIndex(m_ui->edit->findData(item));
}

QVariant UtilItemPicker::getSelectedItem() const
{
    return m_ui->edit->currentData();
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
