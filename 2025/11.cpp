#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
    bool partb = AOC::PartB(argv[0]);
    map<string, set<string>> graph;
    map<string, int> ind;
    string line;
    while(getline(cin, line))
    {
        auto token = AOC::readToVector<string>(line, ' ');
        string src = token[0];
        src.pop_back();
        token.erase(token.begin());
        for(auto d : token)
        {
            graph[src].insert(d);
            ind[src];
            ind[d]++;
        }
    }
    // topo sort
    if(!partb)
    {
        map<string, int64_t> paths;
        paths["you"] = 1;
        while(!ind.empty())
        {
            auto p = find_if(ind.begin(), ind.end(), [](auto& p){return p.second == 0;});
            string curr = p->first;
            if(curr == "out")
            {
                cout << paths[curr] << "\n";
                return 0;
            }
            ind.erase(p);
            for(auto& s : graph[curr])
            {
                paths[s] += paths[curr];
                ind[s]--;
            }
        }
    }
    else
    {
        map<string, tuple<int64_t, int64_t, int64_t, int64_t>> paths;
        paths["svr"] = {1, 0, 0, 0};
        while(!ind.empty())
        {
            auto p = find_if(ind.begin(), ind.end(), [](auto& p){return p.second == 0;});
            string curr = p->first;
            if(curr == "out")
            {
                cout << get<3>(paths[curr]) << "\n";
                return 0;
            }
            ind.erase(p);
            if(curr == "dac")
            {
                for(auto& s : graph[curr])
                {
                    get<2>(paths[s]) += get<0>(paths[curr]);
                    get<3>(paths[s]) += get<1>(paths[curr]);
                    get<2>(paths[s]) += get<2>(paths[curr]);
                    get<3>(paths[s]) += get<3>(paths[curr]);
                    ind[s]--;
                }
            }
            else if(curr == "fft")
            {
                for(auto& s : graph[curr])
                {
                    get<1>(paths[s]) += get<0>(paths[curr]);
                    get<1>(paths[s]) += get<1>(paths[curr]);
                    get<3>(paths[s]) += get<2>(paths[curr]);
                    get<3>(paths[s]) += get<3>(paths[curr]);
                    ind[s]--;
                }
            }
            else
            {
                for(auto& s : graph[curr])
                {
                    get<0>(paths[s]) += get<0>(paths[curr]);
                    get<1>(paths[s]) += get<1>(paths[curr]);
                    get<2>(paths[s]) += get<2>(paths[curr]);
                    get<3>(paths[s]) += get<3>(paths[curr]);
                    ind[s]--;
                }
            }
        }
    }
}