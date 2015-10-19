// by: Kwabena W. Agyeman

#ifndef UTILVECTORDIALOGWOWH_H
#define UTILVECTORDIALOGWOWH_H

#include <QtCore>
#include <QtWidgets>

namespace Ui
{
    class UtilVectorDialogWOWH;
}

class UtilVectorDialogWOWH : public QDialog
{
    Q_OBJECT

public:

    explicit UtilVectorDialogWOWH(const QString &title, QWidget *parent=NULL);
    virtual ~UtilVectorDialogWOWH();

    void setSaveEntireImage(bool on);
    bool getSaveEntireImage() const;

    void setSaveCurrentView(bool on);
    bool getSaveCurrentView() const;

    void setFileTitle(const QString &fileTitle);
    QString getFileTitle() const;

    void setFileDescription(const QString &fileDescription);
    QString getFileDescription() const;

private:

    Q_DISABLE_COPY(UtilVectorDialogWOWH)
    Ui::UtilVectorDialogWOWH *m_ui;
};

#endif // UTILVECTORDIALOGWOWH_H
