#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for (int i=0;i<userList.size();i++)
    {
        for(int j=0;j<userList[i].getAccount().size();j++)
        {
            accountList.push_back(&userList[i].getAccount()[j]);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readUsers()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/",
                                                    tr("Json File (*.json)"));
    QFile inFile(fileName);
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
            QString fullName =val.toObject().value("fullName").toString();
            QString username=val.toObject().value("username").toString();
            QString password=val.toObject().value("password").toString();
            QString nCode=val.toObject().value("nCode").toString();
            bool admin=val.toObject().value("admin").toBool();
            QDate birthDate = QDate::fromString(val.toObject().value("birthDate").toString(),"dd/MM/yyyy");
            QVector<PersonalLog> logs;
            QJsonArray logArray=val.toObject().value("logs").toArray();
            foreach(const QJsonValue & log, logArray)
            {
                logs.push_back(PersonalLog(QDate::fromString(log.toObject().value("logDate").toString(),"dd/MM/yyyy"),QTime::fromString(log.toObject().value("logTime").toString(),"HH/mm/ss"),log.toObject().value("type").toBool()));
            }


            QVector<BankAccount> accounts;
            QJsonArray accountArray=val.toObject().value("accounts").toArray();

            foreach(const QJsonValue & account, accountArray)
            {
                QString accountNumber =account.toObject().value("accountNumber").toString();
                int type=account.toObject().value("type").toInt();
                int status=account.toObject().value("status").toInt();
                int balance=account.toObject().value("balance").toInt();
                bool hasCard=val.toObject().value("hasCard").toBool();

                QJsonObject cardObj=account.toObject().value("card").toObject();
                unsigned int password=cardObj.value("secPass").toObject().value("password").toInt();
                QDate createdDate=QDate::fromString(cardObj.value("secPass").toObject().value("dateCreated").toString(),"dd/MM/yyyy");
                QTime createdTime=QTime::fromString(cardObj.value("secPass").toObject().value("timeCreated").toString(),"HH/mm/ss");
                QTime validTime=QTime::fromString(cardObj.value("secPass").toObject().value("validTime").toString(),"HH/mm/ss");
                int cardStatus=cardObj.value("status").toInt();
                int cvv2=cardObj.value("cvv2").toInt();
                QString cardNumber =cardObj.value("cardNumber").toString();
                QDate expireDate=QDate::fromString(cardObj.value("expireDate").toString(),"dd/MM/yyyy");
                Card card=Card(cardNumber,cvv2,expireDate,SecurePassword(password,createdTime,validTime,createdDate),cardStatus);
                QVector<transaction> transactionList;
                QJsonArray transactionArray=account.toObject().value("transactionList").toArray();
                foreach(const QJsonValue & Transaction, transactionArray)
                {
                     QJsonObject _originAcc=Transaction.toObject().value("originAcc").toObject();
                     QString originAccountNumber =_originAcc.value("accountNumber").toString();
                     int originAccountBalance =_originAcc.value("balance").toInt();
                     QJsonObject _desAcc=Transaction.toObject().value("desAcc").toObject();
                     QString desAccountNumber =_desAcc.value("accountNumber").toString();
                     int desAccountBalance =_desAcc.value("balance").toInt();


                    BankAccount originAcc(originAccountNumber,originAccountBalance);
                    BankAccount desAcc(desAccountNumber,desAccountBalance);
                    int amount=Transaction.toObject().value("amount").toInt();
                    bool type=Transaction.toObject().value("type").toBool();
                    QDate date = QDate::fromString(Transaction.toObject().value("trDate").toString(),"dd/MM/yyyy");
                    QTime time = QTime::fromString(Transaction.toObject().value("trTime").toString(),"HH/mm/ss");
                    transactionList.push_back(transaction(originAcc,desAcc,amount,type,time,date));
                }

                accounts.push_back(BankAccount(accountNumber,card,balance, type,status,hasCard,transactionList));

            }
            userList.push_back(User(fullName,nCode,birthDate,username,password,logs,admin,id,accounts));
        }
    }
}

