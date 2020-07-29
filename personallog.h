#ifndef PERSONALLOG_H
#define PERSONALLOG_H

    #include<QTime>
    #include <QDate>
class PersonalLog
{
    QDate logDate;
    QTime logTime;

    enum TYPE{
        LOGIN,LOGOUT
    };
    TYPE type;
public:
    PersonalLog(TYPE);
};

#endif // PERSONALLOG_H
