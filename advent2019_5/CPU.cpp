#include "pch.h"
#include "CPU.h"
using namespace std;

CPU::CPU(vector<int>& ram) :
    m_ram(ram),
    m_pc(0)
{
    m_dispatch[1] = &CPU::ADD;
    m_dispatch[2] = &CPU::MUL;
    m_dispatch[3] = &CPU::INP;
    m_dispatch[4] = &CPU::OUT;
    m_dispatch[5] = &CPU::JT;
    m_dispatch[6] = &CPU::JF;
    m_dispatch[7] = &CPU::LT;
    m_dispatch[8] = &CPU::EQ;
    m_dispatch[99] = &CPU::END;
}

CPU::~CPU()
{
}

bool CPU::ExecuteNext()
{
    m_modes = 1;    // Instruction fetch is always immediate!
    int opcode = GetNextOperand();

    if (opcode == 99) return false;

    // Extract modes
    m_modes = opcode / 100;
    opcode = opcode % 100;

    // Dispatch
    if (m_dispatch.find(opcode) == m_dispatch.end())
    {
        throw new exception("Opcode out of range");
    }

    InstrFn dispatch = m_dispatch[opcode];
    (this->*dispatch)();
    return true;
}

void CPU::Ensure(int s)
{
    if (s < 0 || s >= m_ram.size())
    {
        throw new exception("Invalid Address");
    }
}

int CPU::GetNextOperand()
{
    Ensure(m_pc);
    int operand = m_ram[m_pc++];
    int mode = m_modes % 10;
    m_modes = m_modes / 10;
    switch (mode)
    {
    case 0:
        // Address
        Ensure(operand);
        return m_ram[operand];
    case 1:
        // Immediate
        return operand;
    default:
        throw new exception("Invalid Mode for Read");
    }
}

void CPU::SetNextOperand(int val)
{
    Ensure(m_pc);
    int operand = m_ram[m_pc++];
    int mode = m_modes % 10;
    m_modes = m_modes / 10;
    switch (mode)
    {
    case 0:
        // Address
        Ensure(operand);
        m_ram[operand] = val;
        return;
    default:
        throw new exception("Invalid Mode for Write");
    }
}

void CPU::ADD()
{
    int s1 = GetNextOperand();
    int s2 = GetNextOperand();

    int d = s1 + s2;
    SetNextOperand(d);
}

void CPU::MUL()
{
    int s1 = GetNextOperand();
    int s2 = GetNextOperand();

    int d = s1 * s2;
    SetNextOperand(d);
}

void CPU::INP()
{
    int v;
    cin >> v;

    SetNextOperand(v);
}

void CPU::OUT()
{
    int v;
    v = GetNextOperand();

    cout << v << endl;
}

void CPU::JT()
{
    int s1 = GetNextOperand();
    int s2 = GetNextOperand();

    if (s1)
    {
        m_pc = s2;
    }
}

void CPU::JF()
{
    int s1 = GetNextOperand();
    int s2 = GetNextOperand();

    if (!s1)
    {
        m_pc = s2;
    }
}

void CPU::LT()
{
    int s1 = GetNextOperand();
    int s2 = GetNextOperand();

    int d = (s1 < s2) ? 1 : 0;
    SetNextOperand(d);
}

void CPU::EQ()
{
    int s1 = GetNextOperand();
    int s2 = GetNextOperand();

    int d = (s1 == s2) ? 1 : 0;
    SetNextOperand(d);
}

void CPU::END()
{
}
