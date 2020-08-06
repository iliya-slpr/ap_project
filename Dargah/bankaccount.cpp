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
BankAccount::BankAccount(int _b, int _t,QString _owners)
{
    srand(time(0));
    accountNumber = QString::number((((long long int)rand())%10000 + 100000));
    balance = _b;
    type = _t;
    status = 2;
    HasCard = false;
    owners=_owners.split(',');
}
BankAccount::BankAccount(QString _accountNumber,Card _card,int _balance, int _type,int _status,bool _hasCard,QVector<transaction>_transactionList,QStringList _owners)
{
    balance = _balance;
    type = _type;
    status = _status;
    HasCard = _hasCard;
    accountNumber=_accountNumber;
    card=_card;
    transactionList=_transactionList;
    owners=_owners;
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
    this->card=Card();
    this->HasCard=true;
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
Card* BankAccount::getCardPointer()
{
    return &card;
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
bool BankAccount::transfer(BankAccount* _desAcc,unsigned int _amount)
{

        this->minusBalance(_amount);
        _desAcc->plusBalance(_amount);
        transactionList.push_back(transaction(*this,*_desAcc,_amount,0));
        _desAcc->transactionList.push_back(transaction(*this,*_desAcc,_amount,1));
        return 1;

}
bool BankAccount::transfer(QString _desAcc,unsigned int _amount,int type)    ///0 send 1 receive
{
        transactionList.push_back(transaction(*this,BankAccount(_desAcc,balance),_amount,type));
        return 1;
}
QStringList BankAccount::getOwnerUsername()
{
    return owners;
}
void BankAccount::changeStatusToActive()
{
    status = 0;
}

void BankAccount::changeStatusToBlock()
{
    status = 1;
}

void BankAccount::changeStatusToPending()
{
   status = 2;
}
void BankAccount::changeStatusToReject()
{
    status = 3;
}

QVector<transaction> *BankAccount::getTransactionsPointer()
{
    return &transactionList;
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



