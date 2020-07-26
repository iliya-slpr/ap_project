#ifndef TIME_H
#define TIME_H


class Time
{
private:
    int hour;
    int minute;
    int second;
public:
    Time();
    Time(int , int , int);
    int getHour();
    int getMinute();
    int getSecond();
    void setHour(int _h);
    void setMinute(int _m);
    void setSecond(int _s);


};

#endif // TIME_H
