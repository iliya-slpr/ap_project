#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "mydate.h"
#include "mytime.h"
#include "personallog.h"
#include <QDate>
#ifndef PERSON_H
#define PERSON_H


class Person
{
    QString fullName;
    QString nCode;
    QDate birthDate;
    QString username;
    QString password;
    QVector<PersonalLog> logs;
    bool admin;

public:
    Person(QString,QString,QDate,QString,QString);
    void editProfile(QString,QString,QDate,QString,QString);
    QString getfullName();
    QString getNationalCode();
    QString getUsername();
    QString getPassword();
    bool isAdmin();






};

#endif // PERSON_H
