#include "mainwindow.h"
#include "loginwindow.h"
#include <QApplication>
#include "adminpanel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *m;
    m=new MainWindow();
    m->setWindowState(Qt::WindowFullScreen);
    m->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    if(m->getLoginSeccess()&&m->getIsAdmin())
    {
        delete m;
        AdminPanel *admin;
        admin= new AdminPanel();
        admin->showMaximized();
        return a.exec();
    }
    if(m->getLoginSeccess())
    {
        m->showMaximized();
        return a.exec();
    }
    else
    {
        delete m;
    }

}
