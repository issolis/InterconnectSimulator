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
#include <sstream>
#include <chrono>
#include "./Processor/headers/ProcessorController.h"

using namespace std::chrono;

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

    ui->PEComboBox->addItem(QString("Compartida"));
    for(int i = 0; i < 8; i++){
        ui->PEComboBox->addItem(QString("PE ").append(std::to_string(i + 1)));
    }

    ui->PEComboBox->setCurrentIndex(0);


    QStringList headers;
    headers << "Bloque" << "Valor" << "Estado";
    ui->MemoryStateTable->setColumnCount(3);
    ui->MemoryStateTable->setHorizontalHeaderLabels(headers);
    headers << "Bloque" << "Valor";
    ui->SharedMemStateTable->setColumnCount(2);
    ui->SharedMemStateTable->setHorizontalHeaderLabels(headers);


    for(int i = 0; i < 128; i++){
        ui->MemoryStateTable->insertRow(i);
        ui->SharedMemStateTable->insertRow(i);
        QString hexValue = QString("0x").append(this->int_to_hex(i));
        this->addItemToTable(ui->MemoryStateTable, hexValue, i, 0);
        this->addItemToTable(ui->SharedMemStateTable, hexValue, i, 0);
        this->addItemToTable(ui->MemoryStateTable, QString("VALID"), i, 2);
        this->addItemToTable(ui->SharedMemStateTable, QString("0"), i, 1);
    }

    for(int i = 128; i < 4096; i++){
        ui->SharedMemStateTable->insertRow(i);
        QString hexValue = QString("0x").append(this->int_to_hex(i));
        this->addItemToTable(ui->SharedMemStateTable, hexValue, i, 0);
        this->addItemToTable(ui->SharedMemStateTable, QString("0"), i, 1);
    }

    this->changeTable(0);

    //Set up del principal

    this->timerSteps = new QTimer(this);
    connect(this->timerSteps, &QTimer::timeout, this, &MainWindow::checkStepperExecution);
    connect(ui->TimeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeStepTime(int)));
    connect(ui->PEComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeTable(int)));
    connect(ui->actionNextView, &QAction::triggered, this, &MainWindow::onActionNextViewTriggered);
    connect(ui->actionPreviousView, &QAction::triggered, this, &MainWindow::onActionPreviousViewTriggered);
    connect(ui->actionPlay, &QAction::triggered, this, &MainWindow::onActionPlayTriggered);
    connect(ui->actionStep, &QAction::triggered, this, &MainWindow::onActionStepTriggered);
    connect(ui->actionRestart, &QAction::triggered, this, &MainWindow::onActionRestartTriggered);
    connect(ui->actionPause, &QAction::triggered, this, &MainWindow::onActionPauseTriggered);
    for(int i = 0; i < 10; i++){
        limitIndex[i] = 0;
    }
}

void MainWindow::changeStepTime(int index){
    switch(index){
        case 0:
            stepDuration = 300;
            break;
        case 1:
            stepDuration = 500;
            break;
        case 2:
            stepDuration = 1000;
            break;
        case 3:
            stepDuration = 3000;
            break;
        case 4:
            stepDuration = 5000;
            break;
    }
}

void MainWindow::checkStepperExecution(){
    if(this->paso < 10){
        this->paso++;
        this->executeSingleStep();
    }else{
        executionState = 4;
        this->timerSteps->stop();
        ui->StateLabel->setText(QString("Finalizado"));
    }
}

void MainWindow::resetExecution(){
    executionState = 1;
    this->paso = 1;
    if(stepChanges->getMarkedBlocksList()->getLength() > 0){
        for(int i = 0; i < stepChanges->getMarkedBlocksList()->getLength(); i++){
            int block = stepChanges->getMarkedBlocksList()->getSlotByIndex(i)->getBlock();
            ui->MemoryStateTable->item(block, 2)->setText("VALID");
        }
    }
    if(stepChanges->getMemoryBlocksUpdated()->getLength() > 0){
        qDebug() << "Length: " << stepChanges->getMemoryBlocksUpdated()->getLength();
        for(int i = 0; i < stepChanges->getMemoryBlocksUpdated()->getLength(); i++){
            int block = stepChanges->getMemoryBlocksUpdated()->getSlotByIndex(i)->getBlock();
            ui->SharedMemStateTable->item(block, 1)->setText(QString("0"));
        }
    }
    stepChanges->returnHome();
    ui->StateLabel->setText(QString("Ejecutando"));
}

void MainWindow::onActionPlayTriggered(){
    if(this->path.length() > 0){
        if(executionState == 4 || executionState < 2){
            if(executionState == 0){
                this->executeStepsInController();
            }
            if(executionState == 4){
                this->resetExecution();
            }
            executionState = 2;
            this->executeSingleStep();
        }else if(executionState==3){
            executionState = 2;
            this->timerSteps->start(stepDuration);
        }
        ui->StateLabel->setText(QString("Ejecutando"));
    }else{
        openDialog(QString("Error"), QString("No hay instrucciones para ejecutar"), 1);
    }
}

void MainWindow::onActionRestartTriggered(){
    if(this->path.length() > 0){
        if(executionState == 0){
            this->executeStepsInController();
        }
        if(executionState == 2){
            this->timerSteps->stop();
        }
        this->resetExecution();
        executionState = 2;
        this->executeSingleStep();
    }else{
        openDialog(QString("Error"), QString("No hay instrucciones cargadas para ejecutar"), 1);
    }
}

void MainWindow::onActionPauseTriggered(){
    if(executionState == 2){
        this->timerSteps->stop();
        executionState = 3;
        ui->StateLabel->setText(QString("Pausa"));
    }
}


void MainWindow::executeSingleStep(){
    QString InstructionsInThisStep = QString("");
    for(int i = limitIndex[this->paso -1]; i < limitIndex[this->paso]; i++){
        InstructionsInThisStep.append(this->fullResponseStack->getInstruction(i)->getInstr());
        InstructionsInThisStep.append("\n");
    }
    ui->ExeOutputTE->setText(InstructionsInThisStep);
    ui->PasoLabel->setText(QString("Paso: ").append(std::to_string(this->paso)));
    SlotsList * slot;
    slot = stepChanges->getCurrent();
    if(slot->getLength() > 0){
        for(int i = 0; i < slot->getLength(); i++){
            int block = slot->getSlotByIndex(i)->getBlock();
            ui->MemoryStateTable->item(block, 2)->setText("INVALID");
        }
    }
    if(slot->getChanges() > 0){
        for(int i = 0; i < slot->getChanges(); i++){
            MemoryChange * change = slot->getChangeByIndex(i);
            //qDebug() << change;
            //qDebug() << change->getBlockUpdated() << ": " << change->getNewValue();
            ui->SharedMemStateTable->item(change->getBlockUpdated(), 1)->setText(QString("").append(std::to_string(change->getNewValue())));
        }
    }
    stepChanges->moveRight();
    this->timerSteps->start(stepDuration);
}

void MainWindow::onActionStepTriggered(){
    if(executionState == 0){
        this->executeStepsInController();
    }
    if(paso < 10){
        paso++;
    }else{
        paso = 1;
    }

}

void MainWindow::executeStepsInController(){
    std::string filePaths [8];
    for(int i = 0; i < 8; i++){
        QString rootPath = this->path;
        filePaths[i] = (rootPath.append("/InstructionsP").append(std::to_string(i + 1)).append(".txt")).toStdString();
    }

    ProcessorController* controller = new ProcessorController(*(this->workers), filePaths);
    int instNum = 0;
    //Obtener todos los valores de los bloques en los procesadores
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 128; j++){
            caches[j + 128*i] = controller->processors[i].cacheMemory->cache[j];
        }
    }
    int tableToPrint = 0;
    if(ui->PEComboBox->currentIndex() > 1){
        tableToPrint = ui->PEComboBox->currentIndex()-1;
    }

    for(int i = 0; i < 128; i++){
        QString value = QString("").append(std::to_string(caches[i + 128*tableToPrint]));
        this->addItemToTable(ui->MemoryStateTable,value, i, 1);
    }

    //Ejecución de cada paso
    for(int i = 1; i < 11; i++){

        auto start = high_resolution_clock::now();
        InstructionList * stepList = controller->step(i);
        auto finish = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(finish - start);

        exeDurations[i-1] = duration.count();

        Instruction * curr = stepList->head;

        while(curr->getNextInstr() != nullptr){
            std::string * newInstructionAdded = new std::string(curr->getInstr());
            fullResponseStack->addInstr(*newInstructionAdded);
            instNum++;
            curr = curr->getNextInstr();
        }
        std::string * newInstructionAdded = new std::string(curr->getInstr());
        fullResponseStack->addInstr(*newInstructionAdded);
        instNum++;
        limitIndex[i] = instNum;

        //Cache state
        SlotsList * slotThisStep = new SlotsList();
        stepChanges->addStep(slotThisStep);
        for(int proc = 0; proc < 8; proc++){
            for(int j = 0; j < 128; j++){
                std::string state = controller->processors[proc].cacheMemory->cacheState[j];
                if(state == "INVALID"){
                    if(!stepChanges->getMarkedBlocksList()->isBlockPresent(j)){
                        stepChanges->getMarkedBlocksList()->addSlot(j);
                        slotThisStep->addSlot(j);
                    }
                }
            }
        }

        //Shared Memory Values
        std::vector<uint32_t> * initialShared = controller->interconnectBus->sharedMemory->sharedMemory;
        for (size_t k = 0; k < initialShared->size(); k++) {
            uint32_t value = (*initialShared)[k];
            if(value != 0){
                //qDebug() << "Non zero: " << k << ","<< value;
                if(!stepChanges->getMemoryBlocksUpdated()->isBlockPresent(k)){
                    stepChanges->getMemoryBlocksUpdated()->addSlot(k);
                    //qDebug() << "Different: " << k << ","<< value;
                }
                slotThisStep->addChange(k, value);
            }
        }
    }

    //Setea la gráfica
    for(int i = 1; i < 11; i++){
        series->append(i, exeDurations[i-1]);
    }

    chart->setTitle(QString("Duración de cada paso"));

    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    QVariant min = QVariant::fromValue(getShortestDuration());
    long longest = getLongestDuration();
    QVariant max = QVariant::fromValue(longest+longest/10);
    chart->axes(Qt::Vertical).first()->setRange(min, max);
    chart->axes(Qt::Vertical).first()->setTitleText("Tiempo de ejecución (μs)");
    chart->axes(Qt::Horizontal).first()->setRange(0,11);
    chart->axes(Qt::Horizontal).first()->setTitleText("Instrucción");
    chart->setVisible(true);

    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setVisible(true);
    graphLayout->addWidget(chartView);
    ui->pageGraph->setLayout(graphLayout);
}

long MainWindow::getLongestDuration(){
    long max = 0;
    for(int i = 0; i < 10; i++){
        if(exeDurations[i] > max){
            max = exeDurations[i];
        }
    }
    return max;
}

long MainWindow::getShortestDuration(){
    long min = LONG_MAX;
    for(int i = 0; i < 10; i++){
        if(exeDurations[i] < min){
            min = exeDurations[i];
        }
    }
    return min;
}

std::string MainWindow::int_to_hex(int decimal) {
    std::stringstream ss;
    ss << std::hex << std::uppercase << decimal;
    return ss.str();
}

void MainWindow::addItemToTable(QTableWidget * table ,QString text, int row, int column){
    QTableWidgetItem * item = new QTableWidgetItem();
    item->setText(text);
    table->setItem(row,column, item);
}

MainWindow::~MainWindow()
{
    delete ui;
    for(int i = 0; i < 8; i++){
        delete PETabs[i];
        delete PETextEdits[i];
    }
    delete workers;
    delete fullResponseStack;
    delete stepChanges;
    delete[] limitIndex;
    delete series;
    delete chart;
    delete graphLayout;
}

void MainWindow::changeTable(int index){
    int memoryBlocks = 128;
    if(index == 0){
        //memoryBlocks = 4096;
        ui->MemoryStateTable->setVisible(false);
        ui->SharedMemStateTable->setVisible(true);
    }else{
        ui->MemoryStateTable->setVisible(true);
        ui->SharedMemStateTable->setVisible(false);
    }
    if(executionState != 0){

        for(int i = 0; i < memoryBlocks; i++){
            QString value = QString("").append(std::to_string(caches[i + 128*(index-1)]));
            ui->MemoryStateTable->item(i, 1)->setText(value);
        }
    }

}

void MainWindow::onActionPreviousViewTriggered(){
    this->current_page -=1;
    if(this->current_page < 0){
        this->current_page = ui->stackedWidget->count() - 1;
    }
    showCurrentPage();
}


void MainWindow::onActionNextViewTriggered(){
    this->current_page += 1;
    if(this->current_page == ui->stackedWidget->count()){
        this->current_page = 0;
    }
    showCurrentPage();
}

void MainWindow::showCurrentPage(){
    ui->stackedWidget->setCurrentIndex(this->current_page);
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
    //qDebug() << this->path;
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
