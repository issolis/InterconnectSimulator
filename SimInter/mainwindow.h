#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QTableWidget>
#include <vector>
#include <thread>
#include <QTimer>
#include "./Processor/headers/InstructionList.h"
#include "./Processor/headers/Instruction.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onActionSavePETriggered();
    void onActionSaveAllPEsTriggered();
    void onActionCargarMemoriasTriggered();
    void onActionPreviousViewTriggered();
    void onActionNextViewTriggered();
    void changeTable(int index);
    void onActionPlayTriggered();
    void onActionStepTriggered();
    void checkStepperExecution();

private:
    Ui::MainWindow *ui;
    QWidget * PETabs [8];
    QTextEdit * PETextEdits[8];
    QString path;
    void cargarArchivosMemoria(QString path);
    int openDialog(QString title, QString text, int type);
    int current_page = 1;
    void showCurrentPage();
    bool saveFile(int peNum);
    QTimer * timerSteps;


    std::string int_to_hex(int decimal);
    void addItemToTable(QTableWidget * table ,QString text, int row, int column);

    //Workers del procesador:
    std::vector<std::thread>* workers = new std::vector<std::thread>();
    int paso = 1;
    bool hasRunController = false;
    bool finishedExecution = false;
    void executeStepsInController();
    void executeSingleStep();

    //Stack de respuestas
    InstructionList * fullResponseStack = new InstructionList();
    int limitIndex [10] = {0,0,0,0,0,0,0,0,0,0};
};
#endif // MAINWINDOW_H
