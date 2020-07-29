#ifndef PERSONALLOG_H
#define PERSONALLOG_H
#include "mydate.h"
#include "mytime.h"
class PersonalLog
{
    MyDate loginDate;
    MyTime loginTime;
    MyDate logoutDate;
    MyTime logoutTime;
    bool isLoggedOut;
public:
    //its only get login Data, because logout isnt a necessary Data and should not exist.
    PersonalLog(MyDate , MyTime);
    void setLogOut(MyDate,MyTime);


};

#endif // PERSONALLOG_H
