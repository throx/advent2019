#include "pch.h"
#include "CPU.h"
#include "Console.h"
#include "Point.h"

using namespace std;
using namespace JadedHoboConsole;


void Load(CPU::Ram& ram)
{
    ifstream data("Data.txt");

    ram.clear();
    while (!data.eof())
    {
        CPU::MemType x;
        data >> x;
        if (!data.eof()) {
            char comma;
            data >> comma;
            if (comma != ',') throw "Not a comma";
        }
        ram.push_back(x);
    }
}

int main()
{
    CPU cpu;

    cpu.SetIO(
        [&]() {
            cout << fg_white;
            char c;
            cin.read(&c, 1);
            if (c == 13) c = 10;
            return (CPU::MemType) c;
        },
        [&](CPU::MemType val) {
            cout << fg_green << (char)val;
        });

    Load(cpu.GetRam());
    while (cpu.ExecuteNext())
        ;
    return 0;
}
