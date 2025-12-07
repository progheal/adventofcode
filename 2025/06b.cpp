#include <functional>
#include <iostream>
#include <numeric>
#include <vector>
#include <string>
#include "aoc/aoc.h"
#include "aoc/util.h"
using namespace std;

int main(int argc, char* argv[])
{
    auto input = AOC::readPicture(cin);
    auto rot = AOC::rotateCCW(input);
    vector<int64_t> val;
    int64_t sum = 0;
    for(auto& s : rot)
    {
        int64_t v;
        try { v = stoll(s); } catch(...) {continue;}
        val.push_back(v);
        switch(s.back())
        {
        case '+':
            sum += reduce(val.begin(), val.end(), 0);
            val.clear();
            break;
        case '*':
            sum += reduce(val.begin(), val.end(), 1LL, std::multiplies<int64_t>());
            val.clear();
            break;
        }
    }
    cout << sum << "\n";
}