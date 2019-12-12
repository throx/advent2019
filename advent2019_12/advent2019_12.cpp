#include "pch.h"

#include "Point.h"
#include "MovingPoint.h"

using namespace std;

typedef MovingPoint<short, 3> Moon;
typedef MovingPoint<short, 1> Moon1;
typedef vector<Moon> Moons;
typedef vector<Moon1> Moons1;
constexpr bool DUMP(false);
constexpr int PART(2);

__int64 gcd(__int64 a, __int64 b)
{
    for (;;)
    {
        if (a == 0) return b;
        b %= a;
        if (b == 0) return a;
        a %= b;
    }
}

__int64 lcm(__int64 a, __int64 b)
{
    int temp = gcd(a, b);

    return temp ? (a / temp * b) : 0;
}


void Dump(const Moons& moons)
{
    for (const auto& m : moons) {
        cout << m << endl;
    }
    cout << "Energy: " << accumulate(moons.begin(), moons.end(), 0, [](auto t, auto x) { return t + x.Energy(); }) << endl;
}

int main()
{
    Moons moons;

    ifstream f("Data.txt");
    while (!f.eof()) {
        Moon::PointT p;
        f >> p;
        if (!f.fail()) {
            moons.push_back(Moon(p));
        }
    }

    if (DUMP) {
        cout << "Iteration " << 0 << endl;
        Dump(moons);
    }

    Moons start_moons = moons;

    if (PART == 1) {
        for (int iteration = 1; iteration <= 1000; ++iteration) {

            // Iterate all pairs
            for (auto& m1 : moons) {
                for (auto& m2 : moons) {
                    if (m1.Is(m2)) continue;

                    m1.Gravitate(m2);
                }
            }

            // Move moons
            for (auto& m : moons) {
                m.Move();
            }

            // Dump results
            if (DUMP) {
                cout << "Iteration " << iteration << endl;
                Dump(moons);
            }

        }
        Dump(moons);
    }
    else {
        // Part 2
        __int64 total = 1;

        for (int dim = 0; dim < 3; ++dim) {
            Moons1 moons1;
            for (const auto& m : moons) {
                moons1.push_back(m.Slice(dim));
            }
            Moons1 start_moons1 = moons1;
            
            __int64 iteration = 0;
            
            while (true) {

                ++iteration;

                // Iterate all pairs
                for (auto& m1 : moons1) {
                    for (auto& m2 : moons1) {
                        if (m1.Is(m2)) continue;

                        m1.Gravitate(m2);
                    }
                }

                // Move moons
                for (auto& m : moons1) {
                    m.Move();
                }

                bool stopped = true;
                for (const auto& m : moons1) {
                    if (!m.Stopped()) {
                        stopped = false;
                        break;
                    }
                }
                if (stopped) {
                    if (moons1 == start_moons1) {
                        cout << "Dimension " << dim << " Iterations " << iteration << endl;
                        total = lcm(total, iteration);
                        break;
                    }
                }
            }
        }

        cout << "Total = " << total << endl;
    }
}
