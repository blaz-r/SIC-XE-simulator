#ifndef REGISTERS_H
#define REGISTERS_H

#include <cstdint>
#include <QObject>
#include <iostream>

class Registers : public QObject
{
    Q_OBJECT

private:

    int32_t regA = 0;
    int32_t regX = 0;
    int32_t regL = 0;
    int32_t regB = 0;
    int32_t regS = 0;
    int32_t regT = 0;
    double regF = 0;

    int32_t regPC = 0;
    int32_t regSW = 0;

    //int32_t* registeerList[10] = {&regA, &regX, &regL, &regB, &regS, &regT, 0, 0, &regPC, &regSW};

signals:
    Q_SIGNAL void valueChanged(std::pair<int, int64_t> regValuePair);


public:
    enum CC {LT = 0, EQ = 0x40, GT = 0x80};
    enum SicReg {A, X, L, B, S, T, F, PC = 8, SW};

    Registers() {}
    ~Registers() {}

    int assertValidValue(int32_t value);

    int32_t getReg(int reg);
    void setReg(int reg, int32_t value);

    int32_t getA();
    void setA(int32_t value);

    int32_t getX();
    void setX(int32_t value);

    int32_t getL();
    void setL(int32_t value);

    int32_t getB();
    void setB(int32_t value);

    int32_t getS();
    void setS(int32_t value);

    int32_t getT();
    void setT(int32_t value);

    double getF();
    void setF(double value);

    int32_t getPC();
    void setPC(int32_t value);

    int32_t getSW();
    void setSW(int32_t value);

    void clearRegisters();

    template<typename N>
    void compareAndSetSw(N value1, N value2)
    {
        if(value1 < value2)
        {
            setSW(LT);
        }
        else if (value1 > value2)
        {
            setSW(GT);
        } 
        else
        {
            setSW(EQ);
        }
    }
};
#endif
