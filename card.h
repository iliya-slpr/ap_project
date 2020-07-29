#ifndef CARD_H
#define CARD_H
#include <QMainWindow>
#include "securepassword.h"
#include <QDate>
class Card
{
    QString cardNumber;
    int cvv2;
    QDate expireDate;
    SecurePassword secPass;
    enum STATUS {
      ACTIVE , BLOCK , PENDING , REJECT
    };
    STATUS status;
    //transaction
public:
    Card();
    QString getCardNumber();
    int getcvv2();
    bool isExpire();
    void getPassword();

};

#endif // CARD_H
