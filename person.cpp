#include "person.h"



Person::Person(QString _fn , QString _nc, QDate _b, QString _u, QString _p)
{
    fullName = _fn;
    nCode = _nc;
    birthDate = _b;
    username = _u;
    password = _p;
    count++;
    srand(time(0));
    personId = (((long long int)rand())%1000000 + 100000);
}
Person::Person(QString _fn , QString _nc, QDate _b, QString _u, QString _p,bool _admin,int _personId)
{
    fullName = _fn;
    nCode = _nc;
    birthDate = _b;
    username = _u;
    password = _p;
    admin=_admin;
    personId = _personId;
}
Person::Person()
{
}
QVector<PersonalLog> Person::getLog()
{
    return logs;
}
QVector<PersonalLog>* Person::getLogPointer()
{
    return &logs;
}
void Person::setLogs(QVector<PersonalLog> list)
{
    logs=list;
}

QDate Person::getBirthDate()
{
    return birthDate;
}

void Person::editProfile(QString _fn , QString _nc, QDate _b, QString _u, QString _p)
{
    fullName = _fn;
    nCode = _nc;
    birthDate = _b;
    username = _u;
    password = _p;
}
QString Person::getFullName()
{
    return fullName;
}
QString Person::getNationalCode()
{
    return nCode;
}
QString Person::getUsername()
{
    return username;
}
QString Person::getPassword()
{
    return password;
}
bool Person::isAdmin()
{
    return admin;
}

int Person::getId()
{
    return personId;
}
int Person::count = 0;
