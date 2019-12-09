#pragma once
class CPU
{
public:
    typedef __int64 MemType;
    typedef std::vector<MemType> Ram;

    // Initialise the CPU using internal private RAM, with standard input/output
    CPU();

    // Initialise the CPU using specified RAM
    CPU(Ram& ram);

    // Initialise the CPU using specified RAM and special IO
    CPU(Ram& ram,
        std::function<MemType()> get_input,
        std::function<void(MemType)> set_input);

    // Bye bye
    ~CPU();

    // Executes next instruction, returning 'false' if it was END (99)
    bool ExecuteNext();

    // Set the IO functions using callbacks
    void SetIO(std::function<MemType()> get_input, std::function<void(MemType)> set_input)
    {
        m_get_input = get_input;
        m_set_input = set_input;
    }

    // Return the RAM being used by this CPU
    Ram& GetRam() { return m_ram; }

    // Set the logging function (the defult logger does nothing)
    void SetLogger(std::function<void(const std::string&)> logger) { m_logger = logger; }

protected:
    void ADD();
    void MUL();
    void INP();
    void OUT();
    void JT();
    void JF();
    void LT();
    void EQ();
    void ADB();

    void END();

    MemType GetNextOperand();
    void SetNextOperand(MemType val);
    void Log(const std::string& s);

private:
    void Ensure(MemType s);
    Ram& m_ram;
    Ram m_private_ram;
    MemType m_pc;
    MemType m_base;

    typedef void(CPU::*InstrFn)();
    std::map<MemType, InstrFn> m_dispatch;

    int m_modes;
    std::function<MemType()> m_get_input;
    std::function<void(MemType)> m_set_input;
    std::function<void(const std::string&)> m_logger;
};

