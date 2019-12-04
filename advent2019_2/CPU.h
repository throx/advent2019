#pragma once
class CPU
{
public:
    CPU(std::vector<int>& ram);
    ~CPU();

    bool ExecuteNext();

protected:
    bool ADD();
    bool MUL();
    bool END();

private:
    void Ensure(int s);
    std::vector<int>& m_ram;
    int m_pc;

    typedef bool(CPU::*InstrFn)();
    std::map<int, InstrFn> m_dispatch;
};

