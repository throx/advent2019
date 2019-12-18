// advent2019_18.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <list>
#include <algorithm>

#include "Point.h"

using namespace std;

typedef vector<string> Map;
typedef vector<bool> Keys;
typedef list<char> Path;
typedef Point<int, 2> Point2;

Point2 g_max;

struct Info {
    char item;
    int dist;
    Point2 pos;
    bool operator<(const Info& other) const { return dist > other.dist; }
};

typedef vector<Info> Infos;

vector<Point2> DIRS = { Point2{-1,0}, Point2{1,0}, Point2{0,-1}, Point2{0,1} };

bool IsKey(char c)
{
    return c >= 'a' && c <= 'z';
}

bool IsDoor(char c)
{
    return c >= 'A' && c <= 'Z';
}

vector<bool> g_done;
vector<bool> g_zero;
int g_best = INT_MAX;
int g_numkeys = 0;
vector<vector<int> > g_dist(26, vector<int>(26));

Infos GetItems(const Map& m, const Point2& pos, const Keys& keys)
{
    // Flood fill
    struct Data { Point2 where; int dist; };
    vector<Data> todo;
    todo.reserve(1000);
    todo.push_back(Data{ pos, 0 });
    g_done = g_zero;
    Infos result;
    while (!todo.empty())
    {
        auto i = todo.back();
        char c = m[i.where[1]][i.where[0]];
        todo.pop_back();
        g_done[i.where[0] + g_max[0] * i.where[1]] = true;
        if (IsKey(c) || IsDoor(c)) {
            if (!keys[tolower(c) - 'a']) {
                if (IsKey(c)) {
                    result.push_back(Info{ c, i.dist, i.where });
                }
                continue;
            }
        }

        for (auto dir : DIRS) {
            Point2 newpos = i.where + dir;
            if (newpos.InBounds(g_max)) {
                if (!g_done[newpos[0] + g_max[0] * newpos[1]]) {
                    if (m[newpos[1]][newpos[0]] != '#') {
                        todo.push_back(Data{ newpos, i.dist + 1 });
                    }
                }
            }
        }
    }
    return result;
}

vector<int> GetDist(const Map& m, const Point2& pos)
{
    struct Data { Point2 where; int dist; };
    vector<Data> todo;
    todo.reserve(1000);
    todo.push_back(Data{ pos, 0 });
    g_done = g_zero;
    vector<int> result;
    result.resize(26);

    while (!todo.empty())
    {
        auto i = todo.back();
        char c = m[i.where[1]][i.where[0]];
        todo.pop_back();
        g_done[i.where[0] + g_max[0] * i.where[1]] = true;

        for (auto dir : DIRS) {
            Point2 newpos = i.where + dir;
            if (newpos.InBounds(g_max)) {
                if (!g_done[newpos[0] + g_max[0] * newpos[1]]) {
                    char c = m[newpos[1]][newpos[0]];
                    if (c != '#') {
                        todo.push_back(Data{ newpos, i.dist + 1 });
                    }
                    if (IsKey(c)) {
                        result[c - 'a'] = i.dist + 1;
                    }
                }
            }
        }
    }

    return result;
}

int GetMinDistIterative(const Map& m, const Point2& pos, Path& bestpath)
{
    int best = 0;

    struct ToDo {
        Keys keys;
        Point2 pos;
        int dist;
        int minleft;
        // Only good for heaps, not stuff that needs strict less
        bool operator< (const ToDo& other) const { return (dist + minleft*0.55) > (other.dist + other.minleft*0.55); }
    };
    vector<ToDo> todo;

    Keys keys;
    keys.resize(26);
    int md = 0;
    for (int i = 0; i < 25; ++i) md += g_dist[i][i + 1];
    todo.push_back(ToDo{ keys, pos, 0, md });

    make_heap(todo.begin(), todo.end());
    while (!todo.empty()) {

        ToDo item = todo.front();
        pop_heap(todo.begin(), todo.end());
        todo.pop_back();

        int nk = 0;
        for (const auto& x : item.keys) if (x) ++nk;
        if (best < nk) {
            cout << nk << ": " << todo.size() << endl;
            best = nk;
        }

        Infos infos = GetItems(m, item.pos, item.keys);
        if (infos.empty()) {
            return item.dist;
        }

        for (const auto& info : infos) {
            if (IsKey(info.item)) {
                ToDo newitem(item);
                newitem.keys[info.item - 'a'] = true;
                newitem.minleft = 0;
                int j = 0;
                for (int i = 1; i < 26; ++i) {
                    if (!newitem.keys[i]) {
                        newitem.minleft += g_dist[i][j];
                        j = i;
                    }
                }
                newitem.pos = info.pos;
                newitem.dist += info.dist;
                todo.push_back(newitem);
                push_heap(todo.begin(), todo.end());
            }
        }
    }

    cout << "ERROR!!";
    return -1;
}

int main()
{
    Map m;

    ifstream f("Data.txt");
    while (!f.eof()) {
        string s;
        getline(f, s);
        m.push_back(s);
    }
    g_max = Point2{ (int)m[0].length(), (int)m.size() };
    g_done.resize(g_max[0] * g_max[1]);
    g_zero.resize(g_max[0] * g_max[1]);

    Point2 pos;

    for (int y = 0; y < g_max[1]; ++y) {
        for (int x = 0; x < g_max[0]; ++x) {
            if (m[y][x] == '@') {
                pos = Point2{ x, y };
                m[y][x] = '.';
            }
            if (IsKey(m[y][x])) {
                g_dist[m[y][x] - 'a'] = GetDist(m, Point2{ x, y });
            }
        }
    }

    Path path;
    int dist = GetMinDistIterative(m, pos, path);
    cout << "Min Dist = " << dist << endl;
    for (auto x : path) cout << x << ", ";
    cout << endl;
}
