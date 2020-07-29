#include "personallog.h"

PersonalLog::PersonalLog(QDate _D, QTime _T)
{
    loginDate = _D;
    loginTime = _T;
    logoutDate = QDate(0,0,0);
    logoutTime = QTime(0,0,0);
    isLoggedOut = false;
}

void PersonalLog::setLogOut(QDate _D, QTime _T)
{
    logoutDate = _D;
    logoutTime = _T;
    isLoggedOut = true;
}
