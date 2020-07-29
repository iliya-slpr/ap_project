#include "bankaccount.h"


BankAccount::BankAccount(int _b, BankAccount::TYPE _t)
{
    balance = _b;
    type = _t;
    status = PENDING;
    HasCard = false;
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


