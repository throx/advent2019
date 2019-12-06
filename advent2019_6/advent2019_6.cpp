// advent2019_6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

using namespace std;
using namespace boost;

// Forward and reverse indexes to integer vertex references (so boost graph lib will work)
typedef map<int, string> Index;
Index g_index;

typedef map<string, int> RevIndex;
RevIndex g_revindex;

// The Boost magic.  Greate an undirected graph, with no props on the vertex, 
// integer weight on the edge, represented internally by vectors.  Note that
// undirected works fine because we're mostly worried about changing orbits up or down
typedef adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_weight_t, int> > Graph;

// A constant weight of 1 for changing orbits
const property<edge_weight_t, int> WEIGHT_ONE = 1;

// Helper for forward and reverse mappings
int IndexOf(const string& str)
{
    // Create the id if we don't have one
    auto it = g_revindex.find(str);
    if (it == g_revindex.end())
    {
        int i = g_revindex.size();
        g_revindex[str] = i;
        g_index[i] = str;
        return i;
    }
    return it->second;
}

// Found I didn't need the forward index...  Oh well!
const string& S(int i)
{
    return g_index[i];
}

// MAIN, YAY!
int main()
{
    // Read in the strings
    ifstream f("Data.txt");
    Graph g;

    while (!f.eof())
    {
        string c, o;
        getline(f, c, ')');
        getline(f, o);
        add_edge(IndexOf(c), IndexOf(o), WEIGHT_ONE, g);
    }

    // Call Dijkstra with COM as the root.
    vector<int> d(num_vertices(g));
    dijkstra_shortest_paths(g, IndexOf("COM"s), distance_map(&d[0]));

    // Print out sum of all distances
    cout << "Part 1: " << accumulate(d.begin(), d.end(), 0) << endl;

    // Call Dijkstra to find distance to SANTA!
    dijkstra_shortest_paths(g, IndexOf("YOU"s), distance_map(&d[0]));
    cout << "Part 2: " << d[IndexOf("SAN"s)] - 2 << endl;
}
