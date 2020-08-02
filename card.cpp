#include "card.h"



Card::Card()
{
    srand(time(0));
    cardNumber = QString::number((((long long int)rand())%100000000 + 1000000000));
    cvv2 = (rand()%10000)+1000;
    expireDate = QDate::currentDate().addYears(4);
}
Card::Card(QString _cardNumber,int _cvv2,QDate _expireDate,SecurePassword _secPass,int _status)
{
    cardNumber =_cardNumber;
    cvv2 = _cvv2;
    expireDate = _expireDate;
    secPass=_secPass;
    status=_status;
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

void Card::createPass()
{
    SecurePassword newpass;
    secPass = newpass;
}

SecurePassword Card::getPassword()
{
    return secPass;
}
void Card::changeStatusToActive()
{
    status = 0;
}

void Card::changeStatusToPending()
{
   status = 2;
}
void Card::changeStatusToReject()
{
   status = 3;
}
QDate Card::getExpireDate()
{
    return expireDate;
}
int Card::getStatus()
{
    return status;
}
