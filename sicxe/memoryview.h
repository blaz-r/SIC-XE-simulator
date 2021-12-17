#ifndef MEMORYVIEW_H
#define MEMORYVIEW_H

#include <QAbstractScrollArea>
#include <QScrollBar>
#include <QPainter>
#include <QMouseEvent>
#include "machine.h"

class MemoryView : public QAbstractScrollArea
{
    Q_OBJECT
public:
    MemoryView(QWidget* parent);
    ~MemoryView();

    void setData(uint8_t* memPtr, int memSize);
    void init();

signals:
    Q_SIGNAL void addressSelected(QString);

protected:
    void paintEvent(QPaintEvent*);

private:
    void adjustContent();
    void mousePressEvent(QMouseEvent*);

    QByteArray data(uint32_t pos = 0, uint32_t count = -42);
    uint8_t* memoryPtr;

    int addressBlockWidth;
    int bytesPerLine;

    int addressWidth;
    int hexWidth;
    int asciiWidth;

    int pxWidth;
    int pxHeight;

    uint32_t windowStartPos;
    uint32_t size;

    int numOfRowsVisible;

    QByteArray dataVisible;
    QByteArray dataHex;

    bool highlight = false;
    int hlX, hlY = 0;
};



#endif // MEMORYVIEW_H
