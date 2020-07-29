#include "personallog.h"

PersonalLog::PersonalLog(MyDate _D, MyTime _T)
{
    loginDate = _D;
    loginTime = _T;
    logoutDate = MyDate();
    logoutTime = MyTime();
    isLoggedOut = false;
}

void PersonalLog::setLogOut(MyDate _D, MyTime _T)
{
    logoutDate = _D;
    logoutTime = _T;
    isLoggedOut = true;
}
