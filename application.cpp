#include "application.h"
#include <QTextDocument>
#include <QPrinter>
Application::Application()
{
    crypt.setKey(10000);
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
BankAccount* Application::findAccount(QString accNum,QString username)     //type// 0 search global     1 search in current User
{
    for(int k=0;k<userList.size();k++)
    {
        if(userList[k].getUsername()==username)
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
            writeUsers();
            if(message.exec()==QMessageBox::Ok)
                return loginStatus;
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
    refresh(getUserIndex());
    currentUser->getLogPointer()->push_back(PersonalLog(1));
    writeUsers();
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

User* Application::findUsername(QString user)
{
    for(int k=0;k<userList.size();k++)
    {
        if(userList[k].getUsername()==user)
        {
            return &userList[k];
        }
    }
}

void Application::makePDF(QString _accountNum, QString _fileName)
{
    BankAccount tmp = *findAccount(_accountNum , 0);
    QString html =
    "<body style=\"background-color: color:#38386;\">"
    "<div align=right>"
       "تاریخ:"
            +QDate::currentDate().toString()+
    "</div>"
    "<div align=left>"
       "شماره حساب: "
       + tmp.getAccountNumber() +

    "</div>"
    "<h1 align=center style=\"color:#38386; font-family: \"B Yekan\";\">ده گردش آخر حساب</h1><hr width=\"3px\">"
    "<div align=center>";
    for(int i = 0; i<10 ; i++){
        transaction trTmp = tmp.getTransactions()[i];
        html += "<p align=center>"
                "تاریخ:"
                +trTmp.getDate().toString()+
                "  زمان"
                +trTmp.getTime().toString()+
                "  از حساب:"
                +trTmp.getOriginBankAcc().getAccountNumber()+
                "  به حساب"
                +trTmp.getOriginBankAcc().getAccountNumber()+
                "  به مبلغ:"
                + QString::number(trTmp.getAmount())+
                "</div><hr style=\"width:5px;color:#38386a;\">";
        if(i+1==tmp.getTransactions().size())break;


    }

    html +="</p>"
    "</body>";

    QTextDocument document;
    document.setHtml(html);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(_fileName);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    document.print(&printer);
}
