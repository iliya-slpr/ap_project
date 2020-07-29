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
public:
    SecurePassword();
    bool isValid();
};

#endif // SECUREPASSWORD_H
