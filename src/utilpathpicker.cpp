/***************************************************************************//**
* @file
* Util Path Picker
*
* @version @n 1.0
* @date @n 10/10/2013
*
* @author @n Kwabena W. Agyeman
* @copyright @n (c) 2013 Kwabena W. Agyeman
* @n All rights reserved - Please see the end of the file for the terms of use
*
* @par Update History:
* @n v1.0 - Original release - 10/10/2013
*******************************************************************************/

#include "utilpathpicker.h"
#include "ui_utilpathpicker.h"

UtilPathPicker::UtilPathPicker(const QString &title, const QString &label,
const QString &defaultDir, QWidget *parent) :
QDialog(parent), m_ui(new Ui::UtilPathPicker)
{
    m_ui->setupUi(this); setWindowTitle(title); setObjectName(title);

    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint |
    Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::Dialog);

    m_ui->button->setIcon(QFileIconProvider().icon(QFileIconProvider::Folder));

    connect(m_ui->button, SIGNAL(clicked()),
            this, SLOT(pick()));

    connect(m_ui->edit, SIGNAL(textChanged(QString)),
            this, SLOT(textChanged(QString)));

    setLabel(label);
    setDefaultDir(defaultDir);
}

UtilPathPicker::~UtilPathPicker()
{
    delete m_ui;
}

void UtilPathPicker::setLabel(const QString &label)
{
    m_ui->label->setText(label);
}

QString UtilPathPicker::getLabel() const
{
    return m_ui->label->text();
}

void UtilPathPicker::setDefaultDir(const QString &defaultDir)
{
    m_ui->edit->setText(defaultDir);
}

QString UtilPathPicker::getDefaultDir() const
{
    return m_ui->edit->text();
}

void UtilPathPicker::pick()
{
    QString defaultDir = QFileDialog::getExistingDirectory(this, windowTitle(),
                                                           m_ui->edit->text());

    if(!defaultDir.isEmpty())
    {
        m_ui->edit->setText(defaultDir);
    }
}

void UtilPathPicker::textChanged(const QString &text)
{
    bool ok = QFileInfo(text).isAbsolute() && (!QFileInfo(text).isFile());

    m_ui->box->button(QDialogButtonBox::Ok)->setDefault(ok);
    m_ui->box->button(QDialogButtonBox::Ok)->setEnabled(ok);
}

void UtilPathPicker::showEvent(QShowEvent *event)
{
    m_ui->edit->setFocus();

    QDialog::showEvent(event);
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
