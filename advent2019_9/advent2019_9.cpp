#include "pch.h"
#include "CPU.h"
using namespace std;

void Load(CPU::Ram& ram)
{
    stringstream data("");

    ram.clear();
    ram.reserve(data.str().length());
    while (!data.eof())
    {
        int x;
        data >> x;
        if (!data.eof()) {
            char comma;
            data >> comma;
        }
        ram.push_back(x);
    }
}

void Dump(const vector<int>& ram)
{
    for (int i = 0; i < ram.size(); ++i)
    {
        cout << setw(5) << i << ": " << setw(5) << ram[i] << ", ";
        if (i % 10 == 9) cout << endl;
    }
    cout << endl;
}

int main()
{
    // Phase vector
    //vector<int> phases = { 0,1,2,3,4 };    // <- Part 1
    vector<int> phases = { 5,6,7,8,9 };    // <- Part 2

    // Variables to store maximums found
    vector<int> max_phases = phases;
    CPU::MemType max_output = 0;

    do {
        // Initialise the CPU arrays.  Each CPU has its own RAM, and its own input queue
        // (needed individual input queues to push the first phase vector in).  Output
        // is shared in a single integer.
        CPU cpu[5];
        list<CPU::MemType> inputs[5];
        CPU::MemType output = 0;
        bool got_output;

        // Individual CPU initialisation.
        for (int i = 0; i < 5; ++i)
        {
            // INP function retrieves next from the input queue.
            // OUT function stores result in the integer and flags that we got output
            auto getval_f = [&, i]() { CPU::MemType x = inputs[i].front(); inputs[i].pop_front(); return x; };
            auto setval_f = [&](CPU::MemType x) { output = x; got_output = true; };
            cpu[i].SetIO(getval_f, setval_f);

            // Preload the input vectors with the phases
            inputs[i].push_back(phases[i]);

            // Load the program
            Load(cpu[i].GetRam());

            // Set up a logger, if we're into CPU debugging
            //cpu[i].SetLogger([i](const string& s) { cout << "  LOG " << i << ": " << s << endl; });
        }

        // Loop around the CPUs while they are all still running.  Break as soon as the first
        // one breaks.  We only want to remember the output from #4 as well.
        int curr_cpu = 0;
        bool running = true;
        CPU::MemType output4 = 0;
        while (running)
        {
            // Push the current output to the CPU
            inputs[curr_cpu].push_back(output);

            // Loop until we either get an output or the CPU program terminates
            got_output = false;
            while (!got_output && running)
            {
                running = cpu[curr_cpu].ExecuteNext();
            }

            // Remember the values from the last CPU, otherwise just move to next in
            // the feedback loop.
            if (curr_cpu == 4)
            {
                output4 = output;
                curr_cpu = 0;
            }
            else
            {
                ++curr_cpu;
            }
        }

        // Check for maximum
        if (output4 > max_output)
        {
            max_output = output4;
            max_phases = phases;
        }

        // STL has a magic "next permutation" function that just makes the whole thing stupid easy!
    } while (next_permutation(phases.begin(), phases.end()));

    // Show the results.
    for (auto phase : max_phases) cout << phase << ' ';
    cout << endl;
    cout << max_output << endl;

    return 0;
}
