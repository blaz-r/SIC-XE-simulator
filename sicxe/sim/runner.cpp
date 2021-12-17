#include "runner.h"
#include "timer.h"

void Runner::resume()
{
    running = true;
}

void Runner::stop()
{
    running = false;
}

void Runner::step()
{
    sicxe.execute();
}

int Runner::getSpeed()
{
    return speed;
}

void Runner::setSpeed(int Hz)
{
    speed = Hz;
    clock.setFreq(speed);
}

void Runner::run()
{
//    double test = 0;
    for(;running;)
    {
//        test++;
        if(clock.tick())
        {
            int ret = sicxe.execute();
            if (ret == HALT)
            {
                return;
            }
        }
//        std::cout << test << std::endl;
    }
}
