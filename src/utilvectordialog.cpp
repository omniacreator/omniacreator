// by: Kwabena W. Agyeman

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
