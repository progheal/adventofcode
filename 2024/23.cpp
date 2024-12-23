#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	set<string> n;
	map<string, set<string>> g;
	for(auto& line : AOC::LineReader(cin))
	{
		auto v = AOC::readToVector<string>(line, '-');
		n.insert(v[0]);
		n.insert(v[1]);
		g[v[0]].insert(v[1]);
		g[v[1]].insert(v[0]);
	}
	if(!partb)
	{
		int count = 0;
		for(auto& [n, c] : g)
		{
			for(auto& u : c)
			for(auto& v : c)
			{
				if(g[u].count(v) != 0)
				{
					if(n[0] == 't' || u[0] == 't' || v[0] == 't') ++count;
				}
			}
		}
		cout << count / 6 << "\n";
	}
	else
	{
		vector<set<string>> maxcliques = {{}};
		auto BronKerbosch = [&](auto self, set<string>& R, set<string>& P, set<string>& X) -> void
		{
			if(P.size() == 0 && X.size() == 0)
			{
				if(R.size() > maxcliques[0].size())
				{
					maxcliques = {R};
				}
				else if(R.size() == maxcliques[0].size())
				{
					maxcliques.push_back(R);
				}
				return;
			}
			string pivot;
			size_t ps = P.size() + 1;
			for(auto& v : P)
			{
				size_t count = count_if(P.begin(), P.end(), [&](auto& w){return g[v].count(w) == 0;});
				if(count < ps) { ps = count; pivot = v; }
			}
			for(auto& v : X)
			{
				size_t count = count_if(P.begin(), P.end(), [&](auto& w){return g[v].count(w) == 0;});
				if(count < ps) { ps = count; pivot = v; }
			}
			for(auto vp = P.begin(); vp != P.end(); )
			{
				string v = *vp;
				if(g[pivot].count(v) > 0)
				{
					++vp; continue;
				}
				set<string> NP, NX;
				R.insert(v);
				auto pP = P.begin(), pX = X.begin();
				for(auto& u : g[v])
				{
					while(pP != P.end() && *pP < u) ++pP;
					if(pP != P.end() && *pP == u) NP.insert(u);
					while(pX != X.end() && *pX < u) ++pX;
					if(pX != X.end() && *pX == u) NX.insert(u);
				}
				self(self, R, NP, NX);
				R.erase(v);
				X.insert(v);
				vp = P.erase(vp);
			}
		};
		set<string> R, P = n, X;
		BronKerbosch(BronKerbosch, R, P, X);
		for(auto& q : maxcliques)
		{
			cout << AOC::join(q.begin(), q.end(), ",") << "\n";
		}
	}

	return 0;
}

