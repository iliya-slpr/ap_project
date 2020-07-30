#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for (int i=0;i<userList.size();i++)
    {
        for(int j=0;j<userList[i].getAccount().size();j++)
        {
            accountList.push_back(&userList[i].getAccount()[j]);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

