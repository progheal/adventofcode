#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>
#include "aoc/aoc.h"
using namespace std;

struct Coord3D
{
    long x, y, z;
};

template<>
Coord3D AOC::convert<Coord3D>(const std::string &s)
{
    Coord3D ret;
    tie(ret.x, ret.y, ret.z) = AOC::takeFirst<3>(AOC::readNumbers(s, true));
    return ret;
}

int disjoint_head(vector<int>& disjoint, int node)
{
    if(disjoint[node] == node)
        return node;
    else
        return disjoint[node] = disjoint_head(disjoint, disjoint[node]);
}

int main(int argc, char* argv[])
{
    bool partb = AOC::PartB(argv[0]);
    int maxcount = 0;
    if(argc > 1) maxcount = atoi(argv[1]);
    if(maxcount == 0) maxcount = 1000;
    auto box = AOC::readPerLineToVector<Coord3D>(cin);
    size_t N = box.size();
    vector<tuple<long, int, int>> edge;
    edge.reserve(N*(N-1)/2);
    for(int i = 0; i < N; i++) for(int j = i+1; j < N; j++)
    {
        long vx = box[i].x - box[j].x,
             vy = box[i].y - box[j].y,
             vz = box[i].z - box[j].z;
        edge.push_back({vx*vx+vy*vy+vz*vz, i, j});
    }
    sort(edge.begin(), edge.end());
    vector<int> disjoint(N);
    iota(disjoint.begin(), disjoint.end(), 0);
    map<int, int> groupsize;
    for(int i = 0; i < N; i++) groupsize[i] = 1;
    int counter = 0;
    for(auto& [_, i, j] : edge)
    {
        int hi = disjoint_head(disjoint, i),
            hj = disjoint_head(disjoint, j);
        disjoint[hj] = hi;
        if(hi != hj)
        {
            groupsize[hi] += groupsize[hj];
            groupsize.erase(hj);
        }
        ++counter;
        if(!partb)
        {
            if(counter == maxcount)
            {
                vector<int> v;
                transform(groupsize.begin(), groupsize.end(), back_inserter(v),
                    [](auto& p){return p.second;});
                sort(v.begin(), v.end(), std::greater<>());
                cout << v[0] * v[1] * v[2] << "\n";
                return 0;
            }
        }
        else // partb
        {
            if(groupsize.size() == 1)
            {
                cout << box[i].x * box[j].x << "\n";
                return 0;
            }
        }
    }
}