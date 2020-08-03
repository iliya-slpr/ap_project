#include "addaccountdialog.h"
#include "ui_addaccountdialog.h"

addAccountDialog::addAccountDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addAccountDialog)
{
    ui->setupUi(this);
    ui->TypeCombo->addItem("قرض الحسنه",0);
    ui->TypeCombo->addItem("کوتاه مدت",1);
    ui->TypeCombo->addItem("کوتاه مدت حقوقی",2);
    ui->TypeCombo->addItem("بلند مدت",3);
    ui->ownersLed->setDisabled(true);

}
addAccountDialog::~addAccountDialog()
{
    delete ui;
}

int addAccountDialog::getBalance()
{
   return ui->balanceLed->text().toInt();
}
int addAccountDialog::getType()
{
    return ui->TypeCombo->currentData().toInt();
}
QString addAccountDialog::getOwners()
{
    return ui->ownersLed->text();
}
void addAccountDialog::on_TypeCombo_currentIndexChanged(int index)
{
    if(index==2)
     ui->ownersLed->setDisabled(false);
    else
     ui->ownersLed->setDisabled(true);
}
