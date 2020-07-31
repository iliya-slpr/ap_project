#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "personallog.h"
#ifndef PERSON_H
#define PERSON_H


class Person
{
    QString fullName;
    QString nCode;
    QDate birthDate;
    QString username;
    QString password;
    bool admin;
    static int count;
    int personId;

protected:
    QVector<PersonalLog> logs;

public:
    Person(QString,QString,QDate,QString,QString);
    Person(QString,QString,QDate,QString,QString,bool,int);
    Person();
    void editProfile(QString,QString,QDate,QString,QString);
    QVector<PersonalLog> getLog();
    int getId();
    QString getFullName();
    QString getNationalCode();
    QDate getBirthDate();
    QString getUsername();
    QString getPassword();
    bool isAdmin();






};

#endif // PERSON_H
