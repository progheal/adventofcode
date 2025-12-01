#include <iostream>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
    bool partb = AOC::PartB(argv[0]);
    int count = 0;
    int point = 50, prev;
    string line;
    while(getline(cin, line))
    {
        prev = point;
        int n = stoi(line.substr(1));
        if(!partb)
        {
            if(line[0] == 'L')
                point -= n;
            else
                point += n;
            if(point % 100 == 0) count++;
        }
        else
        {
            if(line[0] == 'L')
            {
                point -= n;
                if(point <= 0)
                {
                    if(prev != 0) ++count;
                    int q = (-point) / 100;
                    point += 100 * q;
                    count += q;
                    if(point < 0) point += 100;
                }
            }
            else
            {
                point += n;
                if(point >= 100)
                {
                    int q = point / 100;
                    point -= 100*q;
                    count += q;
                }
            }
        }
    }
    cout << count << "\n";
}