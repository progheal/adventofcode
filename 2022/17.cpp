#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	string wind;
	cin >> wind;
	int wsz = wind.size();

	vector<int> stones[5] = {
		{0x1E}, {0x08, 0x1C, 0x08}, {0x1C, 0x04, 0x04},
		{0x10, 0x10, 0x10, 0x10}, {0x18, 0x18}
	};
	int side[5][2] = {
		{-2, 1}, {-2, 2}, {-2, 2}, {-2, 4}, {-2, 3}
	};
	vector<int> field;
	int sp = 0, wp = 0;

	field.push_back(0x7F);

	auto stonefit = [&](int stone, int shift, int bp) -> bool
	{
		for(int r = 0; r < stones[stone].size(); r++)
		{
			int sv = shift > 0 ? stones[stone][r] >> shift : stones[stone][r] << -shift;
			if(bp+r < field.size() && (field[bp+r]&sv))
				return false;
		}
		return true;
	};

	vector<int> heights;
	map<pair<int,int>, vector<int>> drops;
	map<int, int> loopcount;

	while(true)
	{
		int bp = field.size() + 3;
		int shift = 0;
		bool drop = true;
		do
		{
			// wind
			switch(wind[wp])
			{
			case '<':
				if(shift != side[sp%5][0] && stonefit(sp%5, shift-1, bp))
					--shift;
				break;
			case '>':
				if(shift != side[sp%5][1] && stonefit(sp%5, shift+1, bp))
					++shift;
				break;
			}
			wp = (wp+1) % wsz;
			// drop
			drop = stonefit(sp%5, shift, bp-1);
			if(drop) --bp;
		} while(drop);
		int top = bp+stones[sp%5].size();
		if(field.size() < top) field.resize(top);
		for(int r = 0; r < stones[sp%5].size(); r++)
		{
			int sv = shift > 0 ? stones[sp%5][r] >> shift : stones[sp%5][r] << -shift;
			field[bp+r] |= sv;
		}
		++sp;
		heights.push_back(field.size()-1);

		auto check = make_pair((sp-1)%5, (wp-1+wsz)%wsz);
		if(partb && drops[check].size() != 0)
		{
			for(int x : drops[check])
			{
				int d = (sp-1)-x;
				loopcount[d]++;
				if(loopcount[d] >= 20)
				{
					//cerr << "Loop from " << x << " to " << (sp-1) << endl;
					int hd = heights[sp-1] - heights[x];
					int64_t TURNS = 1'000'000'000'000;
					int64_t loops = (TURNS - x) / d;
					int64_t inLoop = TURNS - loops * d;
					cout << heights[inLoop] + hd * loops - 1 << endl;
					return 0;
				}
			}
		}
		drops[check].push_back(sp-1);
		if(!partb && sp == 2022)
		{
			cout << field.size()-1 << endl;
			return 0;
		}
	}

	return 0;
}

