#include "time.h"
Time::Time(){
    hour = 0;
    minute = 0;
    second = 0;
}

Time::Time(int _h , int _m , int _s)
{
    hour = _h;
    minute = _m;
    second = _s;
}

int Time::getHour()
{
    return this->hour;
}

int Time::getMinute()
{
    return this->minute;
}

int Time::getSecond()
{
    return this->second;
}

void Time::setHour(int _h)
{
    hour = _h;

}

void Time::setMinute(int _m)
{
    minute = _m;

}

void Time::setSecond(int _s)
{
    second = _s;

}


