#include "pch.h"
#include "CPU.h"
#include "Console.h"
#include "Point.h"

using namespace std;
using namespace JadedHoboConsole;

typedef Point<int, 2> Point2;
typedef vector<string> Map;

int g_xx = 0;
int g_yy = 0;

void Load(CPU::Ram& ram)
{
    stringstream data("109,424,203,1,21102,11,1,0,1105,1,282,21102,1,18,0,1106,0,259,2101,0,1,221,203,1,21102,1,31,0,1106,0,282,21102,38,1,0,1105,1,259,20101,0,23,2,22101,0,1,3,21101,1,0,1,21101,57,0,0,1105,1,303,2101,0,1,222,21001,221,0,3,21002,221,1,2,21101,0,259,1,21102,80,1,0,1106,0,225,21102,89,1,2,21102,91,1,0,1105,1,303,2101,0,1,223,20101,0,222,4,21101,0,259,3,21102,1,225,2,21102,225,1,1,21102,118,1,0,1106,0,225,20101,0,222,3,21101,136,0,2,21101,133,0,0,1106,0,303,21202,1,-1,1,22001,223,1,1,21101,148,0,0,1105,1,259,1202,1,1,223,20102,1,221,4,21001,222,0,3,21102,18,1,2,1001,132,-2,224,1002,224,2,224,1001,224,3,224,1002,132,-1,132,1,224,132,224,21001,224,1,1,21102,195,1,0,106,0,108,20207,1,223,2,20102,1,23,1,21101,-1,0,3,21101,214,0,0,1105,1,303,22101,1,1,1,204,1,99,0,0,0,0,109,5,1202,-4,1,249,21201,-3,0,1,22102,1,-2,2,21202,-1,1,3,21102,1,250,0,1105,1,225,21201,1,0,-4,109,-5,2105,1,0,109,3,22107,0,-2,-1,21202,-1,2,-1,21201,-1,-1,-1,22202,-1,-2,-2,109,-3,2105,1,0,109,3,21207,-2,0,-1,1206,-1,294,104,0,99,22102,1,-2,-2,109,-3,2105,1,0,109,5,22207,-3,-4,-1,1206,-1,346,22201,-4,-3,-4,21202,-3,-1,-1,22201,-4,-1,2,21202,2,-1,-1,22201,-4,-1,1,21201,-2,0,3,21102,343,1,0,1106,0,303,1105,1,415,22207,-2,-3,-1,1206,-1,387,22201,-3,-2,-3,21202,-2,-1,-1,22201,-3,-1,3,21202,3,-1,-1,22201,-3,-1,2,21202,-4,1,1,21102,384,1,0,1105,1,303,1106,0,415,21202,-4,-1,-4,22201,-4,-3,-4,22202,-3,-2,-2,22202,-2,-4,-4,22202,-3,-2,-3,21202,-4,-1,-2,22201,-3,-2,1,21202,1,1,-4,109,-5,2106,0,0");

    ram.clear();
    ram.reserve(data.str().length());
    while (!data.eof())
    {
        CPU::MemType x;
        data >> x;
        if (!data.eof()) {
            char comma;
            data >> comma;
        }
        ram.push_back(x);
    }
}

void Print(char val)
{
    cout << val;
}

void Dump(const Map& map)
{
    for (auto x : map) {
        cout << x << endl;
    }
}

bool IsIntersection(const Map& map, int x, int y)
{
    if (x == 0 || x == g_xx - 1 || y == 0 || y == g_yy - 1) return false;
    return (map[y][x] == '#' &&
        map[y - 1][x] == '#' &&
        map[y + 1][x] == '#' &&
        map[y][x - 1] == '#' &&
        map[y][x + 1] == '#');
}

Point2 TurnLeft(const Point2& p)
{
    return Point2{ p[1], -p[0] };
}

Point2 TurnRight(const Point2& p)
{
    return Point2{ -p[1], p[0] };
}

char IsSafe(const Map& m, const Point2& p)
{
    if (p[0] < 0 || p[0] >= g_xx || p[1] < 0 || p[1] >= g_yy) return false;
    return m[p[1]][p[0]] == '#';
}

bool IsPulled(int x, int y)
{
    bool result;
    int c = 0;

    CPU cpu;
    //cpu.SetLogger([](const string& s) { cout << "  LOG: " << s << endl; });
    cpu.SetIO(
        [&]() {
            ++c;
            if (c == 1) return x;
            if (c == 2) return y;
            return 0;
        },
        [&](CPU::MemType val) {
            result = (val > 0);
        });
    Load(cpu.GetRam());
    while (cpu.ExecuteNext())
        ;

    return result;
}

int main()
{
    Map map;
    map.push_back(string());

    SMALL_RECT winsize = { 0, 0, 100, 60 };
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &winsize);

    cout << clr;
    int part = 2;

    if (part == 1) {
        int sum = 0;
        vector<vector<bool> > m;
        for (int y = 0; y < 50; ++y) {
            m.push_back(vector<bool>(50));
            for (int x = 0; x < 50; ++x) {
                bool b = IsPulled(x, y);
                m.back()[x] = b;
                cout << (char)(b ? '#' : '.');
                if (b) ++sum;
            }
            cout << endl;
        }
        cout << "Sum: " << sum << endl;
    }
    if (part == 2) {
        // Start in a known spot
        int x = 100;
        int y = 0;

        // Find the edge
        while (!IsPulled(x, y)) ++y;

        // Now we can walk the edge until the opposite corner fits (x-100, y+100)
        while (!IsPulled (x - 99, y + 99)) {
            ++x;
            while (!IsPulled(x, y)) ++y;
        }

        // Belt and braces
        for (int i = 0; i < 100; ++i) {
            if (!IsPulled(x - i, y)) cout << "boom1";
            if (!IsPulled(x - i, y + 99)) cout << "boom2";
            if (!IsPulled(x, y + i)) cout << "boom3";
            if (!IsPulled(x - 99, y + i)) cout << "boom4";
        }
        cout << "X: " << x - 99 << "Y: " << y;
    }
}
