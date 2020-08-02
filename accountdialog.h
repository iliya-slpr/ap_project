#ifndef ACCOUNTDIALOG_H
#define ACCOUNTDIALOG_H

//#include <QDialog>
//#include "user.h"
//#include <QVector>
//#include <QMainWindow>
//#include <QFileDialog>
//#include <QJsonArray>
//#include <QJsonDocument>
//#include <QJsonObject>
//#include <QMessageBox>
#include "application.h"
#include "addaccountdialog.h"
namespace Ui {
class AccountDialog;
}

class AccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AccountDialog(QWidget *parent = nullptr);
    ~AccountDialog();
    QString getAccNum();
    int getAmount();
    void readUsers();
    void writeUsers();



public slots:
    void receiveAccount(QString);
private slots:
    void on_tabWidget_tabBarClicked(int index);

    void on_transferBtn_clicked();

    void on_cardbtn_clicked();

    void on_pushButton_clicked();

private:
    Ui::AccountDialog *ui;
    Application application;
    QMessageBox message;
    QString currentAccNum;
    QVector<User> userList;
    bool transfered=false;
};

#endif // ACCOUNTDIALOG_H
