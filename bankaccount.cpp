#include "bankaccount.h"

enum Type
{
    send,recive
};
BankAccount::BankAccount(int _b, BankAccount::TYPE _t)
{
    balance = _b;
    type = _t;
    status = PENDING;
    HasCard = false;
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
transaction::transaction()
{

}

BankAccount transaction::getOriginBankAcc(){return originAcc;}
BankAccount transaction::getDesBankAcc(){return desAcc;}
unsigned int transaction::getAmount(){return amount;}
QDate transaction::getDate(){return trDate;}
QTime transaction::getTime(){return trTime;}



