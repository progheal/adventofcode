#pragma once

#include <vector>
#include <string>
#include <array>
#include <sstream>
#include <type_traits>

namespace AOC
{

// 判斷是否有 'b' 字元，會傳入 main 的 argv[0] 判斷是否以 01b 之類的名字呼叫
inline char PartB(char* argv0)
{
	return std::string(argv0).find('b') != std::string::npos;
}

// 轉換函數，將字串轉成 T 型態
// 支援整數、浮點數、字串；其他型態會產生編譯錯誤
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

// 判斷是否 whitespace；此為未指定分界字元時的預設分界
inline bool isWhitespace(char c) {return c == ' ' || c == '\t' || c == '\n';}

// 由串流讀取資料，並將每個元素之字串代入 callback 呼叫
// delim 為分界字元，0 表 whitespace
// keepEmpty 指定是否留下空元素
template <class T, class Func>
void read(std::istream& in, Func callback, char delim = 0, bool keepEmpty = false)
{
	std::string token;
	if(delim != 0)
	{
		// 分界字元非空時使用 getline 機制分割，並去掉頭尾空白
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
		// 否則手動分割
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

// 由字串讀取，其餘同上
template <class T, class Func>
void read(const std::string& s, Func callback, char delim = 0, bool keepEmpty = false)
{
	std::istringstream ss {s};
	read<T>(ss, callback, delim, keepEmpty);
}

// 讀取一個二維圖形輸入，例如迷宮
// 主要是不做切割去空白等處理
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

// 設定以換行切割，用在輸入以行為單位
// keepEmpty 在此表保留空行
template <class Func>
void readPerLine(std::istream& in, Func callback, bool keepEmpty = false)
{
	read<std::string>(in, callback, '\n', keepEmpty);
}

// 由串流讀入 std::vector<T> 並回傳
template <class T>
std::vector<T> readToVector(std::istream& in, char delim = 0, bool keepEmpty = false)
{
	std::vector<T> ret;
	read<T>(in, [&](const T& value){ret.push_back(value);}, delim, keepEmpty);
	return ret;
}

// 由字串讀入 std::vector<T> 並回傳
// 這即是一個簡易的字串切割函數
template <class T>
auto readToVector(const std::string& s, char delim = 0, bool keepEmpty = false)
{
	std::istringstream ss {s};
	return readToVector<T>(ss, delim, keepEmpty);
}

// 由字串讀入所有數字
inline auto readNumbers(const std::string& s, bool hasNegative = false)
{
	std::vector<int> ret;
	int v = 0, sign = 1;
	bool inDigit = false;
	for(char c : s)
	{
		if(c >= '0' && c <= '9')
		{
			inDigit = true;
			v = v * 10 + (c - '0');
		}
		else if(c == '-' && hasNegative)
		{
			inDigit = true;
			sign = -1;
		}
		else if(inDigit)
		{
			ret.push_back(v * sign);
			v = 0;
			sign = 1;
			inDigit = false;
		}
	}
	if(inDigit)
	{
		ret.push_back(v * sign);
	}
	return ret;
}

// 由串流讀入每行，將每行切割後呼叫 callback
template <class Func>
void readPerLineTokenized(std::istream& in, Func callback, char delim = 0, bool keepEmpty = false)
{
	read<std::string>(in,
		[&](const std::string& line)
		{
			callback(readToVector<std::string>(line, delim));
		}, '\n', keepEmpty);
}

// 由串流讀入每行，將每行切割為數字後呼叫 callback
template <class Func>
void readPerLineNumbers(std::istream& in, Func callback, bool hasNegative = false)
{
	read<std::string>(in,
		[&](const std::string& line)
		{
			callback(readNumbers(line, hasNegative));
		});
}

// 由串流讀入二維矩陣
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

// 由字串讀入二維矩陣
template <class T>
auto readToMatrix(const std::string& s, char delim = 0, bool keepEmpty = false)
{
	std::istringstream ss {s};
	return readToMatrix<T>(ss, delim, keepEmpty);
}

// 由串流讀入二維數字
inline auto readNumberMatrix(std::istream& in, bool hasNegative = false)
{
	std::vector<std::vector<int>> ret;
	read<std::string>(in,
		[&](const std::string& line)
		{
			ret.push_back(readNumbers(line, hasNegative));
		});
	return ret;
}

// 由字串讀入二維數字
inline auto readNumberMatrix(const std::string& s, bool hasNegative = false)
{
	std::istringstream ss {s};
	return readNumberMatrix(ss, hasNegative);
}

// 將 std::vector 的前 N 個元素取出做成 std::array
// 使其可以使用在 std::tie 或 structured binding
template<int N, class T>
auto takeFirst(const std::vector<T>& v)
{
    if(v.size() < N)
        throw std::length_error("vector should have at least " + std::to_string(N) + " values.");
    std::array<T, N> arr;
	std::copy_n(v.begin(), N, arr.begin());
	return arr;
}

}

