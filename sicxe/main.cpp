#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    w.setLayout(new QVBoxLayout);

//    MemoryViewer viewer(&w);
//    viewer.setData(QByteArray("Hello World"));
//    w.layout()->addWidget(&viewer);

    w.show();
    return a.exec();
}
