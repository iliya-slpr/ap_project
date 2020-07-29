#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H
#include <card.h>

class BankAccount
{
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


public:
    BankAccount(int, TYPE);
    void requestCard();
    int getBalance();
    //transfer
    bool hasACard();
    //getlast10Transaction
    Card getCard();
    void minusBalance(int);
    QVector<int> owners;
};


#endif // BANKACCOUNT_H
