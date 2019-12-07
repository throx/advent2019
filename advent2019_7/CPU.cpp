#include "pch.h"
#include "CPU.h"
using namespace std;

CPU::CPU() :
    CPU(m_private_ram)
{
}

CPU::CPU(vector<int>& ram) :
    CPU(ram, [&]() { int v; cin >> v; return v; },
        [=](int v) { cout << v << endl; })
{
}

CPU::CPU(std::vector<int>& ram,
        std::function<int()> get_input,
        std::function<void(int)> set_input) :
    m_ram(ram),
    m_get_input(get_input),
    m_set_input(set_input),
    m_pc(0),
    m_logger(nullptr)
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

void CPU::Log(const string& s)
{
    if (m_logger) m_logger(s);
}

bool CPU::ExecuteNext()
{
    m_modes = 1;    // Instruction fetch is always immediate!
    int opcode = GetNextOperand();

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
    return (opcode != 99);
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
    Log("ADD");
    int s1 = GetNextOperand();
    int s2 = GetNextOperand();

    int d = s1 + s2;
    SetNextOperand(d);
}

void CPU::MUL()
{
    Log("MUL");
    int s1 = GetNextOperand();
    int s2 = GetNextOperand();

    int d = s1 * s2;
    SetNextOperand(d);
}

void CPU::INP()
{
    Log("INP");
    int v = m_get_input();

    SetNextOperand(v);
}

void CPU::OUT()
{
    Log("OUT");
    int v;
    v = GetNextOperand();

    m_set_input(v);
}

void CPU::JT()
{
    Log("JT");
    int s1 = GetNextOperand();
    int s2 = GetNextOperand();

    if (s1)
    {
        m_pc = s2;
    }
}

void CPU::JF()
{
    Log("JF");
    int s1 = GetNextOperand();
    int s2 = GetNextOperand();

    if (!s1)
    {
        m_pc = s2;
    }
}

void CPU::LT()
{
    Log("LT");
    int s1 = GetNextOperand();
    int s2 = GetNextOperand();

    int d = (s1 < s2) ? 1 : 0;
    SetNextOperand(d);
}

void CPU::EQ()
{
    Log("EQ");
    int s1 = GetNextOperand();
    int s2 = GetNextOperand();

    int d = (s1 == s2) ? 1 : 0;
    SetNextOperand(d);
}

void CPU::END()
{
    Log("END");
}
