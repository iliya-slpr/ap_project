#ifndef ADMINPANEL_H
#define ADMINPANEL_H
#include "mainwindow.h"
#include "loginwindow.h"
#include <QMainWindow>
#include"adduser.h"
#include "simplecrypt.h"
namespace Ui {
class AdminPanel;
}

class AdminPanel : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminPanel(QWidget *parent = nullptr);
    ~AdminPanel();
    QVector<User> userList;
    QVector<BankAccount*> accountList;
    void readUsers();
    void writeUsers();
    void setupUserTable();
    void removeUser(QString);
    void setupPendingAccountsTable();
    void setupPendingCardsTable();
    void setupTransactions();
    QString getType(int s);
    QString getStatus(int s);
    void setupManagementTable();
    void changeCardStatus(QString nameToChange, int accountToChange);
    void changeAccountStatus(QString nameToChange,int accountToChange);




signals:
    void sendLogsToDialog(QString, QString);
private slots:
    void on_deleteBtn_clicked();
    void getDataToAdd(QString ,QString,QDate,QString,QString);
    void on_addUserBtn_clicked();

    //void on_tabWidget_currentChanged(int index);

    void on_logsBtn_clicked();
    //void on_pushButton_clicked();

    void on_confirmBtn_clicked();

    void on_rejectBtn_clicked();

    void on_confirmCardBtn_clicked();

    void on_rejeCardBtn_clicked();

    void on_changeStatusBtn_clicked();

    void on_changeCardStatusBtn_clicked();

    void on_longAccountBtn_clicked();

    void on_shortAccountBtn_clicked();

    void on_managementTable_cellClicked(int row, int column);

private:
    QMessageBox message;
    Ui::AdminPanel *ui;
    SimpleCrypt crypt;
};

#endif // ADMINPANEL_H
