// by: Kwabena W. Agyeman

#ifndef UTILRASTERDIALOG_H
#define UTILRASTERDIALOG_H

#include <QtCore>
#include <QtWidgets>

namespace Ui
{
    class UtilRasterDialog;
}

class UtilRasterDialog : public QDialog
{
    Q_OBJECT

public:

    explicit UtilRasterDialog(const QString &title, QWidget *parent=NULL);
    virtual ~UtilRasterDialog();

    void setWidth(int width);
    int getWidth() const;

    void setHeight(int height);
    int getHeight() const;

    void setSaveEntireImage(bool on);
    bool getSaveEntireImage() const;

    void setSaveCurrentView(bool on);
    bool getSaveCurrentView() const;

private:

    Q_DISABLE_COPY(UtilRasterDialog)
    Ui::UtilRasterDialog *m_ui;
};

#endif // UTILRASTERDIALOG_H
