#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
    bool partb = AOC::PartB(argv[0]);
    vector<pair<int64_t, int64_t>> ranges;
    string line;
    while(getline(cin, line))
    {
        if(line.size() == 0) break;
        ranges.push_back(AOC::takeFirst<2>(AOC::readNumbers<int64_t>(line)));
    }
    if(!partb)
    {
        int count = 0;
        int64_t item;
        while(cin >> item)
        {
            if(any_of(ranges.begin(), ranges.end(),
                [item](auto p){return p.first <= item && item <= p.second;}))
                ++count;
        }
        cout << count << endl;
    }
    else
    {
        sort(ranges.begin(), ranges.end());
        for(auto p = ranges.begin(), q = p; (q = next(p)) != ranges.end(); )
        {
            if(p->second < q->first)
                ++p;
            else
            {
                p->second = max(p->second, q->second);
                ranges.erase(q);
            }
        }
        int64_t count = 0;
        for(auto [a, b] : ranges)
        {
            count += b-a+1;
        }
        cout << count << endl;
    }
}