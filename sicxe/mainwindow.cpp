#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <memoryview.h>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->scroll_memory->init();

    setRegOut();

    connect(ui->btn_run, &QPushButton::clicked,
            this, &MainWindow::start);
    connect(ui->menu_load, &QAction::triggered,
            this, &MainWindow::loadObj);
    connect(ui->btn_reset, &QPushButton::clicked,
            this, &MainWindow::reset);
    connect(ui->btn_stop, &QPushButton::clicked,
            this, &MainWindow::stop);
    connect(&sicxe.reg, SIGNAL(valueChanged(std::pair<int,int64_t>)),
            this, SLOT(updateReg(std::pair<int,int64_t>)));
    connect(ui->btn_step, &QPushButton::clicked,
            this, &MainWindow::step);
    connect(&sicxe, SIGNAL(valueWritten(std::pair<uint32_t,int64_t>)),
            this, SLOT(updateWrite(std::pair<uint32_t,int64_t>)));
    connect(&sicxe, SIGNAL(floatWritten(std::pair<uint32_t,double>)),
            this, SLOT(updateWriteFloat(std::pair<uint32_t,double>)));
    connect(ui->scroll_memory, SIGNAL(addressSelected(QString)),
            this, SLOT(displayAddress(QString)));

    updateMemoryView();
}

MainWindow::~MainWindow()
{
    delete ui;
    runnerThread->quit();
    delete runnerThread;
}

void MainWindow::start()
{
    ui->btn_run->setText("Running");

    runnerThread->setSpeed(ui->speed->value());
    runnerThread->resume();

    if(!runnerThread->isRunning())
    {
        runnerThread->start();
    }
}

void MainWindow::loadObj()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open obj file"),
                                        "./", tr("Object files (*.obj)"));
    QMessageBox msgBox;
    if(!fileName.isEmpty() && !fileName.isNull())
    {
        try
        {
            sicxe.loadSection(fileName.toStdString());
            clearWrites();
        }
        catch(const std::exception& e)
        {
            msgBox.warning(this, "Loading obj error", QString("%1: %2").arg("Error reading .obj file", e.what()));
        }
    }
    else
    {
        msgBox.warning(this, "File open warning", "No file selected");
    }
    stop();
}

void MainWindow::reset()
{
    stop();
    sicxe.reset();
    clearWrites();
}

void MainWindow::stop()
{
    runnerThread->stop();
    ui->btn_run->setText("Run");
}

void MainWindow::step()
{
    runnerThread->step();
}

void MainWindow::updateReg(std::pair<int, int64_t> pair)
{
    ((QLineEdit*)regOut[pair.first])->setText(QString::number(pair.second, 16).toUpper());
    if(pair.first == 6)
    {
        double transform;
        memcpy(&transform, &pair.second, 8);
        ui->lned_float_dec->setText(QString::number(transform));
    }
}

void MainWindow::setRegOut()
{
    regOut[0] = ui->lned_regA;
    regOut[1] = ui->lned_regX;
    regOut[2] = ui->lned_regL;
    regOut[3] = ui->lned_regB;
    regOut[4] = ui->lned_regS;
    regOut[5] = ui->lned_regT;
    regOut[6] = ui->lned_regF;
    regOut[8] = ui->lned_regPC;
    regOut[9] = ui->lned_regSW;
}

void MainWindow::updateWrite(std::pair<uint32_t, int64_t> pair)
{
    ui->text_writes->append(QString("(0x%1) -> 0x%2 | %3 | ascii{ %4 }").arg(QString::number(pair.first, 16).toUpper(),
                                                                           QString::number(pair.second, 16).toUpper(),
                                                                           QString::number(pair.second),
                                                                           QString(char(pair.second))));
    updateMemoryView();
}

void MainWindow::updateWriteFloat(std::pair<uint32_t, double> pair)
{
    uint64_t transform;
    memcpy(&transform, &pair.second, 8);
    ui->text_writes->append(QString("(0x%1) -> 0x%2 | %3 | ascii{ %4 }").arg(QString::number(pair.first, 16).toUpper(),
                                                                           QString::number(transform, 16).toUpper(),
                                                                           QString::number(pair.second),
                                                                           QString(char(pair.second))));
    updateMemoryView();
}

void MainWindow::clearWrites()
{
    ui->text_writes->clear();
}

void MainWindow::updateMemoryView()
{
    ui->scroll_memory->setData(sicxe.memory, MAX_ADDRESS);
}

void MainWindow::displayAddress(QString addr)
{
    ui->lned_addr->setText(addr);
}
