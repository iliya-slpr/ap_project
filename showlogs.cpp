#include "showlogs.h"
#include "ui_showlogs.h"

showLogs::showLogs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showLogs)
{
    ui->setupUi(this);

}

void showLogs::getLogs(QString _i , QString _o)
{
 ui->loginLogTE->setText(_i);
 ui->logoutLogTE->setText(_o);
}

showLogs::~showLogs()
{
    delete ui;
}
