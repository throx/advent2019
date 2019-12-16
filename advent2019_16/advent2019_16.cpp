// advent2019_16.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>

using namespace std;

typedef vector<int> IntVec;
const IntVec PAT = { 0, 1, 0, -1 };
int g_skip = 1;
int g_total = 0;

IntVec Load()
{
    //string is("12345678");
    //string is("80871224585914546619083218645595");
    //string is("19617804207202209144916044189917");
    //string is("69317163492948606335995924319873");
    string is("59775675999083203307460316227239534744196788252810996056267313158415747954523514450220630777434694464147859581700598049220155996171361500188470573584309935232530483361639265796594588423475377664322506657596419440442622029687655170723364080344399753761821561397734310612361082481766777063437812858875338922334089288117184890884363091417446200960308625363997089394409607215164553325263177638484872071167142885096660905078567883997320316971939560903959842723210017598426984179521683810628956529638813221927079630736290924180307474765551066444888559156901159193212333302170502387548724998221103376187508278234838899434485116047387731626309521488967864391");

    g_skip = atoi(is.substr(0, 7).c_str());
    g_total = is.length() * 10000;

    //g_total = is.length();

    IntVec v;
    for (int i = g_skip; i < g_total; ++i) {
        v.push_back(is[i % is.length()] - '0');
    }

    return v;
}

IntVec Round(const IntVec& v)
{
    IntVec r;
    r.resize(v.size());

    for (int i = 1; i <= v.size(); ++i) {
        int sum = 0;
        int patindex = 1;
        int patcount = 0;
        for (int j = i-1; j < v.size(); ++j) {
            if (patindex == 1) sum += v[j];
            if (patindex == 3) sum -= v[j];

            ++patcount;
            if (patcount == (i + g_skip)) {
                patcount = 0;
                ++patindex;
                patindex = patindex & 0x3;
                if (patindex == 0 || patindex == 2) {
                    j += (i + g_skip);
                    ++patindex;
                }
            }
        }
        r[i - 1] = (sum >= 0) ? (sum % 10) : ((-sum) % 10);
        if (i % 10000 == 0) cout << i << "/" << v.size() << endl;
    }
    return r;
}

void Dump(const IntVec& v)
{
    for (auto x : v) {
        cout << x;
    }
    cout << endl;
}

int main()
{
    IntVec v = { 1,2,3,4,5,6,7,8 };
    g_skip = 0;
    for (int i = 0; i < 4; ++i) v = Round(v);
    Dump(v);
    cout << "^^ should be 01029498" << endl << endl;

    v = { 5,6,7,8 };
    g_skip = 4;
    for (int i = 0; i < 4; ++i) v = Round(v);
    Dump(v);
    cout << "^^ should be 9498" << endl << endl;

    v = Load();
    for (int i = 0; i < 100; ++i) {
        v = Round(v);
        cout << "Round " << i << endl;
    }
    Dump(v);
}
