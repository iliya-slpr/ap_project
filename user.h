#ifndef USER_H
#define USER_H
#include "person.h"
#include "bankaccount.h"
#include <QMainWindow>

class User : public Person
{
private:
    QVector<BankAccount> accounts;
public:
    User(QString,QString,QDate,QString,QString);
    BankAccount getAccount(int n);

};

#endif // USER_H
