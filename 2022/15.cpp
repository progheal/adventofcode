#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	vector<vector<int>> coords = AOC::readNumberMatrix(cin, true);

	auto rowExclusion = [&](int inspectY, bool withBeacon)
	{
		vector<pair<int, int>> exclusion;
		for(const vector<int>& row : coords)
		{
			auto [sx, sy, bx, by] = AOC::takeFirst<4>(row);
			int sbdist = abs(sx - bx) + abs(sy - by);
			int ydist = abs(sy - inspectY);
			int xdist = sbdist - ydist;
			if(xdist >= 0)
			{
				int rl = sx - xdist;
				int rr = sx + xdist;
				if(!withBeacon && by == inspectY)
				{
					if(rl == bx) rl++;
					else if(rr == bx) rr--;
				}
				exclusion.push_back({rl, rr});
			}
		}
		sort(exclusion.begin(), exclusion.end());
		vector<pair<int, int>> eret;
		for(auto [u, v]: exclusion)
		{
			if(eret.size() == 0)
				eret.push_back({u, v});
			else if(eret.back().second < u-1)
				eret.push_back({u, v});
			else if(eret.back().second < v)
				eret.back().second = v;
		}
		return eret;
	};

	if(!partb)
	{
		int inspectY = 2'000'000;
		if(argc > 1) inspectY = atoi(argv[1]);
		auto exclusion = rowExclusion(inspectY, false);
		int count = 0;
		for(auto& e : exclusion)
		{
			count += (e.second - e.first + 1);
		}
		cout << count << endl;
	}
	else
	{
		int range = 4'000'000;
		if(argc > 1) range = 2 * atoi(argv[1]);
		for(int y = 0; y <= range; y++)
		{
			auto exc = rowExclusion(y, true);
			auto p = find_if(exc.begin(), exc.end(),
				[](const auto& p){return p.second >= 0;});
			auto q = find_if(exc.begin(), exc.end(),
				[=](const auto& p){return p.first > range;});
			--q;
			if(p->first > 0)
			{
				cout << 0 * INT64_C(4'000'000) + y << endl;
				return 0;
			}
			else if(q->second < range)
			{
				cout << range * INT64_C(4'000'000) + y << endl;
				return 0;
			}
			else if(p != q)
			{
				int e = p->second + 1;
				cout << e * INT64_C(4'000'000) + y << endl;
				return 0;
			}
		}
	}

	return 0;
}

