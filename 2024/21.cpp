#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "aoc/aoc.h"
#include "aoc/cache.h"
#include "aoc/grid.h"
using namespace std;

typedef map<char, AOC::Coord> PadType;
typedef array<array<int64_t, 5>, 5> DPType;
const string dirDPOrder = "<^>vA";

PadType numpad = {
	{'7', AOC::Coord{0, 0}},
	{'8', AOC::Coord{0, 1}},
	{'9', AOC::Coord{0, 2}},
	{'4', AOC::Coord{1, 0}},
	{'5', AOC::Coord{1, 1}},
	{'6', AOC::Coord{1, 2}},
	{'1', AOC::Coord{2, 0}},
	{'2', AOC::Coord{2, 1}},
	{'3', AOC::Coord{2, 2}},
	{'X', AOC::Coord{3, 0}},
	{'0', AOC::Coord{3, 1}},
	{'A', AOC::Coord{3, 2}},
};

PadType dirpad = {
	{'X', AOC::Coord{0, 0}},
	{'^', AOC::Coord{0, 1}},
	{'A', AOC::Coord{0, 2}},
	{'<', AOC::Coord{1, 0}},
	{'v', AOC::Coord{1, 1}},
	{'>', AOC::Coord{1, 2}},
};

map<char, AOC::Vector> moves = {
	{'^', AOC::Vector::NORTH},
	{'>', AOC::Vector::EAST},
	{'<', AOC::Vector::WEST},
	{'v', AOC::Vector::SOUTH}
};

vector<string> getPossibleMoves_impl(const AOC::Coord& cf, const AOC::Coord& ct, const AOC::Coord& forbid)
{
	vector<string> comb;
	string ud = cf.x > ct.x ? string(cf.x - ct.x, '^') : string(ct.x - cf.x, 'v');
	string lr = cf.y > ct.y ? string(cf.y - ct.y, '<') : string(ct.y - cf.y, '>');
	string s = lr + ud; // '<' and '>' are smaller than '^' and 'v'
	do
	{
		AOC::Coord trace = cf;
		bool ok = true;
		for(char c : s)
		{
			trace += moves[c];
			if(trace == forbid) ok = false;
		}
		if(ok) comb.push_back(s+"A");
	} while (next_permutation(s.begin(), s.end()));
	return comb;
}

auto getPossibleMoves = stringkey_cache(getPossibleMoves_impl);

int64_t getLength(const DPType& dirTable, const string& seq)
{
	char curr = 'A';
	int64_t len = 0;
	for(char c : seq)
	{
		size_t fi = dirDPOrder.find(curr), fj = dirDPOrder.find(c);
		len += dirTable[fi][fj];
		curr = c;
	}
	return len;
}

int64_t moveCount(const DPType& dp, const PadType& pad, char from, char to)
{
	int64_t ret = std::numeric_limits<int64_t>::max();
	AOC::Coord cf = pad.at(from), ct = pad.at(to), forbid = pad.at('X');
	for(auto& s : getPossibleMoves(cf, ct, forbid))
	{
		int64_t len = getLength(dp, s);
		if(len < ret) ret = len;
	}
	return ret;
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	DPType dp = {
		1LL, 1LL, 1LL, 1LL, 1LL,
		1LL, 1LL, 1LL, 1LL, 1LL,
		1LL, 1LL, 1LL, 1LL, 1LL,
		1LL, 1LL, 1LL, 1LL, 1LL,
		1LL, 1LL, 1LL, 1LL, 1LL
	}, dpNext;

	int loopN = partb ? 25 : 2;
	for(int loop = 0; loop < loopN; loop++)
	{
		for(int i = 0; i < 5; i++)
		for(int j = 0; j < 5; j++)
		{
			dpNext[i][j] = moveCount(dp, dirpad, dirDPOrder[i], dirDPOrder[j]);
		}
		/*
		cerr << "Loop " << loop << ":\n";
		for(auto& row : dpNext)
		{
			for(auto x : row) cerr << x << " ";
			cerr << "\n";
		}
		*/
		swap(dp, dpNext);
	}
	int64_t total = 0;
	for(const string& line : AOC::LineReader(cin))
	{
		int curr = 'A';
		int64_t len = 0;
		for(char c : line)
		{
			len += moveCount(dp, numpad, curr, c);
			curr = c;
		}
		// cerr << line << " " << len << "\n";
		total += len * stol(line);;
	}
	cout << total << "\n";

	return 0;
}

