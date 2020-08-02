#include "user.h"

User::User(QString _fn , QString _nc, QDate _b, QString _u, QString _p):Person( _fn ,  _nc,  _b,  _u,  _p)
{

}
User::User(QString _fullName,QString _nCode,QDate _bitrhDate,QString _username,QString _password,QVector<PersonalLog> _logs,bool _admin,int _personId,QVector<BankAccount> _accounts):Person( _fullName,_nCode,_bitrhDate,_username,_password,_admin,_personId)
{
    logs=_logs;
    accounts=_accounts;
}
User::User()
{

}

QVector<BankAccount> User::getAccount()
{
    return accounts;
}
QVector<BankAccount>* User::getAccountsPointer()
{
    return &accounts;
}
BankAccount* User::getAccountPointer(int i)
{
    return &accounts[i];
}


