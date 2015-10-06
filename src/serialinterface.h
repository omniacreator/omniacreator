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

#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include <QtCore>
#include <QtWidgets>

#include "serialwindow.h"

namespace Ui
{
    class SerialInterface;
}

class MyGroupBox : public QGroupBox ///////////////////////////////////////////
{
    Q_OBJECT

public:

    explicit MyGroupBox(const QString &name) : QGroupBox(name)
    {
        m_layout = new QFormLayout(this);
    }

    QFormLayout *m_layout; QMap<int, QWidget *> m_children;

private:

    Q_DISABLE_COPY(MyGroupBox) ////////////////////////////////////////////////
};

class MyPushButton : public QPushButton ///////////////////////////////////////
{
    Q_OBJECT

public:

    explicit MyPushButton(const QString &name) : QPushButton(name)
    {
        connect(this, SIGNAL(pressed()),
                this, SLOT(pressedSignal()));

        connect(this, SIGNAL(released()),
                this, SLOT(releasedSignal()));

        connect(this, SIGNAL(clicked()),
                this, SLOT(clickedSignal()));
    }

    int m_windowHandle, m_groupBox, m_interfaceBox;

private slots:

    void pressedSignal()
    {
        emit dataPacket(INTERFACE_PUSH_BUTTON_SIGNAL,
                        m_windowHandle, m_groupBox, m_interfaceBox,
                        BS_PRESSED);
    }

    void releasedSignal()
    {
        emit dataPacket(INTERFACE_PUSH_BUTTON_SIGNAL,
                        m_windowHandle, m_groupBox, m_interfaceBox,
                        BS_RELEASED);
    }

    void clickedSignal()
    {
        emit dataPacket(INTERFACE_PUSH_BUTTON_SIGNAL,
                        m_windowHandle, m_groupBox, m_interfaceBox,
                        BS_CLICKED);
    }

signals:

    void dataPacket(SerialFunction function,
                    int windowHandle,
                    int groupBox,
                    int interfaceBox,
                    const QVariant &data);

private:

    Q_DISABLE_COPY(MyPushButton) //////////////////////////////////////////////
};

class MyRadioButton : public QRadioButton /////////////////////////////////////
{
    Q_OBJECT

public:

    explicit MyRadioButton(const QString &name) : QRadioButton(name)
    {
        connect(this, SIGNAL(toggled(bool)),
                this, SLOT(toggledSignal(bool)));
    }

    int m_windowHandle, m_groupBox, m_interfaceBox;

private slots:

    void toggledSignal(bool checked)
    {
        emit dataPacket(INTERFACE_RADIO_BUTTON_SIGNAL,
                        m_windowHandle, m_groupBox, m_interfaceBox,
                        checked);
    }

signals:

    void dataPacket(SerialFunction function,
                    int windowHandle,
                    int groupBox,
                    int interfaceBox,
                    const QVariant &data);

private:

    Q_DISABLE_COPY(MyRadioButton) /////////////////////////////////////////////
};

class MyCheckBox : public QCheckBox ///////////////////////////////////////////
{
    Q_OBJECT

public:

    explicit MyCheckBox(const QString &name) : QCheckBox(name)
    {
        connect(this, SIGNAL(toggled(bool)),
                this, SLOT(toggledSignal(bool)));
    }

    int m_windowHandle, m_groupBox, m_interfaceBox;

private slots:

    void toggledSignal(bool checked)
    {
        emit dataPacket(INTERFACE_CHECK_BOX_SIGNAL,
                        m_windowHandle, m_groupBox, m_interfaceBox,
                        checked);
    }

signals:

    void dataPacket(SerialFunction function,
                    int windowHandle,
                    int groupBox,
                    int interfaceBox,
                    const QVariant &data);

private:

    Q_DISABLE_COPY(MyCheckBox) ////////////////////////////////////////////////
};

class MyLineEdit : public QLineEdit ///////////////////////////////////////////
{
    Q_OBJECT

public:

    explicit MyLineEdit() : QLineEdit()
    {
        m_m = new QStringListModel(this);
        m_c = new QCompleter(m_m, this);

        m_blockReturnPressedSignal = false;
        m_backupList = QStringList();

        setCompleter(m_c);

        connect(this, SIGNAL(returnPressed()),
                this, SLOT(returnPressedSignal()));

        connect(this, SIGNAL(textChanged(QString)),
                this, SLOT(textChangedSignal(QString)));

        setClearButtonEnabled(true);
        setDragEnabled(true);
        setMaxLength((MAX_LAYER_1_DATA_SIZE - 1) / sizeof(ushort));
    }

    int m_windowHandle, m_groupBox, m_interfaceBox;

    QStringListModel *m_m; QCompleter *m_c;
    bool m_blockReturnPressedSignal; QStringList m_backupList;

private slots:

    void returnPressedSignal()
    {
        if(!m_blockReturnPressedSignal)
        {
            QStringList list = m_m->stringList(); list.append(text());

            if(m_m->stringList().size() > m_c->maxVisibleItems())
            {
                list.removeFirst();
            }

            m_m->setStringList(list);
        }
    }

    void textChangedSignal(const QString &text)
    {
        emit dataPacket(INTERFACE_LINE_EDIT_SIGNAL,
                        m_windowHandle, m_groupBox, m_interfaceBox,
                        text);
    }

signals:

    void dataPacket(SerialFunction function,
                    int windowHandle,
                    int groupBox,
                    int interfaceBox,
                    const QVariant &data);

private:

    Q_DISABLE_COPY(MyLineEdit) ////////////////////////////////////////////////
};

class MyIntSpinBox : public QSpinBox //////////////////////////////////////////
{
    Q_OBJECT

public:

    explicit MyIntSpinBox() : QSpinBox()
    {
        connect(this, SIGNAL(valueChanged(int)),
                this, SLOT(valueChangedSignal(int)));

        setAccelerated(true);
        setDisplayIntegerBase(10);
    }

    int m_windowHandle, m_groupBox, m_interfaceBox;

private slots:

    void valueChangedSignal(int i)
    {
        emit dataPacket(INTERFACE_INT_SPIN_BOX_SIGNAL,
                        m_windowHandle, m_groupBox, m_interfaceBox,
                        i);
    }

signals:

    void dataPacket(SerialFunction function,
                    int windowHandle,
                    int groupBox,
                    int interfaceBox,
                    const QVariant &data);

private:

    Q_DISABLE_COPY(MyIntSpinBox) //////////////////////////////////////////////
};

class MyBinSpinBox : public QSpinBox //////////////////////////////////////////
{
    Q_OBJECT

public:

    explicit MyBinSpinBox() : QSpinBox()
    {
        connect(this, SIGNAL(valueChanged(int)),
                this, SLOT(valueChangedSignal(int)));

        setAccelerated(true);
        setDisplayIntegerBase(2);
    }

    int m_windowHandle, m_groupBox, m_interfaceBox;

private slots:

    void valueChangedSignal(int i)
    {
        emit dataPacket(INTERFACE_BIN_SPIN_BOX_SIGNAL,
                        m_windowHandle, m_groupBox, m_interfaceBox,
                        i);
    }

signals:

    void dataPacket(SerialFunction function,
                    int windowHandle,
                    int groupBox,
                    int interfaceBox,
                    const QVariant &data);

private:

    Q_DISABLE_COPY(MyBinSpinBox) //////////////////////////////////////////////
};

class MyHexSpinBox : public QSpinBox //////////////////////////////////////////
{
    Q_OBJECT

public:

    explicit MyHexSpinBox() : QSpinBox()
    {
        connect(this, SIGNAL(valueChanged(int)),
                this, SLOT(valueChangedSignal(int)));

        setAccelerated(true);
        setDisplayIntegerBase(16);
    }

    int m_windowHandle, m_groupBox, m_interfaceBox;

private slots:

    void valueChangedSignal(int i)
    {
        emit dataPacket(INTERFACE_HEX_SPIN_BOX_SIGNAL,
                        m_windowHandle, m_groupBox, m_interfaceBox,
                        i);
    }

signals:

    void dataPacket(SerialFunction function,
                    int windowHandle,
                    int groupBox,
                    int interfaceBox,
                    const QVariant &data);

private:

    Q_DISABLE_COPY(MyHexSpinBox) //////////////////////////////////////////////
};

class MyDoubleSpinBox : public QDoubleSpinBox /////////////////////////////////
{
    Q_OBJECT

public:

    explicit MyDoubleSpinBox() : QDoubleSpinBox()
    {
        connect(this, SIGNAL(valueChanged(double)),
                this, SLOT(valueChangedSignal(double)));

        setAccelerated(true);
        setDecimals(2);
    }

    int m_windowHandle, m_groupBox, m_interfaceBox;

private slots:

    void valueChangedSignal(double d)
    {
        emit dataPacket(INTERFACE_DOUBLE_SPIN_BOX_SIGNAL,
                        m_windowHandle, m_groupBox, m_interfaceBox,
                        d);
    }

signals:

    void dataPacket(SerialFunction function,
                    int windowHandle,
                    int groupBox,
                    int interfaceBox,
                    const QVariant &data);

private:

    Q_DISABLE_COPY(MyDoubleSpinBox) ///////////////////////////////////////////
};

class MyTimeEdit : public QTimeEdit ///////////////////////////////////////////
{
    Q_OBJECT

public:

    explicit MyTimeEdit() : QTimeEdit()
    {
        connect(this, SIGNAL(timeChanged(QTime)),
                this, SLOT(timeChangedSignal(QTime)));

        setAccelerated(true);
        setCalendarPopup(true);
    }

    int m_windowHandle, m_groupBox, m_interfaceBox;

private slots:

    void timeChangedSignal(const QTime &time)
    {
        emit dataPacket(INTERFACE_TIME_EDIT_SIGNAL,
                        m_windowHandle, m_groupBox, m_interfaceBox,
                        time.toString(Qt::ISODate));
    }

signals:

    void dataPacket(SerialFunction function,
                    int windowHandle,
                    int groupBox,
                    int interfaceBox,
                    const QVariant &data);

private:

    Q_DISABLE_COPY(MyTimeEdit) ////////////////////////////////////////////////
};

class MyDateEdit : public QDateEdit ///////////////////////////////////////////
{
    Q_OBJECT

public:

    explicit MyDateEdit() : QDateEdit()
    {
        connect(this, SIGNAL(dateChanged(QDate)),
                this, SLOT(dateChangedSignal(QDate)));

        setAccelerated(true);
        setCalendarPopup(true);
    }

    int m_windowHandle, m_groupBox, m_interfaceBox;

private slots:

    void dateChangedSignal(const QDate &date)
    {
        emit dataPacket(INTERFACE_DATE_EDIT_SIGNAL,
                        m_windowHandle, m_groupBox, m_interfaceBox,
                        date.toString(Qt::ISODate));
    }

signals:

    void dataPacket(SerialFunction function,
                    int windowHandle,
                    int groupBox,
                    int interfaceBox,
                    const QVariant &data);

private:

    Q_DISABLE_COPY(MyDateEdit) ////////////////////////////////////////////////
};

class MyDateTimeEdit : public QDateTimeEdit ///////////////////////////////////
{
    Q_OBJECT

public:

    explicit MyDateTimeEdit() : QDateTimeEdit()
    {
        connect(this, SIGNAL(dateTimeChanged(QDateTime)),
                this, SLOT(dateTimeChangedSignal(QDateTime)));

        setAccelerated(true);
        setCalendarPopup(true);
    }

    int m_windowHandle, m_groupBox, m_interfaceBox;

private slots:

    void dateTimeChangedSignal(const QDateTime &dateTime)
    {
        emit dataPacket(INTERFACE_DATE_TIME_EDIT_SIGNAL,
                        m_windowHandle, m_groupBox, m_interfaceBox,
                        dateTime.toString(Qt::ISODate));
    }

signals:

    void dataPacket(SerialFunction function,
                    int windowHandle,
                    int groupBox,
                    int interfaceBox,
                    const QVariant &data);

private:

    Q_DISABLE_COPY(MyDateTimeEdit) ////////////////////////////////////////////
};

class MySlider : public QWidget ///////////////////////////////////////////////
{
    Q_OBJECT

public:

    explicit MySlider() : QWidget()
    {
        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->setMargin(0);

        m_slider = new QSlider(Qt::Horizontal);
        m_label = new QLabel(QString::number(m_slider->value()));

        layout->addWidget(m_slider);
        layout->addWidget(m_label);

        connect(m_slider, SIGNAL(valueChanged(int)),
                m_label, SLOT(setNum(int)));

        connect(m_slider, SIGNAL(valueChanged(int)),
                this, SLOT(valueChangedSignal(int)));

        m_slider->setTickPosition(QSlider::TicksBothSides);
    }

    int m_windowHandle, m_groupBox, m_interfaceBox;

    QSlider *m_slider; QLabel *m_label;

private slots:

    void valueChangedSignal(int value)
    {
        emit dataPacket(INTERFACE_SLIDER_SIGNAL,
                        m_windowHandle, m_groupBox, m_interfaceBox,
                        value);
    }

signals:

    void dataPacket(SerialFunction function,
                    int windowHandle,
                    int groupBox,
                    int interfaceBox,
                    const QVariant &data);

private:

    Q_DISABLE_COPY(MySlider) //////////////////////////////////////////////////
};

class MyLabel : public QLabel /////////////////////////////////////////////////
{
    Q_OBJECT

public:

    explicit MyLabel(const QString &name) : QLabel(name)
    {
        setOpenExternalLinks(true);
        setWordWrap(true);
    }

private:

    Q_DISABLE_COPY(MyLabel) ///////////////////////////////////////////////////
};

class MyProgressBar : public QProgressBar /////////////////////////////////////
{
    Q_OBJECT

public:

    explicit MyProgressBar() : QProgressBar()
    {
        setTextVisible(false);
    }

private:

    Q_DISABLE_COPY(MyProgressBar) /////////////////////////////////////////////
};

class MySeperator : public QFrame /////////////////////////////////////////////
{
    Q_OBJECT

public:

    explicit MySeperator() : QFrame()
    {
        setFrameShadow(QFrame::Sunken);
        setFrameShape(QFrame::HLine);
    }

private:

    Q_DISABLE_COPY(MySeperator) ///////////////////////////////////////////////
};

class SerialInterface : public SerialWindow
{
    Q_OBJECT

public:

    explicit SerialInterface(const QString &title,
                             QSettings *settings = NULL,
                             QWidget *parent = NULL);

    virtual ~SerialInterface();

    virtual bool importExportEnabled() const;

    virtual QString keyGroup() const;
    virtual QString keyState() const;
    virtual QString keyGeometry() const;

    bool newGroupBox(int groupBox,
                     const QString &name);

    bool newPushButton(int groupBox,
                       int interfaceBox,
                       const QString &name);

    bool newRadioButton(int groupBox,
                        int interfaceBox,
                        const QString &name);

    bool newCheckBox(int groupBox,
                     int interfaceBox,
                     const QString &name);

    bool newLineEdit(int groupBox,
                     int interfaceBox,
                     const QString &name);

    bool newIntSpinBox(int groupBox,
                       int interfaceBox,
                       const QString &name);

    bool newBinSpinBox(int groupBox,
                       int interfaceBox,
                       const QString &name);

    bool newHexSpinBox(int groupBox,
                       int interfaceBox,
                       const QString &name);

    bool newDoubleSpinBox(int groupBox,
                          int interfaceBox,
                          const QString &name);

    bool newTimeEdit(int groupBox,
                     int interfaceBox,
                     const QString &name);

    bool newDateEdit(int groupBox,
                     int interfaceBox,
                     const QString &name);

    bool newDateTimeEdit(int groupBox,
                         int interfaceBox,
                         const QString &name);

    bool newSlider(int groupBox,
                   int interfaceBox,
                   const QString &name);

    bool newLabel(int groupBox,
                  int interfaceBox,
                  const QString &text);

    bool newProgressBar(int groupBox,
                        int interfaceBox,
                        const QString &name);

    bool newSeperator(int groupBox,
                      int interfaceBox);

    ///////////////////////////////////////////////////////////////////////////

    bool setGroupBoxName(int groupBox,
                         const QString &name);

    QString getGroupBoxName(int groupBox,
                            bool *ok);

    ///////////////////////////////////////////////////////////////////////////

    bool setPushButtonName(int groupBox,
                           int interfaceBox,
                           const QString &name);

    QString getPushButtonName(int groupBox,
                              int interfaceBox,
                              bool *ok);

    ///////////////////////////////////////////////////////////////////////////

    bool setRadioButtonName(int groupBox,
                            int interfaceBox,
                            const QString &name);

    QString getRadioButtonName(int groupBox,
                               int interfaceBox,
                               bool *ok);

    bool setRadioButtonChecked(int groupBox,
                               int interfaceBox,
                               bool checked);

    bool getRadioButtonChecked(int groupBox,
                               int interfaceBox,
                               bool *ok);

    ///////////////////////////////////////////////////////////////////////////

    bool setCheckBoxName(int groupBox,
                         int interfaceBox,
                         const QString &name);

    QString getCheckBoxName(int groupBox,
                            int interfaceBox,
                            bool *ok);

    bool setCheckBoxChecked(int groupBox,
                            int interfaceBox,
                            bool checked);

    bool getCheckBoxChecked(int groupBox,
                            int interfaceBox,
                            bool *ok);

    ///////////////////////////////////////////////////////////////////////////

    bool setLineEditName(int groupBox,
                         int interfaceBox,
                         const QString &name);

    QString getLineEditName(int groupBox,
                            int interfaceBox,
                            bool *ok);

    bool setLineEditEchoMode(int groupBox,
                             int interfaceBox,
                             QLineEdit::EchoMode echoMode);

    QLineEdit::EchoMode getLineEditEchoMode(int groupBox,
                                            int interfaceBox,
                                            bool *ok);

    bool setLineEditInputMask(int groupBox,
                              int interfaceBox,
                              const QString &inputMask);

    QString getLineEditInputMask(int groupBox,
                                 int interfaceBox,
                                 bool *ok);

    bool setLineEditPlaceholderText(int groupBox,
                                    int interfaceBox,
                                    const QString &placeholderText);

    QString getLineEditPlaceholderText(int groupBox,
                                       int interfaceBox,
                                       bool *ok);

    bool setLineEditText(int groupBox,
                         int interfaceBox,
                         const QString &text);

    QString getLineEditText(int groupBox,
                            int interfaceBox,
                            bool *ok);

    ///////////////////////////////////////////////////////////////////////////

    bool resetIntSpinBox(int groupBox,
                         int interfaceBox);

    bool setIntSpinBoxName(int groupBox,
                           int interfaceBox,
                           const QString &name);

    QString getIntSpinBoxName(int groupBox,
                              int interfaceBox,
                              bool *ok);

    bool setIntSpinBoxPrefix(int groupBox,
                             int interfaceBox,
                             const QString &prefix);

    QString getIntSpinBoxPrefix(int groupBox,
                                int interfaceBox,
                                bool *ok);

    bool setIntSpinBoxSuffix(int groupBox,
                             int interfaceBox,
                             const QString &suffix);

    QString getIntSpinBoxSuffix(int groupBox,
                                int interfaceBox,
                                bool *ok);

    bool setIntSpinBoxSingleStep(int groupBox,
                                 int interfaceBox,
                                 int singleStep);

    int getIntSpinBoxSingleStep(int groupBox,
                                int interfaceBox,
                                bool *ok);

    bool setIntSpinBoxPageStep(int groupBox,
                               int interfaceBox,
                               int pageStep);

    int getIntSpinBoxPageStep(int groupBox,
                              int interfaceBox,
                              bool *ok);

    bool setIntSpinBoxMinimum(int groupBox,
                              int interfaceBox,
                              int min);

    int getIntSpinBoxMinimum(int groupBox,
                             int interfaceBox,
                             bool *ok);

    bool setIntSpinBoxMaximum(int groupBox,
                              int interfaceBox,
                              int max);

    int getIntSpinBoxMaximum(int groupBox,
                             int interfaceBox,
                             bool *ok);

    bool setIntSpinBoxValue(int groupBox,
                            int interfaceBox,
                            int value);

    int getIntSpinBoxValue(int groupBox,
                           int interfaceBox,
                           bool *ok);

    bool setIntSpinBoxSpecialValueText(int groupBox,
                                       int interfaceBox,
                                       const QString &specialValueText);

    QString getIntSpinBoxSpecialValueText(int groupBox,
                                          int interfaceBox,
                                          bool *ok);

    ///////////////////////////////////////////////////////////////////////////

    bool resetBinSpinBox(int groupBox,
                         int interfaceBox);

    bool setBinSpinBoxName(int groupBox,
                           int interfaceBox,
                           const QString &name);

    QString getBinSpinBoxName(int groupBox,
                              int interfaceBox,
                              bool *ok);

    bool setBinSpinBoxPrefix(int groupBox,
                             int interfaceBox,
                             const QString &prefix);

    QString getBinSpinBoxPrefix(int groupBox,
                                int interfaceBox,
                                bool *ok);

    bool setBinSpinBoxSuffix(int groupBox,
                             int interfaceBox,
                             const QString &suffix);

    QString getBinSpinBoxSuffix(int groupBox,
                                int interfaceBox,
                                bool *ok);

    bool setBinSpinBoxSingleStep(int groupBox,
                                 int interfaceBox,
                                 int singleStep);

    int getBinSpinBoxSingleStep(int groupBox,
                                int interfaceBox,
                                bool *ok);

    bool setBinSpinBoxPageStep(int groupBox,
                               int interfaceBox,
                               int pageStep);

    int getBinSpinBoxPageStep(int groupBox,
                              int interfaceBox,
                              bool *ok);

    bool setBinSpinBoxMinimum(int groupBox,
                              int interfaceBox,
                              int min);

    int getBinSpinBoxMinimum(int groupBox,
                             int interfaceBox,
                             bool *ok);

    bool setBinSpinBoxMaximum(int groupBox,
                              int interfaceBox,
                              int max);

    int getBinSpinBoxMaximum(int groupBox,
                             int interfaceBox,
                             bool *ok);

    bool setBinSpinBoxValue(int groupBox,
                            int interfaceBox,
                            int value);

    int getBinSpinBoxValue(int groupBox,
                           int interfaceBox,
                           bool *ok);

    bool setBinSpinBoxSpecialValueText(int groupBox,
                                       int interfaceBox,
                                       const QString &specialValueText);

    QString getBinSpinBoxSpecialValueText(int groupBox,
                                          int interfaceBox,
                                          bool *ok);

    ///////////////////////////////////////////////////////////////////////////

    bool resetHexSpinBox(int groupBox,
                         int interfaceBox);

    bool setHexSpinBoxName(int groupBox,
                           int interfaceBox,
                           const QString &name);

    QString getHexSpinBoxName(int groupBox,
                              int interfaceBox,
                              bool *ok);

    bool setHexSpinBoxPrefix(int groupBox,
                             int interfaceBox,
                             const QString &prefix);

    QString getHexSpinBoxPrefix(int groupBox,
                                int interfaceBox,
                                bool *ok);

    bool setHexSpinBoxSuffix(int groupBox,
                             int interfaceBox,
                             const QString &suffix);

    QString getHexSpinBoxSuffix(int groupBox,
                                int interfaceBox,
                                bool *ok);

    bool setHexSpinBoxSingleStep(int groupBox,
                                 int interfaceBox,
                                 int singleStep);

    int getHexSpinBoxSingleStep(int groupBox,
                                int interfaceBox,
                                bool *ok);

    bool setHexSpinBoxPageStep(int groupBox,
                               int interfaceBox,
                               int pageStep);

    int getHexSpinBoxPageStep(int groupBox,
                              int interfaceBox,
                              bool *ok);

    bool setHexSpinBoxMinimum(int groupBox,
                              int interfaceBox,
                              int min);

    int getHexSpinBoxMinimum(int groupBox,
                             int interfaceBox,
                             bool *ok);

    bool setHexSpinBoxMaximum(int groupBox,
                              int interfaceBox,
                              int max);

    int getHexSpinBoxMaximum(int groupBox,
                             int interfaceBox,
                             bool *ok);

    bool setHexSpinBoxValue(int groupBox,
                            int interfaceBox,
                            int value);

    int getHexSpinBoxValue(int groupBox,
                           int interfaceBox,
                           bool *ok);

    bool setHexSpinBoxSpecialValueText(int groupBox,
                                       int interfaceBox,
                                       const QString &specialValueText);

    QString getHexSpinBoxSpecialValueText(int groupBox,
                                          int interfaceBox,
                                          bool *ok);

    ///////////////////////////////////////////////////////////////////////////

    bool resetDoubleSpinBox(int groupBox,
                            int interfaceBox);

    bool setDoubleSpinBoxName(int groupBox,
                              int interfaceBox,
                              const QString &name);

    QString getDoubleSpinBoxName(int groupBox,
                                 int interfaceBox,
                                 bool *ok);

    bool setDoubleSpinBoxPrefix(int groupBox,
                                int interfaceBox,
                                const QString &prefix);

    QString getDoubleSpinBoxPrefix(int groupBox,
                                   int interfaceBox,
                                   bool *ok);

    bool setDoubleSpinBoxSuffix(int groupBox,
                                int interfaceBox,
                                const QString &suffix);

    QString getDoubleSpinBoxSuffix(int groupBox,
                                   int interfaceBox,
                                   bool *ok);

    bool setDoubleSpinBoxSingleStep(int groupBox,
                                    int interfaceBox,
                                    double singleStep);

    double getDoubleSpinBoxSingleStep(int groupBox,
                                      int interfaceBox,
                                      bool *ok);

    bool setDoubleSpinBoxPageStep(int groupBox,
                                  int interfaceBox,
                                  double pageStep);

    double getDoubleSpinBoxPageStep(int groupBox,
                                    int interfaceBox,
                                    bool *ok);

    bool setDoubleSpinBoxMinimum(int groupBox,
                                 int interfaceBox,
                                 double min);

    double getDoubleSpinBoxMinimum(int groupBox,
                                   int interfaceBox,
                                   bool *ok);

    bool setDoubleSpinBoxMaximum(int groupBox,
                                 int interfaceBox,
                                 double max);

    double getDoubleSpinBoxMaximum(int groupBox,
                                   int interfaceBox,
                                   bool *ok);

    bool setDoubleSpinBoxValue(int groupBox,
                               int interfaceBox,
                               double value);

    double getDoubleSpinBoxValue(int groupBox,
                                 int interfaceBox,
                                 bool *ok);

    bool setDoubleSpinBoxSpecialValueText(int groupBox,
                                          int interfaceBox,
                                          const QString &specialValueText);

    QString getDoubleSpinBoxSpecialValueText(int groupBox,
                                             int interfaceBox,
                                             bool *ok);

    ///////////////////////////////////////////////////////////////////////////

    bool resetTimeEdit(int groupBox,
                       int interfaceBox);

    bool setTimeEditName(int groupBox,
                         int interfaceBox,
                         const QString &name);

    QString getTimeEditName(int groupBox,
                            int interfaceBox,
                            bool *ok);

    bool setTimeEditMinimum(int groupBox,
                            int interfaceBox,
                            const QString &min);

    QString getTimeEditMinimum(int groupBox,
                               int interfaceBox,
                               bool *ok);

    bool setTimeEditMaximum(int groupBox,
                            int interfaceBox,
                            const QString &max);

    QString getTimeEditMaximum(int groupBox,
                               int interfaceBox,
                               bool *ok);

    bool setTimeEditValue(int groupBox,
                          int interfaceBox,
                          const QString &value);

    QString getTimeEditValue(int groupBox,
                             int interfaceBox,
                             bool *ok);

    bool setTimeEditSpecialValueText(int groupBox,
                                     int interfaceBox,
                                     const QString &specialValueText);

    QString getTimeEditSpecialValueText(int groupBox,
                                        int interfaceBox,
                                        bool *ok);

    ///////////////////////////////////////////////////////////////////////////

    bool resetDateEdit(int groupBox,
                       int interfaceBox);

    bool setDateEditName(int groupBox,
                         int interfaceBox,
                         const QString &name);

    QString getDateEditName(int groupBox,
                            int interfaceBox,
                            bool *ok);

    bool setDateEditMinimum(int groupBox,
                            int interfaceBox,
                            const QString &min);

    QString getDateEditMinimum(int groupBox,
                               int interfaceBox,
                               bool *ok);

    bool setDateEditMaximum(int groupBox,
                            int interfaceBox,
                            const QString &max);

    QString getDateEditMaximum(int groupBox,
                               int interfaceBox,
                               bool *ok);

    bool setDateEditValue(int groupBox,
                          int interfaceBox,
                          const QString &value);

    QString getDateEditValue(int groupBox,
                             int interfaceBox,
                             bool *ok);

    bool setDateEditSpecialValueText(int groupBox,
                                     int interfaceBox,
                                     const QString &specialValueText);

    QString getDateEditSpecialValueText(int groupBox,
                                        int interfaceBox,
                                        bool *ok);

    ///////////////////////////////////////////////////////////////////////////

    bool resetDateTimeEdit(int groupBox,
                           int interfaceBox);

    bool setDateTimeEditName(int groupBox,
                             int interfaceBox,
                             const QString &name);

    QString getDateTimeEditName(int groupBox,
                                int interfaceBox,
                                bool *ok);

    bool setDateTimeEditMinimum(int groupBox,
                                int interfaceBox,
                                const QString &min);

    QString getDateTimeEditMinimum(int groupBox,
                                   int interfaceBox,
                                   bool *ok);

    bool setDateTimeEditMaximum(int groupBox,
                                int interfaceBox,
                                const QString &max);

    QString getDateTimeEditMaximum(int groupBox,
                                   int interfaceBox,
                                   bool *ok);

    bool setDateTimeEditValue(int groupBox,
                              int interfaceBox,
                              const QString &value);

    QString getDateTimeEditValue(int groupBox,
                                 int interfaceBox,
                                 bool *ok);

    bool setDateTimeEditSpecialValueText(int groupBox,
                                         int interfaceBox,
                                         const QString &specialValueText);

    QString getDateTimeEditSpecialValueText(int groupBox,
                                            int interfaceBox,
                                            bool *ok);

    ///////////////////////////////////////////////////////////////////////////

    bool resetSlider(int groupBox,
                     int interfaceBox);

    bool setSliderName(int groupBox,
                       int interfaceBox,
                       const QString &name);

    QString getSliderName(int groupBox,
                          int interfaceBox,
                          bool *ok);

    bool setSliderSingleStep(int groupBox,
                             int interfaceBox,
                             int singleStep);

    int getSliderSingleStep(int groupBox,
                            int interfaceBox,
                            bool *ok);

    bool setSliderPageStep(int groupBox,
                           int interfaceBox,
                           int pageStep);

    int getSliderPageStep(int groupBox,
                          int interfaceBox,
                          bool *ok);

    bool setSliderMinimum(int groupBox,
                          int interfaceBox,
                          int min);

    int getSliderMinimum(int groupBox,
                         int interfaceBox,
                         bool *ok);

    bool setSliderMaximum(int groupBox,
                          int interfaceBox,
                          int max);

    int getSliderMaximum(int groupBox,
                         int interfaceBox,
                         bool *ok);

    bool setSliderValue(int groupBox,
                        int interfaceBox,
                        int value);

    int getSliderValue(int groupBox,
                       int interfaceBox,
                       bool *ok);

    ///////////////////////////////////////////////////////////////////////////

    bool setLabelText(int groupBox,
                      int interfaceBox,
                      const QString &text);

    QString getLabelText(int groupBox,
                         int interfaceBox,
                         bool *ok);

    ///////////////////////////////////////////////////////////////////////////

    bool resetProgressBar(int groupBox,
                          int interfaceBox);

    bool setProgressBarName(int groupBox,
                            int interfaceBox,
                            const QString &name);

    QString getProgressBarName(int groupBox,
                               int interfaceBox,
                               bool *ok);

    bool setProgressBarMinimum(int groupBox,
                               int interfaceBox,
                               int min);

    int getProgressBarMinimum(int groupBox,
                              int interfaceBox,
                              bool *ok);

    bool setProgressBarMaximum(int groupBox,
                               int interfaceBox,
                               int max);

    int getProgressBarMaximum(int groupBox,
                              int interfaceBox,
                              bool *ok);

    bool setProgressBarValue(int groupBox,
                             int interfaceBox,
                             int value);

    int getProgressBarValue(int groupBox,
                            int interfaceBox,
                            bool *ok);

    ///////////////////////////////////////////////////////////////////////////

    bool deleteInterfaceBox(int groupBox, int interfaceBox);
    bool deleteAllInterfaceBoxes(int groupBox);
    void deleteAllInterfaceBoxes();
    bool deleteGroupBox(int groupBox);
    void deleteAllGroupBoxes();

private slots:

    void generalHelp();
    void interfaceHelp();

signals:

    void dataPacket(SerialFunction function,
                    int windowHandle,
                    int groupBox,
                    int interfaceBox,
                    const QVariant &data);

private:

    Q_DISABLE_COPY(SerialInterface)

    void
    setInterfaceBoxName(QPair<MyGroupBox *, QWidget *> pair,
                        const QString &name);

    QString
    getInterfaceBoxName(QPair<MyGroupBox *, QWidget *> pair);

    template <class T>
    QPair<MyGroupBox *, T *>
    interfaceBoxCast(const QString &FName,
                     int groupBox,
                     int interfaceBox);

    bool notInfinite(const QString &FName,
                     const QString &groupBoxName,
                     const QString &interfaceBoxName,
                     const QString &VName,
                     double value);

    bool notZero(const QString &FName,
                 const QString &groupBoxName,
                 const QString &interfaceBoxName,
                 const QString &VName,
                 double value);

    bool notInvalid(const QString &FName,
                    const QString &groupBoxName,
                    const QString &interfaceBoxName,
                    const QString &VName,
                    double value,
                    double minValue,
                    double maxValue);

    QPair<MyGroupBox *, QWidget *>
    interfaceBoxExists(const QString &FName,
                       int groupBox,
                       int interfaceBox);

    MyGroupBox *interfaceBoxDoesNotExist(const QString &FName,
                                         int groupBox,
                                         int interfaceBox);

    MyGroupBox *groupBoxExists(const QString &FName,
                               int groupBox);

    bool groupBoxDoesNotExist(const QString &FName, int groupBox);

    QHBoxLayout *m_layout; QMap<int, MyGroupBox *> m_children;

    Ui::SerialInterface *m_ui;
};

#endif // SERIALINTERFACE_H

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
