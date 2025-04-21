#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextEdit>
#include <QLayout>
#include <QFileDialog>
#include <QDialog>
#include <QLabel>
#include <QIcon>
#include <QStyleFactory>
#include <fstream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Set up para el espacio de escritura de memoria

    for(int i = 0; i < 8; i++){
        this->PETabs[i] = new QWidget(ui->pageLoad);
        ui->PEMemoriesTW->addTab(this->PETabs[i], QString("PE ").append(std::to_string(i + 1)));
        this->PETextEdits[i] = new QTextEdit();
        QVBoxLayout *layout = new QVBoxLayout(this->PETabs[i]);
        layout->addWidget(this->PETextEdits[i]);
        this->PETabs[i]->setLayout(layout);
    }

    connect(ui->actionSavePE, &QAction::triggered, this, &MainWindow::onActionSavePETriggered);
    connect(ui->actionSaveAllPEs, &QAction::triggered, this, &MainWindow::onActionSaveAllPEsTriggered);
    connect(ui->actionCargarMemorias, &QAction::triggered, this, &MainWindow::onActionCargarMemoriasTriggered);

    //Set up del espacio de display para los bloques de memoria
}

MainWindow::~MainWindow()
{
    delete ui;
    for(int i = 0; i < 8; i++){
        delete PETabs[i];
        delete PETextEdits[i];
    }
}

bool MainWindow::saveFile(int peNum){
    if(this->path.length() == 0){
        this->path = QFileDialog::getExistingDirectory(this, QString("Seleccione la carpeta donde se guardará la información"), QDir::currentPath());
    }
    QString pathToFile = this->path;
    pathToFile.append("/InstructionsP").append(std::to_string(peNum + 1)).append(".txt");
    std::ofstream PEFile(pathToFile.toStdString());
    if(PEFile.is_open()){
        PEFile  << this->PETextEdits[peNum]->toPlainText().toStdString();
        PEFile.close();
        return true;
    }
    else{
        return false;
    }
}

void MainWindow::onActionSavePETriggered()
{
    int tab = this->ui->PEMemoriesTW->currentIndex();
    if(saveFile(tab)){
        openDialog(QString("Guardado"), QString("La información se guardó exitosamente"), 0);
    }else{
        openDialog(QString("Error"), QString("No se pudo guardar el archivo correctamente"), 1);
    }
}

void MainWindow::onActionSaveAllPEsTriggered()
{
    bool allSaved = true;
    for(int i = 0; i < 8; i++){
        if(!saveFile(i)){
            openDialog(QString("Error"), QString("El archivo de PE ").append(std::to_string(i)).append(" no se pudo guardar correctamente."), 1);
            allSaved = false;
        }
    }
    if(allSaved){
        openDialog(QString("Guardado"), QString("La información se guardó exitosamente"), 0);
    }

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

int MainWindow::openDialog(QString title, QString text, int type){
    QDialog * dialog = new QDialog();
    dialog->setWindowTitle(title);
    QString iconName = QString("dialog-");

    switch (type) {
    case 0:
        iconName.append("information");
        break;
    case 1:
        iconName.append("warning");
        break;
    default:
        iconName = "";
        break;
    }
    dialog->setMinimumSize(210, 100);

    QIcon dialogIcon = QIcon::fromTheme(iconName);
    QLabel * iconSpace = new QLabel();
    iconSpace->setPixmap(dialogIcon.pixmap(32,32));
    QLabel * textLabel = new QLabel();
    textLabel->setWordWrap(true);
    textLabel->setText(text);

    QHBoxLayout * layout = new QHBoxLayout(dialog);

    layout->addWidget(iconSpace);
    layout->addWidget(textLabel);
    dialog->setLayout(layout);

    dialog->show();

    return 0;
}
