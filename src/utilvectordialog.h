// by: Kwabena W. Agyeman

#ifndef UTILVECTORDIALOG_H
#define UTILVECTORDIALOG_H

#include <QtCore>
#include <QtWidgets>

namespace Ui
{
    class UtilVectorDialog;
}

class UtilVectorDialog : public QDialog
{
    Q_OBJECT

public:

    explicit UtilVectorDialog(const QString &title, QWidget *parent=NULL);
    virtual ~UtilVectorDialog();

    void setWidth(int width);
    int getWidth() const;

    void setHeight(int height);
    int getHeight() const;

    void setSaveEntireImage(bool on);
    bool getSaveEntireImage() const;

    void setSaveCurrentView(bool on);
    bool getSaveCurrentView() const;

    void setFileTitle(const QString &fileTitle);
    QString getFileTitle() const;

    void setFileDescription(const QString &fileDescription);
    QString getFileDescription() const;

private:

    Q_DISABLE_COPY(UtilVectorDialog)
    Ui::UtilVectorDialog *m_ui;
};

#endif // UTILVECTORDIALOG_H
