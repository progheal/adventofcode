#include <iostream>
#include <vector>
#include "aoc/aoc.h"
#include "aoc/grid.h"
using namespace std;

int main(int argc, char* argv[])
{
    bool partb = AOC::PartB(argv[0]);
    auto in = AOC::readPicture(cin);
    AOC::Grid g (in, '.');
    vector<AOC::Coord> accessible;
    int total = 0;
    do
    {
        accessible.clear();
        for(auto p = g.begin(); p != g.end(); p++)
        {
            if(*p != '@') continue;
            int count = 0;
            for(auto v : AOC::Vector::MooreNeighborhood)
            {
                count += (g[p.position()+v] == '@');
            }
            if(count < 4) accessible.push_back(p.position());
        }
        if(!partb)
        {
            cout << accessible.size() << endl;
            return 0;
        }
        for(auto p : accessible)
        {
            g[p] = '.';
        }
        total += accessible.size();
    } while(accessible.size() != 0);
    cout << total << endl;
}