#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "machine.h"
#include "registers.h"
#include "runner.h"
#include <QFileDialog>
#include <QString>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void start();
    void reset();
    void stop();
    void step();

    void loadObj();
    void updateReg(std::pair<int,int64_t>);
    void updateWrite(std::pair<uint32_t,int64_t>);
    void updateWriteFloat(std::pair<uint32_t,double>);

    void displayAddress(QString);

private:
    Ui::MainWindow *ui;
    Machine sicxe;
    Runner* runnerThread = new Runner(sicxe);
    QObject* regOut[10];

    void clearWrites();
    void updateMemoryView();

    void setRegOut();
};
#endif // MAINWINDOW_H
