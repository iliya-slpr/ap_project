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
    User* findUser(QString);
    bool login(QString username,QString password);
    void logout();
    void setCurrentUserAccount(QString accNum);
    void refresh(QString);
    void refresh(int);
    int getUserIndex();


private:
    QMessageBox message;
    int userIndex;
    bool loginStatus;

};

#endif // APPLICATION_H