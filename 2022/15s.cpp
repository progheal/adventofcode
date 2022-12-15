#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <numeric>
#include <algorithm>
#include "aoc/aoc.h"
using namespace std;

struct Diamond
{
	int umin, umax, vmin, vmax;
};

int main(int argc, char* argv[])
{
	int arg = 2'000'000;
	if(argc > 1) arg = atoi(argv[1]);

	vector<vector<int>> coords = AOC::readNumberMatrix(cin, true);
	vector<Diamond> d;
	/* u = x-y
	 * v = x+y
	 *
	 * |x-x0|+|y-y0|<=d
	 * => |u-u0|<=d && |v-v0|<=d
	 */
	transform(coords.begin(), coords.end(), back_inserter(d),
		[](const vector<int>& v)
		{
			auto [sx, sy, bx, by] = AOC::takeFirst<4>(v);
			int dis = abs(sx-bx) + abs(sy-by);
			return Diamond{sx-sy-dis, sx-sy+dis, sx+sy-dis, sx+sy+dis};
		});
	
	// Sweep on U axis
	// The order when a sensor entering sweep or leaving sweep
	// Positive value is entering, negative value is leaving
	vector<int> dorder(d.size() * 2);
	iota(dorder.begin(), dorder.end(), -d.size());
	sort(dorder.begin(), dorder.end(),
		[&](int di, int dj)
		{
			int vi = di >= 0 ? d[di].umin : (d[~di].umax+1);
			int vj = dj >= 0 ? d[dj].umin : (d[~dj].umax+1);
			if(vi == vj)
				return di > dj;
			else
				return vi < vj;
		});
	
	// Sort by V range
	auto rowcomp = [&](int di, int dj)
	{
		if(d[di].vmin == d[dj].vmin)
			return d[di].vmax < d[dj].vmax;
		else
			return d[di].vmin < d[dj].vmin;
	};
	set<int, decltype(rowcomp)> sd (rowcomp);
	// Process in the U sweep order
	for(int x : dorder)
	{
		if(x >= 0)
		{
			// Entering sweep
			sd.insert(x);
		}
		else
		{
			// Leaving sweep
			sd.erase(~x);
			// Leaving U value
			int u = d[~x].umax + 1;
			// Scan for gap, keep a current max covered
			int vvmax = d[*sd.begin()].vmin;
			for(auto px : sd)
			{
				// next V range extends
				if(vvmax + 1 >= d[px].vmin)
				{
					vvmax = max(vvmax, d[px].vmax);
				}
				else
				{
					// next V range has a gap, this gap is our value
					int v = vvmax + 1;
					int x = (u+v)/2, y = (v-u)/2;
					if(x >= 0 && x <= arg*2 && y >= 0 && y <= arg*2)
					{
						cerr << "u = " << u << ", v = " << v << endl;
						cerr << "x = " << x << ", y = " << y << endl;
						cout << x * INT64_C(4'000'000) + y << endl;
						return 0;
					}
				}
			}
		}
	}

	return 0;
}
