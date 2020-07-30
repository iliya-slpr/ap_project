#include "user.h"

User::User(QString _fn , QString _nc, QDate _b, QString _u, QString _p):Person( _fn ,  _nc,  _b,  _u,  _p)
{

}

BankAccount User::getAccount(int n)
{
    return accounts[n];
}


