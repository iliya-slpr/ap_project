#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    User admin("arefasmand","12312312",QDate::currentDate(),"arefasmand","arefasmand");
    userList.push_back(admin);

    login=new loginDialog();
    int result=login->exec();
    while(1)
    {
        if(result==QDialog::Accepted)
        {
            for(int i=0;i<userList.size();i++)
            {
                if(login->getUsername()==userList[i].getUsername()&&login->getPassword()==userList[i].getPassword())
                {
                    currentUser=userList[i];
                    message.setText("با موفقیت وارد شدید!");
                    message.setWindowTitle("موفق");
                    message.setStandardButtons(QMessageBox::Ok);
                    loginSeccess=true;
                    if(message.exec()==QMessageBox::Ok)
                        break;
                }
            }
            if(loginSeccess==false)
            {
                message.setText("نام کاربری یا رمز عبور نادرست است !");
                message.setWindowTitle("خطا");
                message.setStandardButtons(QMessageBox::Ok);
                if(message.exec()==QMessageBox::Ok)
                    result=login->exec();
            }
        }
        else if(result==QDialog::Rejected)
        {
            this->close();
            break;
        }
        if(loginSeccess==true)
            break;
    }
    if(loginSeccess==false)
        this->close();
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
    //    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
    //                                                    "/",
    //                                                    tr("Json File (*.json)"));
    QFile inFile("/.datas.json");
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

void MainWindow::writeUsers()
{
    QFile file("/.datas.json");
    QJsonObject mainObj;
    QJsonArray mainArray;
    for(int i = 0 ; i<userList.size() ; i++){
        QJsonObject userObj;
        userObj["personId"]=userList[i].getId();
        userObj["name"]=userList[i].getFullName();
        userObj["nCode"]=userList[i].getNationalCode();
        userObj["birthDate"]=userList[i].getBirthDate().toString("dd/MM/yyyy");
        userObj["username"]=userList[i].getUsername();
        userObj["password"]=userList[i].getPassword();
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
            accountToAdd["accountNumber"]=userList[i].getAccount()[k].getAccountNumber();
            accountToAdd["balance"]=userList[i].getAccount()[k].getBalance();
            accountToAdd["type"]=userList[i].getAccount()[k].getType();
            accountToAdd["status"]=userList[i].getAccount()[k].getStatus();
            accountToAdd["hasCard"]=userList[i].getAccount()[k].hasACard();
            QJsonObject cardObj;
            if(userList[i].getAccount()[k].hasACard()){
                cardObj["cardNumber"]= userList[i].getAccount()[k].getCard().getCardNumber();
                cardObj["cvv2"]= userList[i].getAccount()[k].getCard().getcvv2();
                cardObj["expireDate"]= userList[i].getAccount()[k].getCard().getExpireDate().toString("dd/MM/yyyy");
                cardObj["secPass"]= 0;
            }
            else {
                cardObj["cardNumber"]= 0;
                cardObj["cvv2"]= 0;
                cardObj["expireDate"]= 0;
                cardObj["secPass"]= 0;
            }
            QJsonArray ownersArray;
            for(int m = 0 ; m< userList[i].getAccount()[k].owners.size();m++){
                QJsonObject ownerToAdd;
                ownerToAdd["personId"] = userList[i].getAccount()[k].owners[m];
                ownersArray.append(ownerToAdd);
            }
            accountToAdd["owners"] = ownersArray;
            accountToAdd["card"]=cardObj;
            QJsonArray transactionsArray;
            for(int l=0; l<userList[i].getAccount()[k].getTransactions().size();l++){
                QJsonObject transactionToAdd;
                transactionToAdd["type"] = userList[i].getAccount()[k].getTransactions()[l].getType();
                transactionToAdd["originAcc"] = userList[i].getAccount()[k].getTransactions()[l].getOriginBankAcc().getAccountNumber();
                transactionToAdd["desAcc"] = userList[i].getAccount()[k].getTransactions()[l].getDesBankAcc().getAccountNumber();
                transactionToAdd["amount"] = QString::number(userList[i].getAccount()[k].getTransactions()[l].getAmount());
                transactionToAdd["trDate"] = userList[i].getAccount()[k].getTransactions()[l].getDate().toString("dd/MM/yyyy");
                transactionToAdd["trTime"] = userList[i].getAccount()[k].getTransactions()[l].getTime().toString("HH/mm/ss");
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

