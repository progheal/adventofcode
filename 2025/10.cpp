#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <vector>
#include <string>
#include "aoc/aoc.h"
using namespace std;

struct Machine
{
    unsigned target;
    vector<unsigned> button;
    vector<int> joltage;
    size_t minPress() const;
    size_t minJolt() const;
private:
    mutable map<string, size_t> joltCache;
    size_t minJoltImpl(const vector<int>& jolt) const;
};

template<>
Machine AOC::convert<Machine>(const string& line)
{
    Machine m;
    auto token = AOC::readToVector<string>(line, ' ');
    m.target = 0;
    string tstr = token[0];
    token.erase(token.begin());
    for(size_t p = 1; p <= tstr.size() - 1; p++)
    {
        m.target |= (tstr[p] == '#' ? 1U : 0U) << (p - 1);
    }
    string jstr = token.back();
    token.pop_back();
    m.joltage = AOC::readNumbers(jstr);
    for(auto& sb : token)
    {
        unsigned button = 0;
        for(auto idx : AOC::readNumbers(sb))
        {
            button |= 1U << idx;
        }
        m.button.push_back(button);
    }
    return m;
}

size_t Machine::minPress() const
{
    size_t btns = button.size();
    unsigned max = 1 << btns;
    size_t minbutton = btns;
    for(unsigned m = 0; m < max; m++)
    {
        unsigned u = 0;
        size_t count = 0;
        for(size_t b = 0; b < btns; b++)
        {
            if(m & (1 << b)) {u ^= button[b]; ++count;}
        }
        if(u == target)
        {
            minbutton = min(minbutton, count);
        }
    }
    return minbutton;
}

size_t Machine::minJolt() const
{
#ifdef DEBUG
    cerr << "=====\n";
#else
    cerr << "Running ";
    for(int j : joltage) cerr << j << "/";
    cerr << "\n";
#endif
    joltCache.clear();
    string z;
    for(int i = 0; i < joltage.size(); i++) z += "0/";
    joltCache[z] = 0;
    return minJoltImpl(joltage);
}

size_t Machine::minJoltImpl(const vector<int>& jolt) const
{
    // Implements solution from https://redd.it/1pk87hl
    string key;
    for(auto j : jolt) key += to_string(j) + "/";
    if(auto p = joltCache.find(key); p != joltCache.end())
    {
#ifdef DEBUG
        cerr << "Cache hit: " << key << " = " << p->second << "\n";
#endif
        return p->second;
    }
#ifdef DEBUG
    cerr << "Begin calculate " << key << "\n";
#endif
    unsigned target = 0;
    for(int i = 0 ; i < jolt.size(); i++) target |= (unsigned)(jolt[i] % 2) << i;
    size_t btns = button.size();
    unsigned max = 1 << btns;
    size_t minbutton = numeric_limits<size_t>::max();
    for(unsigned m = 0; m < max; m++)
    {
        size_t count = 0;
        vector<int> smalljolt = jolt;
        for(size_t b = 0; b < btns; b++)
        {
            if(m & (1 << b))
            {
                ++count;
                for(int i = 0; i < jolt.size(); i++)
                {
                    smalljolt[i] -= button[b] & (1 << i) ? 1 : 0;
                }
            }
        }
        bool canRecurse = true;
        for(int i = 0; i < jolt.size(); i++)
        {
            if(smalljolt[i] % 2 != 0 || smalljolt[i] < 0)
            {
                canRecurse = false;
                break;
            }
            smalljolt[i] /= 2;
        }
        if(canRecurse)
        {
#ifdef DEBUG
            cerr << key << " - ";
            for(int i = 0; i < btns; i++) cerr << (m & (1 << i) ? '1' : '0');
            cerr << " = ";
            for(int i = 0; i < jolt.size(); i++) cerr << smalljolt[i] << "/";
            cerr << "\n";
#endif
            size_t res = minJoltImpl(smalljolt);
            if(res < numeric_limits<size_t>::max())
            {
                res = res * 2 + count;
                minbutton = min(minbutton, res);
            }
        }
    }
#ifdef DEBUG
    cerr << "Calculate " << key << " = " << minbutton << "\n";
#endif
    return joltCache[key] = minbutton;
}

int main(int argc, char* argv[])
{
    bool partb = AOC::PartB(argv[0]);
    int total = 0;
    for(auto& m : AOC::readPerLineToVector<Machine>(cin))
    {
        if(partb)
            total += m.minJolt();
        else
            total += m.minPress();
    }
    cout << total << "\n";
}