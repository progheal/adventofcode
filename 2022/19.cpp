#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "aoc/aoc.h"
using namespace std;

struct BluePrint
{
	int id;
	int oreCostOre;
	int clayCostOre;
	int obsidianCostOre;
	int obsidianCostClay;
	int geodeCostOre;
	int geodeCostObsidian;
};

int project(int item, int bot, int target)
{
	if(bot == 0) return INT_MAX;
	if(target <= item) return 0;
	return (target - item + (bot - 1)) / bot;
}

struct GameState
{
	int ore, clay, obsidian, geode;
	int oreBot, clayBot, obsidianBot, geodeBot;
	void advance(int turn)
	{
		ore += oreBot * turn;
		clay += clayBot * turn;
		obsidian += obsidianBot * turn;
		geode += geodeBot * turn;
	}
	bool noWorseThan(const GameState& gs) const
	{
		return ore >= gs.ore && oreBot >= gs.oreBot &&
			clay >= gs.clay && clayBot >= gs.clayBot &&
			obsidian >= gs.obsidian && obsidianBot >= gs.obsidianBot &&
			geode >= gs.geode && geodeBot >= gs.geodeBot;
	}
};

ostream& operator << (ostream& out, const GameState& gs)
{
	out << 'O' << gs.ore << '+' << gs.oreBot
		<< 'C' << gs.clay << '+' << gs.clayBot
		<< 'B' << gs.obsidian << '+' << gs.obsidianBot
		<< 'G' << gs.geode << '+' << gs.geodeBot;
	return out;
}

int simulate(const BluePrint& bp, int time)
{
	map<int, vector<GameState>> gsm;
	vector<GameState> visited;

	int maxgeode = 0;
	gsm[time].push_back({0, 0, 0, 0, 1, 0, 0, 0});
	for(int t = time; t >= 0; t--)
	{
		visited.clear();
		sort(gsm[t].begin(), gsm[t].end(),
			[](const GameState& gs1, const GameState& gs2)
			{
				if(gs1.geodeBot != gs2.geodeBot)
					return gs1.geodeBot < gs2.geodeBot;
				else if(gs1.obsidianBot != gs2.obsidianBot)
					return gs1.obsidianBot < gs2.obsidianBot;
				else if(gs1.clayBot != gs2.clayBot)
					return gs1.clayBot < gs2.clayBot;
				else
					return gs1.oreBot < gs2.oreBot;
			});
		int geodeBotNum = gsm[t].empty() ? 0 : gsm[t].back().geodeBot;
		cerr << "t = " << t << ", qsize = " << gsm[t].size() << ", gbn = " << geodeBotNum;
		while(!gsm[t].empty() && gsm[t].back().geodeBot >= geodeBotNum - 1)
		{
			GameState gs = gsm[t].back();
			gsm[t].pop_back();
			if(any_of(visited.begin(), visited.end(),
				[&](const GameState& vgs){return vgs.noWorseThan(gs);}))
				continue;
			visited.push_back(gs);
			//cerr << gs << 'T' << t << endl;
			int projectGeodeBot = max(project(gs.ore, gs.oreBot, bp.geodeCostOre),
									project(gs.obsidian, gs.obsidianBot, bp.geodeCostObsidian));
			if(projectGeodeBot < t - 1)
			{
				GameState ngs = gs;
				ngs.advance(projectGeodeBot + 1);
				ngs.ore -= bp.geodeCostOre;
				ngs.obsidian -= bp.geodeCostObsidian;
				ngs.geodeBot++;
				gsm[t-projectGeodeBot-1].push_back(ngs);
			}
			int projectObsidianBot = max(project(gs.ore, gs.oreBot, bp.obsidianCostOre),
										project(gs.clay, gs.clayBot, bp.obsidianCostClay));
			if(projectObsidianBot < t - 1)
			{
				GameState ngs = gs;
				ngs.advance(projectObsidianBot + 1);
				ngs.ore -= bp.obsidianCostOre;
				ngs.clay -= bp.obsidianCostClay;
				ngs.obsidianBot++;
				gsm[t-projectObsidianBot-1].push_back(ngs);
			}
			int projectClayBot = project(gs.ore, gs.oreBot, bp.clayCostOre);
			if(projectClayBot < t - 1)
			{
				GameState ngs = gs;
				ngs.advance(projectClayBot + 1);
				ngs.ore -= bp.clayCostOre;
				ngs.clayBot++;
				gsm[t-projectClayBot-1].push_back(ngs);
			}
			int projectOreBot = project(gs.ore, gs.oreBot, bp.oreCostOre);
			if(projectOreBot < t - 1)
			{
				GameState ngs = gs;
				ngs.advance(projectOreBot + 1);
				ngs.ore -= bp.oreCostOre;
				ngs.oreBot++;
				gsm[t-projectOreBot-1].push_back(ngs);
			}
			gs.advance(t);
			maxgeode = max(maxgeode, gs.geode);
		}
		cerr << ", vis = " << visited.size() << endl;
	}
	return maxgeode;
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<BluePrint> vbp;
	AOC::readPerLineNumbers(cin,
		[&](const vector<int>& v){
			vbp.push_back({v[0], v[1], v[2], v[3], v[4], v[5], v[6]});
		});

	if(!partb)
	{
		int qsum = 0;
		for(auto& bp : vbp)
		{
			int geode = simulate(bp, 24);
			qsum += geode * bp.id;
			cerr << bp.id << " " << geode << endl;
		}
		cout << qsum << endl;
	}
	else
	{
		int g1 = simulate(vbp[0], 32);
		cerr << "1 " << g1 << endl;
		int g2 = simulate(vbp[1], 32);
		cerr << "2 " << g2 << endl;
		int g3 = simulate(vbp[2], 32);
		cerr << "3 " << g3 << endl;
		cout << g1 * g2 * g3 << endl;
	}

	return 0;
}

