#include <iostream>
#include <map>
#include <numeric>
#include <vector>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
    bool partb = AOC::PartB(argv[0]);
    auto manifold = AOC::readPicture(cin);
    map<int, int64_t> curr, next;
    curr[manifold[0].find('S')] = 1;
    int counter = 0;
    for(size_t i = 1; i < manifold.size(); i++)
    {
        for(auto j : curr)
        {
            if(manifold[i][j.first] == '^')
            {
                next[j.first-1] += j.second;
                next[j.first+1] += j.second;
                counter++;
            }
            else
                next[j.first] += j.second;
        }
        curr.swap(next);
        next.clear();
    }
    if(partb)
    {
        cout << reduce(curr.begin(), curr.end(), 0LL,
            [](int64_t sum, auto& p){return sum + p.second;})
            << "\n";
    }
    else
        cout << counter << endl;
}