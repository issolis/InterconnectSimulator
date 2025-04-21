#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>

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

private:
    Ui::MainWindow *ui;
    QWidget * PETabs [8];
    QTextEdit * PETextEdits[8];
    QString path;
    void cargarArchivosMemoria(QString path);
};
#endif // MAINWINDOW_H
