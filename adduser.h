#ifndef ADDUSER_H
#define ADDUSER_H
#include <QDate>
#include <QDialog>

namespace Ui {
class addUser;
}

class addUser : public QDialog
{
    Q_OBJECT

public:
    explicit addUser(QWidget *parent = nullptr);
    ~addUser();
signals:
    void sendDataToAdd(QString ,QString,QDate,QString,QString);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::addUser *ui;
};

#endif // ADDUSER_H
