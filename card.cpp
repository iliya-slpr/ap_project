#include "card.h"



Card::Card()
{
    srand(time(0));
    cardNumber = QString::number((((long long int)rand())%100000000 + 1000000000));
    cvv2 = (rand()%10000)+1000;
    expireDate = QTime::currentTime();

}

QString Card::getCardNumber()
{
    return cardNumber;

}

int Card::getcvv2()
{
    return cvv2;
}

bool Card::isExpire()
{
    if(expireDate>QTime::currentTime())return false;
    else return true;
}

void Card::getPassword()
{
    SecurePassword newpass;
    secPass = newpass;

}
