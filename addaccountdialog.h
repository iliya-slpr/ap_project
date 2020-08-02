#ifndef ADDACCOUNTDIALOG_H
#define ADDACCOUNTDIALOG_H

#include <QDialog>

namespace Ui {
class addAccountDialog;
}

class addAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addAccountDialog(QWidget *parent = nullptr);
    ~addAccountDialog();
    int getBalance();
    int getType();

private:
    Ui::addAccountDialog *ui;
};

#endif // ADDACCOUNTDIALOG_H
