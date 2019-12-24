// advent2019_24.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <set>

#include "Point.h"

typedef Point<int, 2> Point2;
typedef Point<int, 3> Point3;

using namespace std;

typedef map<int, int> Map;

vector<Point2> DIRS = { Point2{-1,0}, Point2{1,0}, Point2{0,-1}, Point2{0,1} };
vector<Point3> DIRS3 = { Point3{-1,0,0}, Point3{1,0,0}, Point3{0,-1,0}, Point3{0,1,0} };
const Point2 MAX{ 5,5 };

Point2 Slice2(const Point3& p3) { return Point2{ p3[0], p3[1] }; }
int Layer(const Point3& p3) { return p3[2]; }
Point3 P3(const Point2& p2, int layer) { return Point3{ p2[0], p2[1], layer }; }

bool IsBug(int l, int x, int y)
{
    return (l & (1 << (x + y * 5))) != 0;
}
bool IsBug(int l, const Point2& p)
{
    if (!p.InBounds(MAX)) return false;
    return IsBug(l, p[0], p[1]);
}
bool IsBug(Map& m, const Point3& p)
{
    if (!Slice2(p).InBounds(MAX)) return false;
    if (m.find(Layer(p)) == m.end()) return false;
    return IsBug(m[Layer(p)], p[0], p[1]);
}

void SetBug(Map& m, const Point3& p)
{
    m[Layer(p)] = m[Layer(p)] | (1 << (p[0] + p[1] * 5));
}

void Dump(Map& m) {
    for (auto a: m) {
        int layer = a.second;
        std::cout << "Layer " << a.first << endl;
        for (int y = 0; y < 5; ++y) {
            for (int x = 0; x < 5; ++x) {
                if (x == 2 && y == 2) {
                    std::cout << '?';
                }
                else if (IsBug(layer, x, y)) {
                    std::cout << '#';
                }
                else {
                    std::cout << '.';
                }
            }
            std::cout << endl;
        }
    }
    std::cout << endl;
}

int Count(Map& m) {
    int c = 0;
    for (auto a : m) {
        int layer = a.second;
        for (int y = 0; y < 5; ++y) {
            for (int x = 0; x < 5; ++x) {
                if (x == 2 && y == 2) continue;
                if (IsBug(layer, x, y)) ++c;
            }
        }
    }
    return c;
}

void Iterate(Map& m) {
    Map newmap;

    // Expand map if needed
    if (m.begin()->second != 0) {
        m[m.begin()->first - 1] = 0;
    }
    if (m.rbegin()->second != 0) {
        m[m.rbegin()->first + 1] = 0;
    }

    for (auto a: m) {
        int layer = a.second;
        newmap[a.first] = 0;
        for (int y = 0; y < 5; ++y) {
            for (int x = 0; x < 5; ++x) {
                if (x == 2 && y == 2) continue; // Never do 2,2

                // Count surrounds
                int c = 0;
                Point2 p{ x,y };
                for (Point2 d : DIRS) {
                    if (IsBug(layer, p+d)) {
                        ++c;
                    }
                }

                // Do special
                if (x == 0) {
                    if (IsBug(m, Point3{ 1, 2, a.first - 1 })) ++c;
                }
                if (x == 4) {
                    if (IsBug(m, Point3{ 3, 2, a.first - 1 })) ++c;
                }
                if (y == 0) {
                    if (IsBug(m, Point3{ 2, 1, a.first - 1 })) ++c;
                }
                if (y == 4) {
                    if (IsBug(m, Point3{ 2, 3, a.first - 1 })) ++c;
                }

                if (x == 1 && y == 2) {
                    for (int i = 0; i < 5; ++i) {
                        if (IsBug(m, Point3{ 0, i, a.first + 1 })) ++c;
                    }
                }
                if (x == 3 && y == 2) {
                    for (int i = 0; i < 5; ++i) {
                        if (IsBug(m, Point3{ 4, i, a.first + 1 })) ++c;
                    }
                }
                if (x == 2 && y == 1) {
                    for (int i = 0; i < 5; ++i) {
                        if (IsBug(m, Point3{ i, 0, a.first + 1 })) ++c;
                    }
                }
                if (x == 2 && y == 3) {
                    for (int i = 0; i < 5; ++i) {
                        if (IsBug(m, Point3{ i, 4, a.first + 1 })) ++c;
                    }
                }

                if (c == 1 || (c == 2 && !IsBug(layer, p))) {
                    SetBug(newmap, P3(p, a.first));
                }
            }
        }
    }
    m = newmap;
}

int main()
{
    Map m;

    ifstream f("Data.txt");
    int b = 1;
    int l = 0;
    while (!f.eof()) {
        string s;
        getline(f, s);
        for (int x = 0; x < s.length(); ++x) {
            if (s[x] == '#') l = l | b;
            b = b * 2;
        }
    }
    m[0] = l;

    Dump(m);

    for (int i = 0; i < 200; ++i) {
        Iterate(m);
    }
    Dump(m);
    cout << "Count = " << Count(m) << endl;
}
