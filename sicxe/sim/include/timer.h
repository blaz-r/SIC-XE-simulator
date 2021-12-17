#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
private:
    std::chrono::time_point<std::chrono::steady_clock> lastTime;
    int running = 0;
    double nanoCount;
public:
    // Hz -> amount of nanoseconds per tick
    Timer(double freq): nanoCount(1000000000 / freq) {}
    ~Timer() {}

    void start()
    {
        lastTime = std::chrono::steady_clock::now();
        running = 1;
    }

    void stop()
    {
        running = 0;
    }

    void setFreq(double freq)
    {
        nanoCount = 1000000000 / freq;
    }

    bool tick()
    {
        if(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - lastTime).count() * running >= nanoCount)
        {
//            std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - lastTime).count() << std::endl;
            lastTime = std::chrono::steady_clock::now();
            return true;
        }
        else
        {
            return false;
        }
    }
};

#endif
