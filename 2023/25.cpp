#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <random>
#include "aoc/aoc.h"
using namespace std;

map<string, int> id;
map<int, vector<int>> adj;

int getId(string s)
{
	if(id.count(s)) return id[s];
	int c = id.size();
	return id[s] = c;
}

void dumpAdj(const map<int, vector<int>>& adj)
{
	for(auto [src, out] : adj)
	{
		cout << src << ":";
		for(auto dest : out)
		{
			cout << " " << dest;
		}
		cout << endl;
	}
	cout << endl;
}

// https://en.wikipedia.org/wiki/Karger%27s_algorithm
template<class Engine>
tuple<int, int, int> KargerTrial(map<int, vector<int>> adj, Engine& engine)
{
	map<int, int> nodeCount;

	for(auto& [src, vec] : adj)
	{
		nodeCount[src] = 1;
	}

	while(adj.size() > 2)
	{
		int k = uniform_int_distribution(0, (int)adj.size() - 1)(engine);
		auto p = next(begin(adj), k);
		int ns = p->first;
		k = uniform_int_distribution(0, (int)p->second.size() - 1)(engine);
		int nt = p->second[k];
		// replace all ns to nt
		for(auto& [src, vec] : adj)
		{
			if(src == nt)
			{
				auto p = remove_if(vec.begin(), vec.end(), [=](int x){return x == ns;});
				vec.resize(p - vec.begin());
			}
			else if(src == ns)
			{
				for(int i : vec)
				{
					if(i != nt) adj[nt].push_back(i);
				}
			}
			else
			{
				for(auto& e : vec) if(e == ns) e = nt;
			}
		}
		nodeCount[nt] += nodeCount[ns];
		nodeCount.erase(ns);
		adj.erase(p);
		//dumpAdj(adj);
	}
	int cutLen = adj.begin()->second.size();
	int na = nodeCount.begin()->second;
	int nb = next(nodeCount.begin())->second;
	return {cutLen, na, nb};
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	mt19937 engine;

	AOC::readPerLine(cin, [&](const string& s)
	{
		size_t colon = s.find(':');
		string left = s.substr(0, colon);
		vector<string> right = AOC::readToVector<string>(s.substr(colon+1));
		int lid = getId(left);
		for(auto& s : right)
		{
			int rid = getId(s);
			adj[lid].push_back(rid);
			adj[rid].push_back(lid);
		}
	});

	//dumpAdj(adj);

	while(true)
	{
		auto [l, a, b] = KargerTrial(adj, engine);
		cerr << l << " " << a << " " << b << endl;
		if(l == 3)
		{
			cout << a * b << endl;
			return 0;
		}
	}
}

