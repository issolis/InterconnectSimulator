#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QTableWidget>
#include <vector>
#include <thread>
#include <QTimer>
#include "./Processor/headers/InstructionList.h"
#include "stepchanges.h"

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
    void onActionRestartTriggered();
    void onActionPauseTriggered();
    void changeStepTime(int index);

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
    void resetExecution();

    std::string int_to_hex(int decimal);
    void addItemToTable(QTableWidget * table ,QString text, int row, int column);

    //Workers del procesador:
    std::vector<std::thread>* workers = new std::vector<std::thread>();
    int paso = 1;
    int executionState = 0;
    /* Execution States:
     * 0: Hasn't run the controller
     * 1: Has run the controller (HRTC) - is not playing
     * 2: HRTC - is playing
     * 3: HRTC - is paused
     * 4: HRTC - has finished
    */
    void executeStepsInController();
    void executeSingleStep();

    //Stack de respuestas
    InstructionList * fullResponseStack = new InstructionList();
    int * limitIndex = new int[10];

    //Lista de Invalidación
    StepChanges * stepChanges = new StepChanges();

    int stepDuration = 300;

    uint16_t caches[128*8];

};
#endif // MAINWINDOW_H
