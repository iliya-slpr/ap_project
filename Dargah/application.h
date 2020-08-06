#ifndef APPLICATION_H
#define APPLICATION_H
#include <QVector>
#include "user.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include "bankaccount.h"
#include "personallog.h"
#include "simplecrypt.h"

class Application
{
public:
    QVector<User> userList;
    User* currentUser;
    BankAccount* currentAccount;
    Application();
    void readUsers();
    void writeUsers();
    BankAccount* findAccount(QString,int);
    BankAccount* findAccount(QString accNum,QString username);
    User* findUser(QString);
    User* findUsername(QString);
    bool login(QString username,QString password);
    void logout();
    void setCurrentUserAccount(QString accNum);
    void refresh(QString);
    void refresh(int);
    int getUserIndex();
    BankAccount *getAccount(int cardNumber);
    QString findNameByCard(int cardNumber);
    Card *getCardPointer(int cardNumber);
    Card getCard(int cardNumber);
    BankAccount getAcc(int cardNumber);


private:
    QMessageBox message;
    int userIndex;
    bool loginStatus;
    SimpleCrypt crypt;

};

#endif // APPLICATION_H
