/***************************************************************************//**
* @file
* Serial Interface
*
* @version @n 1.0
* @date @n 12/14/2013
*
* @author @n Kwabena W. Agyeman
* @copyright @n (c) 2013 Kwabena W. Agyeman
* @n All rights reserved - Please see the end of the file for the terms of use
*
* @par Update History:
* @n v1.0 - Original release - 12/14/2013
*******************************************************************************/

#include "serialinterface.h"
#include "ui_serialinterface.h"

SerialInterface::SerialInterface(const QString &title,
QSettings *settings, QWidget *parent) :
SerialWindow(title, settings, parent), m_ui(new Ui::SerialInterface)
{
    m_ui->setupUi(this); m_ui->statusBar->hide();

    QWidget *widget = new QWidget;

    m_layout = new QHBoxLayout(widget);
    m_ui->centralWidget->setWidget(widget);

    connect(m_ui->actionClose_Window, SIGNAL(triggered()),
            this, SLOT(close()));

    connect(m_ui->actionGeneral_Help, SIGNAL(triggered()),
            this, SLOT(generalHelp()));

    connect(m_ui->actionInterface_Help, SIGNAL(triggered()),
            this, SLOT(interfaceHelp()));

    restoreState();
}

SerialInterface::~SerialInterface()
{
    delete m_ui;
}

bool SerialInterface::importExportEnabled() const
{
    return false;
}

QString SerialInterface::keyGroup() const
{
    return SERIAL_INTERFACE_KEY_GROUP;
}

QString SerialInterface::keyState() const
{
    return SERIAL_INTERFACE_KEY_STATE;
}

QString SerialInterface::keyGeometry() const
{
    return SERIAL_INTERFACE_KEY_GEOMETRY;
}

bool SerialInterface::newGroupBox(int groupBox,
                                  const QString &name)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    bool ok =
    groupBoxDoesNotExist(__FUNCTION__, groupBox);

    if(!ok)
    {
        // Silently handle error...

        QWidget *tempWidget = m_children.value(groupBox);

        if(qobject_cast<MyGroupBox *>(tempWidget))
        {
            return setGroupBoxName(groupBox, name);
        }
        else // Replace object instead of error...
        {
            deleteGroupBox(groupBox);
        }
    }

    MyGroupBox *myWidget = new MyGroupBox(name);

    QMap<int, MyGroupBox *>::iterator
    i = m_children.insert(groupBox, myWidget),
    j = m_children.begin(); int index = std::distance(j, i);

    m_layout->insertWidget(index, myWidget);

    myWidget->show();

    return true;
}

bool SerialInterface::newPushButton(int groupBox,
                                    int interfaceBox,
                                    const QString &name)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    MyGroupBox *ok =
    interfaceBoxDoesNotExist(__FUNCTION__, groupBox, interfaceBox);

    MyGroupBox *gBox = m_children.value(groupBox);

    if(gBox)
    {
        if(!ok)
        {
            // Silently handle error...

            QWidget *tempWidget = gBox->m_children.value(interfaceBox);

            if(qobject_cast<MyPushButton *>(tempWidget))
            {
                return setPushButtonName(groupBox, interfaceBox, name);
            }
            else // Replace object instead of error...
            {
                deleteInterfaceBox(groupBox, interfaceBox);
            }
        }

        MyPushButton *myWidget = new MyPushButton(name);

        myWidget->m_windowHandle = getWindowHandle();
        myWidget->m_groupBox = groupBox;
        myWidget->m_interfaceBox = interfaceBox;

        connect(myWidget,
                SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)),
                this, SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)));

        QMap<int, QWidget *>::iterator
        i = gBox->m_children.insert(interfaceBox, myWidget),
        j = gBox->m_children.begin(); int index = std::distance(j, i);

        gBox->m_layout->insertRow(index, myWidget);

        myWidget->show();
    }

    return gBox;
}

bool SerialInterface::newRadioButton(int groupBox,
                                     int interfaceBox,
                                     const QString &name)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    MyGroupBox *ok =
    interfaceBoxDoesNotExist(__FUNCTION__, groupBox, interfaceBox);

    MyGroupBox *gBox = m_children.value(groupBox);

    if(gBox)
    {
        if(!ok)
        {
            // Silently handle error...

            QWidget *tempWidget = gBox->m_children.value(interfaceBox);

            if(qobject_cast<MyRadioButton *>(tempWidget))
            {
                return setRadioButtonName(groupBox, interfaceBox, name);
            }
            else // Replace object instead of error...
            {
                deleteInterfaceBox(groupBox, interfaceBox);
            }
        }

        MyRadioButton *myWidget = new MyRadioButton(name);

        myWidget->m_windowHandle = getWindowHandle();
        myWidget->m_groupBox = groupBox;
        myWidget->m_interfaceBox = interfaceBox;

        connect(myWidget,
                SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)),
                this, SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)));

        QMap<int, QWidget *>::iterator
        i = gBox->m_children.insert(interfaceBox, myWidget),
        j = gBox->m_children.begin(); int index = std::distance(j, i);

        gBox->m_layout->insertRow(index, myWidget);

        myWidget->show();
    }

    return gBox;
}

bool SerialInterface::newCheckBox(int groupBox,
                                  int interfaceBox,
                                  const QString &name)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    MyGroupBox *ok =
    interfaceBoxDoesNotExist(__FUNCTION__, groupBox, interfaceBox);

    MyGroupBox *gBox = m_children.value(groupBox);

    if(gBox)
    {
        if(!ok)
        {
            // Silently handle error...

            QWidget *tempWidget = gBox->m_children.value(interfaceBox);

            if(qobject_cast<MyCheckBox *>(tempWidget))
            {
                return setCheckBoxName(groupBox, interfaceBox, name);
            }
            else // Replace object instead of error...
            {
                deleteInterfaceBox(groupBox, interfaceBox);
            }
        }

        MyCheckBox *myWidget = new MyCheckBox(name);

        myWidget->m_windowHandle = getWindowHandle();
        myWidget->m_groupBox = groupBox;
        myWidget->m_interfaceBox = interfaceBox;

        connect(myWidget,
                SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)),
                this, SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)));

        QMap<int, QWidget *>::iterator
        i = gBox->m_children.insert(interfaceBox, myWidget),
        j = gBox->m_children.begin(); int index = std::distance(j, i);

        gBox->m_layout->insertRow(index, myWidget);

        myWidget->show();
    }

    return gBox;
}

bool SerialInterface::newLineEdit(int groupBox,
                                  int interfaceBox,
                                  const QString &name)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    MyGroupBox *ok =
    interfaceBoxDoesNotExist(__FUNCTION__, groupBox, interfaceBox);

    MyGroupBox *gBox = m_children.value(groupBox);

    if(gBox)
    {
        if(!ok)
        {
            // Silently handle error...

            QWidget *tempWidget = gBox->m_children.value(interfaceBox);

            if(qobject_cast<MyLineEdit *>(tempWidget))
            {
                return setLineEditName(groupBox, interfaceBox, name);
            }
            else // Replace object instead of error...
            {
                deleteInterfaceBox(groupBox, interfaceBox);
            }
        }

        MyLineEdit *myWidget = new MyLineEdit;

        myWidget->m_windowHandle = getWindowHandle();
        myWidget->m_groupBox = groupBox;
        myWidget->m_interfaceBox = interfaceBox;

        connect(myWidget,
                SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)),
                this, SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)));

        QMap<int, QWidget *>::iterator
        i = gBox->m_children.insert(interfaceBox, myWidget),
        j = gBox->m_children.begin(); int index = std::distance(j, i);

        gBox->m_layout->insertRow(index, name, myWidget);

        myWidget->show();
    }

    return gBox;
}

bool SerialInterface::newIntSpinBox(int groupBox,
                                    int interfaceBox,
                                    const QString &name)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    MyGroupBox *ok =
    interfaceBoxDoesNotExist(__FUNCTION__, groupBox, interfaceBox);

    MyGroupBox *gBox = m_children.value(groupBox);

    if(gBox)
    {
        if(!ok)
        {
            // Silently handle error...

            QWidget *tempWidget = gBox->m_children.value(interfaceBox);

            if(qobject_cast<MyIntSpinBox *>(tempWidget))
            {
                return setIntSpinBoxName(groupBox, interfaceBox, name);
            }
            else // Replace object instead of error...
            {
                deleteInterfaceBox(groupBox, interfaceBox);
            }
        }

        MyIntSpinBox *myWidget = new MyIntSpinBox;

        myWidget->m_windowHandle = getWindowHandle();
        myWidget->m_groupBox = groupBox;
        myWidget->m_interfaceBox = interfaceBox;

        connect(myWidget,
                SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)),
                this, SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)));

        QMap<int, QWidget *>::iterator
        i = gBox->m_children.insert(interfaceBox, myWidget),
        j = gBox->m_children.begin(); int index = std::distance(j, i);

        gBox->m_layout->insertRow(index, name, myWidget);

        myWidget->show();
    }

    return gBox;
}

bool SerialInterface::newBinSpinBox(int groupBox,
                                    int interfaceBox,
                                    const QString &name)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    MyGroupBox *ok =
    interfaceBoxDoesNotExist(__FUNCTION__, groupBox, interfaceBox);

    MyGroupBox *gBox = m_children.value(groupBox);

    if(gBox)
    {
        if(!ok)
        {
            // Silently handle error...

            QWidget *tempWidget = gBox->m_children.value(interfaceBox);

            if(qobject_cast<MyBinSpinBox *>(tempWidget))
            {
                return setBinSpinBoxName(groupBox, interfaceBox, name);
            }
            else // Replace object instead of error...
            {
                deleteInterfaceBox(groupBox, interfaceBox);
            }
        }

        MyBinSpinBox *myWidget = new MyBinSpinBox;

        myWidget->m_windowHandle = getWindowHandle();
        myWidget->m_groupBox = groupBox;
        myWidget->m_interfaceBox = interfaceBox;

        connect(myWidget,
                SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)),
                this, SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)));

        QMap<int, QWidget *>::iterator
        i = gBox->m_children.insert(interfaceBox, myWidget),
        j = gBox->m_children.begin(); int index = std::distance(j, i);

        gBox->m_layout->insertRow(index, name, myWidget);

        myWidget->show();
    }

    return gBox;
}

bool SerialInterface::newHexSpinBox(int groupBox,
                                    int interfaceBox,
                                    const QString &name)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    MyGroupBox *ok =
    interfaceBoxDoesNotExist(__FUNCTION__, groupBox, interfaceBox);

    MyGroupBox *gBox = m_children.value(groupBox);

    if(gBox)
    {
        if(!ok)
        {
            // Silently handle error...

            QWidget *tempWidget = gBox->m_children.value(interfaceBox);

            if(qobject_cast<MyHexSpinBox *>(tempWidget))
            {
                return setHexSpinBoxName(groupBox, interfaceBox, name);
            }
            else // Replace object instead of error...
            {
                deleteInterfaceBox(groupBox, interfaceBox);
            }
        }

        MyHexSpinBox *myWidget = new MyHexSpinBox;

        myWidget->m_windowHandle = getWindowHandle();
        myWidget->m_groupBox = groupBox;
        myWidget->m_interfaceBox = interfaceBox;

        connect(myWidget,
                SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)),
                this, SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)));

        QMap<int, QWidget *>::iterator
        i = gBox->m_children.insert(interfaceBox, myWidget),
        j = gBox->m_children.begin(); int index = std::distance(j, i);

        gBox->m_layout->insertRow(index, name, myWidget);

        myWidget->show();
    }

    return gBox;
}

bool SerialInterface::newDoubleSpinBox(int groupBox,
                                       int interfaceBox,
                                       const QString &name)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    MyGroupBox *ok =
    interfaceBoxDoesNotExist(__FUNCTION__, groupBox, interfaceBox);

    MyGroupBox *gBox = m_children.value(groupBox);

    if(gBox)
    {
        if(!ok)
        {
            // Silently handle error...

            QWidget *tempWidget = gBox->m_children.value(interfaceBox);

            if(qobject_cast<MyDoubleSpinBox *>(tempWidget))
            {
                return setDoubleSpinBoxName(groupBox, interfaceBox, name);
            }
            else // Replace object instead of error...
            {
                deleteInterfaceBox(groupBox, interfaceBox);
            }
        }

        MyDoubleSpinBox *myWidget = new MyDoubleSpinBox;

        myWidget->m_windowHandle = getWindowHandle();
        myWidget->m_groupBox = groupBox;
        myWidget->m_interfaceBox = interfaceBox;

        connect(myWidget,
                SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)),
                this, SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)));

        QMap<int, QWidget *>::iterator
        i = gBox->m_children.insert(interfaceBox, myWidget),
        j = gBox->m_children.begin(); int index = std::distance(j, i);

        gBox->m_layout->insertRow(index, name, myWidget);

        myWidget->show();
    }

    return gBox;
}

bool SerialInterface::newTimeEdit(int groupBox,
                                  int interfaceBox,
                                  const QString &name)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    MyGroupBox *ok =
    interfaceBoxDoesNotExist(__FUNCTION__, groupBox, interfaceBox);

    MyGroupBox *gBox = m_children.value(groupBox);

    if(gBox)
    {
        if(!ok)
        {
            // Silently handle error...

            QWidget *tempWidget = gBox->m_children.value(interfaceBox);

            if(qobject_cast<MyTimeEdit *>(tempWidget))
            {
                return setTimeEditName(groupBox, interfaceBox, name);
            }
            else // Replace object instead of error...
            {
                deleteInterfaceBox(groupBox, interfaceBox);
            }
        }

        MyTimeEdit *myWidget = new MyTimeEdit;

        myWidget->m_windowHandle = getWindowHandle();
        myWidget->m_groupBox = groupBox;
        myWidget->m_interfaceBox = interfaceBox;

        connect(myWidget,
                SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)),
                this, SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)));

        QMap<int, QWidget *>::iterator
        i = gBox->m_children.insert(interfaceBox, myWidget),
        j = gBox->m_children.begin(); int index = std::distance(j, i);

        gBox->m_layout->insertRow(index, name, myWidget);

        myWidget->show();
    }

    return gBox;
}

bool SerialInterface::newDateEdit(int groupBox,
                                  int interfaceBox,
                                  const QString &name)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    MyGroupBox *ok =
    interfaceBoxDoesNotExist(__FUNCTION__, groupBox, interfaceBox);

    MyGroupBox *gBox = m_children.value(groupBox);

    if(gBox)
    {
        if(!ok)
        {
            // Silently handle error...

            QWidget *tempWidget = gBox->m_children.value(interfaceBox);

            if(qobject_cast<MyDateEdit *>(tempWidget))
            {
                return setDateEditName(groupBox, interfaceBox, name);
            }
            else // Replace object instead of error...
            {
                deleteInterfaceBox(groupBox, interfaceBox);
            }
        }

        MyDateEdit *myWidget = new MyDateEdit;

        myWidget->m_windowHandle = getWindowHandle();
        myWidget->m_groupBox = groupBox;
        myWidget->m_interfaceBox = interfaceBox;

        connect(myWidget,
                SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)),
                this, SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)));

        QMap<int, QWidget *>::iterator
        i = gBox->m_children.insert(interfaceBox, myWidget),
        j = gBox->m_children.begin(); int index = std::distance(j, i);

        gBox->m_layout->insertRow(index, name, myWidget);

        myWidget->show();
    }

    return gBox;
}

bool SerialInterface::newDateTimeEdit(int groupBox,
                                      int interfaceBox,
                                      const QString &name)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    MyGroupBox *ok =
    interfaceBoxDoesNotExist(__FUNCTION__, groupBox, interfaceBox);

    MyGroupBox *gBox = m_children.value(groupBox);

    if(gBox)
    {
        if(!ok)
        {
            // Silently handle error...

            QWidget *tempWidget = gBox->m_children.value(interfaceBox);

            if(qobject_cast<MyDateTimeEdit *>(tempWidget))
            {
                return setDateTimeEditName(groupBox, interfaceBox, name);
            }
            else // Replace object instead of error...
            {
                deleteInterfaceBox(groupBox, interfaceBox);
            }
        }

        MyDateTimeEdit *myWidget = new MyDateTimeEdit;

        myWidget->m_windowHandle = getWindowHandle();
        myWidget->m_groupBox = groupBox;
        myWidget->m_interfaceBox = interfaceBox;

        connect(myWidget,
                SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)),
                this, SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)));

        QMap<int, QWidget *>::iterator
        i = gBox->m_children.insert(interfaceBox, myWidget),
        j = gBox->m_children.begin(); int index = std::distance(j, i);

        gBox->m_layout->insertRow(index, name, myWidget);

        myWidget->show();
    }

    return gBox;
}

bool SerialInterface::newSlider(int groupBox,
                                int interfaceBox,
                                const QString &name)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    MyGroupBox *ok =
    interfaceBoxDoesNotExist(__FUNCTION__, groupBox, interfaceBox);

    MyGroupBox *gBox = m_children.value(groupBox);

    if(gBox)
    {
        if(!ok)
        {
            // Silently handle error...

            QWidget *tempWidget = gBox->m_children.value(interfaceBox);

            if(qobject_cast<MySlider *>(tempWidget))
            {
                return setSliderName(groupBox, interfaceBox, name);
            }
            else // Replace object instead of error...
            {
                deleteInterfaceBox(groupBox, interfaceBox);
            }
        }

        MySlider *myWidget = new MySlider;

        myWidget->m_windowHandle = getWindowHandle();
        myWidget->m_groupBox = groupBox;
        myWidget->m_interfaceBox = interfaceBox;

        connect(myWidget,
                SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)),
                this, SIGNAL(dataPacket(SerialFunction,int,int,int,QVariant)));

        QMap<int, QWidget *>::iterator
        i = gBox->m_children.insert(interfaceBox, myWidget),
        j = gBox->m_children.begin(); int index = std::distance(j, i);

        gBox->m_layout->insertRow(index, name, myWidget);

        myWidget->show();
    }

    return gBox;
}

bool SerialInterface::newLabel(int groupBox,
                               int interfaceBox,
                               const QString &text)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    MyGroupBox *ok =
    interfaceBoxDoesNotExist(__FUNCTION__, groupBox, interfaceBox);

    MyGroupBox *gBox = m_children.value(groupBox);

    if(gBox)
    {
        if(!ok)
        {
            // Silently handle error...

            QWidget *tempWidget = gBox->m_children.value(interfaceBox);

            if(qobject_cast<MyLabel *>(tempWidget))
            {
                return setLabelText(groupBox, interfaceBox, text);
            }
            else // Replace object instead of error...
            {
                deleteInterfaceBox(groupBox, interfaceBox);
            }
        }

        MyLabel *myWidget = new MyLabel(text);

        QMap<int, QWidget *>::iterator
        i = gBox->m_children.insert(interfaceBox, myWidget),
        j = gBox->m_children.begin(); int index = std::distance(j, i);

        gBox->m_layout->insertRow(index, myWidget);

        myWidget->show();
    }

    return gBox;
}

bool SerialInterface::newProgressBar(int groupBox,
                                     int interfaceBox,
                                     const QString &name)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    MyGroupBox *ok =
    interfaceBoxDoesNotExist(__FUNCTION__, groupBox, interfaceBox);

    MyGroupBox *gBox = m_children.value(groupBox);

    if(gBox)
    {
        if(!ok)
        {
            // Silently handle error...

            QWidget *tempWidget = gBox->m_children.value(interfaceBox);

            if(qobject_cast<MyProgressBar *>(tempWidget))
            {
                return setProgressBarName(groupBox, interfaceBox, name);
            }
            else // Replace object instead of error...
            {
                deleteInterfaceBox(groupBox, interfaceBox);
            }
        }

        MyProgressBar *myWidget = new MyProgressBar;

        QMap<int, QWidget *>::iterator
        i = gBox->m_children.insert(interfaceBox, myWidget),
        j = gBox->m_children.begin(); int index = std::distance(j, i);

        gBox->m_layout->insertRow(index, name, myWidget);

        myWidget->show();
    }

    return gBox;
}

bool SerialInterface::newSeperator(int groupBox,
                                   int interfaceBox)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    MyGroupBox *ok =
    interfaceBoxDoesNotExist(__FUNCTION__, groupBox, interfaceBox);

    MyGroupBox *gBox = m_children.value(groupBox);

    if(gBox)
    {
        if(!ok)
        {
            // Silently handle error...

            QWidget *tempWidget = gBox->m_children.value(interfaceBox);

            if(qobject_cast<MySeperator *>(tempWidget))
            {
                return true;
            }
            else // Replace object instead of error...
            {
                deleteInterfaceBox(groupBox, interfaceBox);
            }
        }

        MySeperator *myWidget = new MySeperator;

        QMap<int, QWidget *>::iterator
        i = gBox->m_children.insert(interfaceBox, myWidget),
        j = gBox->m_children.begin(); int index = std::distance(j, i);

        gBox->m_layout->insertRow(index, myWidget);

        myWidget->show();
    }

    return gBox;
}

bool SerialInterface::setGroupBoxName(int groupBox,
                                      const QString &name)
{
    MyGroupBox *gBox = groupBoxExists(__FUNCTION__, groupBox);

    if(gBox)
    {
        gBox->setTitle(name);
    }

    return gBox;
}

QString SerialInterface::getGroupBoxName(int groupBox,
                                         bool *ok)
{
    MyGroupBox *gBox = groupBoxExists(__FUNCTION__, groupBox);

    if((*ok = gBox))
    {
        return gBox->title();
    }

    return QString();
}

bool SerialInterface::setPushButtonName(int groupBox,
                                        int interfaceBox,
                                        const QString &name)
{
    QPair<MyGroupBox *, MyPushButton *> pair = interfaceBoxCast
    <MyPushButton>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        setInterfaceBoxName(pair, name);
    }

    return pair.second;
}

QString SerialInterface::getPushButtonName(int groupBox,
                                           int interfaceBox,
                                           bool *ok)
{
    QPair<MyGroupBox *, MyPushButton *> pair = interfaceBoxCast
    <MyPushButton>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return getInterfaceBoxName(pair);
    }

    return QString();
}

bool SerialInterface::setRadioButtonName(int groupBox,
                                         int interfaceBox,
                                         const QString &name)
{
    QPair<MyGroupBox *, MyRadioButton *> pair = interfaceBoxCast
    <MyRadioButton>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        setInterfaceBoxName(pair, name);
    }

    return pair.second;
}

QString SerialInterface::getRadioButtonName(int groupBox,
                                            int interfaceBox,
                                            bool *ok)
{
    QPair<MyGroupBox *, MyRadioButton *> pair = interfaceBoxCast
    <MyRadioButton>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return getInterfaceBoxName(pair);
    }

    return QString();
}

bool SerialInterface::setRadioButtonChecked(int groupBox,
                                            int interfaceBox,
                                            bool checked)
{
    QPair<MyGroupBox *, MyRadioButton *> pair = interfaceBoxCast
    <MyRadioButton>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setChecked(checked);
    }

    return pair.second;
}

bool SerialInterface::getRadioButtonChecked(int groupBox,
                                            int interfaceBox,
                                            bool *ok)
{
    QPair<MyGroupBox *, MyRadioButton *> pair = interfaceBoxCast
    <MyRadioButton>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->isChecked();
    }

    return bool();
}

bool SerialInterface::setCheckBoxName(int groupBox,
                                      int interfaceBox,
                                      const QString &name)
{
    QPair<MyGroupBox *, MyCheckBox *> pair = interfaceBoxCast
    <MyCheckBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        setInterfaceBoxName(pair, name);
    }

    return pair.second;
}

QString SerialInterface::getCheckBoxName(int groupBox,
                                         int interfaceBox,
                                         bool *ok)
{
    QPair<MyGroupBox *, MyCheckBox *> pair = interfaceBoxCast
    <MyCheckBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return getInterfaceBoxName(pair);
    }

    return QString();
}

bool SerialInterface::setCheckBoxChecked(int groupBox,
                                         int interfaceBox,
                                         bool checked)
{
    QPair<MyGroupBox *, MyCheckBox *> pair = interfaceBoxCast
    <MyCheckBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setChecked(checked);
    }

    return pair.second;
}

bool SerialInterface::getCheckBoxChecked(int groupBox,
                                         int interfaceBox,
                                         bool *ok)
{
    QPair<MyGroupBox *, MyCheckBox *> pair = interfaceBoxCast
    <MyCheckBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->isChecked();
    }

    return bool();
}

bool SerialInterface::setLineEditName(int groupBox,
                                      int interfaceBox,
                                      const QString &name)
{
    QPair<MyGroupBox *, MyLineEdit *> pair = interfaceBoxCast
    <MyLineEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        setInterfaceBoxName(pair, name);
    }

    return pair.second;
}

QString SerialInterface::getLineEditName(int groupBox,
                                         int interfaceBox,
                                         bool *ok)
{
    QPair<MyGroupBox *, MyLineEdit *> pair = interfaceBoxCast
    <MyLineEdit>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return getInterfaceBoxName(pair);
    }

    return QString();
}

bool SerialInterface::setLineEditEchoMode(int groupBox,
                                          int interfaceBox,
                                          QLineEdit::EchoMode echoMode)
{
    QPair<MyGroupBox *, MyLineEdit *> pair = interfaceBoxCast
    <MyLineEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        QString groupBoxName = pair.first->title();
        QString interfaceBoxName = getInterfaceBoxName(pair);

        int min = QLineEdit::Normal;
        int max = QLineEdit::PasswordEchoOnEdit;

        bool ok = notInvalid(__FUNCTION__, groupBoxName, interfaceBoxName,
                             "echoMode", echoMode, min, max);

        if(ok)
        {
            if((pair.second->echoMode() != QLineEdit::Normal)
            && (echoMode == QLineEdit::Normal))
            {
                pair.second->m_blockReturnPressedSignal = false;
                pair.second->m_m->setStringList(pair.second->m_backupList);
                pair.second->m_backupList = QStringList();
            }

            if((pair.second->echoMode() == QLineEdit::Normal)
            && (echoMode != QLineEdit::Normal))
            {
                pair.second->m_blockReturnPressedSignal = true;
                pair.second->m_backupList = pair.second->m_m->stringList();
                pair.second->m_m->setStringList(QStringList());
            }

            pair.second->setEchoMode(echoMode);
        }

        return ok;
    }

    return pair.second;
}

QLineEdit::EchoMode SerialInterface::getLineEditEchoMode(int groupBox,
                                                         int interfaceBox,
                                                         bool *ok)
{
    QPair<MyGroupBox *, MyLineEdit *> pair = interfaceBoxCast
    <MyLineEdit>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->echoMode();
    }

    return QLineEdit::EchoMode();
}

bool SerialInterface::setLineEditInputMask(int groupBox,
                                           int interfaceBox,
                                           const QString &
                                           inputMask)
{
    QPair<MyGroupBox *, MyLineEdit *> pair = interfaceBoxCast
    <MyLineEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setInputMask(inputMask);
    }

    return pair.second;
}

QString SerialInterface::getLineEditInputMask(int groupBox,
                                              int interfaceBox,
                                              bool *ok)
{
    QPair<MyGroupBox *, MyLineEdit *> pair = interfaceBoxCast
    <MyLineEdit>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->inputMask();
    }

    return QString();
}

bool SerialInterface::setLineEditPlaceholderText(int groupBox,
                                                 int interfaceBox,
                                                 const QString &
                                                 placeholderText)
{
    QPair<MyGroupBox *, MyLineEdit *> pair = interfaceBoxCast
    <MyLineEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setPlaceholderText(placeholderText);
    }

    return pair.second;
}

QString SerialInterface::getLineEditPlaceholderText(int groupBox,
                                                    int interfaceBox,
                                                    bool *ok)
{
    QPair<MyGroupBox *, MyLineEdit *> pair = interfaceBoxCast
    <MyLineEdit>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->placeholderText();
    }

    return QString();
}

bool SerialInterface::setLineEditText(int groupBox,
                                      int interfaceBox,
                                      const QString &text)
{
    QPair<MyGroupBox *, MyLineEdit *> pair = interfaceBoxCast
    <MyLineEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setText(text);
    }

    return pair.second;
}

QString SerialInterface::getLineEditText(int groupBox,
                                         int interfaceBox,
                                         bool *ok)
{
    QPair<MyGroupBox *, MyLineEdit *> pair = interfaceBoxCast
    <MyLineEdit>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->text();
    }

    return QString();
}

bool SerialInterface::resetIntSpinBox(int groupBox,
                                      int interfaceBox)
{
    QPair<MyGroupBox *, MyIntSpinBox *> pair = interfaceBoxCast
    <MyIntSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setValue(pair.second->minimum());
    }

    return pair.second;
}

bool SerialInterface::setIntSpinBoxName(int groupBox,
                                        int interfaceBox,
                                        const QString &name)
{
    QPair<MyGroupBox *, MyIntSpinBox *> pair = interfaceBoxCast
    <MyIntSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        setInterfaceBoxName(pair, name);
    }

    return pair.second;
}

QString SerialInterface::getIntSpinBoxName(int groupBox,
                                           int interfaceBox,
                                           bool *ok)
{
    QPair<MyGroupBox *, MyIntSpinBox *> pair = interfaceBoxCast
    <MyIntSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return getInterfaceBoxName(pair);
    }

    return QString();
}

bool SerialInterface::setIntSpinBoxPrefix(int groupBox,
                                          int interfaceBox,
                                          const QString &prefix)
{
    QPair<MyGroupBox *, MyIntSpinBox *> pair = interfaceBoxCast
    <MyIntSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setPrefix(prefix);
    }

    return pair.second;
}

QString SerialInterface::getIntSpinBoxPrefix(int groupBox,
                                             int interfaceBox,
                                             bool *ok)
{
    QPair<MyGroupBox *, MyIntSpinBox *> pair = interfaceBoxCast
    <MyIntSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->prefix();
    }

    return QString();
}

bool SerialInterface::setIntSpinBoxSuffix(int groupBox,
                                          int interfaceBox,
                                          const QString &suffix)
{
    QPair<MyGroupBox *, MyIntSpinBox *> pair = interfaceBoxCast
    <MyIntSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setSuffix(suffix);
    }

    return pair.second;
}

QString SerialInterface::getIntSpinBoxSuffix(int groupBox,
                                             int interfaceBox,
                                             bool *ok)
{
    QPair<MyGroupBox *, MyIntSpinBox *> pair = interfaceBoxCast
    <MyIntSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->suffix();
    }

    return QString();
}

bool SerialInterface::setIntSpinBoxSingleStep(int groupBox,
                                              int interfaceBox,
                                              int singleStep)
{
    QPair<MyGroupBox *, MyIntSpinBox *> pair = interfaceBoxCast
    <MyIntSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setSingleStep(singleStep);
    }

    return pair.second;
}

int SerialInterface::getIntSpinBoxSingleStep(int groupBox,
                                             int interfaceBox,
                                             bool *ok)
{
    QPair<MyGroupBox *, MyIntSpinBox *> pair = interfaceBoxCast
    <MyIntSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->singleStep();
    }

    return int();
}

bool SerialInterface::setIntSpinBoxPageStep(int groupBox,
                                            int interfaceBox,
                                            int pageStep)
{
    QPair<MyGroupBox *, MyIntSpinBox *> pair = interfaceBoxCast
    <MyIntSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        Q_UNUSED(pageStep); // pair.second->setPageStep(pageStep);
    }

    return pair.second;
}

int SerialInterface::getIntSpinBoxPageStep(int groupBox,
                                           int interfaceBox,
                                           bool *ok)
{
    QPair<MyGroupBox *, MyIntSpinBox *> pair = interfaceBoxCast
    <MyIntSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        // return pair.second->pageStep();
    }

    return int();
}

bool SerialInterface::setIntSpinBoxMinimum(int groupBox,
                                           int interfaceBox,
                                           int min)
{
    QPair<MyGroupBox *, MyIntSpinBox *> pair = interfaceBoxCast
    <MyIntSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setMinimum(min);
    }

    return pair.second;
}

int SerialInterface::getIntSpinBoxMinimum(int groupBox,
                                          int interfaceBox,
                                          bool *ok)
{
    QPair<MyGroupBox *, MyIntSpinBox *> pair = interfaceBoxCast
    <MyIntSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->minimum();
    }

    return int();
}

bool SerialInterface::setIntSpinBoxMaximum(int groupBox,
                                           int interfaceBox,
                                           int max)
{
    QPair<MyGroupBox *, MyIntSpinBox *> pair = interfaceBoxCast
    <MyIntSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setMaximum(max);
    }

    return pair.second;
}

int SerialInterface::getIntSpinBoxMaximum(int groupBox,
                                          int interfaceBox,
                                          bool *ok)
{
    QPair<MyGroupBox *, MyIntSpinBox *> pair = interfaceBoxCast
    <MyIntSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->maximum();
    }

    return int();
}

bool SerialInterface::setIntSpinBoxValue(int groupBox,
                                         int interfaceBox,
                                         int value)
{
    QPair<MyGroupBox *, MyIntSpinBox *> pair = interfaceBoxCast
    <MyIntSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        QString groupBoxName = pair.first->title();
        QString interfaceBoxName = getInterfaceBoxName(pair);

        int min = pair.second->minimum();
        int max = pair.second->maximum();

        bool ok = notInvalid(__FUNCTION__, groupBoxName, interfaceBoxName,
                             "value", value, min, max);

        if(ok)
        {
            pair.second->setValue(value);
        }

        return ok;
    }

    return pair.second;
}

int SerialInterface::getIntSpinBoxValue(int groupBox,
                                        int interfaceBox,
                                        bool *ok)
{
    QPair<MyGroupBox *, MyIntSpinBox *> pair = interfaceBoxCast
    <MyIntSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->value();
    }

    return int();
}

bool SerialInterface::setIntSpinBoxSpecialValueText(int groupBox,
                                                    int interfaceBox,
                                                    const QString &
                                                    specialValueText)
{
    QPair<MyGroupBox *, MyIntSpinBox *> pair = interfaceBoxCast
    <MyIntSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setSpecialValueText(specialValueText);
    }

    return pair.second;
}

QString SerialInterface::getIntSpinBoxSpecialValueText(int groupBox,
                                                       int interfaceBox,
                                                       bool *ok)
{
    QPair<MyGroupBox *, MyIntSpinBox *> pair = interfaceBoxCast
    <MyIntSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->specialValueText();
    }

    return QString();
}

bool SerialInterface::resetBinSpinBox(int groupBox,
                                      int interfaceBox)
{
    QPair<MyGroupBox *, MyBinSpinBox *> pair = interfaceBoxCast
    <MyBinSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setValue(pair.second->minimum());
    }

    return pair.second;
}

bool SerialInterface::setBinSpinBoxName(int groupBox,
                                        int interfaceBox,
                                        const QString &name)
{
    QPair<MyGroupBox *, MyBinSpinBox *> pair = interfaceBoxCast
    <MyBinSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        setInterfaceBoxName(pair, name);
    }

    return pair.second;
}

QString SerialInterface::getBinSpinBoxName(int groupBox,
                                           int interfaceBox,
                                           bool *ok)
{
    QPair<MyGroupBox *, MyBinSpinBox *> pair = interfaceBoxCast
    <MyBinSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return getInterfaceBoxName(pair);
    }

    return QString();
}

bool SerialInterface::setBinSpinBoxPrefix(int groupBox,
                                          int interfaceBox,
                                          const QString &prefix)
{
    QPair<MyGroupBox *, MyBinSpinBox *> pair = interfaceBoxCast
    <MyBinSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setPrefix(prefix);
    }

    return pair.second;
}

QString SerialInterface::getBinSpinBoxPrefix(int groupBox,
                                             int interfaceBox,
                                             bool *ok)
{
    QPair<MyGroupBox *, MyBinSpinBox *> pair = interfaceBoxCast
    <MyBinSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->prefix();
    }

    return QString();
}

bool SerialInterface::setBinSpinBoxSuffix(int groupBox,
                                          int interfaceBox,
                                          const QString &suffix)
{
    QPair<MyGroupBox *, MyBinSpinBox *> pair = interfaceBoxCast
    <MyBinSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setSuffix(suffix);
    }

    return pair.second;
}

QString SerialInterface::getBinSpinBoxSuffix(int groupBox,
                                             int interfaceBox,
                                             bool *ok)
{
    QPair<MyGroupBox *, MyBinSpinBox *> pair = interfaceBoxCast
    <MyBinSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->suffix();
    }

    return QString();
}

bool SerialInterface::setBinSpinBoxSingleStep(int groupBox,
                                              int interfaceBox,
                                              int singleStep)
{
    QPair<MyGroupBox *, MyBinSpinBox *> pair = interfaceBoxCast
    <MyBinSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setSingleStep(singleStep);
    }

    return pair.second;
}

int SerialInterface::getBinSpinBoxSingleStep(int groupBox,
                                             int interfaceBox,
                                             bool *ok)
{
    QPair<MyGroupBox *, MyBinSpinBox *> pair = interfaceBoxCast
    <MyBinSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->singleStep();
    }

    return int();
}

bool SerialInterface::setBinSpinBoxPageStep(int groupBox,
                                            int interfaceBox,
                                            int pageStep)
{
    QPair<MyGroupBox *, MyBinSpinBox *> pair = interfaceBoxCast
    <MyBinSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        Q_UNUSED(pageStep); // pair.second->setPageStep(pageStep);
    }

    return pair.second;
}

int SerialInterface::getBinSpinBoxPageStep(int groupBox,
                                           int interfaceBox,
                                           bool *ok)
{
    QPair<MyGroupBox *, MyBinSpinBox *> pair = interfaceBoxCast
    <MyBinSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        // return pair.second->pageStep();
    }

    return int();
}

bool SerialInterface::setBinSpinBoxMinimum(int groupBox,
                                           int interfaceBox,
                                           int min)
{
    QPair<MyGroupBox *, MyBinSpinBox *> pair = interfaceBoxCast
    <MyBinSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setMinimum(min);
    }

    return pair.second;
}

int SerialInterface::getBinSpinBoxMinimum(int groupBox,
                                          int interfaceBox,
                                          bool *ok)
{
    QPair<MyGroupBox *, MyBinSpinBox *> pair = interfaceBoxCast
    <MyBinSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->minimum();
    }

    return int();
}

bool SerialInterface::setBinSpinBoxMaximum(int groupBox,
                                           int interfaceBox,
                                           int max)
{
    QPair<MyGroupBox *, MyBinSpinBox *> pair = interfaceBoxCast
    <MyBinSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setMaximum(max);
    }

    return pair.second;
}

int SerialInterface::getBinSpinBoxMaximum(int groupBox,
                                          int interfaceBox,
                                          bool *ok)
{
    QPair<MyGroupBox *, MyBinSpinBox *> pair = interfaceBoxCast
    <MyBinSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->maximum();
    }

    return int();
}

bool SerialInterface::setBinSpinBoxValue(int groupBox,
                                         int interfaceBox,
                                         int value)
{
    QPair<MyGroupBox *, MyBinSpinBox *> pair = interfaceBoxCast
    <MyBinSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        QString groupBoxName = pair.first->title();
        QString interfaceBoxName = getInterfaceBoxName(pair);

        int min = pair.second->minimum();
        int max = pair.second->maximum();

        bool ok = notInvalid(__FUNCTION__, groupBoxName, interfaceBoxName,
                             "value", value, min, max);

        if(ok)
        {
            pair.second->setValue(value);
        }

        return ok;
    }

    return pair.second;
}

int SerialInterface::getBinSpinBoxValue(int groupBox,
                                        int interfaceBox,
                                        bool *ok)
{
    QPair<MyGroupBox *, MyBinSpinBox *> pair = interfaceBoxCast
    <MyBinSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->value();
    }

    return int();
}

bool SerialInterface::setBinSpinBoxSpecialValueText(int groupBox,
                                                    int interfaceBox,
                                                    const QString &
                                                    specialValueText)
{
    QPair<MyGroupBox *, MyBinSpinBox *> pair = interfaceBoxCast
    <MyBinSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setSpecialValueText(specialValueText);
    }

    return pair.second;
}

QString SerialInterface::getBinSpinBoxSpecialValueText(int groupBox,
                                                       int interfaceBox,
                                                       bool *ok)
{
    QPair<MyGroupBox *, MyBinSpinBox *> pair = interfaceBoxCast
    <MyBinSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->specialValueText();
    }

    return QString();
}

bool SerialInterface::resetHexSpinBox(int groupBox,
                                      int interfaceBox)
{
    QPair<MyGroupBox *, MyHexSpinBox *> pair = interfaceBoxCast
    <MyHexSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setValue(pair.second->minimum());
    }

    return pair.second;
}

bool SerialInterface::setHexSpinBoxName(int groupBox,
                                        int interfaceBox,
                                        const QString &name)
{
    QPair<MyGroupBox *, MyHexSpinBox *> pair = interfaceBoxCast
    <MyHexSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        setInterfaceBoxName(pair, name);
    }

    return pair.second;
}

QString SerialInterface::getHexSpinBoxName(int groupBox,
                                           int interfaceBox,
                                           bool *ok)
{
    QPair<MyGroupBox *, MyHexSpinBox *> pair = interfaceBoxCast
    <MyHexSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return getInterfaceBoxName(pair);
    }

    return QString();
}

bool SerialInterface::setHexSpinBoxPrefix(int groupBox,
                                          int interfaceBox,
                                          const QString &prefix)
{
    QPair<MyGroupBox *, MyHexSpinBox *> pair = interfaceBoxCast
    <MyHexSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setPrefix(prefix);
    }

    return pair.second;
}

QString SerialInterface::getHexSpinBoxPrefix(int groupBox,
                                             int interfaceBox,
                                             bool *ok)
{
    QPair<MyGroupBox *, MyHexSpinBox *> pair = interfaceBoxCast
    <MyHexSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->prefix();
    }

    return QString();
}

bool SerialInterface::setHexSpinBoxSuffix(int groupBox,
                                          int interfaceBox,
                                          const QString &suffix)
{
    QPair<MyGroupBox *, MyHexSpinBox *> pair = interfaceBoxCast
    <MyHexSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setSuffix(suffix);
    }

    return pair.second;
}

QString SerialInterface::getHexSpinBoxSuffix(int groupBox,
                                             int interfaceBox,
                                             bool *ok)
{
    QPair<MyGroupBox *, MyHexSpinBox *> pair = interfaceBoxCast
    <MyHexSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->suffix();
    }

    return QString();
}

bool SerialInterface::setHexSpinBoxSingleStep(int groupBox,
                                              int interfaceBox,
                                              int singleStep)
{
    QPair<MyGroupBox *, MyHexSpinBox *> pair = interfaceBoxCast
    <MyHexSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setSingleStep(singleStep);
    }

    return pair.second;
}

int SerialInterface::getHexSpinBoxSingleStep(int groupBox,
                                             int interfaceBox,
                                             bool *ok)
{
    QPair<MyGroupBox *, MyHexSpinBox *> pair = interfaceBoxCast
    <MyHexSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->singleStep();
    }

    return int();
}

bool SerialInterface::setHexSpinBoxPageStep(int groupBox,
                                            int interfaceBox,
                                            int pageStep)
{
    QPair<MyGroupBox *, MyHexSpinBox *> pair = interfaceBoxCast
    <MyHexSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        Q_UNUSED(pageStep); // pair.second->setPageStep(pageStep);
    }

    return pair.second;
}

int SerialInterface::getHexSpinBoxPageStep(int groupBox,
                                           int interfaceBox,
                                           bool *ok)
{
    QPair<MyGroupBox *, MyHexSpinBox *> pair = interfaceBoxCast
    <MyHexSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        // return pair.second->pageStep();
    }

    return int();
}

bool SerialInterface::setHexSpinBoxMinimum(int groupBox,
                                           int interfaceBox,
                                           int min)
{
    QPair<MyGroupBox *, MyHexSpinBox *> pair = interfaceBoxCast
    <MyHexSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setMinimum(min);
    }

    return pair.second;
}

int SerialInterface::getHexSpinBoxMinimum(int groupBox,
                                          int interfaceBox,
                                          bool *ok)
{
    QPair<MyGroupBox *, MyHexSpinBox *> pair = interfaceBoxCast
    <MyHexSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->minimum();
    }

    return int();
}

bool SerialInterface::setHexSpinBoxMaximum(int groupBox,
                                           int interfaceBox,
                                           int max)
{
    QPair<MyGroupBox *, MyHexSpinBox *> pair = interfaceBoxCast
    <MyHexSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setMaximum(max);
    }

    return pair.second;
}

int SerialInterface::getHexSpinBoxMaximum(int groupBox,
                                          int interfaceBox,
                                          bool *ok)
{
    QPair<MyGroupBox *, MyHexSpinBox *> pair = interfaceBoxCast
    <MyHexSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->maximum();
    }

    return int();
}

bool SerialInterface::setHexSpinBoxValue(int groupBox,
                                         int interfaceBox,
                                         int value)
{
    QPair<MyGroupBox *, MyHexSpinBox *> pair = interfaceBoxCast
    <MyHexSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        QString groupBoxName = pair.first->title();
        QString interfaceBoxName = getInterfaceBoxName(pair);

        int min = pair.second->minimum();
        int max = pair.second->maximum();

        bool ok = notInvalid(__FUNCTION__, groupBoxName, interfaceBoxName,
                             "value", value, min, max);

        if(ok)
        {
            pair.second->setValue(value);
        }

        return ok;
    }

    return pair.second;
}

int SerialInterface::getHexSpinBoxValue(int groupBox,
                                        int interfaceBox,
                                        bool *ok)
{
    QPair<MyGroupBox *, MyHexSpinBox *> pair = interfaceBoxCast
    <MyHexSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->value();
    }

    return int();
}

bool SerialInterface::setHexSpinBoxSpecialValueText(int groupBox,
                                                    int interfaceBox,
                                                    const QString &
                                                    specialValueText)
{
    QPair<MyGroupBox *, MyHexSpinBox *> pair = interfaceBoxCast
    <MyHexSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setSpecialValueText(specialValueText);
    }

    return pair.second;
}

QString SerialInterface::getHexSpinBoxSpecialValueText(int groupBox,
                                                       int interfaceBox,
                                                       bool *ok)
{
    QPair<MyGroupBox *, MyHexSpinBox *> pair = interfaceBoxCast
    <MyHexSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->specialValueText();
    }

    return QString();
}

bool SerialInterface::resetDoubleSpinBox(int groupBox,
                                         int interfaceBox)
{
    QPair<MyGroupBox *, MyDoubleSpinBox *> pair = interfaceBoxCast
    <MyDoubleSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setValue(pair.second->minimum());
    }

    return pair.second;
}

bool SerialInterface::setDoubleSpinBoxName(int groupBox,
                                           int interfaceBox,
                                           const QString &name)
{
    QPair<MyGroupBox *, MyDoubleSpinBox *> pair = interfaceBoxCast
    <MyDoubleSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        setInterfaceBoxName(pair, name);
    }

    return pair.second;
}

QString SerialInterface::getDoubleSpinBoxName(int groupBox,
                                              int interfaceBox,
                                              bool *ok)
{
    QPair<MyGroupBox *, MyDoubleSpinBox *> pair = interfaceBoxCast
    <MyDoubleSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return getInterfaceBoxName(pair);
    }

    return QString();
}

bool SerialInterface::setDoubleSpinBoxPrefix(int groupBox,
                                             int interfaceBox,
                                             const QString &prefix)
{
    QPair<MyGroupBox *, MyDoubleSpinBox *> pair = interfaceBoxCast
    <MyDoubleSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setPrefix(prefix);
    }

    return pair.second;
}

QString SerialInterface::getDoubleSpinBoxPrefix(int groupBox,
                                                int interfaceBox,
                                                bool *ok)
{
    QPair<MyGroupBox *, MyDoubleSpinBox *> pair = interfaceBoxCast
    <MyDoubleSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->prefix();
    }

    return QString();
}

bool SerialInterface::setDoubleSpinBoxSuffix(int groupBox,
                                             int interfaceBox,
                                             const QString &suffix)
{
    QPair<MyGroupBox *, MyDoubleSpinBox *> pair = interfaceBoxCast
    <MyDoubleSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setSuffix(suffix);
    }

    return pair.second;
}

QString SerialInterface::getDoubleSpinBoxSuffix(int groupBox,
                                                int interfaceBox,
                                                bool *ok)
{
    QPair<MyGroupBox *, MyDoubleSpinBox *> pair = interfaceBoxCast
    <MyDoubleSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->suffix();
    }

    return QString();
}

bool SerialInterface::setDoubleSpinBoxSingleStep(int groupBox,
                                                 int interfaceBox,
                                                 double singleStep)
{
    QPair<MyGroupBox *, MyDoubleSpinBox *> pair = interfaceBoxCast
    <MyDoubleSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        QString groupBoxName = pair.first->title();
        QString interfaceBoxName = getInterfaceBoxName(pair);

        bool ok = notInfinite(__FUNCTION__, groupBoxName, interfaceBoxName,
                              "singleStep", singleStep);

        if(ok)
        {
            pair.second->setSingleStep(singleStep);
        }

        return ok;
    }

    return pair.second;
}

double SerialInterface::getDoubleSpinBoxSingleStep(int groupBox,
                                                   int interfaceBox,
                                                   bool *ok)
{
    QPair<MyGroupBox *, MyDoubleSpinBox *> pair = interfaceBoxCast
    <MyDoubleSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->singleStep();
    }

    return double();
}

bool SerialInterface::setDoubleSpinBoxPageStep(int groupBox,
                                               int interfaceBox,
                                               double pageStep)
{
    QPair<MyGroupBox *, MyDoubleSpinBox *> pair = interfaceBoxCast
    <MyDoubleSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        QString groupBoxName = pair.first->title();
        QString interfaceBoxName = getInterfaceBoxName(pair);

        bool ok = notInfinite(__FUNCTION__, groupBoxName, interfaceBoxName,
                              "pageStep", pageStep);

        if(ok)
        {
            Q_UNUSED(pageStep); // pair.second->setPageStep(pageStep);
        }

        return ok;
    }

    return pair.second;
}

double SerialInterface::getDoubleSpinBoxPageStep(int groupBox,
                                                 int interfaceBox,
                                                 bool *ok)
{
    QPair<MyGroupBox *, MyDoubleSpinBox *> pair = interfaceBoxCast
    <MyDoubleSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        // return pair.second->pageStep();
    }

    return double();
}

bool SerialInterface::setDoubleSpinBoxMinimum(int groupBox,
                                              int interfaceBox,
                                              double min)
{
    QPair<MyGroupBox *, MyDoubleSpinBox *> pair = interfaceBoxCast
    <MyDoubleSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        QString groupBoxName = pair.first->title();
        QString interfaceBoxName = getInterfaceBoxName(pair);

        bool ok = notInfinite(__FUNCTION__, groupBoxName, interfaceBoxName,
                              "min", min);

        if(ok)
        {
            pair.second->setMinimum(min);
        }

        return ok;
    }

    return pair.second;
}

double SerialInterface::getDoubleSpinBoxMinimum(int groupBox,
                                                int interfaceBox,
                                                bool *ok)
{
    QPair<MyGroupBox *, MyDoubleSpinBox *> pair = interfaceBoxCast
    <MyDoubleSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->minimum();
    }

    return double();
}

bool SerialInterface::setDoubleSpinBoxMaximum(int groupBox,
                                              int interfaceBox,
                                              double max)
{
    QPair<MyGroupBox *, MyDoubleSpinBox *> pair = interfaceBoxCast
    <MyDoubleSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        QString groupBoxName = pair.first->title();
        QString interfaceBoxName = getInterfaceBoxName(pair);

        bool ok = notInfinite(__FUNCTION__, groupBoxName, interfaceBoxName,
                              "max", max);

        if(ok)
        {
            pair.second->setMaximum(max);
        }

        return ok;
    }

    return pair.second;
}

double SerialInterface::getDoubleSpinBoxMaximum(int groupBox,
                                                int interfaceBox,
                                                bool *ok)
{
    QPair<MyGroupBox *, MyDoubleSpinBox *> pair = interfaceBoxCast
    <MyDoubleSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->maximum();
    }

    return double();
}


bool SerialInterface::setDoubleSpinBoxValue(int groupBox,
                                            int interfaceBox,
                                            double value)
{
    QPair<MyGroupBox *, MyDoubleSpinBox *> pair = interfaceBoxCast
    <MyDoubleSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        QString groupBoxName = pair.first->title();
        QString interfaceBoxName = getInterfaceBoxName(pair);

        double min = pair.second->minimum();
        double max = pair.second->maximum();

        bool ok = notInvalid(__FUNCTION__, groupBoxName, interfaceBoxName,
                             "value", value, min, max);

        if(ok)
        {
            pair.second->setValue(value);
        }

        return ok;
    }

    return pair.second;
}

double SerialInterface::getDoubleSpinBoxValue(int groupBox,
                                              int interfaceBox,
                                              bool *ok)
{
    QPair<MyGroupBox *, MyDoubleSpinBox *> pair = interfaceBoxCast
    <MyDoubleSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->value();
    }

    return double();
}

bool SerialInterface::setDoubleSpinBoxSpecialValueText(int groupBox,
                                                       int interfaceBox,
                                                       const QString &
                                                       specialValueText)
{
    QPair<MyGroupBox *, MyDoubleSpinBox *> pair = interfaceBoxCast
    <MyDoubleSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setSpecialValueText(specialValueText);
    }

    return pair.second;
}

QString SerialInterface::getDoubleSpinBoxSpecialValueText(int groupBox,
                                                          int interfaceBox,
                                                          bool *ok)
{
    QPair<MyGroupBox *, MyDoubleSpinBox *> pair = interfaceBoxCast
    <MyDoubleSpinBox>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->specialValueText();
    }

    return QString();
}

bool SerialInterface::resetTimeEdit(int groupBox,
                                    int interfaceBox)
{
    QPair<MyGroupBox *, MyTimeEdit *> pair = interfaceBoxCast
    <MyTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setTime(pair.second->minimumTime());
    }

    return pair.second;
}

bool SerialInterface::setTimeEditName(int groupBox,
                                      int interfaceBox,
                                      const QString &name)
{
    QPair<MyGroupBox *, MyTimeEdit *> pair = interfaceBoxCast
    <MyTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        setInterfaceBoxName(pair, name);
    }

    return pair.second;
}

QString SerialInterface::getTimeEditName(int groupBox,
                                         int interfaceBox,
                                         bool *ok)
{
    QPair<MyGroupBox *, MyTimeEdit *> pair = interfaceBoxCast
    <MyTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return getInterfaceBoxName(pair);
    }

    return QString();
}

bool SerialInterface::setTimeEditMinimum(int groupBox,
                                         int interfaceBox,
                                         const QString &min)
{
    QPair<MyGroupBox *, MyTimeEdit *> pair = interfaceBoxCast
    <MyTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setMinimumTime(QTime::fromString(min, Qt::ISODate));
    }

    return pair.second;
}

QString SerialInterface::getTimeEditMinimum(int groupBox,
                                            int interfaceBox,
                                            bool *ok)
{
    QPair<MyGroupBox *, MyTimeEdit *> pair = interfaceBoxCast
    <MyTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->minimumTime().toString(Qt::ISODate);
    }

    return QString();
}

bool SerialInterface::setTimeEditMaximum(int groupBox,
                                         int interfaceBox,
                                         const QString &max)
{
    QPair<MyGroupBox *, MyTimeEdit *> pair = interfaceBoxCast
    <MyTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setMaximumTime(QTime::fromString(max, Qt::ISODate));
    }

    return pair.second;
}

QString SerialInterface::getTimeEditMaximum(int groupBox,
                                            int interfaceBox,
                                            bool *ok)
{
    QPair<MyGroupBox *, MyTimeEdit *> pair = interfaceBoxCast
    <MyTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->maximumTime().toString(Qt::ISODate);
    }

    return QString();
}

bool SerialInterface::setTimeEditValue(int groupBox,
                                       int interfaceBox,
                                       const QString &value)
{
    QPair<MyGroupBox *, MyTimeEdit *> pair = interfaceBoxCast
    <MyTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setTime(QTime::fromString(value, Qt::ISODate));
    }

    return pair.second;
}

QString SerialInterface::getTimeEditValue(int groupBox,
                                          int interfaceBox,
                                          bool *ok)
{
    QPair<MyGroupBox *, MyTimeEdit *> pair = interfaceBoxCast
    <MyTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->time().toString(Qt::ISODate);
    }

    return QString();
}

bool SerialInterface::setTimeEditSpecialValueText(int groupBox,
                                                  int interfaceBox,
                                                  const QString &
                                                  specialValueText)
{
    QPair<MyGroupBox *, MyTimeEdit *> pair = interfaceBoxCast
    <MyTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setSpecialValueText(specialValueText);
    }

    return pair.second;
}

QString SerialInterface::getTimeEditSpecialValueText(int groupBox,
                                                     int interfaceBox,
                                                     bool *ok)
{
    QPair<MyGroupBox *, MyTimeEdit *> pair = interfaceBoxCast
    <MyTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->specialValueText();
    }

    return QString();
}

bool SerialInterface::resetDateEdit(int groupBox,
                                    int interfaceBox)
{
    QPair<MyGroupBox *, MyDateEdit *> pair = interfaceBoxCast
    <MyDateEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setDate(pair.second->minimumDate());
    }

    return pair.second;
}

bool SerialInterface::setDateEditName(int groupBox,
                                      int interfaceBox,
                                      const QString &name)
{
    QPair<MyGroupBox *, MyDateEdit *> pair = interfaceBoxCast
    <MyDateEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        setInterfaceBoxName(pair, name);
    }

    return pair.second;
}

QString SerialInterface::getDateEditName(int groupBox,
                                         int interfaceBox,
                                         bool *ok)
{
    QPair<MyGroupBox *, MyDateEdit *> pair = interfaceBoxCast
    <MyDateEdit>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return getInterfaceBoxName(pair);
    }

    return QString();
}

bool SerialInterface::setDateEditMinimum(int groupBox,
                                         int interfaceBox,
                                         const QString &min)
{
    QPair<MyGroupBox *, MyDateEdit *> pair = interfaceBoxCast
    <MyDateEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setMinimumDate(QDate::fromString(min, Qt::ISODate));
    }

    return pair.second;
}

QString SerialInterface::getDateEditMinimum(int groupBox,
                                            int interfaceBox,
                                            bool *ok)
{
    QPair<MyGroupBox *, MyDateEdit *> pair = interfaceBoxCast
    <MyDateEdit>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->minimumDate().toString(Qt::ISODate);
    }

    return QString();
}

bool SerialInterface::setDateEditMaximum(int groupBox,
                                         int interfaceBox,
                                         const QString &max)
{
    QPair<MyGroupBox *, MyDateEdit *> pair = interfaceBoxCast
    <MyDateEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setMaximumDate(QDate::fromString(max, Qt::ISODate));
    }

    return pair.second;
}

QString SerialInterface::getDateEditMaximum(int groupBox,
                                            int interfaceBox,
                                            bool *ok)
{
    QPair<MyGroupBox *, MyDateEdit *> pair = interfaceBoxCast
    <MyDateEdit>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->maximumDate().toString(Qt::ISODate);
    }

    return QString();
}

bool SerialInterface::setDateEditValue(int groupBox,
                                       int interfaceBox,
                                       const QString &value)
{
    QPair<MyGroupBox *, MyDateEdit *> pair = interfaceBoxCast
    <MyDateEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setDate(QDate::fromString(value, Qt::ISODate));
    }

    return pair.second;
}

QString SerialInterface::getDateEditValue(int groupBox,
                                          int interfaceBox,
                                          bool *ok)
{
    QPair<MyGroupBox *, MyDateEdit *> pair = interfaceBoxCast
    <MyDateEdit>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->date().toString(Qt::ISODate);
    }

    return QString();
}

bool SerialInterface::setDateEditSpecialValueText(int groupBox,
                                                  int interfaceBox,
                                                  const QString &
                                                  specialValueText)
{
    QPair<MyGroupBox *, MyDateEdit *> pair = interfaceBoxCast
    <MyDateEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setSpecialValueText(specialValueText);
    }

    return pair.second;
}

QString SerialInterface::getDateEditSpecialValueText(int groupBox,
                                                     int interfaceBox,
                                                     bool *ok)
{
    QPair<MyGroupBox *, MyDateEdit *> pair = interfaceBoxCast
    <MyDateEdit>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->specialValueText();
    }

    return QString();
}

bool SerialInterface::resetDateTimeEdit(int groupBox,
                                        int interfaceBox)
{
    QPair<MyGroupBox *, MyDateTimeEdit *> pair = interfaceBoxCast
    <MyDateTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setDateTime(pair.second->minimumDateTime());
    }

    return pair.second;
}

bool SerialInterface::setDateTimeEditName(int groupBox,
                                          int interfaceBox,
                                          const QString &name)
{
    QPair<MyGroupBox *, MyDateTimeEdit *> pair = interfaceBoxCast
    <MyDateTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        setInterfaceBoxName(pair, name);
    }

    return pair.second;
}

QString SerialInterface::getDateTimeEditName(int groupBox,
                                             int interfaceBox,
                                             bool *ok)
{
    QPair<MyGroupBox *, MyDateTimeEdit *> pair = interfaceBoxCast
    <MyDateTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return getInterfaceBoxName(pair);
    }

    return QString();
}

bool SerialInterface::setDateTimeEditMinimum(int groupBox,
                                             int interfaceBox,
                                             const QString &min)
{
    QPair<MyGroupBox *, MyDateTimeEdit *> pair = interfaceBoxCast
    <MyDateTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setMinimumDateTime(QDateTime::fromString(min,
                                                              Qt::ISODate));
    }

    return pair.second;
}

QString SerialInterface::getDateTimeEditMinimum(int groupBox,
                                                int interfaceBox,
                                                bool *ok)
{
    QPair<MyGroupBox *, MyDateTimeEdit *> pair = interfaceBoxCast
    <MyDateTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->minimumDateTime().toString(Qt::ISODate);
    }

    return QString();
}

bool SerialInterface::setDateTimeEditMaximum(int groupBox,
                                             int interfaceBox,
                                             const QString &max)
{
    QPair<MyGroupBox *, MyDateTimeEdit *> pair = interfaceBoxCast
    <MyDateTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setMaximumDateTime(QDateTime::fromString(max,
                                                              Qt::ISODate));
    }

    return pair.second;
}

QString SerialInterface::getDateTimeEditMaximum(int groupBox,
                                                int interfaceBox,
                                                bool *ok)
{
    QPair<MyGroupBox *, MyDateTimeEdit *> pair = interfaceBoxCast
    <MyDateTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->maximumDateTime().toString(Qt::ISODate);
    }

    return QString();
}

bool SerialInterface::setDateTimeEditValue(int groupBox,
                                           int interfaceBox,
                                           const QString &value)
{
    QPair<MyGroupBox *, MyDateTimeEdit *> pair = interfaceBoxCast
    <MyDateTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setDateTime(QDateTime::fromString(value, Qt::ISODate));
    }

    return pair.second;
}

QString SerialInterface::getDateTimeEditValue(int groupBox,
                                              int interfaceBox,
                                              bool *ok)
{
    QPair<MyGroupBox *, MyDateTimeEdit *> pair = interfaceBoxCast
    <MyDateTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->dateTime().toString(Qt::ISODate);
    }

    return QString();
}

bool SerialInterface::setDateTimeEditSpecialValueText(int groupBox,
                                                      int interfaceBox,
                                                      const QString &
                                                      specialValueText)
{
    QPair<MyGroupBox *, MyDateTimeEdit *> pair = interfaceBoxCast
    <MyDateTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setSpecialValueText(specialValueText);
    }

    return pair.second;
}

QString SerialInterface::getDateTimeEditSpecialValueText(int groupBox,
                                                         int interfaceBox,
                                                         bool *ok)
{
    QPair<MyGroupBox *, MyDateTimeEdit *> pair = interfaceBoxCast
    <MyDateTimeEdit>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->specialValueText();
    }

    return QString();
}

bool SerialInterface::resetSlider(int groupBox,
                                  int interfaceBox)
{
    QPair<MyGroupBox *, MySlider *> pair = interfaceBoxCast
    <MySlider>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        int min = pair.second->m_slider->minimum();
        int max = pair.second->m_slider->maximum();

        pair.second->m_slider->setValue(((min + max + 0) / 2));
    }

    return pair.second;
}

bool SerialInterface::setSliderName(int groupBox,
                                    int interfaceBox,
                                    const QString &name)
{
    QPair<MyGroupBox *, MySlider *> pair = interfaceBoxCast
    <MySlider>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        setInterfaceBoxName(pair, name);
    }

    return pair.second;
}

QString SerialInterface::getSliderName(int groupBox,
                                       int interfaceBox,
                                       bool *ok)
{
    QPair<MyGroupBox *, MySlider *> pair = interfaceBoxCast
    <MySlider>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return getInterfaceBoxName(pair);
    }

    return QString();
}

bool SerialInterface::setSliderSingleStep(int groupBox,
                                          int interfaceBox,
                                          int singleStep)
{
    QPair<MyGroupBox *, MySlider *> pair = interfaceBoxCast
    <MySlider>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->m_slider->setSingleStep(singleStep);
    }

    return pair.second;
}

int SerialInterface::getSliderSingleStep(int groupBox,
                                         int interfaceBox,
                                         bool *ok)
{
    QPair<MyGroupBox *, MySlider *> pair = interfaceBoxCast
    <MySlider>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->m_slider->singleStep();
    }

    return int();
}

bool SerialInterface::setSliderPageStep(int groupBox,
                                        int interfaceBox,
                                        int pageStep)
{
    QPair<MyGroupBox *, MySlider *> pair = interfaceBoxCast
    <MySlider>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->m_slider->setPageStep(pageStep);
    }

    return pair.second;
}

int SerialInterface::getSliderPageStep(int groupBox,
                                       int interfaceBox,
                                       bool *ok)
{
    QPair<MyGroupBox *, MySlider *> pair = interfaceBoxCast
    <MySlider>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->m_slider->pageStep();
    }

    return int();
}

bool SerialInterface::setSliderMinimum(int groupBox,
                                       int interfaceBox,
                                       int min)
{
    QPair<MyGroupBox *, MySlider *> pair = interfaceBoxCast
    <MySlider>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->m_slider->setMinimum(min);
    }

    return pair.second;
}

int SerialInterface::getSliderMinimum(int groupBox,
                                      int interfaceBox,
                                      bool *ok)
{
    QPair<MyGroupBox *, MySlider *> pair = interfaceBoxCast
    <MySlider>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->m_slider->minimum();
    }

    return int();
}

bool SerialInterface::setSliderMaximum(int groupBox,
                                       int interfaceBox,
                                       int max)
{
    QPair<MyGroupBox *, MySlider *> pair = interfaceBoxCast
    <MySlider>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->m_slider->setMaximum(max);
    }

    return pair.second;
}

int SerialInterface::getSliderMaximum(int groupBox,
                                      int interfaceBox,
                                      bool *ok)
{
    QPair<MyGroupBox *, MySlider *> pair = interfaceBoxCast
    <MySlider>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->m_slider->maximum();
    }

    return int();
}

bool SerialInterface::setSliderValue(int groupBox,
                                     int interfaceBox,
                                     int value)
{
    QPair<MyGroupBox *, MySlider *> pair = interfaceBoxCast
    <MySlider>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        QString groupBoxName = pair.first->title();
        QString interfaceBoxName = getInterfaceBoxName(pair);

        int min = pair.second->m_slider->minimum();
        int max = pair.second->m_slider->maximum();

        bool ok = notInvalid(__FUNCTION__, groupBoxName, interfaceBoxName,
                             "value", value, min, max);

        if(ok)
        {
            pair.second->m_slider->setValue(value);
        }

        return ok;
    }

    return pair.second;
}

int SerialInterface::getSliderValue(int groupBox,
                                    int interfaceBox,
                                    bool *ok)
{
    QPair<MyGroupBox *, MySlider *> pair = interfaceBoxCast
    <MySlider>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->m_slider->value();
    }

    return int();
}

bool SerialInterface::setLabelText(int groupBox,
                                   int interfaceBox,
                                   const QString &text)
{
    QPair<MyGroupBox *, MyLabel *> pair = interfaceBoxCast
    <MyLabel>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        setInterfaceBoxName(pair, text);
    }

    return pair.second;
}

QString SerialInterface::getLabelText(int groupBox,
                                      int interfaceBox,
                                      bool *ok)
{
    QPair<MyGroupBox *, MyLabel *> pair = interfaceBoxCast
    <MyLabel>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return getInterfaceBoxName(pair);
    }

    return QString();
}

bool SerialInterface::resetProgressBar(int groupBox,
                                       int interfaceBox)
{
    QPair<MyGroupBox *, MyProgressBar *> pair = interfaceBoxCast
    <MyProgressBar>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->reset();
    }

    return pair.second;
}

bool SerialInterface::setProgressBarName(int groupBox,
                                         int interfaceBox,
                                         const QString &name)
{
    QPair<MyGroupBox *, MyProgressBar *> pair = interfaceBoxCast
    <MyProgressBar>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        setInterfaceBoxName(pair, name);
    }

    return pair.second;
}

QString SerialInterface::getProgressBarName(int groupBox,
                                            int interfaceBox,
                                            bool *ok)
{
    QPair<MyGroupBox *, MyProgressBar *> pair = interfaceBoxCast
    <MyProgressBar>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return getInterfaceBoxName(pair);
    }

    return QString();
}

bool SerialInterface::setProgressBarMinimum(int groupBox,
                                            int interfaceBox,
                                            int min)
{
    QPair<MyGroupBox *, MyProgressBar *> pair = interfaceBoxCast
    <MyProgressBar>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setMinimum(min);
    }

    return pair.second;
}

int SerialInterface::getProgressBarMinimum(int groupBox,
                                           int interfaceBox,
                                           bool *ok)
{
    QPair<MyGroupBox *, MyProgressBar *> pair = interfaceBoxCast
    <MyProgressBar>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->minimum();
    }

    return int();
}

bool SerialInterface::setProgressBarMaximum(int groupBox,
                                            int interfaceBox,
                                            int max)
{
    QPair<MyGroupBox *, MyProgressBar *> pair = interfaceBoxCast
    <MyProgressBar>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.second->setMaximum(max);
    }

    return pair.second;
}

int SerialInterface::getProgressBarMaximum(int groupBox,
                                           int interfaceBox,
                                           bool *ok)
{
    QPair<MyGroupBox *, MyProgressBar *> pair = interfaceBoxCast
    <MyProgressBar>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->maximum();
    }

    return int();
}

bool SerialInterface::setProgressBarValue(int groupBox,
                                          int interfaceBox,
                                          int value)
{
    QPair<MyGroupBox *, MyProgressBar *> pair = interfaceBoxCast
    <MyProgressBar>(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        QString groupBoxName = pair.first->title();
        QString interfaceBoxName = getInterfaceBoxName(pair);

        int min = pair.second->minimum();
        int max = pair.second->maximum();

        bool ok = notInvalid(__FUNCTION__, groupBoxName, interfaceBoxName,
                             "value", value, min, max);

        if(ok)
        {
            pair.second->setValue(value);
        }

        return ok;
    }

    return pair.second;
}

int SerialInterface::getProgressBarValue(int groupBox,
                                         int interfaceBox,
                                         bool *ok)
{
    QPair<MyGroupBox *, MyProgressBar *> pair = interfaceBoxCast
    <MyProgressBar>(__FUNCTION__, groupBox, interfaceBox);

    if((*ok = pair.second))
    {
        return pair.second->value();
    }

    return int();
}

bool SerialInterface::deleteInterfaceBox(int groupBox, int interfaceBox)
{
    QPair<MyGroupBox *, QWidget *> pair =
    interfaceBoxExists(__FUNCTION__, groupBox, interfaceBox);

    if(pair.second)
    {
        pair.first->m_children.remove(interfaceBox);

        QWidget *label = pair.first->m_layout->labelForField(pair.second);

        if(label)
        {
            pair.first->m_layout->removeWidget(label);

            delete label;
        }

        pair.first->m_layout->removeWidget(pair.second);

        delete pair.second;
    }

    return pair.second;
}

bool SerialInterface::deleteAllInterfaceBoxes(int groupBox)
{
    MyGroupBox *gBox = groupBoxExists(__FUNCTION__, groupBox);

    if(gBox)
    {
        foreach(int child, gBox->m_children.keys())
        {
            deleteInterfaceBox(groupBox, child);
        }
    }

    return gBox;
}

void SerialInterface::deleteAllInterfaceBoxes()
{
    foreach(int child, m_children.keys())
    {
        deleteAllInterfaceBoxes(child);
    }
}

bool SerialInterface::deleteGroupBox(int groupBox)
{
    MyGroupBox *gBox = groupBoxExists(__FUNCTION__, groupBox);

    if(gBox)
    {
        m_children.remove(groupBox);
        m_layout->removeWidget(gBox);

        delete gBox;
    }

    return gBox;
}

void SerialInterface::deleteAllGroupBoxes()
{
    foreach(int child, m_children.keys())
    {
        deleteGroupBox(child);
    }
}

void SerialInterface::generalHelp()
{
    if(!QDesktopServices::openUrl(QUrl("http://" PROJECT_DOMAIN_NAME_STR "/"
    "help/general/")))
    {
        QMessageBox::critical(this, tr("Open General Help Error"),
        tr("Unable to open the URL to the General Help page"));
    }
}

void SerialInterface::interfaceHelp()
{
    if(!QDesktopServices::openUrl(QUrl("http://" PROJECT_DOMAIN_NAME_STR "/"
    "help/widgets/interface/")))
    {
        QMessageBox::critical(this, tr("Open Interface Help Error"),
        tr("Unable to open the URL to the Interface Help page"));
    }
}

void
SerialInterface::setInterfaceBoxName(QPair<MyGroupBox *, QWidget *> pair,
                                     const QString &name)
{
    if(qobject_cast<QAbstractButton *>(pair.second))
    {
        static_cast<QAbstractButton *>(pair.second)->setText(name);
    }
    else if(qobject_cast<MyLabel *>(pair.second))
    {
        static_cast<MyLabel *>(pair.second)->setText(name);
    }
    else if(qobject_cast<MySeperator *>(pair.second))
    {
        return;
    }
    else if(pair.second)
    {
        QWidget *wBox = pair.first->m_layout->labelForField(pair.second);
        QLabel *lBox = qobject_cast<QLabel *>(wBox);

        q_check_ptr<QLabel>(lBox)->setText(name);
    }

    return;
}

QString
SerialInterface::getInterfaceBoxName(QPair<MyGroupBox *, QWidget *> pair)
{
    if(qobject_cast<QAbstractButton *>(pair.second))
    {
        return static_cast<QAbstractButton *>(pair.second)->text();
    }
    else if(qobject_cast<MyLabel *>(pair.second))
    {
        return static_cast<MyLabel *>(pair.second)->text();
    }
    else if(qobject_cast<MySeperator *>(pair.second))
    {
        return QString();
    }
    else if(pair.second)
    {
        QWidget *wBox = pair.first->m_layout->labelForField(pair.second);
        QLabel *lBox = qobject_cast<QLabel *>(wBox);

        return q_check_ptr<QLabel>(lBox)->text();
    }

    return QString();
}

template <class T>
QPair<MyGroupBox *, T *>
SerialInterface::interfaceBoxCast(const QString &FName,
                                  int groupBox,
                                  int interfaceBox)
{
    QPair<MyGroupBox *, QWidget *> pair =
    interfaceBoxExists(FName, groupBox, interfaceBox);

    if(pair.second)
    {
        T *iBox = qobject_cast<T *>(pair.second);

        if(!iBox)
        {
            QString instanceClassName = pair.second->metaObject()->className();
            QString templateClassName = T::staticMetaObject.className();

            emit errorMessage(QString(metaObject()->className()) +
            "::" + FName + "[" + windowTitle() + "][" +
            pair.first->title() + "] -> " +
            tr("Interface Box %L1 type cast error (%L2 != %L3)").
            arg(interfaceBox).
            arg(instanceClassName.mid(2)).
            arg(templateClassName.mid(2)));
        }

        return QPair<MyGroupBox *, T *>(pair.first, iBox);
    }

    return QPair<MyGroupBox *, T *>(NULL, NULL);
}

bool SerialInterface::notInfinite(const QString &FName,
                                  const QString &groupBoxName,
                                  const QString &interfaceBoxName,
                                  const QString &VName,
                                  double value)
{
    bool ok = qIsFinite(value);

    if(!ok)
    {
        emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "][" +
        groupBoxName + "][" + interfaceBoxName + "] -> " +
        tr("Argument (%L1 == %L2) is an infinite value").
        arg(VName).arg(value));
    }

    return ok;
}

bool SerialInterface::notZero(const QString &FName,
                              const QString &groupBoxName,
                              const QString &interfaceBoxName,
                              const QString &VName,
                              double value)
{
    bool ok = !qFuzzyCompare(1.0 + value, 1.0);

    if(!ok)
    {
        emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "][" +
        groupBoxName + "][" + interfaceBoxName + "] -> " +
        tr("Argument (%L1 == %L2) is a zero value").
        arg(VName).arg(value));
    }

    return ok;
}

bool SerialInterface::notInvalid(const QString &FName,
                                 const QString &groupBoxName,
                                 const QString &interfaceBoxName,
                                 const QString &VName,
                                 double value,
                                 double minValue,
                                 double maxValue)
{
    bool ok = ((minValue <= value) && (value <= maxValue));

    if(!ok)
    {
        emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "][" +
        groupBoxName + "][" + interfaceBoxName + "] -> " +
        tr("Argument (%L1 == %L2) is out of the valid range between "
        "[%L3 : %L4]").arg(VName).arg(value).arg(minValue).arg(maxValue));
    }

    return ok;
}

QPair<MyGroupBox *, QWidget *>
SerialInterface::interfaceBoxExists(const QString &FName,
                                    int groupBox,
                                    int interfaceBox)
{
    MyGroupBox *gBox = groupBoxExists(FName, groupBox);

    if(gBox)
    {
        QWidget *iBox = gBox->m_children.value(interfaceBox);

        if(!iBox)
        {
            emit errorMessage(QString(metaObject()->className()) +
            "::" + FName + "[" + windowTitle() + "][" +
            gBox->title() + "] -> " +
            tr("Interface Box %L1 does not exist").
            arg(interfaceBox));
        }

        return QPair<MyGroupBox *, QWidget *>(gBox, iBox);
    }

    return QPair<MyGroupBox *, QWidget *>(NULL, NULL);
}

MyGroupBox *SerialInterface::interfaceBoxDoesNotExist(const QString &FName,
                                                      int groupBox,
                                                      int interfaceBox)
{
    Q_UNUSED(FName); // Silently handle error...

    MyGroupBox *gBox = groupBoxExists(FName, groupBox);

    if(gBox)
    {
        bool ok = !gBox->m_children.contains(interfaceBox);

        if(!ok)
        {
            /* emit errorMessage(QString(metaObject()->className()) +
            "::" + FName + "[" + windowTitle() + "][" +
            gBox->title() + "] -> " +
            tr("Interface Box %L1 already exists").
            arg(interfaceBox)); */
        }

        return ok ? gBox : NULL;
    }

    return NULL;
}

MyGroupBox *SerialInterface::groupBoxExists(const QString &FName,
                                            int groupBox)
{
    MyGroupBox *gBox = m_children.value(groupBox);

    if(!gBox)
    {
        emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "] -> " +
        tr("Group Box %L1 does not exist").
        arg(groupBox));
    }

    return gBox;
}

bool SerialInterface::groupBoxDoesNotExist(const QString &FName, int groupBox)
{
    Q_UNUSED(FName); // Silently handle error...

    bool ok = !m_children.contains(groupBox);

    if(!ok)
    {
        /* emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "] -> " +
        tr("Group Box %L1 already exists").
        arg(groupBox)); */
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
