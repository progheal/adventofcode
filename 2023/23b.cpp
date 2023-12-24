#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include "aoc/aoc.h"
#include "aoc/bfs.h"
#include "aoc/grid.h"
using namespace std;

unordered_map<AOC::Coord, unordered_multimap<AOC::Coord, int>> graph;
unordered_map<AOC::Coord, int> numeric;
map<int, map<int, int>> adj;
AOC::Coord START, END;

int getMap(AOC::Coord pos)
{
	auto p = numeric.find(pos);
	if(p != numeric.end())
		return p->second;
	else
	{
		int n = numeric.size();
		numeric[pos] = n;
		return n;
	}
}

class PathState
{
	AOC::Coord pos;
	AOC::Coord src;
public:
	static AOC::Grid<vector<string>>* g;
	typedef int cost_t;
	PathState(const AOC::Coord& p, const AOC::Coord& srcjunct)
		: pos(p), src(srcjunct) {}
	bool isFinish() const {return false;}
	int openings() const
	{
		return count_if(
			begin(AOC::Vector::VonNeumannNeighborhood),
			end(AOC::Vector::VonNeumannNeighborhood),
			[this](const AOC::Vector& dir)
			{
				return (*g)[pos + dir] != '#';
			});
	}
	vector<pair<PathState, int>> nextMoves() const
	{
		vector<pair<PathState, int>> ret;
		if(openings() == 2 || pos == src)
		{
			for(auto& dir : AOC::Vector::VonNeumannNeighborhood)
			{
				auto np = pos + dir;
				char n = (*g)[np];
				if(n == '#') continue;
				ret.push_back({PathState(np, src), 1});
			}
		}
		return ret;
	}
	auto id() const
	{
		return pos.hash();
	}
	void visit(int cost) const
	{
		if(openings() == 2 || pos == src) return;
		graph[src].insert({pos, cost});
	}
};

AOC::Grid<vector<string>>* PathState::g;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<string> pic = AOC::readPicture(cin);
	AOC::Grid g(pic);
	g.setOOB('#');
	PathState::g = &g;

	size_t height = pic.size();
	size_t dotN = pic[0].find('.');
	size_t dotS = pic[height-1].find('.');

	START = AOC::Coord{0, (int)dotN};
	END = AOC::Coord{(int)(height-1), (int)dotS};
	getMap(START);
	getMap(END);
	queue<AOC::Coord> junctions;
	junctions.push(START);

	while(!junctions.empty())
	{
		AOC::Coord junct = junctions.front();
		junctions.pop();
		if(graph.count(junct) > 0) continue;
		if(junct.x == height-1) continue;
		PathState start (junct, junct);
		AOC::BFS(start);
		int nsrc = getMap(junct);
		for(auto& [dest, cost] : graph[junct])
		{
			if(graph.count(dest) == 0) junctions.push(dest);
			int ndest = getMap(dest);
			adj[nsrc][ndest] = cost;
		}
	}

	for(auto& [pos, nadj] : adj)
	{
		cout << pos << ": ";
		for(auto& [dest, cost] : nadj)
		{
			cout << dest << " = " << cost << ", ";
		}
		cout << endl;
	}

	int longest = 0;
	map<int, map<uint64_t, int>> current, nextstep;
	current[0][1] = 0;
	int moves = 0;
	while(!current.empty())
	{
		++moves;
		cerr << "Move " << moves << endl;
		for(auto& [curr, past] : current)
		{
			if(curr == 1)
			{
				for(auto& [pastMask, spent] : past)
				{
					// cerr << curr << " " << hex << setw(9) << setfill('0') << pastMask << dec << " " << spent << endl;
					longest = max(longest, spent);
				}
			}
			else
			{
				for(auto& [pastMask, spent] : past)
				{
					// cerr << curr << " " << hex << setw(9) << setfill('0') << pastMask << dec << " " << spent << endl;
					for(auto& [dest, cost] : adj[curr])
					{
						if(pastMask & (1UL << dest)) continue;
						uint64_t nextMask = pastMask | (1UL << dest);
						nextstep[dest][nextMask] = max(nextstep[dest][nextMask], spent+cost);
					}
				}
			}
		}
		current.swap(nextstep);
		nextstep.clear();
	}
	cout << longest << endl;

	return 0;
}

