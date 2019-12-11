#include "pch.h"
#include "CPU.h"
using namespace std;

CPU::CPU() :
    CPU(m_private_ram)
{
}

CPU::CPU(CPU::Ram& ram) :
    CPU(ram, [&]() { CPU::MemType v; cin >> v; return v; },
        [=](CPU::MemType v) { cout << v << endl; })
{
}

CPU::CPU(CPU::Ram& ram,
        std::function<CPU::MemType()> get_input,
        std::function<void(CPU::MemType)> set_input) :
    m_ram(ram),
    m_get_input(get_input),
    m_set_input(set_input),
    m_pc(0),
    m_modes(0),
    m_base(0),
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
    m_dispatch[9] = &CPU::ADB;
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
    CPU::MemType opcode = GetNextOperand();

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

void CPU::Ensure(CPU::MemType s)
{
    if (s < 0)
    {
        throw new exception("Invalid Address");
    }

    if (s >= m_ram.size())
    {
        m_ram.resize(0x1000 + (s & ~0xfff));
    }
}

CPU::MemType CPU::GetNextOperand()
{
    Ensure(m_pc);
    CPU::MemType operand = m_ram[m_pc++];
    CPU::MemType mode = m_modes % 10;
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
    case 2:
        // Based
        Ensure(operand + m_base);
        return m_ram[operand + m_base];
    default:
        throw new exception("Invalid Mode for Read");
    }
}

void CPU::SetNextOperand(CPU::MemType val)
{
    Ensure(m_pc);
    CPU::MemType operand = m_ram[m_pc++];
    CPU::MemType mode = m_modes % 10;
    m_modes = m_modes / 10;
    switch (mode)
    {
    case 0:
        // Address
        Ensure(operand);
        m_ram[operand] = val;
        return;
    case 2:
        // Based
        Ensure(operand + m_base);
        m_ram[operand + m_base] = val;
        return;
    default:
        throw new exception("Invalid Mode for Write");
    }
}

void CPU::ADD()
{
    Log("ADD");
    CPU::MemType s1 = GetNextOperand();
    CPU::MemType s2 = GetNextOperand();

    CPU::MemType d = s1 + s2;
    SetNextOperand(d);
}

void CPU::MUL()
{
    Log("MUL");
    CPU::MemType s1 = GetNextOperand();
    CPU::MemType s2 = GetNextOperand();

    CPU::MemType d = s1 * s2;
    SetNextOperand(d);
}

void CPU::INP()
{
    Log("INP");
    CPU::MemType v = m_get_input();

    SetNextOperand(v);
}

void CPU::OUT()
{
    Log("OUT");
    CPU::MemType v;
    v = GetNextOperand();

    m_set_input(v);
}

void CPU::JT()
{
    Log("JT");
    CPU::MemType s1 = GetNextOperand();
    CPU::MemType s2 = GetNextOperand();

    if (s1)
    {
        m_pc = s2;
    }
}

void CPU::JF()
{
    Log("JF");
    CPU::MemType s1 = GetNextOperand();
    CPU::MemType s2 = GetNextOperand();

    if (!s1)
    {
        m_pc = s2;
    }
}

void CPU::LT()
{
    Log("LT");
    CPU::MemType s1 = GetNextOperand();
    CPU::MemType s2 = GetNextOperand();

    CPU::MemType d = (s1 < s2) ? 1 : 0;
    SetNextOperand(d);
}

void CPU::EQ()
{
    Log("EQ");
    CPU::MemType s1 = GetNextOperand();
    CPU::MemType s2 = GetNextOperand();

    CPU::MemType d = (s1 == s2) ? 1 : 0;
    SetNextOperand(d);
}

void CPU::ADB()
{
    Log("ADB");
    CPU::MemType s1 = GetNextOperand();
    m_base += s1;
}

void CPU::END()
{
    Log("END");
}
