#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
using namespace std;

struct Accumulator
{
    int64_t value;
    Accumulator(int64_t init) : value(init) {}
    virtual void add(int64_t) = 0;
    operator int64_t() const {return value;}
};

struct AccumulatorPlus : public Accumulator
{
    AccumulatorPlus() : Accumulator(0) {}
    void add(int64_t v) override {value += v;}
};

struct AccumulatorMultiply : public Accumulator
{
    AccumulatorMultiply() : Accumulator(1) {}
    void add(int64_t v) override {value *= v;}
};

int main(int argc, char* argv[])
{
    auto lines = AOC::readPerLineToVector<string>(cin);
    vector<Accumulator*> op;
    for(string s : AOC::readToVector<string>(lines.back()))
    {
        if(s == "+")
            op.push_back(new AccumulatorPlus());
        else
            op.push_back(new AccumulatorMultiply());
    }
    lines.pop_back();
    for(auto numstr : lines)
    {
        auto num = AOC::readToVector<int64_t>(numstr);
        transform(begin(op), end(op), begin(num), begin(op),
            [](Accumulator* acc, int64_t value){
                acc->add(value);
                return acc;
            });
    }
    int64_t sum = 0;
    for(auto acc : op)
    {
        sum += *acc;
    }
    cout << sum << "\n";
}