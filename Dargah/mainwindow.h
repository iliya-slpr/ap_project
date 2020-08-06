#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "application.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    Application application;
    ~MainWindow();

private slots:
    void on_transferButton_clicked();

    void on_pushButton_clicked();

private:
    int confirm;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
