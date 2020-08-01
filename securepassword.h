#ifndef SECUREPASSWORD_H
#define SECUREPASSWORD_H
#include <stdlib.h>
#include <ctime>

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
    bool isValid();
};

#endif // SECUREPASSWORD_H
