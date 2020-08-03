#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H
#pragma once
#include <card.h>

class transaction;

class BankAccount
{
    QString accountNumber;
    int balance;
    Card card;
    int type;    ///// SAVING=0 , SHORT_TERM=1 , SHORT_TERM_LEGAL=2 , LONG_TERM=3
    int status;   ///// ACTIVE=0 , BLOCK=1 , PENDIN2 , REJECT=3
    bool HasCard;
    QVector<transaction> transactionList;
    QStringList owners;

public:
    BankAccount(int, int);
    BankAccount(int, int,QString);
    BankAccount(QString,Card,int, int,int,bool,QVector<transaction>,QStringList);
    BankAccount(QString,int);
    BankAccount();
    void requestCard();
    int getBalance();
    QString getAccountNumber();
    int getStatus();
    int getType();
    bool transfer(BankAccount* _desAcc,unsigned int _amount);
    bool transfer(QString _desAcc,unsigned int _amount,int type);
    bool hasACard();
    QVector<transaction> getTransactions();
    Card getCard();
    Card* getCardPointer();
    void minusBalance(int);
    void plusBalance(int);
    QStringList getOwnerUsername();
    void changeStatusToActive();
    void changeStatusToBlock();
    void changeStatusToPending();
    void changeStatusToReject();
};


class transaction
{
private:
    bool type;
    BankAccount originAcc;
    BankAccount desAcc;
    unsigned int amount;
    QTime trTime;
    QDate trDate;
public:
    transaction(BankAccount _originAcc,BankAccount _desAcc,unsigned int _amount,bool _type);
    transaction(BankAccount _originAcc,BankAccount _desAcc,unsigned int _amount,bool _type,QTime,QDate);
    transaction();
    BankAccount getOriginBankAcc();
    BankAccount getDesBankAcc();
    unsigned int getAmount();
    QTime getTime();
    QDate getDate();
    bool getType();

};
#endif // BANKACCOUNT_H
