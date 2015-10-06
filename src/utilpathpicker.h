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

#ifndef UTILPATHPICKER_H
#define UTILPATHPICKER_H

#include <QtCore>
#include <QtWidgets>

namespace Ui
{
    class UtilPathPicker;
}

class UtilPathPicker : public QDialog
{
    Q_OBJECT

public:

    explicit UtilPathPicker(const QString &title,
                            const QString &label,
                            const QString &defaultDir,
                            QWidget *parent = NULL);

    virtual ~UtilPathPicker();

    void setLabel(const QString &label);
    QString getLabel() const;

    void setDefaultDir(const QString &defaultDir);
    QString getDefaultDir() const;

private slots:

    void pick();

    void textChanged(const QString &text);

protected:

    virtual void showEvent(QShowEvent *event);

private:

    Q_DISABLE_COPY(UtilPathPicker)

    Ui::UtilPathPicker *m_ui;
};

#endif // UTILPATHPICKER_H

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
