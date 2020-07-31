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
