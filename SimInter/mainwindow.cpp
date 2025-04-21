#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextEdit>
#include <QLayout>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Crear las tabs para la memoria

    for(int i = 0; i < 8; i++){
        this->PETabs[i] = new QWidget(ui->loadMemoryWidget);
        ui->PEMemoriesTW->addTab(this->PETabs[i], QString("PE ").append(std::to_string(i)));
        this->PETextEdits[i] = new QTextEdit();
        this->PETextEdits[i]->setText(QString("PE ").append(std::to_string(i)).append(" worked"));

        QVBoxLayout *layout = new QVBoxLayout(this->PETabs[i]);
        layout->addWidget(this->PETextEdits[i]);
        this->PETabs[i]->setLayout(layout);

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
