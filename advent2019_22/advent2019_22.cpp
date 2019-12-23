// advent2019_22.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include <boost/multiprecision/cpp_int.hpp>

using namespace std;

// C function for extended Euclidean Algorithm 
__int64 gcdExtended(__int64 a, __int64 b, __int64* x, __int64* y);

// Function to find modulo inverse of a 
__int64 modInverse(__int64 a, __int64 m)
{
    __int64 x, y;
    __int64 g = gcdExtended(a, m, &x, &y);
    if (g != 1)
        throw exception("Inverse doesn't exist");
    else
    {
        // m is added to handle negative x 
        return (x % m + m) % m;
    }
}

// C function for extended Euclidean Algorithm 
__int64 gcdExtended(__int64 a, __int64 b, __int64* x, __int64* y)
{
    // Base Case 
    if (a == 0)
    {
        *x = 0, * y = 1;
        return b;
    }

    __int64 x1, y1; // To store results of recursive call 
    __int64 gcd = gcdExtended(b % a, a, &x1, &y1);

    // Update x and y using results of recursive 
    // call 
    *x = y1 - (b / a) * x1;
    *y = x1;

    return gcd;
}




typedef vector<string> Commands;

struct Linear {
    __int64 mult;
    __int64 add;
    Linear() : mult(1), add(0) {}
    Linear(__int64 m, __int64 a) : mult(m), add(a) {}
};

void NewStack(Linear& linear, __int64 m)
{
    linear.mult = -linear.mult + m;
    linear.add = -linear.add - 1;
    while (linear.add < 0) linear.add += m;
}

void Cut(Linear& linear, __int64 n, __int64 m)
{
    linear.add = linear.add - n;
    if (linear.add >= m) linear.add -= m;
    if (linear.add < 0) linear.add += m;
}

__int64 MultMod(__int64 a, __int64 b, __int64 m)
{
    boost::multiprecision::int128_t a128(a);
    boost::multiprecision::int128_t b128(b);
    boost::multiprecision::int128_t r = a128 * b128;
    r %= m;
    return static_cast<__int64>(r);
}

void DealIncr(Linear& linear, __int64 n, __int64 m)
{
    linear.mult = MultMod(linear.mult, n, m);
    linear.add = MultMod(linear.add, n, m);
}

__int64 NewPos(Linear& linear, __int64 p, __int64 m)
{
    p = MultMod(linear.mult, p, m);
    p += linear.add;
    if (p >= m) p -= m;
    if (p < 0) p += m;
    return p;
}

Linear Invert(const Linear& linear, __int64 m)
{
    __int64 d = modInverse(linear.mult, m);
    Linear r( d, -MultMod(d, linear.add, m) );
    if (r.add < 0) r.add += m;
    return r;
}

__int64 Power(__int64 b, __int64 e, __int64 m)
{
    if (e == 0) return 1;
    if (e == 1) return b % m;

    __int64 t = Power(b, e / 2, m);
    t = MultMod(t, t, m);

    if (e % 2 == 0) return t;
    else return MultMod(b, t, m);
}

Linear Repeat(const Linear& linear, __int64 rpt, __int64 m)
{
    // A^n*x + (A^n-1) / (A-1) * B, from https://www.reddit.com/r/adventofcode/comments/ee0rqi/2019_day_22_solutions/fbnifwk/
    // Yes, I had to look up the solution.  So annoyed because I was so close and should
    // have been able to figure this out.
    __int64 mult = Power(linear.mult, rpt, m);
    __int64 add = mult - 1;
    add = MultMod(add, modInverse(linear.mult - 1, m), m);
    add = MultMod(add, linear.add, m);
    Linear r(mult, add);
    return r;
}

Linear Convert(const Commands& commands, __int64 m)
{
    Linear lin;
    for (auto c : commands) {
        if (c == "deal into new stack"s) NewStack(lin, m);
        else if (c.substr(0, 3) == "cut"s) {
            int n = atoi(c.substr(4).c_str());
            Cut(lin, n, m);
        }
        else if (c.substr(0, 19) == "deal with increment"s) {
            int n = atoi(c.substr(20).c_str());
            DealIncr(lin, n, m);
        }
    }

    return lin;
}

__int64 Part1(const Commands& commands, __int64 pos, __int64 m)
{
    Linear lin = Convert(commands, m);
    pos = NewPos(lin, pos, m);
    return pos;
}

__int64 Part2(const Commands& commands, __int64 pos, __int64 m, __int64 rpt)
{
    Linear lin = Convert(commands, m);
    lin = Invert(lin, m);
    lin = Repeat(lin, rpt, m);
    pos = NewPos(lin, pos, m);
    return pos;
}

int main()
{
    Commands commands;
    ifstream f("Data.txt");
    while (!f.eof()) {
        string s;
        getline(f, s);
        commands.push_back(s);
    }

    __int64 pos;

    pos = Part1(commands, 2019, 10007);
    cout << "Part 1: " << pos << endl;

    pos = Part2(commands, pos, 10007, 1);
    cout << "Reverse Check: " << pos << endl  << endl;

    pos = Part1(commands, 2019, 10007);
    pos = Part1(commands, pos, 10007);
    pos = Part1(commands, pos, 10007);
    pos = Part1(commands, pos, 10007);
    pos = Part1(commands, pos, 10007);
    cout << "Part 1*5: " << pos << endl;

    pos = Part2(commands, pos, 10007, 5);
    cout << "Reverse Check: " << pos << endl;

    pos = Part2(commands, 2020, 119315717514047i64, 101741582076661i64);
    cout << "Part 2: " << pos << endl;
}
