#ifndef DATE_H
#define DATE_H


class Date
{
public:
private:
    int year;
    int mounth;
    int day;
public:
    Date(int , int , int);
    Date();
    int getYear();
    int getMounth();
    int getDay();
    void setYear(int _y);
    void setMounth(int _m);
    void setDay(int _d);

};

#endif // DATE_H
