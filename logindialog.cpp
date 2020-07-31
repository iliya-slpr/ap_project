#include "logindialog.h"
#include "ui_logindialog.h"

loginDialog::loginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDialog)
{
    ui->setupUi(this);
}

loginDialog::~loginDialog()
{
    delete ui;
}
QString loginDialog::getUsername()
{
    return ui->usernameLineEdit->text().toUtf8().constData();
}
QString loginDialog::getPassword()
{
    return ui->passwordLineEdit->text().toUtf8().constData();
}
void loginDialog::on_buttonBox_rejected()
{
    this->close();
}
