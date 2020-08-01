#include "adduser.h"
#include "ui_adduser.h"

addUser::addUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addUser)
{
    ui->setupUi(this);
}

addUser::~addUser()
{
    delete ui;
}

void addUser::on_buttonBox_accepted()
{
    QString nameToAdd = ui->nameToAdd->text();
    QString nCodeToAdd = ui->nCodeToAdd->text();
    QDate dateToAdd = ui->DateToAdd->date();
    QString usernameToAdd = ui->usernameToAdd->text();
    QString passwordToadd = ui->passwordToAdd->text();
    emit sendDataToAdd(nameToAdd,nCodeToAdd,dateToAdd,usernameToAdd,passwordToadd);
}
