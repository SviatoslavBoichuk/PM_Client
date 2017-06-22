//#include "mainwindow.h"
#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    login logWindow;

    logWindow.setFixedSize(330, 240);
    logWindow.move(600,200);
    logWindow.show();

    return a.exec();
}
