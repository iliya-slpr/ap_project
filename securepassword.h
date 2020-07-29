#ifndef SECUREPASSWORD_H
#define SECUREPASSWORD_H
#include <stdlib.h>
#include <ctime>
#include "mydate.h"
#include "mytime.h"
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
