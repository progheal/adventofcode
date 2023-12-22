#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <iterator>
#include "aoc/aoc.h"
using namespace std;

struct Brick
{
	int x1, y1, z1, x2, y2, z2;
};

namespace AOC
{
template<>
Brick convert<Brick>(const string& s)
{
	Brick b;
	tie(b.x1, b.y1, b.z1, b.x2, b.y2, b.z2) = AOC::takeFirst<6>(AOC::readNumbers(s));
	return b;
}
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<Brick> bricks = AOC::readPerLineToVector<Brick>(cin);

	sort(begin(bricks), end(bricks), [](const Brick& b1, const Brick& b2)
	{
		if(b1.z1 != b2.z1) return b1.z1 < b2.z1;
		if(b1.x1 != b2.x1) return b1.x1 < b2.x1;
		if(b1.y1 != b2.y1) return b1.y1 < b2.y1;
		return false;
	});

	struct _hmap
	{
		int height;
		int brick;
		_hmap() : height(0), brick(-1) {}
	};
	map<int, map<int, _hmap>> hmap;
	vector<set<int>> landsOn (bricks.size()), supports (bricks.size());
	for(size_t bid = 0; bid < bricks.size(); bid++)
	{
		auto& br = bricks[bid];
		int highest = 0;
		set<int>& below = landsOn[bid];
		for(int i = br.x1; i <= br.x2; i++) for(int j = br.y1; j <= br.y2; j++)
		{
			if(highest < hmap[i][j].height)
			{
				highest = hmap[i][j].height;
				below.clear();
				below.insert(hmap[i][j].brick);
			}
			else if(highest == hmap[i][j].height)
			{
				if(hmap[i][j].brick >= 0)
					below.insert(hmap[i][j].brick);
			}
		}
		int distance = br.z1 - highest - 1;
		int top = br.z2 - distance;
		for(int i = br.x1; i <= br.x2; i++) for(int j = br.y1; j <= br.y2; j++)
		{
			hmap[i][j].height = top;
			hmap[i][j].brick = bid;
		}
		for(auto id : below)
		{
			supports[id].insert(bid);
		}
	}

	if(!partb)
	{
		int count = 0;
		for(int i = 0; i < bricks.size(); i++)
		{
			bool mayFall = false;
			for(auto sup : supports[i])
			{
				if(landsOn[sup].size() == 1)
				{
					mayFall = true;
					break;
				}
			}
			if(!mayFall) count++;
		}
		cout << count << endl;
	}
	else
	{
		int csum = 0;
		vector<set<int>> fallIf (bricks.size());
		for(int i = 0; i < bricks.size(); i++)
		{
			if(landsOn[i].size() == 0) continue;
			int init = -1;
			set<int>& x = fallIf[i];
			for(auto& s : landsOn[i])
			{
				if(init == -1) {x = fallIf[s]; init = s; continue;}
				set<int> y;
				set_intersection(x.begin(), x.end(),
					fallIf[s].begin(), fallIf[s].end(),
					inserter(y, end(y)));
				x.swap(y);
				if(x.size() == 0) break;
			}
			if(landsOn[i].size() == 1) x.insert(init);
			csum += fallIf[i].size();
		}
		cout << csum << endl;
	}

	return 0;
}

