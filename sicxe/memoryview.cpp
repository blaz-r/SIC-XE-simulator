#include "memoryview.h"


MemoryView::MemoryView(QWidget* parent) : QAbstractScrollArea(parent)
{
    init();
}

MemoryView::~MemoryView() {}


void MemoryView::init()
{
    connect(verticalScrollBar(), &QScrollBar::valueChanged,
            this, &MemoryView::adjustContent);
    connect(horizontalScrollBar(), &QScrollBar::valueChanged,
            this, &MemoryView::adjustContent);

    addressBlockWidth = 2;
    bytesPerLine = 16;

    pxWidth = 8;
    pxHeight = 13;

    addressWidth = (addressBlockWidth * 4 + addressBlockWidth - 1) * pxWidth;
    hexWidth = (bytesPerLine * 3 + 1) * pxWidth;
    asciiWidth = (bytesPerLine * 2 + 1) * pxWidth;
}

void MemoryView::setData(uint8_t* memPtr, int memSize)
{
    memoryPtr = memPtr;
    size = memSize;
    adjustContent();
}

QByteArray MemoryView::data(uint32_t pos, uint32_t count)
{
  if (pos >= size)
    count = size;
  else if ((pos + count) > size)
    count = size - pos;

  return QByteArray((char*)&memoryPtr[pos], count);
}

void MemoryView::paintEvent(QPaintEvent*)
{
    QPainter painter(viewport());

    if (highlight)
    {
        int bx = (hlX - addressWidth + 3 * pxWidth) * bytesPerLine / hexWidth;
        int by = hlY / pxHeight;
        highlight = false;
        painter.fillRect(addressWidth + 3 * pxWidth * bx - 14, pxHeight * by + 1, pxWidth*2+2, pxHeight,
        QBrush(QColor(90, 180, 190, 128)));
    }

    int offsetX = horizontalScrollBar()->value();

    int y = pxHeight;

    painter.setPen(viewport()->palette().color(QPalette::WindowText));

    // address column
    QString prev_addr = "00000";
    for (int row = 1; row <= dataVisible.size() / bytesPerLine + 1; row++)
    {
        QString curr_addr = QString("%1")
                            .arg(windowStartPos + bytesPerLine * row - 1, 5, 16, QChar('0'))
                            .toUpper();

        QString combined = QString("%1-%2").arg(prev_addr, curr_addr);
        prev_addr = QString("%1")
                    .arg(windowStartPos + bytesPerLine * row, 5, 16, QChar('0'))
                    .toUpper();

        painter.drawText(pxWidth / 2 - offsetX, y, combined);
        y += pxHeight;
    }

    int x;
    int viewOffset = addressWidth + pxWidth;
    painter.drawLine(viewOffset - offsetX, 0, viewOffset - offsetX, height());
    viewOffset += pxWidth / 2;
    y = pxHeight;

    // hex data column
    int bPos = 0;
    for (int row = 0; row < numOfRowsVisible; row++)
    {
        x = viewOffset - offsetX;
        for (int col = 0; (col < bytesPerLine) && (bPos < dataHex.size()); col++)
        {
            QString str = dataHex.mid(bPos * 2, 2).toUpper();
            painter.drawText(x, y, str);
            x += 3 * pxWidth;
            bPos += 1;
        }
        y += pxHeight;
    }

    // offset for ascii column
    viewOffset = addressWidth + hexWidth;
    painter.drawLine(viewOffset - offsetX, 0, viewOffset - offsetX, height());

    viewOffset += pxWidth / 2;

    bPos = 0;
    y = pxHeight;
    int ch;
    for (int row = 0; row < numOfRowsVisible; row++)
    {
        x = viewOffset - offsetX;
        for (int col = 0; (col < bytesPerLine) && (bPos < dataVisible.size()); col++)
        {
            ch = char(dataVisible.at(bPos));
            if (ch < 0x20)
            {
                ch = '.';
            }
            painter.drawText(x, y, QChar(ch));
            x += 2 * pxWidth;
            bPos += 1;
        }
        y += pxHeight;
    }
}

void MemoryView::adjustContent()
{
    numOfRowsVisible = viewport()->height() / pxHeight;
    int vertical_val = verticalScrollBar()->value();
    windowStartPos = vertical_val * bytesPerLine;

    int lineCount = size / bytesPerLine;
    verticalScrollBar()->setRange(0, lineCount - numOfRowsVisible);
    verticalScrollBar()->setPageStep(numOfRowsVisible);

    dataVisible = data(windowStartPos, numOfRowsVisible * bytesPerLine);
    dataHex = dataVisible.toHex();
    viewport()->update();
}

void MemoryView::mousePressEvent(QMouseEvent* ev)
{
    int vertical_val = verticalScrollBar()->value();

    QPointF pos = ev->position();
    if (pos.x() > addressWidth + hexWidth)
    {
        return;
    }
    int x = (pos.x() - addressWidth - pxWidth) / (3 * pxWidth);
    int y = vertical_val + pos.y() / pxHeight;
    int addr = y * bytesPerLine + x;

    emit addressSelected(QString("%1").arg(addr, 5, 16, QChar('0')).toUpper());
    hlX = pos.x();
    hlY = pos.y();
    highlight = true;
    viewport()->repaint();
}
