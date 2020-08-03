#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QVector>
#include "user.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include "logindialog.h"
#include "addaccountdialog.h"
#include "accountdialog.h"
#include "application.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QVector<User> userList;
//    QVector<BankAccount*> accountList;
    void readUsers();
    void writeUsers();
    bool getLoginSeccess();
    bool getIsAdmin();

signals:
void sendCurrentAcc(QString);

private slots:
    void on_editProfileTab_tabBarClicked(int index);

    void on_pushButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_selectAccount_clicked();

    void on_addAccount_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_accountTable_cellClicked(int row, int column);

private:
    addAccountDialog *addform;
    Application application;
    AccountDialog *accountDialog;
    bool loginSeccess=false;
    bool isAdmin=false;
    loginDialog * login;
    QMessageBox message;
    Ui::MainWindow *ui;
    QStringList headers ={"شماره حساب","نوع حساب","مانده", "وضعیت حساب"};
};




#endif // MAINWINDOW_H
