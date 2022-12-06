#include <iostream>
#include <string_view>
#include <regex>
#include <numeric>
#include <string>
#include "aoc/aoc.h"
using namespace std::string_literals;

namespace AOC
{

namespace { // detail

template <class T>
class DelimiterFinder
{
	T criteria;
public:
	DelimiterFinder(T t) : criteria(t) {}
	std::pair<size_t, size_t> next(std::string_view source);
};

template <>
std::pair<size_t, size_t> DelimiterFinder<std::string>::next(std::string_view source)
{
	size_t start = source.find(criteria);
	if(start == std::string_view::npos) return {start, start};
	size_t end = start + criteria.size();
	return {start, end};
}

template<>
std::pair<size_t, size_t> DelimiterFinder<std::regex>::next(std::string_view source)
{
	std::match_results<std::string_view::const_iterator> m;
	bool matched = std::regex_search(source.begin(), source.end(), m, criteria);
	if(!matched)
		return {std::string_view::npos, std::string_view::npos};
	else
		return {m[0].first - source.begin(), m[0].second - source.begin()};
}

template <class T>
std::vector<std::string> tokenize_impl(std::string_view source, DelimiterFinder<T> finder)
{
	std::vector<std::string> ret;
	while(true)
	{
		auto [p, q] = finder.next(source);
		if(p == std::string_view::npos)
		{
			ret.push_back(std::string(source));
			return ret;
		}
		else
		{
			ret.push_back(std::string(source.substr(0, p)));
			source.remove_prefix(q);
		}
	}
}

}

std::vector<std::string> tokenize(std::string_view source, std::regex delimiter)
{
	return tokenize_impl(source, DelimiterFinder(delimiter));
}

std::vector<std::string> tokenize(std::string_view source, std::string delimiter)
{
	return tokenize_impl(source, DelimiterFinder(delimiter));
}

///////////////////////////////////////

std::vector<std::string> rotateCW(const std::vector<std::string>& vs)
{
	size_t width = std::accumulate(vs.begin(), vs.end(), (size_t)0,
		[](size_t m, const std::string& s){return std::max(m, s.size());});

	std::vector<std::string> ret;
	for(size_t i = 0; i < width; i++)
	{
		ret.push_back(std::accumulate(vs.rbegin(), vs.rend(), ""s,
			[i](std::string s, const std::string& line)
			{
				return s + (i < line.size() ? line[i] : ' ');
			}));
	}

	return ret;
}

std::vector<std::string> rotateCCW(const std::vector<std::string>& vs)
{
	size_t width = std::accumulate(vs.begin(), vs.end(), (size_t)0,
		[](size_t m, const std::string& s){return std::max(m, s.size());});

	std::vector<std::string> ret;
	for(size_t i = width; i-- > 0; )
	{
		ret.push_back(std::accumulate(vs.begin(), vs.end(), ""s,
			[i](std::string s, const std::string& line)
			{
				return s + (i < line.size() ? line[i] : ' ');
			}));
	}

	return ret;
}

std::vector<std::string> rotate180(const std::vector<std::string>& vs)
{
	size_t width = std::accumulate(vs.begin(), vs.end(), (size_t)0,
		[](size_t m, const std::string& s){return std::max(m, s.size());});

	std::vector<std::string> ret;
	std::transform(vs.rbegin(), vs.rend(), std::back_inserter(ret),
		[width](std::string s)
		{
			s.resize(width, ' ');
			return std::string{s.rbegin(), s.rend()};
		});
	return ret;
}

}
