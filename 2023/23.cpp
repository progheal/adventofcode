#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "aoc/aoc.h"
#include "aoc/bfs.h"
#include "aoc/grid.h"
using namespace std;

unordered_map<AOC::Coord, unordered_map<AOC::Coord, int>> dag;
unordered_map<AOC::Vector, char> slope2c {
	{AOC::Vector::NORTH, '^'},
	{AOC::Vector::SOUTH, 'v'},
	{AOC::Vector::EAST, '>'},
	{AOC::Vector::WEST, '<'}
};
unordered_map<char, AOC::Vector> c2slope {
	{'^', AOC::Vector::NORTH},
	{'v', AOC::Vector::SOUTH},
	{'>', AOC::Vector::EAST},
	{'<', AOC::Vector::WEST}
};

class State
{
	AOC::Coord pos;
	AOC::Coord src;
public:
	static AOC::Grid<vector<string>>* g;
	typedef int cost_t;
	State(const AOC::Coord& p, const AOC::Coord& srcSlope)
		: pos(p), src(srcSlope) {}
	bool isFinish() const {return false;}
	vector<pair<State, int>> nextMoves() const
	{
		vector<pair<State, int>> ret;
		if((*g)[pos] == '.')
		{
			for(auto& dir : AOC::Vector::VonNeumannNeighborhood)
			{
				auto np = pos + dir;
				char n = (*g)[np];
				if(n == '#') continue;
				if(c2slope[n] == -dir) continue;
				ret.push_back({State(np, src), 1});
			}
		}
		else if(pos == src)
		{
			auto dir = c2slope[(*g)[pos]];
			ret.push_back({State(pos + dir, src), 1});
		}
		return ret;
	}
	auto id() const
	{
		return pos.hash();
	}
	void visit(int cost) const
	{
		if((*g)[pos] == '.') return;
		if(pos == src) return;
		dag[src][pos] = cost;
	}
};

AOC::Grid<vector<string>>* State::g;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<string> pic = AOC::readPicture(cin);
	AOC::Grid g(pic);
	g.setOOB('#');
	State::g = &g;

	size_t height = pic.size();
	size_t dotN = pic[0].find('.');
	pic[0][dotN] = 'v';
	size_t dotS = pic[height-1].find('.');
	pic[height-1][dotS] = 'v';

	AOC::Coord START (0, (int)dotN);
	AOC::Coord END (height-1, (int)dotS);
	queue<AOC::Coord> slopes;
	slopes.push(START);
	unordered_map<AOC::Coord, int> incident;
	incident[START] = 0;

	while(!slopes.empty())
	{
		AOC::Coord slope = slopes.front();
		slopes.pop();
		if(dag.count(slope) > 0) continue;
		if(slope.x == height-1) continue;
		State start (slope, slope);
		AOC::BFS(start);
		for(auto& [dest, cost] : dag[slope])
		{
			slopes.push(dest);
			incident[dest]++;
		}
	}

	vector<AOC::Coord> topo;
	while(!incident.empty())
	{
		auto p = find_if(incident.begin(), incident.end(),
			[](auto& p){return p.second == 0;});
		topo.push_back(p->first);
		for(auto& [dest, cost] : dag[p->first])
		{
			incident[dest]--;
		}
		incident.erase(p);
	}

	unordered_map<AOC::Coord, int> longest;
	longest[START] = 0;
	for(auto& c : topo)
	{
		int curr = longest[c];
		for(auto& [dest, cost] : dag[c])
		{
			longest[dest] = max(longest[dest], curr + cost);
		}
	}
	cout << longest[END] << endl;

	return 0;
}

