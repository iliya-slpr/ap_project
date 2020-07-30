#ifndef USER_H
#define USER_H
#pragma once
#include "person.h"
#include "bankaccount.h"
#include <QMainWindow>

class User : public Person
{
private:
    QVector<BankAccount> accounts;
public:
    User(QString,QString,QDate,QString,QString);
    User();
    QVector<BankAccount> getAccount();

};

#endif // USER_H
