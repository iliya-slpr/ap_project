#ifndef CARD_H
#define CARD_H
#include <QMainWindow>
#include "securepassword.h"
#include <QTime>


class Card
{
    QString cardNumber;
    int cvv2;
    QDate expireDate;
    SecurePassword secPass;
    int status;     /////ACTIVE=0 , BLOCK=1 , PENDING=2 , REJECT=3
public:
    Card();
    Card(QString,int,QDate,SecurePassword,int);
    QString getCardNumber();
    int getcvv2();
    bool isExpire();
    void getPassword();
    void changeStatusToPending();
    void changeStatusToReject();
    void changeStatusToActive();
    int getStatus();
    QDate getExpireDate();

};


#endif // CARD_H
