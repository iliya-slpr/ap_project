#include "adminpanel.h"
#include "ui_adminpanel.h"
#include <QDebug>
#include "showlogs.h"
#include "bankaccount.h"
#include "card.h"
#include "securepassword.h"
#include <QtGui>

#define ROW ui->usersTable->rowCount()-1
#define ROW_AP ui->pendingAccountsTable->rowCount()-1
#define ROW_CP ui->pendingCardTable->rowCount()-1
#define ROW_M ui->managementTable->rowCount()-1

AdminPanel::AdminPanel(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminPanel)
{
    crypt.setKey(10000);
    readUsers();
    ui->setupUi(this);
    setupUserTable();
    setupPendingAccountsTable();
    setupPendingCardsTable();
    setupManagementTable();
    setupTransactions();
}

AdminPanel::~AdminPanel()
{
    delete ui;
}
void AdminPanel::readUsers()
{
    QFile inFile("E:\\data.json");
    inFile.open(QIODevice::ReadOnly|QIODevice::Text);
    QByteArray data = inFile.readAll();
    inFile.close();

    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(data, &errorPtr);
    if (doc.isNull()) {
        message.setText("Parse faild!");
        message.show();
    }
    else
    {
        QJsonObject rootObj = doc.object();
        QJsonArray ptsArray = rootObj.value("users").toArray();
        userList.erase(userList.begin(),userList.end());
        foreach(const QJsonValue & val, ptsArray)
        {
            int id= val.toObject().value("personId").toInt();
            QString fullName =crypt.decryptToString(val.toObject().value("fullName").toString());
            QString username=crypt.decryptToString(val.toObject().value("username").toString());
            QString password=crypt.decryptToString(val.toObject().value("password").toString());
            QString nCode = crypt.decryptToString(val.toObject().value("nCode").toString());
            bool admin=val.toObject().value("admin").toBool();
            QDate birthDate = QDate::fromString(crypt.decryptToString(val.toObject().value("birthDate").toString()),"dd/MM/yyyy");
            QVector<PersonalLog> logs;
            QJsonArray logArray=val.toObject().value("logs").toArray();
            foreach(const QJsonValue & log, logArray)
            {
                logs.push_back(PersonalLog(QDate::fromString(log.toObject().value("logDate").toString(),"dd/MM/yyyy"),QTime::fromString(log.toObject().value("logTime").toString(),"HH/mm/ss"),log.toObject().value("logType").toBool()));
            }
            QVector<BankAccount> accounts;
            QJsonArray accountArray=val.toObject().value("accounts").toArray();

            foreach(const QJsonValue & account, accountArray)
            {
                QString accountNumber =crypt.decryptToString(account.toObject().value("accountNumber").toString());
                int type=account.toObject().value("type").toInt();
                int status=account.toObject().value("status").toInt();
                int balance=crypt.decryptToString(account.toObject().value("balance").toString()).toInt();
                bool hasCard=account.toObject().value("hasCard").toBool();
                QJsonArray ownerslist=account.toObject().value("owners").toArray();
                QStringList owners;
                foreach(const QJsonValue & owner, ownerslist)
                {
                    owners.push_back(crypt.decryptToString(owner.toObject().value("username").toString()));
                }
                QJsonObject cardObj=account.toObject().value("card").toObject();
                unsigned int password=crypt.decryptToString(cardObj.value("secPass").toObject().value("password").toString()).toUInt();
                QDate createdDate=QDate::fromString(crypt.decryptToString(cardObj.value("secPass").toObject().value("createdDate").toString()),"dd/MM/yyyy");
                QTime createdTime=QTime::fromString(crypt.decryptToString(cardObj.value("secPass").toObject().value("createdTime").toString()),"HH/mm/ss");
                QTime validTime=QTime::fromString(crypt.decryptToString(cardObj.value("secPass").toObject().value("validTime").toString()),"HH/mm/ss");
                int cardStatus=cardObj.value("status").toInt();
                int cvv2=crypt.decryptToString(cardObj.value("cvv2").toString()).toInt();
                QString cardNumber =crypt.decryptToString(cardObj.value("cardNumber").toString());
                QDate expireDate=QDate::fromString(crypt.decryptToString(cardObj.value("expireDate").toString()),"dd/MM/yyyy");
                Card card=Card(cardNumber,cvv2,expireDate,SecurePassword(password,createdTime,validTime,createdDate),cardStatus);
                QVector<transaction> transactionList;
                QJsonArray transactionArray=account.toObject().value("transactionList").toArray();
                foreach(const QJsonValue & Transaction, transactionArray)
                {
                    QJsonObject _originAcc=Transaction.toObject().value("originAcc").toObject();
                    QString originAccountNumber =crypt.decryptToString(_originAcc.value("accountNumber").toString());
                    int originAccountBalance =crypt.decryptToString(_originAcc.value("balance").toString()).toInt();
                    QJsonObject _desAcc=Transaction.toObject().value("desAcc").toObject();
                    QString desAccountNumber =crypt.decryptToString(_desAcc.value("accountNumber").toString());
                    int desAccountBalance =crypt.decryptToString(_desAcc.value("balance").toString()).toInt();


                    BankAccount originAcc(originAccountNumber,originAccountBalance);
                    BankAccount desAcc(desAccountNumber,desAccountBalance);
                    int amount=crypt.decryptToString(Transaction.toObject().value("amount").toString()).toInt();
                    bool type=Transaction.toObject().value("type").toBool();
                    QDate date = QDate::fromString(crypt.decryptToString(Transaction.toObject().value("trDate").toString()),"dd/MM/yyyy");
                    QTime time = QTime::fromString(crypt.decryptToString(Transaction.toObject().value("trTime").toString()),"HH/mm/ss");
                    transactionList.push_back(transaction(originAcc,desAcc,amount,type,time,date));
                }

                accounts.push_back(BankAccount(accountNumber,card,balance, type,status,hasCard,transactionList,owners));

            }
            userList.push_back(User(fullName,nCode,birthDate,username,password,logs,admin,id,accounts));
        }
    }}
void AdminPanel::writeUsers()
{
    QFile file("E:\\data.json");
    QJsonObject mainObj;
    QJsonArray mainArray;
    for(int i = 0 ; i<userList.size() ; i++){
        QJsonObject userObj;
        userObj["personId"]=userList[i].getId();
        userObj["fullName"]=crypt.encryptToString(userList[i].getFullName());
        userObj["nCode"]=crypt.encryptToString(userList[i].getNationalCode());
        userObj["birthDate"]=crypt.encryptToString(userList[i].getBirthDate().toString("dd/MM/yyyy"));
        userObj["username"]=crypt.encryptToString(userList[i].getUsername());
        userObj["password"]=crypt.encryptToString(userList[i].getPassword());
        QJsonArray logsArray;
        for(int j = 0 ; j < userList[i].getLog().size(); j++){
            QJsonObject logToAdd;
            logToAdd["logDate"] = userList[i].getLog()[j].getLogDate().toString("dd/MM/yyyy");
            logToAdd["logTime"] = userList[i].getLog()[j].getLogTime().toString("HH/mm/ss");
            logToAdd["logType"] = userList[i].getLog()[j].getLogType();
            logsArray.append(logToAdd);
        }
        userObj["logs"]=logsArray;
        QJsonArray accountsArray;
        for(int k=0 ; k < userList[i].getAccount().size() ; k++){
            QJsonObject accountToAdd;
            accountToAdd["accountNumber"]=crypt.encryptToString(userList[i].getAccount()[k].getAccountNumber());
            accountToAdd["balance"]=crypt.encryptToString(QString::number(userList[i].getAccount()[k].getBalance()));
            accountToAdd["type"]=userList[i].getAccount()[k].getType();
            accountToAdd["status"]=userList[i].getAccount()[k].getStatus();
            accountToAdd["hasCard"]=userList[i].getAccount()[k].hasACard();
            if(userList[i].getAccount()[k].hasACard())
            {
                QJsonObject cardObj;
                cardObj["cardNumber"]= crypt.encryptToString(userList[i].getAccount()[k].getCard().getCardNumber());
                cardObj["cvv2"]= crypt.encryptToString(QString::number(userList[i].getAccount()[k].getCard().getcvv2()));
                cardObj["status"]= userList[i].getAccount()[k].getCard().getStatus();
                cardObj["expireDate"]=crypt.encryptToString(userList[i].getAccount()[k].getCard().getExpireDate().toString("dd/MM/yyyy"));
                QJsonObject pass;
                pass["password"]=crypt.encryptToString(QString::number(userList[i].getAccount()[k].getCard().getPassword().getPasswordOfSecPass()));
                pass["createdTime"]=crypt.encryptToString(userList[i].getAccount()[k].getCard().getPassword().getCreatedTime().toString("HH/mm/ss"));
                pass["validTime"]=crypt.encryptToString(userList[i].getAccount()[k].getCard().getPassword().getExpireTime().toString("HH/mm/ss"));
                pass["createdDate"]=crypt.encryptToString(userList[i].getAccount()[k].getCard().getPassword().getCreatedDate().toString("dd/MM/yyyy"));
                cardObj["secPass"]= pass;
                accountToAdd["card"]=cardObj;
            }
            else accountToAdd["card"]=0;
            QJsonArray ownersArray;
            for(int m = 0 ; m< userList[i].getAccount()[k].getOwnerUsername().size();m++){
                QJsonObject ownerToAdd;
                ownerToAdd["username"] = crypt.encryptToString(userList[i].getAccount()[k].getOwnerUsername()[m]);
                ownersArray.append(ownerToAdd);
            }
            accountToAdd["owners"] = ownersArray;
            QJsonArray transactionsArray;
            for(int l=0; l<userList[i].getAccount()[k].getTransactions().size();l++){
                QJsonObject transactionToAdd;
                transactionToAdd["type"] = userList[i].getAccount()[k].getTransactions()[l].getType();
                QJsonObject originAcc;
                originAcc["accountNumber"]=crypt.encryptToString(userList[i].getAccount()[k].getTransactions()[l].getOriginBankAcc().getAccountNumber());
                originAcc["balance"]=crypt.encryptToString(QString::number(userList[i].getAccount()[k].getTransactions()[l].getOriginBankAcc().getBalance()));
                transactionToAdd["originAcc"] = originAcc;
                QJsonObject desAcc;
                desAcc["accountNumber"]=crypt.encryptToString(userList[i].getAccount()[k].getTransactions()[l].getDesBankAcc().getAccountNumber());
                desAcc["balance"]=crypt.encryptToString(QString::number(userList[i].getAccount()[k].getTransactions()[l].getDesBankAcc().getBalance()));
                transactionToAdd["desAcc"] = desAcc;
                transactionToAdd["amount"] = crypt.encryptToString(QString::number(userList[i].getAccount()[k].getTransactions()[l].getAmount()));
                transactionToAdd["trDate"] = crypt.encryptToString(userList[i].getAccount()[k].getTransactions()[l].getDate().toString("dd/MM/yyyy"));
                transactionToAdd["trTime"] = crypt.encryptToString(userList[i].getAccount()[k].getTransactions()[l].getTime().toString("HH/mm/ss"));
                transactionsArray.append(transactionToAdd);
            }
            accountToAdd["transactionList"]= transactionsArray;
            accountsArray.append(accountToAdd);
        }
        userObj["accounts"]=accountsArray;
        mainArray.append(userObj);
    }
    mainObj["users"] = mainArray;
    QJsonDocument mainDoc(mainObj);


    file.open(QIODevice::WriteOnly);
    file.write(mainDoc.toJson());

}

void AdminPanel::setupUserTable()
{
    int rows = ui->usersTable->rowCount();
    for(int i = 0 ; i < rows ; i++)
        ui->usersTable->removeRow(0);

    QStringList titles;
    ui->usersTable->setColumnCount(5);
    titles<<"نام"<<"کدملی"<<"تاریخ تولد"<<"نام کاربری"<<"تعداد اکانت";
    ui->usersTable->setHorizontalHeaderLabels(titles);
    for(int i = 0 ; i < userList.size() ; i++){
        ui->usersTable->insertRow(ui->usersTable->rowCount());
        ui->usersTable->setItem(ROW,0,new QTableWidgetItem(userList[i].getFullName()));
        ui->usersTable->setItem(ROW,1,new QTableWidgetItem(userList[i].getNationalCode()));
        ui->usersTable->setItem(ROW,2,new QTableWidgetItem(userList[i].getBirthDate().toString("yyyy/MM/dd")));
        ui->usersTable->setItem(ROW,3,new QTableWidgetItem(userList[i].getUsername()));
        ui->usersTable->setItem(ROW,4,new QTableWidgetItem(QString::number(userList[i].getAccount().size())));
    }
}
void AdminPanel::removeUser(QString nameToRemove){
    for(int i = 0; i< userList.size() ; i++){
        if(userList[i].getFullName()==nameToRemove){
            userList.remove(i);
            setupUserTable();
            break;

        }
    }
}

void AdminPanel::setupPendingAccountsTable()
{
    int rows = ui->pendingAccountsTable->rowCount();
    for(int i = 0 ; i < rows ; i++)
        ui->pendingAccountsTable->removeRow(0);

    QStringList titles;
    ui->pendingAccountsTable->setColumnCount(7);
    titles<<"نام"<<"کدملی"<<"تاریخ تولد"<<"شماره حساب"<<"موجودی"<<"userId"<<"accId";
    ui->pendingAccountsTable->setHorizontalHeaderLabels(titles);


    for(int i = 0 ; i < userList.size() ; i++){
        for(int j = 0 ; j < userList[i].getAccount().size() ; j++)
            if(userList[i].getAccount()[j].getStatus()==2){
                ui->pendingAccountsTable->insertRow(ui->pendingAccountsTable->rowCount());

                ui->pendingAccountsTable->setItem(ROW_AP,0,new QTableWidgetItem(userList[i].getFullName()));
                ui->pendingAccountsTable->setItem(ROW_AP,1,new QTableWidgetItem(userList[i].getNationalCode()));
                ui->pendingAccountsTable->setItem(ROW_AP,2,new QTableWidgetItem(userList[i].getBirthDate().toString("yyyy/MM/dd")));
                ui->pendingAccountsTable->setItem(ROW_AP,3,new QTableWidgetItem(userList[i].getAccount()[j].getAccountNumber()));
                ui->pendingAccountsTable->setItem(ROW_AP,4,new QTableWidgetItem(QString::number(userList[i].getAccount()[j].getBalance())));
                ui->pendingAccountsTable->setItem(ROW_AP,5,new QTableWidgetItem(QString::number(i)));
                ui->pendingAccountsTable->setItem(ROW_AP,6,new QTableWidgetItem(QString::number(j)));
                qDebug()<<i;
                qDebug()<<j;
            }
    }


}

void AdminPanel::setupPendingCardsTable()
{
    int rows = ui->pendingCardTable->rowCount();
    for(int i = 0 ; i < rows ; i++)
        ui->pendingCardTable->removeRow(0);

    QStringList titles;
    ui->pendingCardTable->setColumnCount(7);
    titles<<"نام"<<"کدملی"<<"شماره حساب"<<"شماره کارت"<<"موجودی"<<"userId"<<"accId";
    ui->pendingCardTable->setHorizontalHeaderLabels(titles);


    for(int i = 0 ; i < userList.size() ; i++){
        for(int j = 0 ; j < userList[i].getAccount().size() ; j++)
            if(  userList[i].getAccount()[j].hasACard() && userList[i].getAccount()[j].getCard().getStatus()==2){
                ui->pendingCardTable->insertRow(ui->pendingCardTable->rowCount());
                ui->pendingCardTable->setItem(ROW_CP,0,new QTableWidgetItem(userList[i].getFullName()));
                ui->pendingCardTable->setItem(ROW_CP,1,new QTableWidgetItem(userList[i].getNationalCode()));
                ui->pendingCardTable->setItem(ROW_CP,2,new QTableWidgetItem(userList[i].getAccount()[j].getAccountNumber()));
                ui->pendingCardTable->setItem(ROW_CP,3,new QTableWidgetItem(userList[i].getAccount()[j].getCard().getCardNumber()));
                ui->pendingCardTable->setItem(ROW_CP,4,new QTableWidgetItem(QString::number(userList[i].getAccount()[j].getBalance())));
                ui->pendingCardTable->setItem(ROW_CP,5,new QTableWidgetItem(QString::number(i)));
                ui->pendingCardTable->setItem(ROW_CP,6,new QTableWidgetItem(QString::number(j)));

            }
    }
}

void AdminPanel::setupTransactions(){
    QString trQString = "";
    qDebug()<<":|";
    for(int i = 0 ; i < userList.size() ; i++){
        for (int j = 0 ;j < userList[i].getAccount().size(); j++) {
            for(int k = 0 ; k <userList[i].getAccount()[j].getTransactions().size() ; k++){
                trQString += "از حساب: ";
                trQString += userList[i].getAccount()[j].getTransactions()[k].getOriginBankAcc().getAccountNumber();
                trQString += "به حساب: ";
                trQString += userList[i].getAccount()[j].getTransactions()[k].getDesBankAcc().getAccountNumber();
                trQString += "به مبلغ: ";
                trQString += QString::number(userList[i].getAccount()[j].getTransactions()[k].getAmount());
                trQString += "در تاریخ: ";
                trQString += userList[i].getAccount()[j].getTransactions()[k].getDate().toString("dd/MM/yyyy");
                trQString += "در ساعت: ";
                trQString += userList[i].getAccount()[j].getTransactions()[k].getTime().toString("HH:mm:ss");
                trQString += "\n\n";
            }
        }
    }
    ui->transactionTE->setText(trQString);



}
void AdminPanel::on_deleteBtn_clicked()
{
    if(!ui->usersTable->selectedItems().isEmpty())
    {
        int ret = QMessageBox::information(this, tr("حذف کاربر"),
                                           "آیا از حذف این کاربر اطمینان دارید؟",
                                           QMessageBox::Ok
                                           | QMessageBox::Cancel,
                                           QMessageBox::Ok);

        if(ret == QMessageBox::Ok){
            QString nameToRemove = ui->usersTable->item(ui->usersTable->currentRow(),0)->text();
            removeUser(nameToRemove);
            writeUsers();
        }
    }
    else
    {
        message.setText("لطفا ابتدا یک ردیف را انتخاب نمایید !");
        message.setWindowTitle("خطا");
        message.show();
    }
}

void AdminPanel::getDataToAdd(QString _fn, QString _nc, QDate _d, QString _u, QString _p)
{
    userList.push_back(User(_fn,_nc , _d, _u , _p));
    writeUsers();
    setupUserTable();
}

void AdminPanel::on_addUserBtn_clicked()
{
    addUser *d;
    d = new addUser();
    connect(d,SIGNAL(sendDataToAdd(QString ,QString,QDate,QString,QString)),this, SLOT(getDataToAdd(QString ,QString,QDate,QString,QString)));
    d->show();
}

void AdminPanel::on_confirmBtn_clicked()
{
    if(!ui->pendingAccountsTable->selectedItems().isEmpty())
    {
        int userToAccept = ui->pendingAccountsTable->item(ui->pendingAccountsTable->currentRow(),5)->text().toInt();
        int accountToAccept = ui->pendingAccountsTable->item(ui->pendingAccountsTable->currentRow(),6)->text().toInt();
        message.setText("آیا از تایید حساب "+userList[userToAccept].getFullName()+" مطمئن هستید؟ ");
        message.setWindowTitle("هشدار");
        message.setStandardButtons(QMessageBox::Yes);
        message.addButton(QMessageBox::No);
        message.setDefaultButton(QMessageBox::No);
        if(message.exec() == QMessageBox::Yes)
        {
            userList[userToAccept].getAccountPointer(accountToAccept)->changeStatusToActive();
            ui->pendingAccountsTable->removeRow(ui->pendingAccountsTable->currentRow());
            writeUsers();
        }
    }
    else
    {
        message.setText("لطفا ابتدا یک ردیف را انتخاب نمایید !");
        message.setWindowTitle("خطا");
        message.show();
    }

}

void AdminPanel::on_rejectBtn_clicked()
{
    if(!ui->pendingAccountsTable->selectedItems().isEmpty())
    {
        int userToAccept = ui->pendingAccountsTable->item(ui->pendingAccountsTable->currentRow(),5)->text().toInt();
        int accountToAccept = ui->pendingAccountsTable->item(ui->pendingAccountsTable->currentRow(),6)->text().toInt();
        message.setText("آیا از رد حساب "+userList[userToAccept].getFullName()+" مطمئن هستید؟ ");
        message.setWindowTitle("هشدار");
        message.setStandardButtons(QMessageBox::Yes);
        message.addButton(QMessageBox::No);
        message.setDefaultButton(QMessageBox::No);
        if(message.exec() == QMessageBox::Yes)
        {
            userList[userToAccept].getAccountPointer(accountToAccept)->changeStatusToBlock();
            ui->pendingAccountsTable->removeRow(ui->pendingAccountsTable->currentRow());
            writeUsers();
        }
    }
    else
    {
        message.setText("لطفا ابتدا یک ردیف را انتخاب نمایید !");
        message.setWindowTitle("خطا");
        message.show();
    }
}

void AdminPanel::on_confirmCardBtn_clicked()
{
    if(!ui->pendingCardTable->selectedItems().isEmpty())
    {
        int userToAccept = ui->pendingCardTable->item(ui->pendingCardTable->currentRow(),5)->text().toInt();
        int accountToAccept = ui->pendingCardTable->item(ui->pendingCardTable->currentRow(),6)->text().toInt();
        message.setText("آیا از تایید کارت "+userList[userToAccept].getFullName()+" مطمئن هستید؟ ");
        message.setWindowTitle("هشدار");
        message.setStandardButtons(QMessageBox::Yes);
        message.addButton(QMessageBox::No);
        message.setDefaultButton(QMessageBox::No);
        if(message.exec() == QMessageBox::Yes)
        {
            userList[userToAccept].getAccountPointer(accountToAccept)->getCardPointer()->changeStatusToActive();
            ui->pendingCardTable->removeRow(ui->pendingCardTable->currentRow());
            writeUsers();
        }
    }
    else
    {
        message.setText("لطفا ابتدا یک ردیف را انتخاب نمایید !");
        message.setWindowTitle("خطا");
        message.show();
    }
}

void AdminPanel::on_rejeCardBtn_clicked()
{
    if(!ui->pendingCardTable->selectedItems().isEmpty())
    {
        int userToAccept = ui->pendingCardTable->item(ui->pendingCardTable->currentRow(),5)->text().toInt();
        int accountToAccept = ui->pendingCardTable->item(ui->pendingCardTable->currentRow(),6)->text().toInt();
        message.setText("آیا از رد کارت "+userList[userToAccept].getFullName()+" مطمئن هستید؟ ");
        message.setWindowTitle("هشدار");
        message.setStandardButtons(QMessageBox::Yes);
        message.addButton(QMessageBox::No);
        message.setDefaultButton(QMessageBox::No);
        if(message.exec() == QMessageBox::Yes)
        {
            userList[userToAccept].getAccountPointer(accountToAccept)->getCardPointer()->changeStatusToBlock();
            ui->pendingCardTable->removeRow(ui->pendingCardTable->currentRow());
            writeUsers();
        }
    }
    else
    {
        message.setText("لطفا ابتدا یک ردیف را انتخاب نمایید !");
        message.setWindowTitle("خطا");
        message.show();
    }

}

void AdminPanel::on_logsBtn_clicked()
{
    if(!ui->usersTable->selectedItems().isEmpty())
    {
        showLogs *d;
        d = new showLogs();
        int selectedUser = ui->usersTable->currentRow();
        User currentUser = userList[selectedUser];
        QString _loginLog ="", _logoutLog="";
        for(int i=currentUser.getLog().size()-1;i>=0;i--)
        {
            QString temp="تاریخ : ";
            temp+=currentUser.getLog()[i].getLogDate().toString("yyyy/MM/dd");
            temp+=" ساعت : ";
            temp+=currentUser.getLog()[i].getLogTime().toString();
            temp+=currentUser.getLog()[i].getLogType()==false?" نوع: ورود":" نوع: خروج ";
            qDebug()<<currentUser.getLog()[i].getLogType();
            if(currentUser.getLog()[i].getLogType()==false)
                _loginLog +=temp+"\n";
            else
                _logoutLog +=temp+"\n";

        }
        connect(this,SIGNAL(sendLogsToDialog(QString ,QString)) ,d,SLOT(getLogs(QString, QString)));
        emit sendLogsToDialog(_loginLog ,_logoutLog );
        d->show();
    }
    else
    {
        message.setText("لطفا ابتدا یک ردیف را انتخاب نمایید !");
        message.setWindowTitle("خطا");
        message.show();
    }

}

void AdminPanel::setupManagementTable()
{
    int rows= ui->managementTable->rowCount();
    for(int i = 0 ; i < rows ; i++)
        ui->managementTable->removeRow(0);
    QStringList titles;
    ui->managementTable->setColumnCount(8);
    titles<<"نام"<<"شماره حساب"<<"نوع حساب"<<"وضعیت حساب"<<"شماره کارت"<<"cvv2"<<"تاریخ انقضای کارت"<<"وضعیت کارت";
    ui->managementTable->setHorizontalHeaderLabels(titles);
    for(int i=0;i< userList.size();i++)
    {
        QVector<BankAccount> accounts=userList[i].getAccount();
        for(int j=0;j<accounts.size();j++)
        {
            ui->managementTable->insertRow(ui->managementTable->rowCount());
            ui->managementTable->setItem(ROW_M,0,new QTableWidgetItem(userList[i].getFullName()));
            ui->managementTable->setItem(ROW_M,1,new QTableWidgetItem(accounts[j].getAccountNumber()));
            ui->managementTable->setItem(ROW_M,2,new QTableWidgetItem(getType(accounts[j].getType())));
            ui->managementTable->setItem(ROW_M,3,new QTableWidgetItem(getStatus(accounts[j].getStatus())));
            Card card=accounts[j].getCard();
            if(accounts[j].hasACard())
            {
                ui->managementTable->setItem(ROW_M,4,new QTableWidgetItem(card.getCardNumber()));
                ui->managementTable->setItem(ROW_M,5,new QTableWidgetItem(QString::number(card.getcvv2())));
                ui->managementTable->setItem(ROW_M,6,new QTableWidgetItem(card.getExpireDate().toString("yyyy/MM/dd")));
                ui->managementTable->setItem(ROW_M,7,new QTableWidgetItem(getStatus(card.getStatus())));
            }
            else
            {
                ui->managementTable->setItem(ROW_M,4,new QTableWidgetItem(""));
                ui->managementTable->setItem(ROW_M,5,new QTableWidgetItem(""));
                ui->managementTable->setItem(ROW_M,6,new QTableWidgetItem(""));
                ui->managementTable->setItem(ROW_M,7,new QTableWidgetItem(""));
            }
        }
    }
    ui->managementTable->setColumnWidth(2,300);
    ui->managementTable->setColumnWidth(4,200);
}

QString AdminPanel::getStatus(int s)
{
    QString status;

    if(s==0)
        status="فعال";
    else if(s==1)
        status="مسدود";
    else if(s==2)
        status="درحال بررسی";


    return status;
}

QString AdminPanel::getType(int s)
{
    QString type;
    if(s==0)
        type="سپرده قرض الحسنه پس انداز";
    else if(s==1)
        type="سپرده سرمایه گذاری کوتاه مدت";
    else if(s==2)
        type="سپرده سرمایه گذاری کوتاه مدت حقوقی";
    else if(s==3)
        type="سپرده سرمایه گذاری بلند مدت";
    return type;
}

void AdminPanel::changeAccountStatus(QString nameToChange,int accountToChange)
{
    for(int i = 0; i< userList.size() ; i++)
    {
        if(userList[i].getFullName()==nameToChange)
        {
            QVector<BankAccount> accounts=userList[i].getAccount();
            for(int j=0;j<userList[i].getAccount().size();j++)
            {
                qDebug()<<j;
                if((accounts[j].getAccountNumber()).toInt()==accountToChange)
                {

                    if(accounts[j].getStatus()==0)
                    {
                        message.setText("آیا از تغییر وضعیت حساب "+accounts[j].getAccountNumber()+" به نام "+nameToChange+" به وضعیت مسدود مطمئن هستید؟ ");
                        message.setWindowTitle("هشدار");
                        message.setStandardButtons(QMessageBox::Yes);
                        message.addButton(QMessageBox::No);
                        message.setDefaultButton(QMessageBox::No);
                        if(message.exec() == QMessageBox::Yes)
                        {
                            userList[i].getAccountPointer(j)->changeStatusToBlock();
                        }
                    }
                    else if(accounts[j].getStatus()==1)
                    {
                        message.setText("آیا از تغییر وضعیت حساب "+accounts[j].getAccountNumber()+" به نام "+nameToChange+" به وضعیت فعال مطمئن هستید؟ ");
                        message.setWindowTitle("هشدار");
                        message.setStandardButtons(QMessageBox::Yes);
                        message.addButton(QMessageBox::No);
                        message.setDefaultButton(QMessageBox::No);
                        if(message.exec() == QMessageBox::Yes)
                        {
                            userList[i].getAccountPointer(j)->changeStatusToActive();
                        }
                    }
                    setupManagementTable();
                    break;
                }
            }
        }
    }
}

void AdminPanel::changeCardStatus(QString nameToChange, int accountToChange)
{
    for(int i = 0; i< userList.size() ; i++)
    {
        if(userList[i].getFullName()==nameToChange)
        {
            QVector<BankAccount> accounts=userList[i].getAccount();
            for(int j=0;j<accounts.size();j++)
            {
                if((accounts[j].getAccountNumber()).toInt()==accountToChange)
                {
                    Card card=accounts[j].getCard();
                    if(card.getStatus()==0)
                    {
                        message.setText("آیا از تغییر وضعیت کارت "+accounts[j].getCard().getCardNumber()+" به نام "+nameToChange+" به وضعیت مسدود مطمئن هستید؟ ");
                        message.setWindowTitle("هشدار");
                        message.setStandardButtons(QMessageBox::Yes);
                        message.addButton(QMessageBox::No);
                        message.setDefaultButton(QMessageBox::No);
                        if(message.exec() == QMessageBox::Yes)
                        {
                            userList[i].getAccountPointer(j)->getCardPointer()->changeStatusToBlock();
                        }
                    }
                    else if(card.getStatus()==1)
                    {
                        message.setText("آیا از تغییر وضعیت کارت "+accounts[j].getCard().getCardNumber()+" به نام "+nameToChange+" به وضعیت فعال مطمئن هستید؟ ");
                        message.setWindowTitle("هشدار");
                        message.setStandardButtons(QMessageBox::Yes);
                        message.addButton(QMessageBox::No);
                        message.setDefaultButton(QMessageBox::No);
                        if(message.exec() == QMessageBox::Yes)
                        {
                            userList[i].getAccountPointer(j)->getCardPointer()->changeStatusToActive();
                        }
                    }

                    setupManagementTable();
                    break;
                }
            }
        }
    }
}

void AdminPanel::on_changeStatusBtn_clicked()
{
    if(!ui->managementTable->selectedItems().isEmpty())
    {
        if(ui->managementTable->item(ui->managementTable->currentRow(),3)->text()=="مسدود" || ui->managementTable->item(ui->managementTable->currentRow(),3)->text()=="فعال")
            changeAccountStatus(ui->managementTable->item(ui->managementTable->currentRow(),0)->text(),(ui->managementTable->item(ui->managementTable->currentRow(),1)->text()).toInt());
        writeUsers();
    }
    else
    {
        message.setText("لطفا ابتدا یک ردیف را انتخاب نمایید !");
        message.setWindowTitle("خطا");
        message.show();
    }
}

void AdminPanel::on_changeCardStatusBtn_clicked()
{
    if(!ui->managementTable->selectedItems().isEmpty())
    {
        if(ui->managementTable->item(ui->managementTable->currentRow(),7)->text()=="فعال" || ui->managementTable->item(ui->managementTable->currentRow(),7)->text()=="مسدود")
            changeCardStatus(ui->managementTable->item(ui->managementTable->currentRow(),0)->text(),(ui->managementTable->item(ui->managementTable->currentRow(),1)->text()).toInt());
        writeUsers();
    }
    else
    {
        message.setText("لطفا ابتدا یک ردیف را انتخاب نمایید !");
        message.setWindowTitle("خطا");
        message.show();
    }
}

void AdminPanel::on_longAccountBtn_clicked()
{
    readUsers();
    for(int i=0;i<userList.size();i++)
    {
        for(int j=0;j<userList[i].getAccount().size();j++)
        {
            if(userList[i].getAccountPointer(j)->getType()==3&&userList[i].getAccountPointer(j)->getStatus()==0)
            {
                userList[i].getAccountPointer(j)->plusBalance(userList[i].getAccountPointer(j)->getBalance()*0.15);
            }
        }
    }
    message.setText("عملیات با موفقیت انجام شد");
    message.setWindowTitle("موفق");
    message.show();
    writeUsers();
}

void AdminPanel::on_shortAccountBtn_clicked()
{
    readUsers();
    for(int i=0;i<userList.size();i++)
    {
        for(int j=0;j<userList[i].getAccount().size();j++)
        {
            if(userList[i].getAccountPointer(j)->getType()==2||userList[i].getAccountPointer(j)->getType()==1)
            {
                if(userList[i].getAccountPointer(j)->getStatus()==0)
                    userList[i].getAccountPointer(j)->plusBalance(userList[i].getAccountPointer(j)->getBalance()*0.05);
            }
        }
    }
    message.setText("عملیات با موفقیت انجام شد");
    message.setWindowTitle("موفق");
    message.show();
    writeUsers();
}

void AdminPanel::on_managementTable_cellClicked(int row, int column)
{
    if(ui->managementTable->item(row,3)->text()=="درحال بررسی")
        ui->changeStatusBtn->setDisabled(true);
    else
        ui->changeStatusBtn->setDisabled(false);

    if(ui->managementTable->item(row,7)->text().isEmpty())
        ui->changeCardStatusBtn->setDisabled(true);
    else if(ui->managementTable->item(row,7)->text()=="درحال بررسی")
        ui->changeCardStatusBtn->setDisabled(true);
    else
        ui->changeCardStatusBtn->setDisabled(false);
}
