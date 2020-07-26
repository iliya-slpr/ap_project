#include "date.h"


Date::Date(int _y, int _m, int _d)
{
    year = _y;
    mounth = _m;
    day = _d;
}

int Date::getYear()
{
    return year;
}
int Date::getMounth()
{
    return mounth;
}
int Date::getDay()
{
    return day;
}
void Date::setYear(int _y){
    year = _y;
}
void Date::setMounth(int _m){
    mounth = _m;
}
void Date::setDay(int _d){
    day = _d;
}
