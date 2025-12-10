#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
#include "aoc/grid.h"
using namespace std;

int main(int argc, char* argv[])
{
    bool partb = AOC::PartB(argv[0]);
    vector<AOC::Coord> reds;
    string line;
    while(getline(cin, line))
    {
        AOC::Coord c;
        tie(c.x, c.y) = AOC::takeFirst<2>(AOC::readNumbers(line, true));
        reds.push_back(c);
    }
    int64_t maxsize = 0;
    for(int i = 0; i < reds.size(); i++)
    {
        for(int j = i+1; j < reds.size(); j++)
        {
            auto v = reds[i] - reds[j];
            int64_t area = (int64_t)(abs(v.x) + 1) * (int64_t)(abs(v.y) + 1);
            maxsize = max(maxsize, area);
        }
    }
    cout << maxsize << "\n";
}