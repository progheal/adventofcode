#pragma once

#include <vector>
#include <string>
#include <array>
#include <sstream>
#include <algorithm>
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

const char DELIM_NONDIGIT = -1;

namespace detail
{

// 讀取的 iterator 類，實際讀取在此發生
template<class T>
class ReaderIterator
{
	std::istream* in;
	char delim;
	bool keepEmpty;
	std::optional<T> value;
	class PostIncrementHolder
	{
		T value;
	public:
		PostIncrementHolder(T value): value(value) {}
		T operator *() {return value;}
	};
public:
	typedef std::ptrdiff_t difference_type;
	typedef const T value_type;
	typedef const T* pointer;
	typedef const T& reference;
	// input iterator 限定只有 single pass
	typedef std::input_iterator_tag iterator_category;

	// 一般狀態
	ReaderIterator(std::istream& in, char delim = 0, bool keepEmpty = false)
		: in(&in), delim(delim), keepEmpty(keepEmpty), value(std::nullopt) {get();}
	// 結束狀態 (預設 C++17 故無法以不同類別做為 sentinel)
	ReaderIterator()
		: in(nullptr), delim(), keepEmpty(), value() {}

	bool operator != (const ReaderIterator& it) const
	{
		if(!value && !it.value) return false;
		if(!value) return it != *this;
		if(!it.value) return value.has_value();
		return true;
	}
	bool operator == (const ReaderIterator& it) const {return !(operator !=(it));}
	const T& operator *() const
	{
		return *value;
	}
	const T* operator ->() const
	{
		return value.operator->();
	}
	ReaderIterator& operator ++ ()
	{
		get();
		return *this;
	}
	PostIncrementHolder operator ++(int)
	{
		PostIncrementHolder h {*value};
		get();
		return h;
	}
private:
	// 讀取下一個值並存起來
	void get()
	{
		std::string token;
		if constexpr (std::is_integral_v<T>)
		{
			// 僅截取數字
			if(delim == DELIM_NONDIGIT)
			{
				char c;
				while(c = in->get(), !in->fail())
				{
					if(c >= '0' && c <= '9')
						token += c;
					else if(keepEmpty && token == "" && c == '-') // keepEmpty 在此覆載為 hasNegative
						token += c;
					else if(token != "")
					{
						in->unget();
						break;
					}
				}
				if(token == "")
					value = std::nullopt;
				else
					value = convert<T>(token);
				return;
			}
		}
		if(delim != 0)
		{
			// 分界字元非空時使用 getline 機制分割，並去掉頭尾空白
			do
			{
				std::getline(*in, token, delim);
				if(in->fail())
				{
					value = std::nullopt;
					return;
				}
			} while(!keepEmpty && token == "");
			token = doTrim(token);
			value = convert<T>(token);
		}
		else
		{
			// 否則手動分割
			char c;
			while(c = in->get(), !in->fail())
			{
				if(!isWhitespace(c))
					token += c;
				else
				{
					if(keepEmpty || token != "") break;
					token = "";
				}
			}
			if(keepEmpty || token != "")
				value = convert<T>(token);
			else
				value = std::nullopt;
		}
	}
	static std::string doTrim(std::string token)
	{
		size_t l = 0, r = token.size() - 1;
		while(r > 0 && isWhitespace(token[r])) r--;
		while(l <= r && isWhitespace(token[l])) l++;
		return token.substr(l, r-l+1);
	}
};

} // namespace AOC::detail

// 讀取的外包裝類，使其可用在 range based for 中
template<class T>
class Reader
{
public:
	Reader(std::istream& in, char delim = 0, bool keepEmpty = false)
		: ss(), in(in), delim(delim), keepEmpty(keepEmpty) {}
	Reader(std::string s, char delim = 0, bool keepEmpty = false)
		: ss(s), in(ss), delim(delim), keepEmpty(keepEmpty) {}
	auto begin() {return detail::ReaderIterator<T>(in, delim, keepEmpty);}
	auto end() {return detail::ReaderIterator<T>();}
private:
	std::istringstream ss;
	std::istream& in;
	char delim;
	bool keepEmpty;
};

// 由串流讀取資料，並將每個元素之字串代入 callback 呼叫
// delim 為分界字元，0 表 whitespace
// keepEmpty 指定是否留下空元素
template <class T, class Func>
void read(std::istream& in, Func callback, char delim = 0, bool keepEmpty = false)
{
	for(const T& value : Reader<T>(in, delim, keepEmpty))
	{
		callback(value);
	}
}

// 由字串讀取，其餘同上
template <class T, class Func>
void read(const std::string& s, Func callback, char delim = 0, bool keepEmpty = false)
{
	for(const T& value : Reader<T>(s, delim, keepEmpty))
	{
		callback(value);
	}
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

// 每行讀取，固定分界字元為 '\n' 及回傳型態為字串
class LineReader : public Reader<std::string>
{
public:
	LineReader(std::istream& in, bool keepEmpty = false)
		: Reader(in, '\n', keepEmpty) {}
	LineReader(std::string s, bool keepEmpty = false)
		: Reader(s, '\n', keepEmpty) {}
};

// 只讀數字
template<class IntType>
class NumberReader : public Reader<IntType>
{
public:
	NumberReader(std::istream& in, bool hasNegative = false)
		: Reader<IntType>(in, DELIM_NONDIGIT, hasNegative) {}
	NumberReader(std::string s, bool hasNegative = false)
		: Reader<IntType>(s, DELIM_NONDIGIT, hasNegative) {}
};

// 設定以換行切割，用在輸入以行為單位
// keepEmpty 在此表保留空行
template <class Func>
void readPerLine(std::istream& in, Func callback, bool keepEmpty = false)
{
	for(auto& line : LineReader(in, keepEmpty))
	{
		callback(line);
	}
}

// 由串流讀入 std::vector<T> 並回傳
template <class T>
std::vector<T> readToVector(std::istream& in, char delim = 0, bool keepEmpty = false)
{
	std::vector<T> ret;
	for(const T& value : Reader<T>(in, delim, keepEmpty))
	{
		ret.push_back(value);
	}
	return ret;
}

// 由字串讀入 std::vector<T> 並回傳
// 這即是一個簡易的字串切割函數
template <class T>
auto readToVector(const std::string& s, char delim = 0, bool keepEmpty = false)
{
	std::vector<T> ret;
	for(const T& value : Reader<T>(s, delim, keepEmpty))
	{
		ret.push_back(value);
	}
	return ret;
}

// 由串流讀入每行，將每行轉換為 T 後存為 std::vector<T> 回傳
// 若 T 為自訂結構則需自行增加 AOC::convert<T>(const string&) 之特化
template <class T>
auto readPerLineToVector(std::istream& in)
{
	std::vector<T> ret;
	for(const auto& line : LineReader(in))
	{
		ret.push_back(convert<T>(line));
	}
	return ret;
}

// 由字串讀入所有數字
template<class IntType = int>
inline auto readNumbers(const std::string& s, bool hasNegative = false)
{
	std::vector<IntType> ret;
	for(auto& v : NumberReader<IntType>(s, hasNegative))
	{
		ret.push_back(v);
	}
	return ret;
}

// 由串流讀入每行，將每行切割後呼叫 callback
template <class Func>
void readPerLineTokenized(std::istream& in, Func callback, char delim = 0, bool keepEmpty = false)
{
	for(const auto& line : LineReader(in, keepEmpty))
	{
		callback(readToVector<std::string>(line, delim));
	}
}

// 由串流讀入每行，將每行切割為數字後呼叫 callback
template <class IntType = int, class Func>
void readPerLineNumbers(std::istream& in, Func callback, bool hasNegative = false)
{
	for(const auto& line : LineReader(in))
	{
		callback(readNumbers<IntType>(line, hasNegative));
	}
}

// 由串流讀入二維矩陣
template <class T>
std::vector<std::vector<T>> readToMatrix(std::istream& in, char delim = 0, bool keepEmpty = false)
{
	std::vector<std::vector<T>> ret;
	for(const auto& line : LineReader(in, keepEmpty))
	{
		std::vector<T> row;
		for(const T& value : Reader<T>(line, delim))
			row.push_back(value);
		ret.push_back(std::move(row));
	}
	return ret;
}

// 由字串讀入二維矩陣
template <class T>
auto readToMatrix(const std::string& s, char delim = 0, bool keepEmpty = false)
{
	std::vector<std::vector<T>> ret;
	for(const auto& line : LineReader(s, keepEmpty))
	{
		std::vector<T> row;
		for(const T& value : Reader<T>(line, delim))
			row.push_back(value);
		ret.push_back(std::move(row));
	}
	return ret;
}

// 由串流讀入二維數字
template <class IntType = int>
inline auto readNumberMatrix(std::istream& in, bool hasNegative = false)
{
	std::vector<std::vector<IntType>> ret;
	for(const auto& line : LineReader(in))
	{
		ret.push_back(readNumbers<IntType>(line, hasNegative));
	}
	return ret;
}

// 由字串讀入二維數字
template <class IntType = int>
inline auto readNumberMatrix(const std::string& s, bool hasNegative = false)
{
	std::vector<std::vector<IntType>> ret;
	for(const auto& line : LineReader(s))
	{
		ret.push_back(readNumbers<IntType>(line, hasNegative));
	}
	return ret;
}

// 回傳一個 proxy tuple 可以取得 vector 的前 N 個值
// 使其可以使用在 std::tie 或 structured binding
// 此 tuple 元素是指向原 vector 元素的參考, 較先前取出元素包為 std::array 減少一次複製
template<class T, std::size_t... I>
auto takeFirstHelper(const std::vector<T>& v, std::index_sequence<I...>)
{
	return std::tuple(std::ref(v[I])...);
}
template<int N, class T>
auto takeFirst(const std::vector<T>& v)
{
    if(v.size() < N)
        throw std::length_error("vector should have at least " + std::to_string(N) + " values.");
	return takeFirstHelper(v, std::make_index_sequence<N>{});
}

}

