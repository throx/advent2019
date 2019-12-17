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
    stringstream data("1,330,331,332,109,2952,1101,1182,0,16,1101,1467,0,24,102,1,0,570,1006,570,36,1002,571,1,0,1001,570,-1,570,1001,24,1,24,1106,0,18,1008,571,0,571,1001,16,1,16,1008,16,1467,570,1006,570,14,21101,0,58,0,1105,1,786,1006,332,62,99,21102,1,333,1,21101,73,0,0,1106,0,579,1101,0,0,572,1101,0,0,573,3,574,101,1,573,573,1007,574,65,570,1005,570,151,107,67,574,570,1005,570,151,1001,574,-64,574,1002,574,-1,574,1001,572,1,572,1007,572,11,570,1006,570,165,101,1182,572,127,101,0,574,0,3,574,101,1,573,573,1008,574,10,570,1005,570,189,1008,574,44,570,1006,570,158,1105,1,81,21101,340,0,1,1105,1,177,21101,477,0,1,1106,0,177,21101,514,0,1,21101,0,176,0,1105,1,579,99,21102,1,184,0,1106,0,579,4,574,104,10,99,1007,573,22,570,1006,570,165,101,0,572,1182,21101,375,0,1,21101,211,0,0,1106,0,579,21101,1182,11,1,21101,222,0,0,1105,1,979,21101,0,388,1,21102,233,1,0,1106,0,579,21101,1182,22,1,21101,0,244,0,1106,0,979,21101,401,0,1,21102,1,255,0,1106,0,579,21101,1182,33,1,21101,266,0,0,1105,1,979,21101,414,0,1,21102,1,277,0,1106,0,579,3,575,1008,575,89,570,1008,575,121,575,1,575,570,575,3,574,1008,574,10,570,1006,570,291,104,10,21101,1182,0,1,21101,313,0,0,1105,1,622,1005,575,327,1101,0,1,575,21102,327,1,0,1106,0,786,4,438,99,0,1,1,6,77,97,105,110,58,10,33,10,69,120,112,101,99,116,101,100,32,102,117,110,99,116,105,111,110,32,110,97,109,101,32,98,117,116,32,103,111,116,58,32,0,12,70,117,110,99,116,105,111,110,32,65,58,10,12,70,117,110,99,116,105,111,110,32,66,58,10,12,70,117,110,99,116,105,111,110,32,67,58,10,23,67,111,110,116,105,110,117,111,117,115,32,118,105,100,101,111,32,102,101,101,100,63,10,0,37,10,69,120,112,101,99,116,101,100,32,82,44,32,76,44,32,111,114,32,100,105,115,116,97,110,99,101,32,98,117,116,32,103,111,116,58,32,36,10,69,120,112,101,99,116,101,100,32,99,111,109,109,97,32,111,114,32,110,101,119,108,105,110,101,32,98,117,116,32,103,111,116,58,32,43,10,68,101,102,105,110,105,116,105,111,110,115,32,109,97,121,32,98,101,32,97,116,32,109,111,115,116,32,50,48,32,99,104,97,114,97,99,116,101,114,115,33,10,94,62,118,60,0,1,0,-1,-1,0,1,0,0,0,0,0,0,1,20,14,0,109,4,2102,1,-3,586,21001,0,0,-1,22101,1,-3,-3,21102,1,0,-2,2208,-2,-1,570,1005,570,617,2201,-3,-2,609,4,0,21201,-2,1,-2,1105,1,597,109,-4,2105,1,0,109,5,2101,0,-4,629,21001,0,0,-2,22101,1,-4,-4,21101,0,0,-3,2208,-3,-2,570,1005,570,781,2201,-4,-3,652,21002,0,1,-1,1208,-1,-4,570,1005,570,709,1208,-1,-5,570,1005,570,734,1207,-1,0,570,1005,570,759,1206,-1,774,1001,578,562,684,1,0,576,576,1001,578,566,692,1,0,577,577,21101,702,0,0,1105,1,786,21201,-1,-1,-1,1106,0,676,1001,578,1,578,1008,578,4,570,1006,570,724,1001,578,-4,578,21101,0,731,0,1105,1,786,1106,0,774,1001,578,-1,578,1008,578,-1,570,1006,570,749,1001,578,4,578,21102,1,756,0,1105,1,786,1105,1,774,21202,-1,-11,1,22101,1182,1,1,21101,0,774,0,1105,1,622,21201,-3,1,-3,1106,0,640,109,-5,2106,0,0,109,7,1005,575,802,20102,1,576,-6,20101,0,577,-5,1106,0,814,21101,0,0,-1,21102,1,0,-5,21101,0,0,-6,20208,-6,576,-2,208,-5,577,570,22002,570,-2,-2,21202,-5,45,-3,22201,-6,-3,-3,22101,1467,-3,-3,1202,-3,1,843,1005,0,863,21202,-2,42,-4,22101,46,-4,-4,1206,-2,924,21102,1,1,-1,1106,0,924,1205,-2,873,21101,0,35,-4,1105,1,924,1201,-3,0,878,1008,0,1,570,1006,570,916,1001,374,1,374,1201,-3,0,895,1102,1,2,0,1201,-3,0,902,1001,438,0,438,2202,-6,-5,570,1,570,374,570,1,570,438,438,1001,578,558,921,21001,0,0,-4,1006,575,959,204,-4,22101,1,-6,-6,1208,-6,45,570,1006,570,814,104,10,22101,1,-5,-5,1208,-5,33,570,1006,570,810,104,10,1206,-1,974,99,1206,-1,974,1101,0,1,575,21101,973,0,0,1106,0,786,99,109,-7,2105,1,0,109,6,21101,0,0,-4,21102,1,0,-3,203,-2,22101,1,-3,-3,21208,-2,82,-1,1205,-1,1030,21208,-2,76,-1,1205,-1,1037,21207,-2,48,-1,1205,-1,1124,22107,57,-2,-1,1205,-1,1124,21201,-2,-48,-2,1106,0,1041,21102,-4,1,-2,1106,0,1041,21102,1,-5,-2,21201,-4,1,-4,21207,-4,11,-1,1206,-1,1138,2201,-5,-4,1059,1202,-2,1,0,203,-2,22101,1,-3,-3,21207,-2,48,-1,1205,-1,1107,22107,57,-2,-1,1205,-1,1107,21201,-2,-48,-2,2201,-5,-4,1090,20102,10,0,-1,22201,-2,-1,-2,2201,-5,-4,1103,1201,-2,0,0,1106,0,1060,21208,-2,10,-1,1205,-1,1162,21208,-2,44,-1,1206,-1,1131,1105,1,989,21101,0,439,1,1105,1,1150,21102,477,1,1,1106,0,1150,21102,1,514,1,21102,1149,1,0,1106,0,579,99,21102,1157,1,0,1105,1,579,204,-2,104,10,99,21207,-3,22,-1,1206,-1,1138,1202,-5,1,1176,2102,1,-4,0,109,-6,2105,1,0,10,11,34,1,9,1,34,1,9,1,7,9,18,1,9,1,7,1,7,1,18,1,9,1,7,1,7,1,18,1,9,1,7,1,7,1,18,1,9,1,7,1,7,1,18,1,9,1,7,1,7,1,18,9,1,13,3,1,26,1,9,1,3,1,3,1,20,11,5,1,1,9,18,1,5,1,3,1,5,1,1,1,1,1,3,1,1,1,18,1,5,1,3,1,5,1,1,1,1,1,3,1,1,1,18,1,5,1,3,1,5,1,1,1,1,1,3,1,1,1,18,1,5,1,1,9,1,1,1,1,3,9,12,1,5,1,3,1,7,1,1,1,5,1,5,1,10,9,3,1,1,9,5,1,5,1,10,1,1,1,9,1,1,1,5,1,7,1,5,14,9,9,7,1,5,2,9,1,13,1,13,1,5,2,7,9,7,1,13,1,5,2,7,1,1,1,5,1,7,1,19,2,7,1,1,1,5,1,7,1,19,2,7,1,1,1,5,1,7,1,19,2,7,1,1,13,1,1,7,14,7,1,7,1,5,1,1,1,7,1,12,1,7,1,7,1,5,1,1,1,7,1,12,1,7,1,7,1,5,1,1,1,7,1,12,9,7,9,7,1,34,1,9,1,34,1,9,1,34,1,9,1,34,11,12");

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

int main()
{
    Map map;
    map.push_back(string());

    SMALL_RECT winsize = { 0, 0, 100, 50 };
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &winsize);

    cout << clr;

    // Load the program
    CPU cpu;
    //cpu.SetLogger([](const string& s) { cout << "  LOG: " << s << endl; });
    cpu.SetIO(
        [&]() { return 0; },
        [&](CPU::MemType val) {
            if (val == 10) {
                map.push_back(string());
            }
            else {
                map.back().push_back((char)val);
            }
        });
    Load(cpu.GetRam());
    while (cpu.ExecuteNext())
        ;

    while (map.back().empty()) map.pop_back();
    g_xx = map.front().length();
    g_yy = map.size();

    Point2 robot;
    int sum = 0;
    for (int y = 0; y < g_yy; ++y) {
        for (int x = 0; x < g_xx; ++x) {
            if (IsIntersection(map, x, y)) {
                sum += (x*y);
            }
            if (map[y][x] == '^') robot = Point2 { x, y };
        }
    }

    Dump(map);

    cout << "Sum: " << sum << endl;

    Point2 robotdir = { 0, -1 };
    bool end = false;
    while (!end) {
        if (IsSafe(map, robot + robotdir)) {
            int t = 0;
            while (IsSafe(map, robot + robotdir)) {
                ++t;
                robot += robotdir;
            }
            cout << t << ',';
        }
        else if (IsSafe(map, robot + TurnLeft(robotdir))) {
            robotdir = TurnLeft(robotdir);
            cout << "L,";
        }
        else if (IsSafe(map, robot + TurnRight(robotdir))) {
            robotdir = TurnRight(robotdir);
            cout << "R,";
        }
        else {
            end = true;
        }
    }
    cout << endl << endl;

    // Part 2
    Map data;
    data.push_back("A,B,B,A,C,A,A,C,B,C"s);
    data.push_back("R,8,L,12,R,8"s);
    data.push_back("R,12,L,8,R,10"s);
    data.push_back("R,8,L,8,L,8,R,8,R,10"s);
    data.push_back("Y"s);

    int row = 0;
    int col = 0;
    bool newline = false;

    cpu.SetIO(
        [&]() {
            if (col == data[row].length()) {
                ++row;
                col = 0;
                cout << endl;
                return (CPU::MemType)10;
            }
            char c = data[row][col];
            ++col;
            cout << fg_white << c;
            return (CPU::MemType)c;
        },
        [&](CPU::MemType val) {
            if (val == 10) {
                if (!newline) {
                    newline = true;
                    cout << endl;
                }
                else {
                    newline = false;
                    gotoxy(0, 0);
                }
            }
            else if (val > 255) {
                newline = false;
                cout << fg_cyan << "Dust: " << val;
            }             
            else {
                newline = false;
                cout << fg_green << (char)val;
            }
        });
    cpu.GetRam()[0] = 2;
    while (cpu.ExecuteNext())
        ;

    return 0;
}
