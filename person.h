#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "mydate.h"
#include "mytime.h"
#include "personallog.h"
#ifndef PERSON_H
#define PERSON_H


class Person
{
    QString fullName;
    QString nCode;
    MyDate birthDate;
    QString username;
    QString password;
    QVector<PersonalLog> logs;
    bool admin;

public:
    Person(QString,QString,MyDate,QString,QString);
    void editProfile(QString,QString,MyDate,QString,QString);
    QString getfullName();
    QString nationalCode();
    QString getUsername();
    QString getPassword();
    bool isAdmin();






};

#endif // PERSON_H
