#include "pch.h"
#include "CPU.h"
using namespace std;

CPU::CPU(vector<int>& ram) :
    m_ram(ram),
    m_pc(0)
{
    m_dispatch[1] = &CPU::ADD;
    m_dispatch[2] = &CPU::MUL;
    m_dispatch[99] = &CPU::END;
}

CPU::~CPU()
{
}

bool CPU::ExecuteNext()
{
    Ensure(m_pc);
    int opcode = m_ram[m_pc];
    ++m_pc;
    if (m_dispatch.find(opcode) == m_dispatch.end())
    {
        throw new exception("Opcode out of range");
    }

    InstrFn dispatch = m_dispatch[opcode];
    return (this->*dispatch)();
}

void CPU::Ensure(int s)
{
    if (s < 0 || s > m_ram.size())
    {
        throw new exception("Invalid Address");
    }
}

bool CPU::ADD()
{
    Ensure(m_pc + 3);

    int s1 = m_ram[m_pc++];
    int s2 = m_ram[m_pc++];
    int d = m_ram[m_pc++];

    Ensure(s1);
    Ensure(s2);
    Ensure(d);

    m_ram[d] = m_ram[s1] + m_ram[s2];
    return true;
}

bool CPU::MUL()
{
    Ensure(m_pc + 3);

    int s1 = m_ram[m_pc++];
    int s2 = m_ram[m_pc++];
    int d = m_ram[m_pc++];

    Ensure(s1);
    Ensure(s2);
    Ensure(d);

    m_ram[d] = m_ram[s1] * m_ram[s2];
    return true;
}

bool CPU::END()
{
    return false;
}