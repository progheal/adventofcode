#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <string>
#include <bitset>
#include "aoc/aoc.h"
#include "aoc/bfs.h"
using namespace std;

vector<vector<int>> adj;
map<string, int> name2id;
map<int, int> value;

struct State
{
	// 還有還沒開的閥門的點
	// Valves that haven't opened
	vector<int> available;
	// 我們的所在及剩餘時間
	// Where we are and how many time we have
	int meNow;
	int meTimeLeft;
	// 大象的所在及剩餘時間
	// Where elephant is and how many time it have
	int eleNow;
	int eleTimeLeft;
	// 估計值快取，因為計算很花時間
	// Cache for estimate value since computing it is costly
	mutable int estimateCache;

	State(int begin, int limit, bool partb)
		: available(),
		  meNow(begin), meTimeLeft(partb ? limit-4 : limit),
		  eleNow(partb ? begin : -1), eleTimeLeft(partb ? limit-4 : -1),
		  estimateCache(-1)
	{
		for(int i = 0; i < name2id.size(); i++)
		{
			if(value[i] != 0) available.push_back(i);
		}
	}
	typedef int cost_t;
	// 若剩餘的閥門走過去時間都不夠就表示到此為止了
	// Finish when all available valves aren't reachable within the remaining time.
	bool isFinish() const
	{
		for(int s : available)
		{
			if(meTimeLeft >= adj[meNow][s]+1 ||
					(eleNow != -1 && eleTimeLeft >= adj[eleNow][s]+1))
				return false;
		}
		return true;
	}
	string id() const
	{
		// 盡量縮短 ID 長度使其能放入 std::string 本身結構中
		// 需紀錄閥門點、兩者所在及所餘時間
		// 閥門點 6 個點一個字，我的輸入有 46 點佔 8 字，後四項各一字計 12 字
		// Shrink down ID length to fit into short-string optimization of std::string
		// We need to record available valves, where we & elephant are and time left
		// My input have 46 valves, 6 valves a byte for total 8 bytes,
		// one byte for the last for items, totaling 12 bytes.
		stringstream ss;
		uint64_t v = 0;
		for(int a : available) v |= ((uint64_t)(1) << a);
		while(v > 0) {ss << (char)((v & 63) + 48); v >>= 6;}
		ss << (char)(meNow + 48) << (char)(meTimeLeft + 48);
		if(eleNow != -1)
			ss << (char)(eleNow + 48) << (char)(eleTimeLeft + 48);
		return ss.str();
	}
	int estimateRemain() const
	{
		if(estimateCache != -1) return estimateCache;
		/* 估計邏輯：
		 * 考慮所有還需要到的點，其值乘以餘下時間求和的最大值。
		 * 由現在地走出去的時間求出最小是 X 分，餘下都假設一分鐘到位，
		 * 那所有餘下時間序列就是現餘時間減 (X+1) 後再連續減 2 到底。
		 * 對乘和最大值由排序不等式可知各別排序後對應項乘起相加即可。
		 * Estimate logic:
		 * Consider all available nodes, the sum of their pressure times possible time,
		 * take the maximum of this sum of products.
		 * For possible time, we can find out the minimum time out of this place
		 * to a valve not already open being X minutes, and the rest use 1 minute as estimate.
		 * So the possible time for valves will be the current leftover time,
		 * minus (X+1), then continuously minus 2 until zero.
		 * To find the maximum sum of product, by rearrangement inequality
		 * we sort the two lists and add the product of corresponding terms.
		 */
		vector<int> left;
		int memin = 1000, elemin = 1000;
		for(int i : available)
		{
			left.push_back(value[i]);
			memin = min(memin, adj[meNow][i]);
			if(eleNow != -1)
				elemin = min(elemin, adj[eleNow][i]);
		}
		vector<int> tm;
		for(int t = meTimeLeft - memin - 1; t > 0; t--) tm.push_back(t);
		if(eleNow != -1)
			for(int t = eleTimeLeft - elemin - 1; t > 0; t--) tm.push_back(t);
		sort(left.begin(), left.end(), greater<int>());
		sort(tm.begin(), tm.end(), greater<int>());
		int sum = 0;
		for(auto p = left.begin(), q = tm.begin();
			p != left.end() && q != tm.end();
			++p, ++q)
		{
			sum += (*p) * (*q);
		}
		// 用負數是 A* 要找最小，取負數就變成找最大
		// Since A* is searching for smallest "cost", negate the value for largest
		return estimateCache = -sum;
	}
	/*void visit(int cost) const
	{
		static int count = 0;
		++count;
		if(count % 1000 == 0)
		{
			int est = estimateRemain();
			cerr << "\r" << count << " " << -cost-est << " "
				 << -cost << " " << id() << "                   " << flush;
		}
	}*/
	auto nextMoves() const
	{
		vector<pair<State, int>> next;
		for(int i : available)
		{
			do
			{
				int timeLeftThere = meTimeLeft - (adj[meNow][i] + 1);
				if(timeLeftThere < 0) break;
				State ns = *this;
				auto p = find(ns.available.begin(), ns.available.end(), i);
				if(*p != ns.available.back())
					*p = ns.available.back();
				ns.available.pop_back();
				ns.meNow = i;
				ns.meTimeLeft = timeLeftThere;
				ns.estimateCache = -1;
				next.push_back({ns, -timeLeftThere * value[i]});
			} while(0);
			do
			{
				if(eleNow == -1) break;
				int timeLeftThere = eleTimeLeft - (adj[eleNow][i] + 1);
				if(timeLeftThere < 0) break;
				State ns = *this;
				auto p = find(ns.available.begin(), ns.available.end(), i);
				if(*p != ns.available.back())
					*p = ns.available.back();
				ns.available.pop_back();
				ns.eleNow = i;
				ns.eleTimeLeft = timeLeftThere;
				ns.estimateCache = -1;
				next.push_back({ns, -timeLeftThere * value[i]});
			} while(0);
		}
		return next;
	}
};

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	auto input = AOC::readToMatrix<string>(cin);
	for(auto& token : input)
	{
		string ns = token[1];
		name2id[ns] = name2id.size();
	}
	int sz = name2id.size();
	adj.resize(sz, vector<int>(sz, 1000));
	for(int i = 0; i < sz; i++) adj[i][i] = 0;

	for(const auto& token : input)
	{
		int n = name2id[token[1]];
		value[n] = stoi(token[4].substr(5));
		for(int i = 9; i < token.size(); i++)
		{
			string ws = token[i];
			if(ws.back() == ',') ws.pop_back();
			int w = name2id[ws];
			adj[n][w] = 1;
		}
	}

	// 使用 Floyd Warshall 找出所有閥門間的最短路徑
	// Use Floyd Warshall algorithm to find all pair shortest path
	// between every pair of valves.
	for(int k = 0; k < sz; k++)
		for(int i = 0; i < sz; i++)
			for(int j = 0; j < sz; j++)
				adj[i][j] = min(adj[i][j], adj[i][k]+adj[k][j]);

	// A*
	int cost = AOC::AStar(State{name2id["AA"], 30, partb});
	cout << -cost << endl;

	return 0;
}

