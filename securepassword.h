#ifndef SECUREPASSWORD_H
#define SECUREPASSWORD_H
#include <stdlib.h>
#include<QTime>
#include <QDate>

class SecurePassword
{
    unsigned int password;
    QTime createdTime;
    QTime validTime;
    QDate createdDate;
public:
    SecurePassword();
    SecurePassword(unsigned int,QTime,QTime,QDate);
    int getPasswordOfSecPass();
    QTime getExpireTime();
    QTime getCreatedTime();
    QDate getCreatedDate();
    bool isValid();
};

#endif // SECUREPASSWORD_H
