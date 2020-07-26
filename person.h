#ifndef PERSON_H
#define PERSON_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "date.h"
#include "time.h"
#include "personallog.h"


class Person
{
    QString fullName;
    QString nCode;
    Date birthDate;
    QString username;
    QString password;
    QVector<PersonalLog> logs;
    bool admin;

public:
    Person(QString,QString,Date,QString,QString);
    void editProfile(QString,QString,Date,QString,QString);
    QString getfullName();
    QString nationalCode();
    QString getUsername();
    QString getPassword();
    bool isAdmin();






};

#endif // PERSON_H
