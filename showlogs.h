#ifndef SHOWLOGS_H
#define SHOWLOGS_H

#include <QDialog>

namespace Ui {
class showLogs;
}

class showLogs : public QDialog
{
    Q_OBJECT

public:
    explicit showLogs(QWidget *parent = nullptr);
    ~showLogs();
private slots:
    void getLogs(QString , QString);


private:
    Ui::showLogs *ui;
};

#endif // SHOWLOGS_H
