// advent2019_10.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <algorithm>
#include <numeric>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

struct Point
{
    int x;
    int y;

    Point(int x, int y) : x(x), y(y) {}
    bool operator==(const Point& other) const { return (x == other.x) && (y == other.y); }
    bool operator<(const Point& other) const { return (x == other.x) ? y < other.y : x < other.x; }
};

typedef vector<string> Map;
typedef map<Point, Map> Asteroids;

size_t w, h;

void Check(Map& space, int x, int y, int dx, int dy)
{
    int tx = x + dx;
    int ty = y + dy;
    bool first = false;

    while (!(tx < 0 || tx >= w || ty < 0 || ty >= h)) {
        if (first) {
            space[ty][tx] = '.';
        }
        else if (space[ty][tx] == '#') {
            first = true;
        }
        tx += dx;
        ty += dy;
    }
}

Map Visible(Map space, int x, int y)
{
    for (int dx = 0; dx < w; ++dx) {
        for (int dy = 0; dy < h; ++dy) {
            if (dx == 0 && dy == 0) continue;
            Check(space, x, y, dx, dy);
            Check(space, x, y, -dx, dy);
            Check(space, x, y, dx, -dy);
            Check(space, x, y, -dx, -dy);
        }
    }
    space[y][x] = '@';
    return space;
}

int CountVis(const Map& space)
{
    int s = 0;
    for (auto x : space) {
        s += count(x.begin(), x.end(), '#');
    }
    return s;
}

int main()
{
    Map space;

    ifstream f("Data.txt");
    while (!f.eof()) {
        string s;
        getline(f, s);
        space.push_back(s);
    }

    w = space.front().size();
    h = space.size();

    Asteroids asters;

    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            if (space[y][x] == '#') {
                asters[Point(x, y)] = Visible(space, x, y);
            }
        }
    }

    int max = 0;
    Point max_item(0, 0);
    for (const auto& i : asters) {
        int c = CountVis(i.second);
        if (max < c) {
            max = c;
            max_item = i.first;
        }
    }

    int ssx = max_item.x;
    int ssy = max_item.y;

    cout << "Max visible = " << max << endl;
    cout << "  at (" << max_item.x << ", " << max_item.y << ")" << endl;
    cout << endl;
    for (const string& s : asters[max_item]) {
        cout << s << endl;
    }

    space[ssy][ssx] = '@';
    int tot = CountVis(space);
    double laser = 0.0;

    for (int a = 1; a <= tot; ++a) {
        Map now = Visible(space, ssx, ssy);
        double minangle = 10.0;
        int boomx = 0, boomy = 0;
        for (int x = 0; x < w; ++x) {
            for (int y = 0; y < h; ++y) {
                if (now[y][x] == '#') {
                    double angle = atan2(x - ssx, ssy - y);
                    angle -= laser;
                    while (angle < 0) angle += M_PI * 2;

                    if (angle < minangle) {
                        minangle = angle;
                        boomx = x;
                        boomy = y;
                    }
                }
            }
        }

        space[boomy][boomx] = '*';
        cout << "Laser #" << a << " -> " << boomx << ", " << boomy << " @ " << (laser + minangle) * 180 / M_PI << endl;
        space[boomy][boomx] = '-';
        laser += minangle + 0.0000001;
    }
}
