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
typedef vector<string> Path;
typedef Point<int, 2> Point2;

Point2 g_max;

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
int g_numbots = 0;

KeyPathMap GetDist(const Map& m, const Point2& pos)
{
    struct Data { Point2 where; int dist; set<char> doors; };
    vector<Data> todo;
    vector<Data> todonext;
    todo.reserve(1000);
    todonext.reserve(1000);
    todo.push_back(Data{ pos, 0 });
    g_done = g_zero;
    KeyPathMap result;

    while (!todo.empty())
    {
        for (auto i : todo) {
            char c = m[i.where[1]][i.where[0]];
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
                            todonext.push_back(Data{ newpos, i.dist + 1, doors });
                        }
                    }
                }
            }
        }
        todo = todonext;
        todonext.clear();
    }

    return result;
}

int FindShortestPathDijkstra(Path& bestpath)
{
    // Dijkstra - set up a queue
    struct Data {
        int dist;
        string pos;
        Path path;
        Keys keys;
        bool operator<(const Data& other) const {
            if (dist == other.dist) {
                if (keys == other.keys) {
                    return pos < other.pos;
                }
                return keys > other.keys;
            }
            return dist < other.dist;
        }
    };

    // Don't bother initing the datamap - assume no entry is infinite
    map<pair<string, Keys>, Data> datamap;
    set<Data> q;
    const Keys allkeys(g_numkeys, true);

    // Init queue with the starting pos at distance 0 and no keys
    string start;
    for (int i = 0; i < g_numbots; ++i) start.push_back((char)('1' + i));
    Data item{ 0, start, Path{start}, Keys(g_numkeys) };
    q.insert(item);

    // Some counters...
    int count = 0;

    while (!q.empty()) {

        // Grab shortest path off queue
        item = *q.cbegin();
        q.erase(q.cbegin());

#ifdef TRACE
        cout << "At " << item.path.back() << " at dist " << item.dist << " with keys [";
        for (int x = 0; x < g_numkeys; ++x) if (item.keys[x]) cout << (char)('a' + x);
        cout << "]" << endl;
#endif

        // Is it a winner?
        if (item.keys == allkeys) {
#ifdef TRACE
            cout << "Try " << item.dist << "? ";
            for (auto x : item.path) cout << x << ", ";
            cout << endl;
#else
            break;
#endif
        }

        // Play with counter
        ++count;
        if (count % 100000 == 0) cout << count << "c: " << q.size() << item.dist << endl;

        // Fill all distance from the queue point out to other points
        // that we can get do (testing doors).
        for (char next = 'a'; next < 'a' + g_numkeys; ++next) {

            // If we already have the key, don't bother
            if (item.keys[next - 'a']) continue;

            // Check all bots
            string botpos = item.pos;
            for (int bot = 0; bot < g_numbots; ++bot) {
                // Get the adjacency item now we have (from, to)
                char from = botpos[bot];
                auto index = make_pair(from, next);

                // Check if there is an adjacency
                auto kp_it = g_adjacency.find(index);
                if (kp_it == g_adjacency.end()) continue;
                const auto& kp = kp_it->second;

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
                newitem.pos[bot] = next;
                newitem.path.push_back(newitem.pos);

                // If we have a new best distance, then remember it and
                // push the result on queue
                auto mapindex = make_pair(newitem.pos, newitem.keys);
                auto it = datamap.find(mapindex);
                if (it == datamap.end()) {
                    q.insert(newitem);
                    datamap[mapindex] = newitem;

#ifdef TRACE
                    cout << "Add " << newitem.pos << " at dist " << newitem.dist << " with keys [";
                    for (int x = 0; x < g_numkeys; ++x) if (newitem.keys[x]) cout << (char)('a'+x);
                    cout << "]" << endl;
#endif
                }
                else if (it->second.dist > newitem.dist) {
#ifdef TRACE
                    cout << "Overwrite " << it->second.pos << " at dist " << it->second.dist << " with keys [";
                    for (int x = 0; x < g_numkeys; ++x) if (it->second.keys[x]) cout << (char)('a' + x);
                    cout << "]" << endl;
#endif
                    q.erase(it->second);
                    q.insert(newitem);
                    datamap[mapindex] = newitem;
#ifdef TRACE
                    cout << "     with " << newitem.pos << " at dist " << newitem.dist << " with keys [";
                    for (int x = 0; x < g_numkeys; ++x) if (newitem.keys[x]) cout << (char)('a' + x);
                    cout << "]" << endl;
#endif
                }
            }
        }
    }

    bestpath = item.path;
    return item.dist;
}

int main()
{
    Map m;

    ifstream f("Data2.txt");
    while (!f.eof()) {
        string s;
        getline(f, s);
        m.push_back(s);
    }
    g_max = Point2{ (int)m[0].length(), (int)m.size() };
    g_done.resize(g_max[0] * g_max[1]);
    g_zero.resize(g_max[0] * g_max[1]);

    for (int y = 0; y < g_max[1]; ++y) {
        for (int x = 0; x < g_max[0]; ++x) {
            char c = m[y][x];
            if (IsKey(c)) g_numkeys++;
            if (IsKey(c) || c == '@') {
                if (c == '@') {
                    ++g_numbots;
                    c = '0' + g_numbots;
                }
                auto km = GetDist(m, Point2{ x, y });
                for (const auto& it : km) {
                    g_adjacency[make_pair(c, it.first)] = it.second;
                }
            }
        }
    }

    Path path;
    int dist = FindShortestPathDijkstra(path);
    cout << "Min Dist = " << dist << endl;
    for (auto x : path) cout << x << ", ";
    cout << endl;
}

