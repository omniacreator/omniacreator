// by: Kwabena W. Agyeman

#include "utilrasterdialog.h"
#include "ui_utilrasterdialog.h"

UtilRasterDialog::UtilRasterDialog(const QString &title,
QWidget *parent) : QDialog(parent), m_ui(new Ui::UtilRasterDialog)
{
    m_ui->setupUi(this); setWindowTitle(title); setObjectName(title);
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint |
    Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::Dialog);
}

UtilRasterDialog::~UtilRasterDialog()
{
    delete m_ui;
}

void UtilRasterDialog::setWidth(int width)
{
    m_ui->widthValue->setValue(width);
}

int UtilRasterDialog::getWidth() const
{
    return m_ui->widthValue->value();
}

void UtilRasterDialog::setHeight(int height)
{
    m_ui->heightValue->setValue(height);
}

int UtilRasterDialog::getHeight() const
{
    return m_ui->heightValue->value();
}

void UtilRasterDialog::setSaveEntireImage(bool on)
{
    m_ui->saveEntireImage->setChecked(on);
}

bool UtilRasterDialog::getSaveEntireImage() const
{
    return m_ui->saveEntireImage->isChecked();
}

void UtilRasterDialog::setSaveCurrentView(bool on)
{
    m_ui->saveCurrentView->setChecked(on);
}

bool UtilRasterDialog::getSaveCurrentView() const
{
    return m_ui->saveCurrentView->isChecked();
}
