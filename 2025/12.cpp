#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
    bool partb = AOC::PartB(argv[0]);
    vector<vector<string>> pieces;
    vector<int> pcount;
    string line;
    while(getline(cin, line))
    {
        if(line.find('x') != string::npos) break;
        int index = stoi(line);
        pieces.push_back({});
        int c = 0;
        while(getline(cin, line), line != "")
        {
            pieces.back().push_back(line);
            for(char ch : line) c += ch == '#';
        }
        pcount.push_back(c);
    }
    int ok = 0, fail = 0, unknown = 0;
    do
    {
        auto spec = AOC::readNumbers<int>(line);
        int w = spec[0], h = spec[1];
        spec.erase(spec.begin(), spec.begin() + 2);
        int trivialCount = (w / 3) * (h / 3);
        int allCount = 0;
        int allPieces = 0;
        for(int i = 0; i < spec.size(); i++)
        {
            allCount += spec[i];
            allPieces += spec[i] * pcount[i];
        }
        if(allCount <= trivialCount) ok++;
        else if(allPieces > w * h) fail++;
        else unknown++;
    } while(getline(cin, line));
    cout << format("{} OK, {} Fail, {} Unknown\n", ok, fail, unknown);
    return 0;
}