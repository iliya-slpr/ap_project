#include "accountdialog.h"
#include "ui_accountdialog.h"

AccountDialog::AccountDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AccountDialog)
{
    ui->setupUi(this);
}

AccountDialog::~AccountDialog()
{
    delete ui;
}

void AccountDialog::receiveAccount(QString current)
{
    currentAccNum=current;
    application.setCurrentUserAccount(current);
}

void AccountDialog::on_tabWidget_tabBarClicked(int index)
{
    if(index==0)
    {
        application.refresh(currentAccNum);
        ui->balanceLbl->setText(QString::number(application.currentAccount->getBalance()));
    }
    else if(index==2)
    {
        application.refresh(currentAccNum);
        ui->transactions10->clear();
        ui->transactions10->clear();
        for(int j=application.currentAccount->getTransactions().size()-1;j>=0;j--)
        {
            QString temp="تاریخ : ";
            temp+=application.currentAccount->getTransactions()[j].getDate().toString();
            temp+=" ساعت : ";
            temp+=application.currentAccount->getTransactions()[j].getTime().toString();
            temp+=application.currentAccount->getTransactions()[j].getType()==0?" نوع: واریز":" نوع: برداشت ";
            temp+=" مبلغ : ";
            temp+=QString::number(application.currentAccount->getTransactions()[j].getAmount());
            temp+=" از حساب ";
            temp+=application.currentAccount->getTransactions()[j].getOriginBankAcc().getAccountNumber();
            temp+=" به حساب : ";
            temp+=application.currentAccount->getTransactions()[j].getDesBankAcc().getAccountNumber();
            ui->transactions10->insertItem(j,new  QListWidgetItem(temp));
            if(j<application.currentAccount->getTransactions().size()-10)
                break;

        }
    }

    else if(index==3)
    {
        if(application.currentAccount->hasACard())
        {
            ui->cardNumlbl->setText(application.currentAccount->getCard().getCardNumber());
            ui->cvv2lbl->setText(QString::number(application.currentAccount->getCard().getcvv2()));
            QString temp;
            if(application.currentAccount->getCard().getStatus()==0)temp="فعال";
            else if(application.currentAccount->getCard().getStatus()==1)temp="مسدود";
            else if(application.currentAccount->getCard().getStatus()==2)temp="در حال بررسی";
            else if(application.currentAccount->getCard().getStatus()==3)temp="رد شده";
            ui->statuslbl->setText(temp);
            ui->expirelbl->setText(application.currentAccount->getCard().getExpireDate().toString());
            ui->cardbtn->setDisabled(true);
        }
    }

}
void AccountDialog::on_transferBtn_clicked()
{
    application.refresh(currentAccNum);
    BankAccount* desAccount=application.findAccount(ui->accNum->text(),0);
    User* desUser=application.findUser(ui->accNum->text());
    QMessageBox msgBox;
    msgBox.setWindowTitle("تایید");
    msgBox.setText("آیا از انتقال وجه به حساب :"+desUser->getFullName()+"مطمئن هستید ؟");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes)
    {
        application.currentAccount->transfer(desAccount,ui->amount->text().toInt());
        message.setText("انتقال با موفقیت انجام شد !");
        application.writeUsers();
        application.refresh(currentAccNum);
        on_tabWidget_tabBarClicked(0);
        message.setWindowTitle("موفق");
        if(message.exec() == QMessageBox::Ok)
        {
            transfered=true;
            ui->amount->setText("");
            ui->accNum->setText("");
        }
    }
}

void AccountDialog::on_cardbtn_clicked()
{
    application.currentAccount->requestCard();
    application.writeUsers();
    application.refresh(currentAccNum);
    message.setText("درخواست شما ثبت و در لیست انتظار تایید مدیر قرار گرفت");
    message.setWindowTitle("تایید");
    message.show();
    on_tabWidget_tabBarClicked(3);
}

void AccountDialog::on_pushButton_clicked()
{
    application.currentAccount->getCardPointer()->createPass();
    ui->passLbl->setText(QString::number(application.currentAccount->getCardPointer()->getPassword().getPasswordOfSecPass()));
    ui->passDatelbl->setText(application.currentAccount->getCardPointer()->getPassword().getExpireTime().toString());
    application.writeUsers();
    application.refresh(currentAccNum);
}
