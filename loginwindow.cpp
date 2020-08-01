#include "loginwindow.h"
#include "ui_loginwindow.h"

loginWindow::loginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::loginWindow)
{
    ui->setupUi(this);
    ui->usernameLineEdit->setFocus();

}

loginWindow::~loginWindow()
{
    delete ui;
}


void loginWindow::on_eraseBtn_clicked()
{
    ui->usernameLineEdit->setText("");
    ui->passwordLineEdit->setText("");
}
QString loginWindow::getUsername()
{
    return ui->usernameLineEdit->text().toUtf8().constData();
}
QString loginWindow::getPassword()
{
    return ui->passwordLineEdit->text().toUtf8().constData();
}


void loginWindow::on_loginBtn_clicked()
{
    ok=true;
}
