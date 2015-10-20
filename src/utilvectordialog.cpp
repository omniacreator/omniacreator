/***************************************************************************//**
* @file
* Util Vector Dialog
*
* @version @n 1.0
* @date @n 12/16/2013
*
* @author @n Kwabena W. Agyeman
* @copyright @n (c) 2013 Kwabena W. Agyeman
* @n All rights reserved - Please see the end of the file for the terms of use
*
* @par Update History:
* @n v1.0 - Original release - 12/16/2013
*******************************************************************************/

#include "utilvectordialog.h"
#include "ui_utilvectordialog.h"

UtilVectorDialog::UtilVectorDialog(const QString &title,
QWidget *parent) : QDialog(parent), m_ui(new Ui::UtilVectorDialog)
{
    m_ui->setupUi(this); setWindowTitle(title); setObjectName(title);

    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint |
    Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::Dialog);
}

UtilVectorDialog::~UtilVectorDialog()
{
    delete m_ui;
}

void UtilVectorDialog::setWidth(int width)
{
    m_ui->widthValue->setValue(width);
}

int UtilVectorDialog::getWidth() const
{
    return m_ui->widthValue->value();
}

void UtilVectorDialog::setHeight(int height)
{
    m_ui->heightValue->setValue(height);
}

int UtilVectorDialog::getHeight() const
{
    return m_ui->heightValue->value();
}

void UtilVectorDialog::setSaveEntireImage(bool on)
{
    m_ui->saveEntireImage->setChecked(on);
}

bool UtilVectorDialog::getSaveEntireImage() const
{
    return m_ui->saveEntireImage->isChecked();
}

void UtilVectorDialog::setSaveCurrentView(bool on)
{
    m_ui->saveCurrentView->setChecked(on);
}

bool UtilVectorDialog::getSaveCurrentView() const
{
    return m_ui->saveCurrentView->isChecked();
}

void UtilVectorDialog::setFileTitle(const QString &fileTitle)
{
    m_ui->tEdit->setText(fileTitle);
}

QString UtilVectorDialog::getFileTitle() const
{
    return m_ui->tEdit->text();
}

void UtilVectorDialog::setFileDescription(const QString &fileDescription)
{
    m_ui->dEdit->setText(fileDescription);
}

QString UtilVectorDialog::getFileDescription() const
{
    return m_ui->dEdit->text();
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
