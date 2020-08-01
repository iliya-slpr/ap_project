#include "mainwindow.h"
#include "loginwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *m;
    m=new MainWindow();
    if(m->getLoginSeccess())
    {
    //m->show();
    return a.exec();
    }
    else
    {
        delete m;
    }

}
