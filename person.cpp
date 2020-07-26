#include "person.h"


Person::Person(QString _fn , QString _nc, Date _b, QString _u, QString _p)
{
    fullName = _fn;
    nCode = _nc;
    birthDate = _b;
    username = _u;
    password = _p;
}

void Person::editProfile(QString _fn , QString _nc, Date _b, QString _u, QString _p)
{
    fullName = _fn;
    nCode = _nc;
    birthDate = _b;
    username = _u;
    password = _p;
}
QString Person::getfullName()
{
    return fullName;
}
QString Person::nationalCode()
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
