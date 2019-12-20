// advent2019_20.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <Windows.h>

#include "Console.h"
#include "Point.h"

using namespace std;

typedef Point<int, 2> Point2;
typedef vector<string> Map;
typedef map<Point2, Point2> Portals;

Point2 g_max;

vector<Point2> DIRS = { Point2{-1,0}, Point2{1,0}, Point2{0,-1}, Point2{0,1} };

char M(const Map& m, const Point2& p) { return p.InBounds(g_max) ? m[p[1]][p[0]] : ' '; }
bool IsPortal(char c) { return c >= 'A' && c <= 'Z'; }

int main()
{
    // Load map
    Map m;
    ifstream f("Test3.txt");
    while (!f.eof()) {
        string s;
        getline(f, s);
        m.push_back(s);
    }

    // Set size
    g_max = Point2{ (int)m[0].length(), (int)m.size() };

    JadedHoboConsole::setsize(max(120, g_max[0] + 5), max(40, g_max[1] + 5));
    cout << JadedHoboConsole::clr;
    for (string s : m) cout << s << endl;

    // Parse map
    map<string, vector<Point2>> out_locs, in_locs;
    for (int y = 0; y < g_max[1]; ++y) {
        for (int x = 0; x < g_max[0]; ++x) {
            Point2 pos{ x, y };
            char c = M(m, pos);
            if (c == '.') {
                // Check for portal near point
                for (auto d : DIRS) {
                    Point2 np = pos + d;
                    char nc = M(m, np);
                    if (IsPortal(nc)) {
                        string portal;
                        if (d[0] < 0 || d[1] < 0) {
                            portal.push_back(M(m, np + d));
                            portal.push_back(nc);
                        }
                        else {
                            portal.push_back(nc);
                            portal.push_back(M(m, np + d));
                        }
                        out_locs[portal].push_back(pos);
                        in_locs[portal].push_back(np);
                    }
                }
            }
        }
    }

    Portals portals;
    for (auto a : in_locs) {
        if (a.first != "AA" && a.first != "ZZ") {
            portals[a.second[0]] = out_locs[a.first][1];
            portals[a.second[1]] = out_locs[a.first][0];
        }
    }

    Point2 start = out_locs["AA"][0];
    Point2 finish = out_locs["ZZ"][0];

    // Find distance with floodfill
    set<Point2> done;
    set<Point2> todo;
    set<Point2> todo_next;
    todo.insert(start);
    int dist = 0;

    JadedHoboConsole::gotoxy(start[0], start[1]);
    cout << JadedHoboConsole::fg_green << (char)219;

    while (!todo.empty())
    {
        ++dist;
        done.insert(todo.begin(), todo.end());

        for (auto p : todo) {
            for (auto dir : DIRS) {
                Point2 newpos = p + dir;
                if (portals.find(newpos) != portals.end()) newpos = portals[newpos];
                if (newpos == finish) goto END;
                if (M(m, newpos) == '.') {
                    if (done.find(newpos) == done.end()) {
                        todo_next.insert(newpos);
                        JadedHoboConsole::gotoxy(newpos[0], newpos[1]);
                        cout << JadedHoboConsole::fg_green << (char)219;
                        Sleep(1);
                    }
                }
            }
        }
        todo = todo_next;
        todo_next.clear();
    }

END:
    JadedHoboConsole::gotoxy(finish[0], finish[1]);
    cout << JadedHoboConsole::fg_green << (char)219;
    JadedHoboConsole::gotoxy(0, g_max[1]);
    cout << JadedHoboConsole::fg_white << "Dist = " << dist << endl;
}
