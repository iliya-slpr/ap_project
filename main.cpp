#include "mainwindow.h"
#include "loginwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loginWindow *w;
    w = new loginWindow();
    w->show();
    return a.exec();
}
