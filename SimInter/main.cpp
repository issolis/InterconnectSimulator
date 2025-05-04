#include "mainwindow.h"
#include <QStyleFactory>
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(QString("InterSim"));
    w.show();

    return a.exec();
}
