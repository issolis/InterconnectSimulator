#include "mainwindow.h"
#include <QStyleFactory>
#include <QApplication>
#include <QDebug>
#include "stepchanges.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

    /*
    StepChanges * stepper = new StepChanges();

    for(int i = 1; i < 11; i++){
        SlotsList * slot = new SlotsList();
        stepper->addStep(slot);
    }

    qDebug() << "Length: " << stepper->getLength();

    delete stepper;
    */
}
