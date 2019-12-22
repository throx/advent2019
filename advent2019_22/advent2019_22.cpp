// advent2019_22.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

constexpr int g_deck_size = 10007;
typedef vector<int> Cards;

typedef vector<string> Commands;

void NewStack(Cards& cards) {
    Cards c2;
    c2.reserve(cards.size());
    for (int i = 0; i < cards.size(); ++i) {
        c2.push_back(cards[cards.size() - i - 1]);
    }
    cards.swap(c2);
}

void Cut(Cards& cards, int n) {
    Cards c2;
    c2.reserve(cards.size());
    if (n < 0) n += cards.size();
    for (int i = 0; i < cards.size(); ++i) {
        c2.push_back(cards[n]);
        ++n;
        if (n >= cards.size()) n -= cards.size();
    }
    cards.swap(c2);
}

void DealIncr(Cards& cards, int n) {
    Cards c2(cards.size());
    int p = 0;
    for (int i = 0; i < cards.size(); ++i) {
        c2[p] = cards[i];
        p += n;
        if (p > cards.size()) p -= cards.size();
    }
    cards.swap(c2);
}

void Dump(const Cards& cards) {
    for (auto x : cards) {
        cout << x << " ";
    }
    cout << endl;
}

int main()
{
    Cards cards;
    for (int i = 0; i < g_deck_size; ++i) cards.push_back(i);

    Commands commands;
    ifstream f("Data.txt");
    while (!f.eof()) {
        string s;
        getline(f, s);
        commands.push_back(s);
    }

    for (string c : commands) {
        if (c == "deal into new stack"s) NewStack(cards);
        else if (c.substr(0, 3) == "cut"s) {
            int n = atoi(c.substr(4).c_str());
            Cut(cards, n);
        }
        else if (c.substr(0, 19) == "deal with increment"s) {
            int n = atoi(c.substr(20).c_str());
            DealIncr(cards, n);
        }

        //cout << c << endl;
        //Dump(cards);
    }

    // Dump cards
    Dump(cards);

    // Find card 2019
    for (int i = 0; i < g_deck_size; ++i) {
        if (cards[i] == 2019) {
            cout << "2019 @ " << i;
        }
    }
}
