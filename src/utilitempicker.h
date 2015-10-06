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

#ifndef UTILITEMPICKER_H
#define UTILITEMPICKER_H

#include <QtCore>
#include <QtWidgets>

namespace Ui
{
    class UtilItemPicker;
}

class UtilItemPicker : public QDialog
{
    Q_OBJECT

public:

    explicit UtilItemPicker(const QString &title,
                            const QString &label,
                            const QList< QPair<QString, QVariant> > &itemData,
                            QWidget *parent = NULL);

    virtual ~UtilItemPicker();

    void setLabel(const QString &label);
    QString getLabel() const;

    void setItemData(const QList< QPair<QString, QVariant> > &itemData);
    QList< QPair<QString, QVariant> > getItemData() const;

    void setSelectedText(const QString &text);
    QString getSelectedText() const;

    void setSelectedItem(const QVariant &item);
    QVariant getSelectedItem() const;

private:

    Q_DISABLE_COPY(UtilItemPicker)

    Ui::UtilItemPicker *m_ui;
};

#endif // UTILITEMPICKER_H

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
