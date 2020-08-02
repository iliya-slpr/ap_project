#include "securepassword.h"
#include <stdlib.h>
#include <ctime>



SecurePassword::SecurePassword()
{
    srand(time(0));
        password = (rand()%1000000)+100000;
        validTime = QTime::currentTime().addSecs(120);
        createdDate=QDate::currentDate();
        createdTime=QTime::currentTime();
}
SecurePassword::SecurePassword(unsigned int _password,QTime _createdTime,QTime _validTime,QDate _createdDate)
{
        password = _password;
        validTime = _validTime;
        createdTime=_createdTime;
        createdDate=_createdDate;
}
QTime SecurePassword::getExpireTime()
{
    return validTime;
}
QTime SecurePassword::getCreatedTime()
{
    return createdTime;
}
QDate SecurePassword::getCreatedDate()
{
    return createdDate;
}

bool SecurePassword::isValid()
{

    if(QTime::currentTime()>validTime&&QDate::currentDate()>createdDate)return false;
    else return true;

}
int SecurePassword::getPasswordOfSecPass()
{
    return  password;
}

