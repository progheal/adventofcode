#pragma once

#include <queue>
#include <unordered_set>
#include <utility>
#include <set>
#include <string>

namespace AOC
{

/*
// 搜尋狀態結構的指定介面
// Interface of states to search
// Note: I default to C++17 so no concept check
struct State
{
	// 花費型態，必須滿足下列兩條件：
	// 1. 是整數，或有 size() 函數可轉為整數
	// 2. 支援 operator + 合併花費
	// Cost type, should satisfy both:
	// 1. Either an integer, or have size() that that returns something convertable to integer
	// 2. Supports operator + to combine costs
	typedef ... cost_t;

	// 此狀態是否為目標狀態；搜尋到此即會跳出
	// 若永遠回傳 false 則相當於將所有能搜的部份都搜完
	// Check for whether this state is finish state
	// If this function always return false, it means exhaustive search
	bool isFinish() const;
	
	// 取得此狀態的相鄰狀態
	// 回傳元素為 pair{次一狀態，移動花費} 的 vector
	// Obtain the list of next state
	// in a vector of pair of {next state, move cost}
	vector<pair<State, cost_t>> nextMoves() const;

	// 能辨識此一狀態的值，用來判斷是否已走過
	// 此型態要能用在 std::unordered_set<>
	// An identifier to check for visited
	// Should be able to use in std::unordered_set<> (ie. has std::hash<>)
	... id() const;

	// A* 用的估計函數，回傳一個**低估的**預估剩餘值
	// 若永遠回傳 0 則 A* 變為 Dijkstra 演算法
	// The estimate function for A*, should **under estimate**
	// If this function always return 0, A* degenerates into Dijkstra algorithm
	cost_t estimateRemain() const;

	// 若有此函數則在走到此狀態時會傳入目前花費呼叫之，可供除錯或紀錄
	// If this function exists, it will be called when a state is visited
	// Suitable for debugging and/or recording
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
// Call C.visit() if C has visit()
template<class C>
inline void doVisit(const C& c, const typename C::cost_t& path)
{
	if constexpr (HasVisit<C>::value)
	{
		c.visit(path);
	}
}

// 將花費型態轉為整數
// Convert cost type to integer
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

// 路徑類別，可作為搜尋的 cost_t 使用
// Path class, can be used as cost_t in state class
template<class Element>
class Path
{
	std::vector<Element> path;
public:
	Path() : path() {}
	Path(const Element& c) : path({c}) {}
	Path(const std::vector<Element>& p) : path(p) {}
	operator int() {return (int)path.size();}
	operator size_t() {return path.size();}
	operator std::vector<Element>() {return path;}
	size_t size() {return path.size();}
	auto begin() const {return path.begin();}
	auto end() const {return path.end();}
	Path& operator += (const Path& p)
	{
		for(auto& c : p.path) path.push_back(c);
		return *this;
	}
	Path operator + (const Path& p) const
	{
		Path ret = *this;
		return ret += p;
	}
};

// BFS 搜尋 (如淹水或走迷宮)，回傳走到目標時的花費 (0 表示走不到)
// BFS Search, return the min cost to target (0 = unreachable)
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
		auto id = s.id();
		if(visited.count(id) > 0) continue;
		visited.insert(id);
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
// A* algorithm, or when estimates are always 0 becomes Dijkstra.
// Return the min cost to target (0 = unreachable)
template<class State>
auto AStar(const State& init)
{
	typedef typename State::cost_t Cost;
	typedef std::pair<State, Cost> queueItem; 
	typedef decltype(std::declval<State>().id()) IdType;
	auto comparator = [](const queueItem& q1, const queueItem& q2) -> bool
	{
		return detail::cost_value(q1.second + q1.first.estimateRemain())
			> detail::cost_value(q2.second + q2.first.estimateRemain());
	};
	std::priority_queue q{comparator, std::vector<queueItem>()};
	q.push({init, Cost{}});
	std::unordered_set<IdType> visited;

	while(!q.empty() && !q.top().first.isFinish())
	{
		auto [s, n] = q.top();
		q.pop();
		auto id = s.id();
		if(visited.count(id) > 0) continue;
		visited.insert(id);
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
