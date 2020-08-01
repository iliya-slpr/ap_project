#include "bankaccount.h"
#include <QMainWindow>
enum Type
{
    send,recive
};
BankAccount::BankAccount(int _b, int _t)
{
    srand(time(0));
    accountNumber = QString::number((((long long int)rand())%10000 + 100000));
    balance = _b;
    type = _t;
    status = 2;
    HasCard = false;
}
BankAccount::BankAccount(QString _accountNumber,Card _card,int _balance, int _type,int _status,bool _hasCard,QVector<transaction>_transactionList)
{
    balance = _balance;
    type = _type;
    status = _status;
    HasCard = _hasCard;
    accountNumber=_accountNumber;
    card=_card;
    transactionList=_transactionList;
}

BankAccount::BankAccount(QString _accountNumber,int _balance)
{
     balance = _balance;
     accountNumber=_accountNumber;
}
BankAccount::BankAccount()
{
}

void BankAccount::requestCard()
{
    card.changeStatusToPending();
}

int BankAccount::getBalance()
{
    return balance;
}

bool BankAccount::hasACard()
{
    return HasCard;
}

Card BankAccount::getCard()
{
    return card;
}
QString BankAccount::getAccountNumber()
{
    return accountNumber;
}
int BankAccount::getStatus()
{
    return status;
}
int BankAccount::getType()
{
    return type;
}

void BankAccount::minusBalance(int _toMinus)
{
    balance -= _toMinus;
}
void BankAccount::plusBalance(int _toplus)
{
    balance += _toplus;
}
bool BankAccount::transfer(BankAccount* _originAcc,BankAccount* _desAcc,unsigned int _amount)
{

        _originAcc->minusBalance(_amount);
        _desAcc->plusBalance(_amount);
        transactionList.push_back(transaction(*_originAcc,*_desAcc,_amount,0));
        _desAcc->transactionList.push_back(transaction(*_originAcc,*_desAcc,_amount,1));
        return 1;

}

transaction::transaction(BankAccount _originAcc,BankAccount _desAcc,unsigned int _amount,bool _type)
{
    originAcc=_originAcc;
    desAcc=_desAcc;
    amount=_amount;
    trDate=QDate::currentDate();
    trTime=QTime::currentTime();
    type=_type;

}
transaction::transaction(BankAccount _originAcc,BankAccount _desAcc,unsigned int _amount,bool _type,QTime _time,QDate _date)
{
    originAcc=_originAcc;
    desAcc=_desAcc;
    amount=_amount;
    trDate=_date;
    trTime=_time;
    type=_type;

}
transaction::transaction()
{

}
QVector<transaction> BankAccount::getTransactions()
{
    return transactionList;
}
BankAccount transaction::getOriginBankAcc(){return originAcc;}
BankAccount transaction::getDesBankAcc(){return desAcc;}
unsigned int transaction::getAmount(){return amount;}
QDate transaction::getDate(){return trDate;}
QTime transaction::getTime(){return trTime;}
bool transaction::getType()
{
    return type;
}



