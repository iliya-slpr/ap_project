#include "personallog.h"

PersonalLog::PersonalLog(TYPE _t)
{
    logDate = QDate::currentDate();
    logTime = QTime::currentTime();
    type = _t;
}

