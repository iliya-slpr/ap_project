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
    User(QString,QString,QDate,QString,QString,QVector<PersonalLog>,bool,int,QVector<BankAccount>);
    User();
    QVector<BankAccount> getAccount();
    QVector<BankAccount>* getAccountsPointer();
    BankAccount* getAccountPointer(int i);

};

#endif // USER_H
