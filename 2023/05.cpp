#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <functional>
#include <compare>
#include "aoc/aoc.h"
using namespace std;

struct Range
{
	int64_t begin, end;
	auto operator <=> (const Range& rhs) const {return begin <=> rhs.begin;}
	auto operator <=> (int64_t v) const
	{
		if(v < begin) return std::strong_ordering::greater;
		if(v >= end) return std::strong_ordering::less;
		return std::strong_ordering::equal;
	}
	bool isEmpty() const {return begin >= end;}
	Range intersect(const Range& rhs) const
	{
		return Range{max(begin, rhs.begin), min(end, rhs.end)};
	}
};

auto operator <=> (int64_t v, const Range& r) {return 0 <=> (r <=> v);}

struct MapperEntry
{
	Range r;
	int64_t offset;
	auto operator <=> (const MapperEntry& rhs) const {return r <=> rhs.r;}
	auto operator <=> (const int64_t v) const {return r <=> v;}
};

auto operator <=> (int64_t v, const MapperEntry& me) {return 0 <=> (me <=> v);}

class Mapper
{
	vector<MapperEntry> m;
public:
	void read(istream& in);
	int64_t map(int64_t src) const;
	vector<Range> map(Range r) const;
};

void Mapper::read(istream& in)
{
	string line;
	while(getline(in, line), in && line.size() != 0)
	{
		auto [dst, src, len] = AOC::takeFirst<3>(AOC::readNumbers<int64_t>(line));
		MapperEntry me;
		me.r.begin = src;
		me.r.end = src + len;
		me.offset = dst - src;
		m.push_back(me);
	}
	sort(begin(m), end(m));
}

int64_t Mapper::map(int64_t src) const
{
	auto p = lower_bound(begin(m), end(m), src);
	if(p != end(m) && (*p <=> src) == 0) return src + p->offset;
	return src;
}

vector<Range> Mapper::map(Range r) const
{
	vector<Range> ret;
	while(!r.isEmpty())
	{
		auto p = lower_bound(begin(m), end(m), r.begin);
		if(p == end(m))
		{
			ret.push_back(r);
			break;
		}
		else if((*p <=> r.begin) == 0)
		{
			Range lbint = r.intersect(p->r);
			ret.push_back(Range{lbint.begin + p->offset, lbint.end + p->offset});
			r.begin = lbint.end;
		}
		else
		{
			Range rangeEq {p == begin(m) ? 0 : (p-1)->r.end, p->r.begin};
			Range lbint = r.intersect(rangeEq);
			ret.push_back(lbint);
			r.begin = lbint.end;
		}
	}
	return ret;
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	string line;
	getline(cin, line);
	vector<int64_t> item = AOC::readNumbers<int64_t>(line);
	getline(cin, line);

	if(!partb)
	{
		while(getline(cin, line), cin)
		{
			Mapper m;
			m.read(cin);
			for(auto& it : item) it = m.map(it);
			//for(auto x : item) cerr << x << " ";
			//cerr << endl;
		}
		cout << *min_element(begin(item), end(item)) << endl;
	}
	else
	{
		vector<Range> itemB;
		for(size_t i = 0; i < item.size(); i += 2)
		{
			itemB.push_back(Range{item[i], item[i]+item[i+1]});
		}
		while(getline(cin, line), cin)
		{
			Mapper m;
			m.read(cin);
			vector<Range> itemBnext;
			for(auto& r : itemB)
			{
				auto rt = m.map(r);
				itemBnext.insert(itemBnext.end(), rt.begin(), rt.end());
			}
			itemB.swap(itemBnext);
			//for(auto& r : itemB) cerr << "[" << r.begin << "," << r.end << "); ";
			//cerr << endl;
		}
		sort(begin(itemB), end(itemB));
		cout << itemB[0].begin << endl;
	}

	return 0;
}

