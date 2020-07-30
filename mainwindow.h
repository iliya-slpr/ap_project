#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QVector>
#include "user.h"
#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
