// by: Kwabena W. Agyeman

#include "utilvectordialogwowh.h"
#include "ui_utilvectordialogwowh.h"

UtilVectorDialogWOWH::UtilVectorDialogWOWH(const QString &title,
QWidget *parent) : QDialog(parent), m_ui(new Ui::UtilVectorDialogWOWH)
{
    m_ui->setupUi(this); setWindowTitle(title); setObjectName(title);
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint |
    Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::Dialog);
}

UtilVectorDialogWOWH::~UtilVectorDialogWOWH()
{
    delete m_ui;
}

void UtilVectorDialogWOWH::setSaveEntireImage(bool on)
{
    m_ui->saveEntireImage->setChecked(on);
}

bool UtilVectorDialogWOWH::getSaveEntireImage() const
{
    return m_ui->saveEntireImage->isChecked();
}

void UtilVectorDialogWOWH::setSaveCurrentView(bool on)
{
    m_ui->saveCurrentView->setChecked(on);
}

bool UtilVectorDialogWOWH::getSaveCurrentView() const
{
    return m_ui->saveCurrentView->isChecked();
}

void UtilVectorDialogWOWH::setFileTitle(const QString &fileTitle)
{
    m_ui->tEdit->setText(fileTitle);
}

QString UtilVectorDialogWOWH::getFileTitle() const
{
    return m_ui->tEdit->text();
}

void UtilVectorDialogWOWH::setFileDescription(const QString &fileDescription)
{
    m_ui->dEdit->setText(fileDescription);
}

QString UtilVectorDialogWOWH::getFileDescription() const
{
    return m_ui->dEdit->text();
}
