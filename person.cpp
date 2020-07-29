#include "person.h"



Person::Person(QString _fn , QString _nc, MyDate _b, QString _u, QString _p)
{
    fullName = _fn;
    nCode = _nc;
    birthDate = _b;
    username = _u;
    password = _p;
	count++;
	personId = count;
}

void Person::editProfile(QString _fn , QString _nc, MyDate _b, QString _u, QString _p)
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
