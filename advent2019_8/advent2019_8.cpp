// advent2019_8.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

constexpr int WIDTH = 25;
constexpr int HEIGHT = 6;

typedef vector<int> Layer;
typedef vector<Layer> LayerVector;

int digitcount(const Layer& l, int d)
{
    return count_if(l.begin(), l.end(), [=](auto x) { return (x == d); });
}

Layer Composite(const Layer& l1, const Layer& l2)
{
    Layer r;
    r.resize(l1.size());
    for (int pos = 0; pos < l1.size(); ++pos)
    {
        r[pos] = (l2[pos] == 2) ? l1[pos] : l2[pos];
    }
    return r;
}

void PrintLayer(const Layer& l)
{
    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            switch (l[x + y * WIDTH]) {
            case 0: cout << (char)219; break;
            case 1: cout << ' '; break;
            default: cout << 'X'; break;
            }
        }
        cout << endl;
    }
}

int main()
{
    LayerVector layers;

    ifstream f("Data.txt");
    while (!f.eof())
    {
        Layer l;
        l.reserve(WIDTH * HEIGHT);
        for (int y = 0; y < HEIGHT; ++y)
        {
            for (int x = 0; x < WIDTH; ++x)
            {
                char c;
                f >> c;
                int i = (c - '0');
                l.push_back(i);
            }
        }

        layers.push_back(l);
    }

    int minzero = WIDTH * HEIGHT;
    Layer layer;
    for (const auto& l : layers)
    {
        int zeros = digitcount(l, 0);
        if (zeros < minzero)
        {
            minzero = zeros;
            layer = l;
        }
    }

    int result = digitcount(layer, 1) * digitcount(layer, 2);
    cout << "Part 1: " << result << endl;

    Layer composite;
    composite.resize(WIDTH * HEIGHT);
    for (LayerVector::const_reverse_iterator ri = layers.rbegin(); ri != layers.rend(); ++ri)
    {
        composite = Composite(composite, *ri);
    }

    PrintLayer(composite);
    return 0;
}
