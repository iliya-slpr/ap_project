#include "personallog.h"

PersonalLog::PersonalLog(Date _D, Time _T)
{
    loginDate = _D;
    loginTime = _T;
    logoutDate = Date();
    logoutTime = Time();
    isLoggedOut = false;
}

void PersonalLog::setLogOut(Date _D, Time _T)
{
    logoutDate = _D;
    logoutTime = _T;
    isLoggedOut = true;
}
