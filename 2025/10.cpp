#include <algorithm>
#include <iostream>
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
    int minJolt() const;
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

int Machine::minJolt() const
{
    // TODO
    return 0;
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