#pragma once
class CPU
{
public:
    // Initialise the CPU using internal private RAM, with standard input/output
    CPU();

    // Initialise the CPU using specified RAM
    CPU(std::vector<int>& ram);

    // Initialise the CPU using specified RAM and special IO
    CPU(std::vector<int>& ram,
        std::function<int()> get_input,
        std::function<void(int)> set_input);

    // Bye bye
    ~CPU();

    // Executes next instruction, returning 'false' if it was END (99)
    bool ExecuteNext();

    // Set the IO functions using callbacks
    void SetIO(std::function<int()> get_input, std::function<void(int)> set_input)
    {
        m_get_input = get_input;
        m_set_input = set_input;
    }

    // Return the RAM being used by this CPU
    std::vector<int>& GetRam() { return m_ram; }

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

    void END();

    int GetNextOperand();
    void SetNextOperand(int val);
    void Log(const std::string& s);

private:
    void Ensure(int s);
    std::vector<int>& m_ram;
    std::vector<int> m_private_ram;
    int m_pc;

    typedef void(CPU::*InstrFn)();
    std::map<int, InstrFn> m_dispatch;

    int m_modes;
    std::function<int()> m_get_input;
    std::function<void(int)> m_set_input;
    std::function<void(const std::string&)> m_logger;
};

