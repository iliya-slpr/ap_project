#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "application.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("درگاه پرداخت");
    application.readUsers();
    confirm=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_transferButton_clicked()
{
    application.readUsers();
    int originCardNumber = ui->SourceCardNumberLineEdit->text().toInt();
    int amountToTransfer = ui->transactionAmountLineEdit->text().toInt();
    int expireMonth = ui->monthLed->text().toInt();
    int expireYear = ui->yearLed->text().toInt();
    int destinationCardNumber = ui->destinationCardNumberLineEdit->text().toInt();
    int cvv2 = ui->cvv2LineEdit->text().toInt();
    int secPass = ui->SecurePasswordLineEdit->text().toInt();
    Card tmp_origin = application.getCard(originCardNumber);
    Card tmp_des = application.getCard(destinationCardNumber);

    QString error;
        try {
            QString e;

            if(ui->SourceCardNumberLineEdit->text().isEmpty() || ui->cvv2LineEdit->text().isEmpty() || ui->SecurePasswordLineEdit->text().isEmpty() || ui->destinationCardNumberLineEdit->text().isEmpty() || ui->transactionAmountLineEdit->text().isEmpty()){
                e="پر کردن تمامی فیلد ها الزامیست!";
                throw e;
            }
            else if(tmp_origin.getcvv2()==0||tmp_des.getcvv2()==0){
                e="یکی از کارت ها وجود ندارد";
                throw e;
            }
            else if(tmp_origin.getcvv2() != cvv2 || tmp_origin.getExpireDate().month() != expireMonth || tmp_origin.getExpireDate().year() != expireYear ){
                e="کارت معتبر نیست";
                throw e;
            }
            else if(tmp_origin.getStatus()!=0 || tmp_des.getStatus()!=0){
                e = "وضعیت یکی از کارت ها فعال نیست";
                throw e;
            }
            else if(tmp_origin.isExpire() || tmp_des.isExpire()){
                e="یکی از کارت ها منقضی شده است";
                throw e;
            }
            else if(tmp_origin.getPassword().getPasswordOfSecPass() != secPass){
                e="رمز وارد شده نادرست است";
                throw e;
            }

            else if(application.getAccount(originCardNumber)->getBalance()<amountToTransfer){
                e="موجودی کافی نیست";
                throw e;
            }
            if(!tmp_origin.getPassword().isValid()){
                e="مدت زمان رمز شما به پایان رسیده است ";
                throw e;
            }
            confirm=1;
        } catch (QString _error) {
            QMessageBox::information(this, tr("خطا"),
                                     _error,
                                     QMessageBox::Ok
                                     ,
                                     QMessageBox::Ok);
            return;
    }
    if(confirm==1){
        application.readUsers();
        BankAccount *originAcc= application.getAccount(originCardNumber);
        BankAccount *desAcc= application.getAccount(destinationCardNumber);

        if(originAcc->getType()==2)
        {
            for(int i=0;i<originAcc->getOwnerUsername().size();i++)
            {
                BankAccount* temp1=application.findAccount(originAcc->getAccountNumber(),originAcc->getOwnerUsername()[i]);
                temp1->minusBalance(amountToTransfer);
                temp1->transfer(desAcc->getAccountNumber(),amountToTransfer,0);
            }
        }
        else
        {
            originAcc->minusBalance(amountToTransfer);
            originAcc->transfer(desAcc->getAccountNumber(),amountToTransfer,0);
        }
        if(desAcc->getType()==2)
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("تایید");
            msgBox.setText("آیا از انتقال وجه به حساب حقوقی مطمئن هستید ؟");
            msgBox.setStandardButtons(QMessageBox::Yes);
            msgBox.addButton(QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            if(msgBox.exec() == QMessageBox::Yes)
            {
                for(int i=0;i<desAcc->getOwnerUsername().size();i++)
                {
                    BankAccount* temp2=application.findAccount(desAcc->getAccountNumber(),desAcc->getOwnerUsername()[i]);
                    temp2->plusBalance(amountToTransfer);
                    temp2->transfer(originAcc->getAccountNumber(),amountToTransfer,1);
                }
            }
        }
        else
        {
            User* desUser=application.findUser(desAcc->getAccountNumber());
            QMessageBox msgBox;
            msgBox.setWindowTitle("تایید");
            msgBox.setText("آیا از انتقال وجه به حساب "+desUser->getFullName()+"مطمئن هستید ؟");
            msgBox.setStandardButtons(QMessageBox::Yes);
            msgBox.addButton(QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            if(msgBox.exec() == QMessageBox::Yes)
            {
                desAcc->plusBalance(amountToTransfer);
                desAcc->transfer(originAcc->getAccountNumber(),amountToTransfer,1);
            }
        }
        application.writeUsers();
        QMessageBox msgBox;
        msgBox.setText("عملیات با موفقیت انجام شد.");
        msgBox.setWindowTitle("موفق");
        msgBox.exec();
        ui->cvv2LineEdit->setText("");
        ui->SecurePasswordLineEdit->setText("");
        ui->SourceCardNumberLineEdit->setText("");
        ui->transactionAmountLineEdit->setText("");
        ui->destinationCardNumberLineEdit->setText("");
    }
}

void MainWindow::on_pushButton_clicked()
{
    this->close();
}
