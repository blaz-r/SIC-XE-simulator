#ifndef RUNNER_H
#define RUNNER_H

#include "machine.h"
#include "timer.h"
#include <QThread>
#include <QSemaphore>

class Runner : public QThread
{
    Q_OBJECT
private:
    Machine& sicxe;
    Timer clock = Timer(1000);
    bool running = true;
    int speed = 1000; // 100 Hz
public:
    Runner(Machine& cpu): sicxe(cpu) { clock.start(); }
    ~Runner() {}

    void stop();
    void resume();
    void step();

    int getSpeed();
    void setSpeed(int kHz);

    void run();
};

#endif
