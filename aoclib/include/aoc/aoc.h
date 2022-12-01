#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <type_traits>

namespace AOC
{

inline char PartB(char* argv0)
{
	return std::string(argv0).find('b') != std::string::npos;
}

template <class T>
T convert(const std::string& s)
{
	if constexpr (std::is_integral_v<T>)
	{
		if constexpr (std::is_unsigned_v<T>)
		{
			return static_cast<T>(stoull(s));
		}
		else
		{
			return static_cast<T>(stoll(s));
		}
	}
	else if constexpr (std::is_same_v<T, float>)
	{
		return stof(s);
	}
	else if constexpr (std::is_same_v<T, double>)
	{
		return stod(s);
	}
	else if constexpr (std::is_same_v<T, std::string>)
	{
		return s;
	}
	else
	{
		static_assert(!std::is_same_v<T, T>);
		return {};
	}
}

inline bool isWhitespace(char c) {return c == ' ' || c == '\t' || c == '\n';}

template <class T, class Func>
void read(std::istream& in, Func callback, char delim = 0, bool keepEmpty = false)
{
	std::string token;
	if(delim != 0)
	{
		while(getline(in, token, delim), !in.fail())
		{
			if(!keepEmpty && token == "") continue;
			size_t l = 0, r = token.size() - 1;
			while(r > 0 && isWhitespace(token[r])) r--;
			while(l <= r && isWhitespace(token[l])) l++;
			token = token.substr(l, r-l+1);
			callback(convert<T>(token));
		}
	}
	else
	{
		char c;
		while(c = in.get(), !in.fail())
		{
			if(isWhitespace(c))
			{
				if(keepEmpty || token != "") callback(convert<T>(token));
				token = "";
			}
			else
			{
				token += c;
			}
		}
		if(keepEmpty || token != "") callback(convert<T>(token));
	}
}

template <class T, class Func>
void read(const std::string& s, Func callback, char delim = 0, bool keepEmpty = false)
{
	std::istringstream ss {s};
	read<T>(ss, callback, delim, keepEmpty);
}

inline std::vector<std::string> readPicture(std::istream& in)
{
	std::vector<std::string> ret;
	std::string line;
	while(getline(in, line), !in.fail())
	{
		ret.push_back(line);
	}
	return ret;
}

template <class Func>
void readPerLine(std::istream& in, Func callback, bool keepEmpty = false)
{
	read<std::string>(in, callback, '\n', keepEmpty);
}

template <class T>
std::vector<T> readToVector(std::istream& in, char delim = 0, bool keepEmpty = false)
{
	std::vector<T> ret;
	read<T>(in, [&](const T& value){ret.push_back(value);}, delim, keepEmpty);
	return ret;
}

template <class T>
auto readToVector(const std::string& s, char delim = 0, bool keepEmpty = false)
{
	std::istringstream ss {s};
	return readToVector<T>(ss, delim, keepEmpty);
}

template <class Func>
void readPerLineTokenized(std::istream& in, Func callback, char delim = 0, bool keepEmpty = false)
{
	read<std::string>(in,
		[&](const std::string& line)
		{
			callback(readToVector<std::string>(line, delim));
		}, '\n', keepEmpty);
}

template <class T>
std::vector<std::vector<T>> readToMatrix(std::istream& in, char delim = 0, bool keepEmpty = false)
{
	std::vector<std::vector<T>> ret;
	read<std::string>(in,
		[&](const std::string& line)
		{
			std::vector<T> row;
			read<T>(line, [&](const T& value){row.push_back(value);}, delim);
			ret.push_back(std::move(row));
		}, '\n', keepEmpty);
	return ret;
}

template <class T>
auto readToMatrix(const std::string& s, char delim = 0, bool keepEmpty = false)
{
	std::istringstream ss {s};
	return readToMatrix<T>(ss, delim, keepEmpty);
}

}

