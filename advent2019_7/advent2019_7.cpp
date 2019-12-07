// advent2019_7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "CPU.h"
using namespace std;

void Load(vector<int>& ram)
{
    stringstream data("3,8,1001,8,10,8,105,1,0,0,21,38,63,76,89,106,187,268,349,430,99999,3,9,1001,9,5,9,102,3,9,9,1001,9,2,9,4,9,99,3,9,101,4,9,9,102,3,9,9,101,4,9,9,1002,9,3,9,101,2,9,9,4,9,99,3,9,101,5,9,9,1002,9,4,9,4,9,99,3,9,101,2,9,9,1002,9,5,9,4,9,99,3,9,1001,9,5,9,1002,9,5,9,1001,9,5,9,4,9,99,3,9,1001,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,1,9,4,9,3,9,1002,9,2,9,4,9,3,9,101,1,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,101,1,9,9,4,9,3,9,102,2,9,9,4,9,3,9,1002,9,2,9,4,9,99,3,9,101,2,9,9,4,9,3,9,101,2,9,9,4,9,3,9,101,2,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,101,2,9,9,4,9,99,3,9,1002,9,2,9,4,9,3,9,1001,9,2,9,4,9,3,9,101,2,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,1,9,4,9,3,9,1002,9,2,9,4,9,3,9,101,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,102,2,9,9,4,9,99,3,9,102,2,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,101,1,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,2,9,4,9,3,9,1001,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,101,1,9,9,4,9,3,9,101,1,9,9,4,9,3,9,101,2,9,9,4,9,99,3,9,1002,9,2,9,4,9,3,9,101,1,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,1,9,4,9,3,9,101,1,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,101,1,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,102,2,9,9,4,9,99");
    //stringstream data("3, 15, 3, 16, 1002, 16, 10, 16, 1, 16, 15, 15, 4, 15, 99, 0, 0");
    //stringstream data("3,23,3,24,1002,24,10,24,1002,23,-1,23, 101, 5, 23, 23, 1, 24, 23, 23, 4, 23, 99, 0, 0");
    //stringstream data("3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,1002, 33, 7, 33, 1, 33, 31, 31, 1, 32, 31, 31, 4, 31, 99, 0, 0, 0");
    //stringstream data("3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26, 27, 4, 27, 1001, 28, -1, 28, 1005, 28, 6, 99, 0, 0, 5");
    //stringstream data("3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,55,26,1001,54, -5, 54, 1105, 1, 12, 1, 53, 54, 53, 1008, 54, 0, 55, 1001, 55, 1, 55, 2, 53, 55, 53, 4, 53, 1001, 56, -1, 56, 1005, 56, 6, 99, 0, 0, 0, 0, 10");

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
    int max_output = 0;

    do {
        // Initialise the CPU arrays.  Each CPU has its own RAM, and its own input queue
        // (needed individual input queues to push the first phase vector in).  Output
        // is shared in a single integer.
        CPU cpu[5];
        list<int> inputs[5];
        int output = 0;
        bool got_output;

        // Individual CPU initialisation.
        for (int i = 0; i < 5; ++i)
        {
            // INP function retrieves next from the input queue.
            // OUT function stores result in the integer and flags that we got output
            auto getval_f = [&, i]() { int x = inputs[i].front(); inputs[i].pop_front(); return x; };
            auto setval_f = [&](int x) { output = x; got_output = true; };
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
        int output4 = 0;
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
