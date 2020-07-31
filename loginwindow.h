#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

namespace Ui {
class loginWindow;
}

class loginWindow : public QMainWindow
{
    Q_OBJECT

public:
    bool ok;
    explicit loginWindow(QWidget *parent = nullptr);
    ~loginWindow();
    QString getUsername();
    QString getPassword();

private slots:
    void on_eraseBtn_clicked();

    void on_loginBtn_clicked();

private:
    Ui::loginWindow *ui;
};

#endif // LOGINWINDOW_H
