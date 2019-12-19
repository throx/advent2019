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
#include <queue>

#include "Point.h"

using namespace std;

typedef vector<string> Map;
typedef vector<bool> Keys;
typedef vector<char> Path;
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

struct KeyPath {
    int dist;
    set<char> doors;
};

typedef map<char, KeyPath> KeyPathMap;
typedef map<pair<char, char>, KeyPath> Adjacency;
Adjacency g_adjacency;
int g_numkeys = 0;

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

KeyPathMap GetDist(const Map& m, const Point2& pos)
{
    struct Data { Point2 where; int dist; set<char> doors; };
    vector<Data> todo;
    todo.reserve(1000);
    todo.push_back(Data{ pos, 0 });
    g_done = g_zero;
    KeyPathMap result;

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
                    set<char> doors = i.doors;
                    if (IsKey(c)) {
                        result[c] = KeyPath{ i.dist + 1, i.doors };
                    }
                    else if (IsDoor(c)) {
                        doors.insert(tolower(c));
                    }
                    if (c != '#') {
                        todo.push_back(Data{ newpos, i.dist + 1, doors });
                    }
                }
            }
        }
    }

    return result;
}

int GetNoDoorDist(char start, const Keys& currkeys)
{
    // Dijkstra - set up a queue
    set<pair<int, char>> q;
    map<char, int> distances;
    set<char> keysleft;

    // Figure out the set of keys we have to work with, and init
    // their distances to a BIG number
    for (char k = 'a'; k < 'a' + g_numkeys; ++k) {
        if (!currkeys[k - 'a']) {
            keysleft.insert(k);
            distances[k] = INT_MAX;
        }
    }

    // Init queue with the starting pos at distance 0
    int dist = 0;
    q.insert(make_pair(0, start));

    while (!q.empty()) {

        // Grab shortest path off queue
        auto p = *q.cbegin();
        q.erase(q.cbegin());
        dist = p.first;
        char from = p.second;

        // Fill all distances from the queue point out to other 
        // points (because we can go everywhere)
        for (char next : keysleft) {
            auto index = make_pair(from, next);
            const auto& kp = g_adjacency[index];
            int newdist = kp.dist + dist;

            // If we have a new best distance the remember it
            // and push result on queue
            if (distances[next] > newdist) {
                q.erase(make_pair(distances[next], next));
                distances[next] = newdist;
                q.insert(make_pair(newdist, next));
            }
        }
    }

    return dist;
}

int FindShortestPath(Path& bestpath)
{
    // A* - set up a queue, with heuristics
    struct Data {
        int dist;
        Path path;
        Keys keys;
        int min_total_dist;
        bool operator<(const Data& other) const {
            if (min_total_dist == other.min_total_dist) {
                if (keys == other.keys) {
                    return dist < other.dist;
                }
                return keys < other.keys;
            }
            return min_total_dist < other.min_total_dist;
        }
    };

    // Don't bother initing the datamap - assume no entry is infinite
    map<pair<char, Keys>, Data> datamap;
    set<Data> q;
    const Keys allkeys(g_numkeys, true);

    // Init queue with the starting pos at distance 0 and no keys
    Data item{ 0, Path{'@'}, Keys(g_numkeys) };
    item.min_total_dist = GetNoDoorDist('@', item.keys);
    q.insert(item);

    // Some counters...
    int count = 0;

    while (!q.empty()) {

        // Grab shortest path off queue
        item = *q.cbegin();
        q.erase(q.cbegin());

        // Is it a winner?
        if (item.keys == allkeys) {
            break;
        }

        // Play with counter
        ++count;
        if (count % 100000 == 0) cout << count << "c: " << q.size() << item.min_total_dist << endl;

        // Fill all distance from the queue point out to other points
        // that we can get do (testing doors).
        for (char next = 'a'; next < 'a' + g_numkeys; ++next) {

            // If we already have the key, don't bother
            if (item.keys[next - 'a']) continue;

            // Get the adjacency item now we have (from, to)
            char from = item.path.back();
            auto index = make_pair(from, next);
            const auto& kp = g_adjacency[index];

            // Check the doors
            bool all_doors = true;
            for (const auto& d : kp.doors) {
                if (!item.keys[d - 'a']) {
                    all_doors = false;
                    break;
                }
            }
            if (!all_doors) continue;

            // Compute new item
            Data newitem(item);
            newitem.dist += kp.dist;
            newitem.keys[next - 'a'] = true;
            newitem.path.push_back(next);
            newitem.min_total_dist = newitem.dist + GetNoDoorDist(next, newitem.keys);

            // If we have a new best distance, then remember it and
            // push the result on queue
            auto mapindex = make_pair(next, newitem.keys);
            auto it = datamap.find(mapindex);
            if (it == datamap.end()) {
                q.insert(newitem);
                datamap[mapindex] = newitem;
            }
            else if (it->second.min_total_dist > newitem.min_total_dist) {
                q.erase(it->second);
                q.insert(newitem);
                datamap[mapindex] = newitem;
            }
        }
    }

    bestpath = item.path;
    return item.dist;
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
            char c = m[y][x];
            if (c == '@') {
                pos = Point2{ x, y };
            }
            if (IsKey(c)) g_numkeys++;
            if (IsKey(c) || c == '@') {
                auto km = GetDist(m, Point2{ x, y });
                for (const auto& it : km) {
                    g_adjacency[make_pair(c, it.first)] = it.second;
                }
            }
        }
    }

    Path path;
    int dist = FindShortestPath(path);
    cout << "Min Dist = " << dist << endl;
    for (auto x : path) cout << x << ", ";
    cout << endl;
}

