#ifndef SECUREPASSWORD_H
#define SECUREPASSWORD_H
#include <stdlib.h>
#include <ctime>
#include "mydate.h"
#include "mytime.h"

class SecurePassword
{
    unsigned int password;
    MyDate validDate;
    MyTime validTime;
public:
    SecurePassword();
    bool isValid(MyDate,MyTime);
};

#endif // SECUREPASSWORD_H
