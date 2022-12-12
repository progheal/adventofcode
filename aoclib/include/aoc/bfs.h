#pragma once

#include <queue>
#include <utility>
#include <string>
#include <set>
#include <unordered_set>

namespace AOC
{

/*
struct State // 搜尋狀態結構的指定介面
{
	// 花費型態，必須滿足下列兩條件：
	// 1. 是整數，或有 size() 函數可轉為整數
	// 2. 支援 operator + 合併花費
	typedef ... cost_t;

	// 此狀態是否為目標狀態；搜尋到此即會跳出
	// 若永遠回傳 false 則相當於將所有能搜的部份都搜完
	bool isFinish() const;
	
	// 取得此狀態的相鄰狀態
	// 回傳元素為 pair{次一狀態，移動花費} 的 vector
	vector<pair<State, cost_t>> nextMoves() const;

	// 能辨識此一狀態的值，用來判斷是否已走過
	// 此型態要能用在 std::unordered_set<>
	... id() const;

	// A* 用的估計函數，回傳一個**低估的**預估剩餘值
	// 若永遠回傳 0 則 A* 變為 Dijkstra 演算法
	int estimateRemain() const;

	// 若有此函數則在走到此狀態時會傳入目前花費呼叫之，可供除錯或紀錄
	void visit(const cost_t& path) const;
};
*/

namespace detail
{

// SFINAE 判斷狀態類別是否有 visit 函數
template<class C, class = void>
class HasVisit : public std::false_type {};

template<class C>
class HasVisit<C, 
	std::void_t<decltype(std::declval<C>().visit(std::declval<typename C::cost_t>()))>
> : public std::true_type {};

// 若有 visit 函數則呼叫之
template<class C>
inline std::enable_if_t<!HasVisit<C>::value> doVisit(const C& c, const typename C::cost_t& path)
{
}

template<class C>
inline std::enable_if_t<HasVisit<C>::value> doVisit(const C& c, const typename C::cost_t& path)
{
	c.visit(path);
}

// 將花費型態轉為整數
template<class T>
inline std::enable_if_t<std::is_arithmetic_v<T>, int> cost_value(const T& value)
{
	return value;
}

template<class T>
inline decltype(std::declval<T>().size(), int()) cost_value(const T& value)
{
	return value.size();
}

} // namespace detail

// BFS 搜尋 (如淹水或走迷宮)，回傳走到目標時的花費 (0 表示走不到)
template<class State>
auto BFS(const State& init)
{
	typedef typename State::cost_t Cost;
	typedef std::pair<State, Cost> queueItem; 
	typedef decltype(std::declval<State>().id()) IdType;
	std::queue<queueItem> q;
	q.push({init, Cost{}});
	std::unordered_set<IdType> visited;

	while(!q.empty() && !q.front().first.isFinish())
	{
		auto [s, n] = q.front();
		q.pop();
		if(visited.count(s.id()) > 0) continue;
		visited.insert(s.id());
		detail::doVisit(s, n);
		for(auto [ns, mv] : s.nextMoves())
		{
			q.push({ns, n+mv});
		}
	}

	if(q.empty())
		return Cost{};
	else
	{
		detail::doVisit(q.front().first, q.front().second);
		return q.front().second;
	}
}

// A* 演算法 (帶估計剩餘量的 BFS 搜尋)，或當估計固定回傳 0 時為 Dijkstra 演算法
// 回傳走到目標時的花費 (0 表示走不到)
template<class State>
auto AStar(const State& init)
{
	typedef typename State::cost_t Cost;
	typedef std::pair<State, Cost> queueItem; 
	typedef decltype(std::declval<State>().id()) IdType;
	auto comparator = [](const queueItem& q1, const queueItem& q2) -> bool
	{
		return detail::cost_value(q1.second) + q1.first.estimateRemain()
			> detail::cost_value(q2.second) + q2.first.estimateRemain();
	};
	std::priority_queue q{comparator, std::vector<queueItem>()};
	q.push({init, Cost{}});
	std::unordered_set<IdType> visited;

	while(!q.empty() && !q.top().first.isFinish())
	{
		auto [s, n] = q.top();
		q.pop();
		if(visited.count(s.id()) > 0) continue;
		visited.insert(s.id());
		detail::doVisit(s, n);
		for(auto [ns, mv] : s.nextMoves())
		{
			q.push({ns, n+mv});
		}
	}

	if(q.empty())
		return Cost{};
	else
	{
		detail::doVisit(q.top().first, q.top().second);
		return q.top().second;
	}
}

} // namespace AOC
