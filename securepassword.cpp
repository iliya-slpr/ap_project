#include "securepassword.h"
#include <stdlib.h>
#include <ctime>
#include "mydate.h"
#include "mytime.h"


SecurePassword::SecurePassword()
{
    srand(time(0));
        password = (rand()%1000000)+100000;
        validTime = QTime::currentTime().addSecs(120);

}

bool SecurePassword::isValid()
{

    if(QTime::currentTime()>validTime)return false;
    else return true;

}

