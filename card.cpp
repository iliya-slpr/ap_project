#include "card.h"



Card::Card()
{
    srand(time(0));
    cardNumber = QString::number((((long long int)rand())%1000000000 + 10000000000));
    cvv2 = (rand()%10000)+1000;
    expireDate = QDate::currentDate().addYears(4);
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
    if(expireDate>QDate::currentDate())return false;
    else return true;
}

void Card::getPassword()
{
    SecurePassword newpass;
    secPass = newpass;
}
