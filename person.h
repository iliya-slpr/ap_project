#ifndef PERSONALLOG_H
#define PERSONALLOG_H
#include "mydate.h"
#include "mytime.h"
    #include<QTime>
    #include <QDate>
class PersonalLog
{
    QDate loginDate;
    QTime loginTime;
    QDate logoutDate;
    QTime logoutTime;
    bool isLoggedOut;
public:
    //its only get login Data, because logout isnt a necessary Data and should not exist.
    PersonalLog(QDate , QTime);
    void setLogOut(QDate, QTime);


};

#endif // PERSONALLOG_H
