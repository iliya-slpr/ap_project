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
    QVector<PersonalLog> logs;
    bool admin;
    static int count;
    int personId;

public:
    Person(QString,QString,QDate,QString,QString);
    Person();
    void editProfile(QString,QString,QDate,QString,QString);
    int getId();
    QString getFullName();
    QString getNationalCode();
    QString getUsername();
    QString getPassword();
    bool isAdmin();






};

#endif // PERSON_H
