#include "pch.h"
#include "CPU.h"
using namespace std;

enum class Direction {UP, RIGHT, DOWN, LEFT};

struct Point
{
    int x;
    int y;

    Point(int x, int y) : x(x), y(y) {}
    bool operator==(const Point& other) const { return (x == other.x) && (y == other.y); }
    bool operator<(const Point& other) const { return (x == other.x) ? y < other.y : x < other.x; }
};

typedef map<Point, CPU::MemType> Paint;

void Load(CPU::Ram& ram)
{
    stringstream data("3,8,1005,8,310,1106,0,11,0,0,0,104,1,104,0,3,8,1002,8,-1,10,101,1,10,10,4,10,1008,8,0,10,4,10,1001,8,0,29,1,2,11,10,1,1101,2,10,2,1008,18,10,2,106,3,10,3,8,1002,8,-1,10,1001,10,1,10,4,10,1008,8,1,10,4,10,102,1,8,67,2,105,15,10,3,8,1002,8,-1,10,101,1,10,10,4,10,1008,8,0,10,4,10,1001,8,0,93,2,1001,16,10,3,8,102,-1,8,10,1001,10,1,10,4,10,1008,8,1,10,4,10,102,1,8,119,3,8,1002,8,-1,10,1001,10,1,10,4,10,1008,8,1,10,4,10,101,0,8,141,2,7,17,10,1,1103,16,10,3,8,1002,8,-1,10,101,1,10,10,4,10,108,0,8,10,4,10,102,1,8,170,3,8,1002,8,-1,10,1001,10,1,10,4,10,1008,8,1,10,4,10,1002,8,1,193,1,7,15,10,2,105,13,10,1006,0,92,1006,0,99,3,8,1002,8,-1,10,101,1,10,10,4,10,108,1,8,10,4,10,101,0,8,228,1,3,11,10,1006,0,14,1006,0,71,3,8,1002,8,-1,10,101,1,10,10,4,10,1008,8,0,10,4,10,101,0,8,261,2,2,2,10,1006,0,4,3,8,102,-1,8,10,101,1,10,10,4,10,108,0,8,10,4,10,101,0,8,289,101,1,9,9,1007,9,1049,10,1005,10,15,99,109,632,104,0,104,1,21101,0,387240009756,1,21101,327,0,0,1105,1,431,21101,0,387239486208,1,21102,1,338,0,1106,0,431,3,10,104,0,104,1,3,10,104,0,104,0,3,10,104,0,104,1,3,10,104,0,104,1,3,10,104,0,104,0,3,10,104,0,104,1,21102,3224472579,1,1,21101,0,385,0,1106,0,431,21101,0,206253952003,1,21102,396,1,0,1105,1,431,3,10,104,0,104,0,3,10,104,0,104,0,21102,709052072296,1,1,21102,419,1,0,1105,1,431,21102,1,709051962212,1,21102,430,1,0,1106,0,431,99,109,2,21202,-1,1,1,21102,1,40,2,21102,462,1,3,21102,452,1,0,1105,1,495,109,-2,2105,1,0,0,1,0,0,1,109,2,3,10,204,-1,1001,457,458,473,4,0,1001,457,1,457,108,4,457,10,1006,10,489,1101,0,0,457,109,-2,2105,1,0,0,109,4,2102,1,-1,494,1207,-3,0,10,1006,10,512,21101,0,0,-3,22101,0,-3,1,21202,-2,1,2,21102,1,1,3,21101,531,0,0,1105,1,536,109,-4,2106,0,0,109,5,1207,-3,1,10,1006,10,559,2207,-4,-2,10,1006,10,559,21202,-4,1,-4,1105,1,627,22102,1,-4,1,21201,-3,-1,2,21202,-2,2,3,21102,1,578,0,1105,1,536,21202,1,1,-4,21102,1,1,-1,2207,-4,-2,10,1006,10,597,21101,0,0,-1,22202,-2,-1,-2,2107,0,-3,10,1006,10,619,21201,-1,0,1,21102,1,619,0,106,0,494,21202,-2,-1,-2,22201,-4,-2,-4,109,-5,2106,0,0");

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

void Dump(CPU::Ram& ram)
{
    for (int i = 0; i < ram.size(); ++i)
    {
        cout << setw(5) << i << ": " << setw(5) << ram[i] << ", ";
        if (i % 10 == 9) cout << endl;
    }
    cout << endl;
}

CPU::MemType GetPaint(const Paint& paint, const Point& pos)
{
    auto it = paint.find(pos);
    if (it == paint.end()) return 0;  // Unpainted black
    return it->second;
}

void DoPaint(Paint& paint, const Point& pos, CPU::MemType val)
{
    paint[pos] = val;
}

void TurnAndMove(Point& pos, Direction& dir, CPU::MemType turn)
{
    if (turn == 0) {
        switch (dir) {
        case Direction::UP:
            dir = Direction::LEFT;
            break;
        case Direction::RIGHT:
            dir = Direction::UP;
            break;
        case Direction::DOWN:
            dir = Direction::RIGHT;
            break;
        case Direction::LEFT:
            dir = Direction::DOWN;
            break;
        }
    }
    else {
        switch (dir) {
        case Direction::UP:
            dir = Direction::RIGHT;
            break;
        case Direction::RIGHT:
            dir = Direction::DOWN;
            break;
        case Direction::DOWN:
            dir = Direction::LEFT;
            break;
        case Direction::LEFT:
            dir = Direction::UP;
            break;
        }
    }

    switch (dir) {
    case Direction::UP:
        --pos.y;
        break;
    case Direction::RIGHT:
        ++pos.x;
        break;
    case Direction::DOWN:
        ++pos.y;
        break;
    case Direction::LEFT:
        --pos.x;
        break;
    }
}

void Dump(const Paint& paint, const Point& pos, const Direction& dir)
{
    for (int y = -5; y < 10; ++y) {
        for (int x = -9; x < 50; ++x) {
            CPU::MemType c = 0;
            auto it = paint.find(Point(x, y));
            if (it != paint.end()) c = it->second;

            if (x == pos.x && y == pos.y) {
                switch (dir) {
                case Direction::UP:
                    cout << '^';
                    break;
                case Direction::RIGHT:
                    cout << '>';
                    break;
                case Direction::DOWN:
                    cout << 'v';
                    break;
                case Direction::LEFT:
                    cout << '<';
                    break;
                }
            }
            else {
                cout << ((c == 0) ? '.' : (char)219);
            }
        }
        cout << endl;
    }
    cout << endl;
}

int main()
{
    Paint paint;
    Point pos(0, 0);
    paint[pos] = 1;
    Direction dir = Direction::UP;
    bool doTurn = false;

    // Load the program
    CPU cpu;
    //cpu.SetLogger([](const string& s) { cout << "  LOG: " << s << endl; });
    cpu.SetIO(
        [&]() { return GetPaint(paint, pos); }, 
        [&](CPU::MemType val) { 
            if (doTurn) {
                TurnAndMove(pos, dir, val);
                //Dump(paint, pos, dir);
            }
            else {
                DoPaint(paint, pos, val);
            }
            doTurn = !doTurn;
        });
    Load(cpu.GetRam());
    while (cpu.ExecuteNext())
        ;

    Dump(paint, pos, dir);
    cout << "Total Painted = " << paint.size() << endl;
    return 0;
}
