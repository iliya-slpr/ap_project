#include "logindialog.h"
#include "ui_logindialog.h"

loginDialog::loginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDialog)
{
    ui->setupUi(this);
    ui->usernameLineEdit->setStyleSheet("QLineEdit { border-radius: 7px; }");
    ui->passwordLineEdit->setStyleSheet("border-radius: 7px;");

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
bool loginDialog::getIsAdmin()
{
    return ui->isAdminCheckBox->isChecked();
}
