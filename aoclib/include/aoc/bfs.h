#pragma once

#include <queue>
#include <utility>
#include <string>
#include <set>

namespace AOC
{

/*
struct State // spec
{
	// Cost type; must
	// 1. Either is integral or have size() convert it to integral
	// 2. Support + to combine cost
	typedef ... cost_t;
	// Is this state finish state
	bool isFinish() const;
	// Get all next moves
	// Return vector of pair of {next state, move cost)
	vector<pair<State, cost_t>> nextMoves() const;
	// Unique identifier of this state
	... id() const;
	// AStar heuristic function
	// return estimated # of remaining steps
	int estimateRemain() const;
	// Visit current path
	// does not call if this function does not exist
	void visit(const Cost& path) const;
};
*/

namespace detail
{

template<class C, class = void>
class HasVisit : public std::false_type {};

template<class C>
class HasVisit<C, 
	std::void_t<decltype(std::declval<C>().visit(std::declval<typename C::cost_t>()))>
> : public std::true_type {};

template<class C>
inline std::enable_if_t<!HasVisit<C>::value> doVisit(const C& c, const typename C::cost_t& path)
{
}

template<class C>
inline std::enable_if_t<HasVisit<C>::value> doVisit(const C& c, const typename C::cost_t& path)
{
	c.visit(path);
}

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

template<class State>
auto BFS(const State& init)
{
	typedef typename State::cost_t Cost;
	typedef std::pair<State, Cost> queueItem; 
	typedef decltype(std::declval<State>().id()) IdType;
	std::queue<queueItem> q;
	q.push({init, Cost{}});
	std::set<IdType> visited;

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
	std::set<IdType> visited;

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
