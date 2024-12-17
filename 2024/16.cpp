#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "aoc/aoc.h"
#include "aoc/bfs.h"
#include "aoc/grid.h"
using namespace std;

struct Global
{
    AOC::Grid<vector<string>>* g;
	map<int, vector<AOC::Mob>, std::greater<int>> record;
	auto operator [] (const AOC::Coord& p)
	{
		return (*g)[p];
	}
} G;

struct State
{
	AOC::Mob mob;
public:
	State(const AOC::Mob& mob) : mob(mob) {}
	typedef int cost_t;
	AOC::Mob& get() {return mob;}
	bool isFinish() const {return G[mob] == 'E';}
	auto id() const {return mob.hash();}
	auto nextMoves() const
	{
		vector<pair<State, int>> ret;
		AOC::Mob newmob = mob;
		newmob.CW90();
		ret.push_back({newmob, 1000});
		newmob.Rotate180();
		ret.push_back({newmob, 1000});
		newmob.CW90();
		newmob.move();
		if(G[mob] != '#')
			ret.push_back({newmob, 1});
		return ret;
	}
	int estimateRemain() const {return 0;}
	void visit(int cost) const
	{
		G.record[cost].push_back(mob);
	}
};

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	auto v = AOC::readPicture(cin);
	AOC::Grid g (v);
	G.g = &g;

	AOC::Coord s = g.find_first_of('S');
	int cost = AOC::AStar(State{AOC::Mob{s, AOC::Vector::EAST}});
	if(!partb)
	{
		cout << cost << "\n";
	}
	else
	{
		int count = 0;
		unordered_set<AOC::Coord> locations;
		for(auto pRec = G.record.begin(); pRec != G.record.end(); pRec++)
		{
			//cerr << "Cost " << pRec->first << "\n";
			for(auto pMob = pRec->second.begin(); pMob != pRec->second.end(); )
			{
				int hasMove = 0;
				//cerr << "Checking " << *pMob << ": ";
				auto next = State{*pMob}.nextMoves();
				if(g[*pMob] != 'E' && none_of(next.begin(), next.end(), [&](auto& p)
					{
						auto& v = G.record[pRec->first + p.second];
						return find(v.begin(), v.end(), p.first.get()) != v.end();
					}))
				{
					//cerr << "Erase\n";
					pMob = pRec->second.erase(pMob);
				}
				else
				{
					//cerr << "Keep\n";
					locations.insert(*pMob);
					++pMob;
				}
			}
		}
		cout << locations.size() << "\n";
	}

	return 0;
}

