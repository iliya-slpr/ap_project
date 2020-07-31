#include "personallog.h"

PersonalLog::PersonalLog(bool _t)
{
    logDate = QDate::currentDate();
    logTime = QTime::currentTime();
    type = _t;
}
PersonalLog::PersonalLog(QDate date,QTime time,bool _t)
{
    logDate = date;
    logTime = time;
    type = _t;
}
PersonalLog::PersonalLog()
{

}

bool PersonalLog::getLogType()
{
    return type;
}
QDate PersonalLog::getLogDate()
{
    return logDate;
}

QTime PersonalLog::getLogTime()
{
    return logTime;
}
