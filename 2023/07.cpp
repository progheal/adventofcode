#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <array>
#include "aoc/aoc.h"
using namespace std;

struct Hand
{
	int card;
	int type;
	int64_t bid;
};

map<char, int> cardRank = {
	{'A', 14}, {'K', 13}, {'Q', 12}, {'J', 11}, {'T', 10}, {'9', 9},
	{'8', 8}, {'7', 7}, {'6', 6}, {'5', 5}, {'4', 4}, {'3', 3}, {'2', 2}
};

int main(int argc, char* argv[])
{
	bool joker = AOC::PartB(argv[0]);

	if(joker) cardRank['J'] = 1;

	vector<Hand> hands;
	AOC::readPerLine(cin, [&](const string& line)
	{
		Hand h = {};
		array<int, 15> tally = {};
		for(int i = 0; i < 5; i++)
		{
			int rank = cardRank[line[i]];
			h.card = h.card * 16 + rank;
			tally[rank]++;
		}
		h.bid = stol(line.substr(6));
		if(!joker)
		{
			sort(tally.begin(), tally.end(), std::greater<int>());
		}
		else
		{
			int j = tally[1];
			tally[1] = 0;
			sort(tally.begin(), tally.end(), std::greater<int>());
			tally[0] += j;
		}
		int sig = 0;
		for(int i = 0; i < 5; i++) sig = sig * 10 + tally[i];
		h.type = sig;
		hands.push_back(h);
	});

	sort(hands.begin(), hands.end(), [](const Hand& h1, const Hand& h2)
	{
		if(h1.type != h2.type) return h1.type < h2.type;
		return h1.card < h2.card;
	});

	int order = 1;
	int64_t sum = 0;
	for(auto& h : hands)
	{
		//cerr << hex << h.card << " " << dec << h.type << " " << h.bid << endl;
		sum += order * h.bid;
		++order;
	}
	cout << dec << sum << endl;

	return 0;
}

