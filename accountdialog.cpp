#include "accountdialog.h"
#include "ui_accountdialog.h"

AccountDialog::AccountDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AccountDialog)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(1);
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
            temp+=application.currentAccount->getTransactions()[j].getType()==0?" نوع: برداشت":" نوع: واریز ";
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
            if(application.currentAccount->getCard().getStatus()==2)
            {
                ui->statuslbl->setText("درخواست شما ثبت و در دست بررسی است");
                return;
            }
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
    if(desAccount==NULL)
    {
        message.setText("حساب مورد نظر یافت نشد");
        message.setWindowTitle("خطا");
        message.show();
        return;
    }

    if(application.currentAccount->getBalance()<ui->amount->text().toInt())
    {
        message.setText("موجودی شما برای این انتقال کافی نیست !");
        message.setWindowTitle("خطا");
        message.show();
        return;
    }
    if(desAccount->getStatus()!=0)
    {
        message.setText("حساب مقصد فعال نیست");
        message.setWindowTitle("خطا");
        message.show();
        return;
    }
    if(application.currentAccount->getType()==3)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("هشدار");
        msgBox.setText("حساب شما بلند مدت بوده و با این انتقال به سپرده کوتاه مدت تبدیل میشود ، آیا مطمئن هستید ؟");
        msgBox.setStandardButtons(QMessageBox::Yes);
        msgBox.addButton(QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        if(msgBox.exec() == QMessageBox::No)
        {
            message.setText("عملیات لغو شد");
            message.setWindowTitle("نتیجه");
            message.show();
            return;
        }
        else
        {
            application.currentAccount->changeTypeToShortTerm();
        }
    }
    if(application.currentAccount->getType()==2)
    {
        for(int i=0;i<application.currentAccount->getOwnerUsername().size();i++)
        {
            BankAccount* temp1=application.findAccount(currentAccNum,application.currentAccount->getOwnerUsername()[i]);
            temp1->minusBalance(ui->amount->text().toInt());
            temp1->transfer(desAccount->getAccountNumber(),ui->amount->text().toInt(),0);
        }
    }
    else
    {
        application.currentAccount->minusBalance(ui->amount->text().toInt());
        application.currentAccount->transfer(desAccount->getAccountNumber(),ui->amount->text().toInt(),0);
    }
    if(desAccount->getType()==2)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("تایید");
        msgBox.setText("آیا از انتقال وجه به حساب حقوقی مطمئن هستید ؟");
        msgBox.setStandardButtons(QMessageBox::Yes);
        msgBox.addButton(QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        if(msgBox.exec() == QMessageBox::Yes)
        {
            for(int i=0;i<desAccount->getOwnerUsername().size();i++)
            {
                BankAccount* temp2=application.findAccount(desAccount->getAccountNumber(),desAccount->getOwnerUsername()[i]);
                temp2->plusBalance(ui->amount->text().toInt());
                temp2->transfer(currentAccNum,ui->amount->text().toInt(),1);
            }
        }
    }
    else
    {
        User* desUser=application.findUser(ui->accNum->text());
        QMessageBox msgBox;
        msgBox.setWindowTitle("تایید");
        msgBox.setText("آیا از انتقال وجه به حساب :"+desUser->getFullName()+"مطمئن هستید ؟");
        msgBox.setStandardButtons(QMessageBox::Yes);
        msgBox.addButton(QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        if(msgBox.exec() == QMessageBox::Yes)
        {
            desAccount->plusBalance(ui->amount->text().toInt());
            desAccount->transfer(application.currentAccount->getAccountNumber(),ui->amount->text().toInt(),1);
        }
    }
    message.setText("انتقال وجه با موفقیت انجام شد !");
    message.setWindowTitle("موفق");
    message.show();
    ui->accNum->setText("");
    ui->amount->setText("");
    application.writeUsers();
    application.refresh(currentAccNum);
}

void AccountDialog::on_cardbtn_clicked()
{
    if(application.currentAccount->getType()!=3)
    {
        application.refresh(currentAccNum);
        application.currentAccount->requestCard();
        application.writeUsers();
        application.refresh(currentAccNum);
        message.setText("درخواست شما ثبت و در لیست انتظار تایید مدیر قرار گرفت");
        message.setWindowTitle("تایید");
        message.show();
        on_tabWidget_tabBarClicked(3);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("تایید");
        msgBox.setText("حساب شما سپرده سرمایه گذاری بلند مدت است و با دریافت کارت به سپرده کوتاه مدت تبدیل میشود، آیا مطمئن هستید؟");
        msgBox.setStandardButtons(QMessageBox::Yes);
        msgBox.addButton(QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        if(msgBox.exec() == QMessageBox::Yes)
        {
            application.refresh(currentAccNum);
            application.currentAccount->requestCard();
            application.currentAccount->changeTypeToShortTerm();
            application.writeUsers();
            application.refresh(currentAccNum);
            message.setText("درخواست شما ثبت و در لیست انتظار تایید مدیر قرار گرفت");
            message.setWindowTitle("تایید");
            message.show();
            on_tabWidget_tabBarClicked(3);
        }
    }
}

void AccountDialog::on_pushButton_clicked()
{
    if(application.currentAccount->hasACard())
    {
        application.currentAccount->getCardPointer()->createPass();
        ui->passLbl->setText(QString::number(application.currentAccount->getCardPointer()->getPassword().getPasswordOfSecPass()));
        ui->passDatelbl->setText(application.currentAccount->getCardPointer()->getPassword().getExpireTime().toString());
        application.writeUsers();
        application.refresh(currentAccNum);
    }
    else
    {
        message.setText("شما کارت فعال ندارید !");
        message.setWindowTitle("خطا");
        message.show();
    }

}

void AccountDialog::on_pushButton_2_clicked()
{
    ui->accNum->setText("");
    ui->amount->setText("");
}

void AccountDialog::on_PDFBtn_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("save PDF"), "E:\\", tr("PDf Files (*.pdf)"));

        application.makePDF(application.currentAccount->getAccountNumber(),fileName);
}
