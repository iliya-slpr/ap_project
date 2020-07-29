#include "securepassword.h"
#include <stdlib.h>
#include <ctime>
#include "mydate.h"
#include "mytime.h"


SecurePassword::SecurePassword()
{
    srand(time(0));
        password = (rand()%1000000)+100000;

}

bool SecurePassword::isValid(MyDate _d, MyTime _t)
{
    //compare times and return true if password valid


}

