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
typedef set<char> Keys;
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

Infos GetItems(const Map& m, const Point2& pos)
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
            result.push_back(Info{ c, i.dist, i.where });
        }
        else {
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
    }
    return result;
}

int GetMinDistIterative(const Map& m, const Point2& pos, Path& bestpath)
{
    struct ToDo {
        Map m;
        Keys keys;
        Point2 pos;
        Path path;
        int dist;
        // Only good for heaps, not stuff that needs strict less
        bool operator< (const ToDo& other) const { return dist > other.dist; }
    };
    vector<ToDo> todo;

    todo.push_back(ToDo{ m, Keys(), pos, Path(), 0 });
    make_heap(todo.begin(), todo.end());
    while (!todo.empty()) {

        ToDo item = todo.front();
        pop_heap(todo.begin(), todo.end());
        todo.pop_back();

        Infos infos = GetItems(item.m, item.pos);
        if (infos.empty()) {
            bestpath = item.path;
            return item.dist;
        }

        for (const auto& info : infos) {
            if (IsKey(info.item)) {
                ToDo newitem(item);
                newitem.m[info.pos[1]][info.pos[0]] = '.';
                newitem.keys.insert(info.item);
                newitem.path.push_back(info.item);
                newitem.pos = info.pos;
                newitem.dist += info.dist;
                todo.push_back(newitem);
                push_heap(todo.begin(), todo.end());
            }
            else {
                if (item.keys.find(tolower(info.item)) != item.keys.end()) {
                    ToDo newitem(item);
                    newitem.m[info.pos[1]][info.pos[0]] = '.';
                    newitem.path.push_back(info.item);
                    newitem.pos = info.pos;
                    newitem.dist += info.dist;
                    todo.push_back(newitem);
                    push_heap(todo.begin(), todo.end());
                }
            }
        }
    }

    cout << "ERROR!!";
}

int GetMinDistRecursive(const Map& m, Keys keys, Point2 pos, int currdist, const Path& currpath)
{
    if (currdist > g_best) {
        return g_best + 1;
    }
    Infos infos = GetItems(m, pos);
    if (infos.empty()) {
        if (g_best > currdist) {
            g_best = currdist;
            cout << g_best << ": ";
            for (auto x : currpath) cout << x << ", ";
            cout << endl;
        }
        return currdist;
    }

    // Try everything recursively
    int mindist = INT_MAX;
    make_heap(infos.begin(), infos.end());
    while (!infos.empty()) {
        const auto& info = infos.front();
        int dist = mindist;
        Path path(currpath);
        Map newmap(m);
        if (IsKey(info.item)) {
            Keys newkeys(keys);
            pos = info.pos;
            newmap[pos[1]][pos[0]] = '.';
            newkeys.insert(info.item);
            path.push_back(info.item);
            dist = GetMinDistRecursive(newmap, newkeys, pos, info.dist + currdist, path);
        }
        else {
            if (keys.find(tolower(info.item)) != keys.end()) {
                pos = info.pos;
                newmap[pos[1]][pos[0]] = '.';
                path.push_back(info.item);
                dist = GetMinDistRecursive(newmap, keys, pos, info.dist + currdist, path);
            }
        }

        if (dist < mindist) {
            mindist = dist;
        }
        pop_heap(infos.begin(), infos.end());
        infos.pop_back();
    }

    return mindist;
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
        }
    }

    Path path;
    int dist = GetMinDistIterative(m, pos, path);
    cout << "Min Dist = " << dist << endl;
    for (auto x : path) cout << x << ", ";
    cout << endl;
}
