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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QVector<User> userList;
    QVector<BankAccount*> accountList;
    void readUsers();
    void writeUsers();

private slots:
    void on_eraseBtn_clicked();

private:
    QMessageBox message;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
