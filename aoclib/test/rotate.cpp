#include <iostream>
#include <vector>
#include <string>
#include "aoc/util.h"
using namespace std;

void print(const vector<string>& vs)
{
    for(auto& s : vs)
    {
        cout << "[" << s << "]" << endl;
    }
    cout << endl;
}

int main()
{
    vector<string> input1 = {
        "ABCDE",
        "FGHIJ",
        "KLMNO",
        "PQRST",
        "UVWXY",
        "abcde",
        "fghij",
        "klmno",
        "pqrst",
        "uvwxy"
    };

    vector<string> input2 = {
        "*",
        "**",
        "***",
        "****",
        "*****"
    };

    print(AOC::rotateCW(input1));
    print(AOC::rotate180(input1));
    print(AOC::rotateCCW(input1));
    print(AOC::rotateCW(input2));
    print(AOC::rotate180(input2));
    print(AOC::rotateCCW(input2));
    return 0;
}