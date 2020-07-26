#ifndef PESONALLOG_H
#define PESONALLOG_H
#include "time.h"
#include "date.h"

class PersonalLog
{
    Date loginDate;
    Time loginTime;
    Date logoutDate;
    Time logoutTime;
    bool isLoggedOut;
public:
    //its only get login Data, because logout isnt a necessary Data and should not exist.
    PersonalLog(Date , Time);
    void setLogOut(Date,Time);



};

#endif // PESONALLOG_H
