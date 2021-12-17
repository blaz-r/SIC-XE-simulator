#include <cstdint>
#include <iostream>
#include "registers.h"

int Registers::assertValidValue(int32_t value)
{
    if(value & (255 << 24))
    {
        std::cout << "Invalid value: " << value << ", registers are 24 bit" << std::endl;
        return 0;
    }
    return 1;
}

int32_t Registers::getReg(int reg)
{
    //return *registeerList[reg];
    switch (reg) {
        case A: return regA;
        case X: return regX;
        case L: return regL;
        case B: return regB;
        case S: return regS;
        case T: return regT;
        case F: return (int32_t) regF;
        case PC: return regPC;
        case SW: return regSW;
        default: 
        std::cout << "Invalid register index: " << reg << std::endl;
    }
    return 0;
}

void Registers::setReg(int reg, int32_t value)
{
    int32_t realValue = value;
    if(value & (1 << 23)) {
        // negative
        realValue |= (255 << 24);
    }
    else 
    {
        // positive
        realValue &= ~(255 << 24);
    }

    bool valid = true;
    switch (reg) {
        case A: regA = realValue; break;
        case X: regX = realValue; break;
        case L: regL = realValue; break;
        case B: regB = realValue; break;
        case S: regS = realValue; break;
        case T: regT = realValue; break;
        case F: regF = realValue; break;
        case PC: regPC = realValue; break;
        case SW: regSW = realValue; break;
        default: 
            std::cout << "Invalid register index: " << reg << std::endl;
            valid = false;
    }
    if (valid)
    {
        emit valueChanged(std::make_pair(reg, value));
    }
}

int32_t Registers::getA()
{
    return getReg(A);
}

void Registers::setA(int32_t value)
{
    setReg(A, value);
}

int32_t Registers::getX()
{
    return getReg(X);
}

void Registers::setX(int32_t value)
{
    setReg(X, value);
}

int32_t Registers::getL()
{
    return getReg(L);
}

void Registers::setL(int32_t value)
{
    setReg(L, value);
}

int32_t Registers::getB()
{
    return getReg(B);
}

void Registers::setB(int32_t value)
{
    setReg(B, value);
}

int32_t Registers::getS()
{
    return getReg(S);
}

void Registers::setS(int32_t value)
{
    setReg(S, value);
}

int32_t Registers::getT()
{
    return getReg(T);
}

void Registers::setT(int32_t value)
{
    setReg(T, value);
}

double Registers::getF()
{
    return regF;
}

void Registers::setF(double value)
{
    regF = value;
    uint64_t transform;
    memcpy(&transform, &value, 8);
    emit valueChanged(std::make_pair(F, transform));
}

int32_t Registers::getPC()
{
    return getReg(PC);
}

void Registers::setPC(int32_t value)
{
    setReg(PC, value);
}

int32_t Registers::getSW()
{
    return getReg(SW);
}

void Registers::setSW(int32_t value)
{
    setReg(SW, value);
}

void Registers::clearRegisters()
{
    int regs[] = {0, 1, 2, 3, 4, 5, 6, 8, 9};

    for (auto &&i : regs)
    {
        setReg(i, 0);
    }
}
