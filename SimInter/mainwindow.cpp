#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextEdit>
#include <QLayout>
#include <QFileDialog>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Crear las tabs para la memoria

    for(int i = 0; i < 8; i++){
        this->PETabs[i] = new QWidget(ui->loadMemoryWidget);
        ui->PEMemoriesTW->addTab(this->PETabs[i], QString("PE ").append(std::to_string(i + 1)));
        this->PETextEdits[i] = new QTextEdit();
        QVBoxLayout *layout = new QVBoxLayout(this->PETabs[i]);
        layout->addWidget(this->PETextEdits[i]);
        this->PETabs[i]->setLayout(layout);



    }
    connect(ui->actionSavePE, &QAction::triggered, this, &MainWindow::onActionSavePETriggered);
    connect(ui->actionSaveAllPEs, &QAction::triggered, this, &MainWindow::onActionSaveAllPEsTriggered);
    connect(ui->actionCargarMemorias, &QAction::triggered, this, &MainWindow::onActionCargarMemoriasTriggered);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onActionSavePETriggered()
{
    if(this->path.length() > 0){
        int tab = this->ui->PEMemoriesTW->currentIndex();
        std::ofstream PEFile(QString("../../InstructionsFile/InstructionsP").append(std::to_string(tab + 1)).append(".txt").toStdString());
        if(PEFile.is_open()){
            PEFile  << this->PETextEdits[tab]->toPlainText().toStdString();
            PEFile.close();
        }
        else{
            qDebug() << "Failed to write";
        }
    }else{

    }
}

void MainWindow::onActionSaveAllPEsTriggered()
{
    qDebug() <<"Clicked 2" ;
}

void MainWindow::onActionCargarMemoriasTriggered(){
    this->path = QFileDialog::getExistingDirectory(this, QString("Seleccione la carpeta con Instrucciones"), QDir::currentPath());
    this->cargarArchivosMemoria(this->path);
}

void MainWindow::cargarArchivosMemoria(QString path){
    //Carga los archivos de las instrucciones en el espacio de texto:
    std::string textInLine;
    for(int i = 0; i < 8; i++){
        QString textInTextEdit = "";
        QString pathToFile = this->path;
        pathToFile.append("/InstructionsP").append(std::to_string(i + 1)).append(".txt");

        std::ifstream PEFile(pathToFile.toStdString());
        while(std::getline(PEFile, textInLine)){
            textInTextEdit.append(textInLine).append("\n");
        }
        this->PETextEdits[i]->setText(textInTextEdit);
        PEFile.close();
    }
}
