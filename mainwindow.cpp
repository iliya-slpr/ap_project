#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    application.readUsers();
    login=new loginDialog();
    int result=login->exec();
    while(1)
    {
        if(result==QDialog::Accepted)
        {
            loginSeccess=application.login(login->getUsername(),login->getPassword());
            application.writeUsers();
            if(loginSeccess)
                break;
        }
        else if(result==QDialog::Rejected)
        {
            this->close();
            break;
        }
    }
    ui->setupUi(this);
    on_editProfileTab_tabBarClicked(0);
    on_editProfileTab_tabBarClicked(1);
    on_lineEdit_textChanged("");

}
bool MainWindow::getLoginSeccess()
{
    return loginSeccess;
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_editProfileTab_tabBarClicked(int index)
{
    if (index==0)
    {
        ui->nameLed->setText(application.currentUser->getFullName());
        ui->nCodeLed->setText(application.currentUser->getNationalCode());
        ui->BdateLed->setDate(application.currentUser->getBirthDate());
        ui->usernameLed->setText(application.currentUser->getUsername());
        ui->passLed->setText(application.currentUser->getPassword());
    }
    if(index==1)
    {
        ui->loginList->clear();
        ui->logoutList->clear();
        for(int i=application.currentUser->getLog().size()-1;i>=0;i--)
        {
            QString temp="تاریخ : ";
            temp+=application.currentUser->getLog()[i].getLogDate().toString();
            temp+=" ساعت : ";
            temp+=application.currentUser->getLog()[i].getLogTime().toString();
            temp+=application.currentUser->getLog()[i].getLogType()==0?" نوع: ورود":" نوع: خروج ";
            if(application.currentUser->getLog()[i].getLogType()==0)
                ui->loginList->insertItem(i,new  QListWidgetItem(temp));
            else
                ui->logoutList->insertItem(i,new  QListWidgetItem(temp));
            if(i<application.currentUser->getLog().size()-10)
                break;

        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    application.currentUser->editProfile(ui->nameLed->text(),ui->nCodeLed->text(),ui->BdateLed->date(),ui->usernameLed->text(),ui->passLed->text());
    application.writeUsers();
    application.refresh(application.getUserIndex());
    message.setText("ویرایش اطلاعات موقیت آمیز بود ! ");
    message.setWindowTitle("موفق");
    message.show();

}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    application.refresh(application.getUserIndex());
    ui->accountTable->clear();
    ui->accountTable->setColumnCount(4);
    ui->accountTable->setHorizontalHeaderLabels(headers);
    ui->accountTable->setRowCount(0);
    for(int i=0;i<application.currentUser->getAccount().size();i++)
    {
        if(strstr(application.currentUser->getAccount()[i].getAccountNumber().toUtf8().constData(),arg1.toUtf8().constData()))
        {
            ui->accountTable->insertRow(ui->accountTable->rowCount());
            int row=ui->accountTable->rowCount()-1;
            ui->accountTable->setItem(row,0,new QTableWidgetItem(application.currentUser->getAccount()[i].getAccountNumber()));
            QString type;
            if(application.currentUser->getAccount()[i].getType()==0)type="قرض الحسنه";
            else if(application.currentUser->getAccount()[i].getType()==1)type="کوتاه مدت";   ///// SAVING=0 , SHORT_TERM=1 , SHORT_TERM_LEGAL=2 , LONG_TERM=3
            else if(application.currentUser->getAccount()[i].getType()==2)type="کوتاه مدت حقوقی";
            else if(application.currentUser->getAccount()[i].getType()==4)type="بلند مدت";
            ui->accountTable->setItem(row,1,new QTableWidgetItem(type));
            ui->accountTable->setItem(row,2,new QTableWidgetItem(QString::number(application.currentUser->getAccount()[i].getBalance())));
            QString status;
            if(application.currentUser->getAccount()[i].getStatus()==0)status="فعال";
            else if(application.currentUser->getAccount()[i].getStatus()==1)status="غیرفعال";      ///// ACTIVE=0 , BLOCK=1 , PENDIN2 , REJECT=3
            else if(application.currentUser->getAccount()[i].getStatus()==2)status="درانتظار تایید";
            else if(application.currentUser->getAccount()[i].getStatus()==4)status="رد شده";
            ui->accountTable->setItem(row,3,new QTableWidgetItem(status));
        }
    }
}

void MainWindow::on_selectAccount_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("تایید");
    msgBox.setText("آیا از انتخاب حساب :"+ui->accountTable->item(ui->accountTable->currentRow(),0)->text()+"مطمئن هستید ؟");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes)
    {
        accountDialog=new AccountDialog();
        connect(this,SIGNAL(sendCurrentAcc(QString)),accountDialog,SLOT(receiveAccount(QString)));
        emit sendCurrentAcc(ui->accountTable->item(ui->accountTable->currentRow(),0)->text());
        accountDialog->show();
    }
}

void MainWindow::on_addAccount_clicked()
{
    addform=new addAccountDialog();
    int a=addform->exec();
    if(a==QDialog::Accepted)
    {
        BankAccount temp(addform->getBalance(),addform->getType());
        application.currentUser->getAccountsPointer()->push_back(temp);
        application.writeUsers();
        application.refresh(application.getUserIndex());
        on_lineEdit_textChanged("");
    }
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if(index==2)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("تایید");
        msgBox.setText("آیا مطمئن هستید ؟");
        msgBox.setStandardButtons(QMessageBox::Yes);
        msgBox.addButton(QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        if(msgBox.exec() == QMessageBox::Yes)
        {
            application.logout();
            this->close();
        }

    }
}
