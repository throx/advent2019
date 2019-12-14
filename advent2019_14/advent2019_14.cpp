#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <list>
#include <string>
#include <set>
#include <vector>
#include <utility>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

struct Ing {
    __int64 quantity;
    string name;
};
typedef list<Ing> IngList;
struct RecipeElement {
    __int64 makes;
    IngList ings;
    RecipeElement() : makes(0) {}
};
typedef map<string, RecipeElement> Recipe;
typedef map<string, __int64> Stash;

Ing Parse(string s)
{
    Ing r;
    stringstream ss(s);
    ss >> r.quantity;
    ss >> r.name;
    trim(r.name);

    return r;
}

Recipe Load()
{
    Recipe r;
    ifstream f("Data.txt");

    while (!f.eof()) {
        string s;
        getline(f, s);

        vector<string> parts;
        split(parts, s, is_any_of("=>"), token_compress_on);
        vector<string> ings;
        split(ings, parts[0], is_any_of(","), token_compress_on);

        Ing result = Parse(parts[1]);
        auto& re = r[result.name];
        re.makes = result.quantity;
        
        for (const string& ing : ings) {
            Ing src = Parse(ing);
            re.ings.push_back(src);
        }
    }

    return r;
}

__int64 GetRequiredOre(const Recipe& r, const string& type, __int64 num, Stash& stash)
{
    __int64 ore = 0;
    __int64 in_stash = stash[type];

    //cout << "Need " << num << " " << type << "> ";

    // Check if we have enough in stash
    if (in_stash > num) {
        stash[type] -= num;
        //cout << "In stash" << endl;
        return 0;
    }

    // Exhaust stash first
    //cout << stash[type] << " from stash, ";
    num -= stash[type];
    stash.erase(type);

    // Get recipe
    auto it = r.find(type);
    if (it == r.end()) throw std::exception("No recipe!");
    auto& re = it->second;

    // Recipe makes X lots per batch, so need to make 
    __int64 to_make = (__int64)ceil(1.0 * num / re.makes);
    //cout << "Making " << to_make * re.makes << "< ";
    //for (auto ing : re.ings) cout << ing.name << ": " << ing.quantity * to_make << " ";
    //cout << endl;

    for (auto ing : re.ings) {
        if (ing.name == "ORE"s) {
            ore += ing.quantity * to_make;
        }
        else {
            ore += GetRequiredOre(r, ing.name, ing.quantity * to_make, stash);
        }
    }

    stash[type] += (to_make * re.makes - num);
    return ore;
}

int main()
{
    Recipe r = Load();
    Stash stash;
    __int64 needs = GetRequiredOre(r, "FUEL", 1, stash);
    cout << "FUEL requires " << needs << " ORE" << endl;
    cout << "Left over:" << endl;
    for (auto x : stash) {
        if (x.second > 0) {
            cout << "  " << x.first << " = " << x.second << endl;
        }
    }

    // Part 2 - for 1e12 ore, how much fuel.  Search!
    __int64 target = 1000000000000i64;
    __int64 guess = target / needs + 1;
    __int64 step = 16777216;
    while (true) {
        stash.clear();
        needs = GetRequiredOre(r, "FUEL", guess, stash);
        cout << "Guess " << guess << " gives " << needs << endl;
        if (needs > target) {
            guess -= step;
        }
        else if (step > 1) {
            step /= 2;
            guess += step;
        }
        else {
            break;
        }
    }
    cout << guess << " FUEL from " << needs << " ORE" << endl;

    return 0;
}
