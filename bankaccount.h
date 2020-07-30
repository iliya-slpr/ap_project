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
    enum TYPE{
        SAVING , SHORT_TERM , SHORT_TERM_LEGAL , LONG_TERM
    };
    TYPE type;
    enum STATUS {
      ACTIVE , BLOCK , PENDING , REJECT
    };
    STATUS status;
    bool HasCard;
    QVector<transaction> transactionList;



public:
    BankAccount(int, TYPE);
    BankAccount();
    void requestCard();
    int getBalance();
    bool transfer(BankAccount* _originAcc,BankAccount* _desAcc,unsigned int _amount);
    bool hasACard();
    QVector<transaction> getlast10Transaction();
    Card getCard();
    void minusBalance(int);
    void plusBalance(int);
    QVector<int> owners;  
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
    transaction();
    BankAccount getOriginBankAcc();
    BankAccount getDesBankAcc();
    unsigned int getAmount();
    QTime getTime();
    QDate getDate();

};
#endif // BANKACCOUNT_H
