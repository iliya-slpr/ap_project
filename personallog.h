#ifndef PERSONALLOG_H
#define PERSONALLOG_H

    #include<QTime>
    #include <QDate>
class PersonalLog
{
    QDate logDate;
    QTime logTime;
    bool type;      /////  0 = login  1 = logout
public:
    PersonalLog(bool);
    PersonalLog(QDate,QTime,bool);
    PersonalLog();
    bool getLogType();
    QDate getLogDate();
    QTime getLogTime();
};

#endif // PERSONALLOG_H
