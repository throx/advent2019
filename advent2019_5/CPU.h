#pragma once
class CPU
{
public:
    CPU(std::vector<int>& ram);
    ~CPU();

    bool ExecuteNext();

protected:
    void ADD();
    void MUL();
    void INP();
    void OUT();
    void JT();
    void JF();
    void LT();
    void EQ();

    void END();

    int GetNextOperand();
    void SetNextOperand(int val);

private:
    void Ensure(int s);
    std::vector<int>& m_ram;
    int m_pc;

    typedef void(CPU::*InstrFn)();
    std::map<int, InstrFn> m_dispatch;

    int m_modes;
};

