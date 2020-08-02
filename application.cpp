#include "application.h"

Application::Application()
{
    readUsers();
}

void Application::readUsers()
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
                logs.push_back(PersonalLog(QDate::fromString(log.toObject().value("logDate").toString(),"dd/MM/yyyy"),QTime::fromString(log.toObject().value("logTime").toString(),"HH/mm/ss"),log.toObject().value("logType").toBool()));
            }


            QVector<BankAccount> accounts;
            QJsonArray accountArray=val.toObject().value("accounts").toArray();

            foreach(const QJsonValue & account, accountArray)
            {
                QString accountNumber =account.toObject().value("accountNumber").toString();
                int type=account.toObject().value("type").toInt();
                int status=account.toObject().value("status").toInt();
                int balance=account.toObject().value("balance").toInt();
                bool hasCard=account.toObject().value("hasCard").toBool();

                QJsonObject cardObj=account.toObject().value("card").toObject();
                unsigned int password=cardObj.value("secPass").toObject().value("password").toInt();
                QDate createdDate=QDate::fromString(cardObj.value("secPass").toObject().value("createdDate").toString(),"dd/MM/yyyy");
                QTime createdTime=QTime::fromString(cardObj.value("secPass").toObject().value("createdTime").toString(),"HH/mm/ss");
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
void Application::writeUsers()
{
    QFile file("E:\\data.json");
    QJsonObject mainObj;
    QJsonArray mainArray;
    for(int i = 0 ; i<userList.size() ; i++){
        QJsonObject userObj;
        userObj["personId"]=userList[i].getId();
        userObj["fullName"]=userList[i].getFullName();
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
                cardObj["status"]= userList[i].getAccount()[k].getCard().getStatus();
                cardObj["expireDate"]= userList[i].getAccount()[k].getCard().getExpireDate().toString("dd/MM/yyyy");
                QJsonObject pass;
                pass["password"]=userList[i].getAccount()[k].getCard().getPassword().getPasswordOfSecPass();
                pass["createdTime"]=userList[i].getAccount()[k].getCard().getPassword().getCreatedTime().toString("HH/mm/ss");
                pass["validTime"]=userList[i].getAccount()[k].getCard().getPassword().getExpireTime().toString("HH/mm/ss");
                pass["createdDate"]=userList[i].getAccount()[k].getCard().getPassword().getCreatedDate().toString("dd/MM/yyyy");
                cardObj["secPass"]= pass;
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
                QJsonObject originAcc;
                originAcc["accountNumber"]=userList[i].getAccount()[k].getTransactions()[l].getOriginBankAcc().getAccountNumber();
                originAcc["balance"]=userList[i].getAccount()[k].getTransactions()[l].getOriginBankAcc().getBalance();
                transactionToAdd["originAcc"] = originAcc;
                QJsonObject desAcc;
                desAcc["accountNumber"]=userList[i].getAccount()[k].getTransactions()[l].getDesBankAcc().getAccountNumber();
                desAcc["balance"]=userList[i].getAccount()[k].getTransactions()[l].getDesBankAcc().getBalance();
                transactionToAdd["desAcc"] = desAcc;
                transactionToAdd["amount"] = (int)userList[i].getAccount()[k].getTransactions()[l].getAmount();
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
BankAccount* Application::findAccount(QString accNum,int type)     //type// 0 search global     1 search in current User
{
    if(type==0)
    {
        for(int k=0;k<userList.size();k++)
        {
            for(int i=0;i<userList[k].getAccount().size();i++)
            {
                if(userList[k].getAccountPointer(i)->getAccountNumber()==accNum)
                {
                    return userList[k].getAccountPointer(i);
                }
            }
        }
    }
    else
    {
        for(int i=0;i<currentUser->getAccount().size();i++)
        {
            if(currentUser->getAccountPointer(i)->getAccountNumber()==accNum)
            {
                return currentUser->getAccountPointer(i);
            }
        }
    }
    return NULL;
}
bool Application::login(QString username,QString password)
{
    for(int i=0;i<userList.size();i++)
    {
        if(username==userList[i].getUsername()&&password==userList[i].getPassword())
        {
            currentUser=&userList[i];
            userIndex=userList[i].getId();
            currentUser->getLogPointer()->push_back(PersonalLog(0));
            message.setText("با موفقیت وارد شدید!");
            message.setWindowTitle("موفق");
            message.setStandardButtons(QMessageBox::Ok);
            loginStatus=true;
            if(message.exec()==QMessageBox::Ok)
                return loginStatus;
            writeUsers();
        }
    }
    if(loginStatus==false)
    {
        message.setText("نام کاربری یا رمز عبور نادرست است !");
        message.setWindowTitle("خطا");
        message.setStandardButtons(QMessageBox::Ok);
        if(message.exec()==QMessageBox::Ok)
            return loginStatus;
    }
}

void Application::logout()
{
    currentUser->getLogPointer()->push_back(PersonalLog(1));
    writeUsers();
    readUsers();
}
void Application::setCurrentUserAccount(QString accNum)
{
    for(int k=0;k<userList.size();k++)
    {
        for(int i=0;i<userList[k].getAccount().size();i++)
        {
            if(userList[k].getAccountPointer(i)->getAccountNumber()==accNum)
            {
                currentUser=&userList[k];
                currentAccount=userList[k].getAccountPointer(i);
                return;
            }
        }
    }
}
User* Application::findUser(QString current)
{
    for(int k=0;k<userList.size();k++)
    {
        for(int i=0;i<userList[k].getAccount().size();i++)
        {
            if(userList[k].getAccountPointer(i)->getAccountNumber()==current)
            {
                return &userList[k];
            }
        }
    }
}
void Application::refresh(QString current)
{
    readUsers();
    setCurrentUserAccount(current);
}
void Application::refresh(int id)
{
    readUsers();
    for(int k=0;k<userList.size();k++)
    {
        if(userList[k].getId()==id)
        {
            currentUser=&userList[k];
            return;
        }
    }
}
int Application::getUserIndex()
{
    return userIndex;
}
