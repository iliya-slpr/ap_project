#ifndef ADMINPANEL_H
#define ADMINPANEL_H
#include "mainwindow.h"
#include "loginwindow.h"
#include <QMainWindow>
#include"adduser.h"
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
signals:
    void sendLogsToDialog(QString, QString);
private slots:
    void on_deleteBtn_clicked();
    void getDataToAdd(QString ,QString,QDate,QString,QString);
    void on_addUserBtn_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_logsBtn_clicked();
private:
    QMessageBox message;
    Ui::AdminPanel *ui;
};

#endif // ADMINPANEL_H
