#include <charconv>
#include <iostream>
#include <limits>
#include <vector>
#include <set>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int64_t dupint(int64_t x, int rep = 2)
{
    string sx = to_string(x);
    string s;
    while(rep--) s += sx;
    int64_t w = numeric_limits<int64_t>::max();
    from_chars(s.c_str(), s.c_str() + s.size(), w);
    // if from_chars errors, w is not changed
    return w;
}

int main(int argc, char* argv[])
{
    bool partb = AOC::PartB(argv[0]);
    string line;
    getline(cin, line);
    vector<pair<int64_t, int64_t>> range;
    for(auto w :AOC::readToVector<string>(line, ','))
    {
        range.push_back(AOC::takeFirst<2>(AOC::readNumbers<int64_t>(w)));
    }
    sort(range.begin(), range.end());
    set<int64_t> invalid;
    int64_t total = 0;
    for(int rep = 2; partb ? dupint(1, rep) <= range.back().second : (rep <= 2); rep++)
    {
        auto p = range.begin();
        int64_t v = 1;
        while(p != range.end())
        {
            int64_t d = dupint(v, rep);
            if(invalid.count(d) > 0)
                ++v;
            else if(d < p->first)
                ++v;
            else if(p->second < d)
                ++p;
            else
            {
                invalid.insert(d);
                cerr << d << "\n";
                total += d;
                ++v;
            }
        }
    }
    cout << total << "\n";
}