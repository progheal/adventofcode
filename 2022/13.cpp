#include <iostream>
#include <vector>
#include <string>
#include <variant>
#include <algorithm>
#include <charconv>
#include "aoc/aoc.h"
using namespace std;

struct Value : public variant<int, vector<Value>>
{
	Value() : variant(0) {}
	Value(int v) : variant(v) {}
	Value(const vector<Value>& v) : variant(v) {}
	bool operator < (const Value& rhs) const;
};

struct CompareVisitor
{
	bool operator ()(int l, int r) const
	{
		return l < r;
	}
	bool operator ()(const vector<Value>& l, const vector<Value>& r) const
	{
		return lexicographical_compare(l.begin(), l.end(), r.begin(), r.end());
	}
	bool operator ()(int l, const vector<Value>& r) const
	{
		return operator()(vector<Value>{l}, r);
	}
	bool operator ()(const vector<Value>& l, int r) const
	{
		return operator()(l, vector<Value>{r});
	}
} compareVisitor;

bool Value::operator < (const Value& rhs) const
{
	return visit(compareVisitor, *this, rhs);
}

struct PrintVisitor
{
	ostream& out;
	void operator () (int v) const
	{
		out << v;
	}
	void operator () (const vector<Value>& v) const
	{
		bool first = true;
		for(auto& child : v)
		{
			out << (first ? "[" : ",");
			visit(*this, child);
			first = false;
		}
		if(first) out << "[";
		out << "]";
	}
};

ostream& operator << (ostream& out, const Value& v)
{
	visit(PrintVisitor{out}, v);
	return out;
}

Value parse(std::string_view& s)
{
	if(s[0] == '[')
	{
		vector<Value> vr;
		if(s[1] != ']')
		{
			while(s[0] != ']')
			{
				s.remove_prefix(1);
				vr.push_back(parse(s));
			}
			s.remove_prefix(1);
		}
		else
		{
			s.remove_prefix(2);
		}
		return vr;
	}
	else
	{
		int v;
		auto p = from_chars(s.data(), s.data() + s.size(), v);
		s.remove_prefix(p.ptr - s.data());
		return v;
	}
}

Value parse(const std::string& s)
{
	string_view sv = s;
	return parse(sv);
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<Value> all;
	int isum = 0;
	AOC::readPerLine(cin,
		[&](const string& line)
		{
			Value v = parse(line);
			cerr << v << endl;
			all.push_back(v);
			if(all.size() % 2 == 0)
			{
				if(all[all.size()-2] < all.back())
				{
					isum += (all.size() / 2);
				}
			}
		});
	if(!partb)
		cout << isum << endl;
	else
	{
		Value v2 = parse("[[2]]"s), v6 = parse("[[6]]"s);
		all.push_back(v2);
		all.push_back(v6);
		sort(all.begin(), all.end());
		size_t p2 = lower_bound(all.begin(), all.end(), v2) - all.begin();
		size_t p6 = lower_bound(all.begin(), all.end(), v6) - all.begin();
		cout << (p2+1) * (p6+1) << endl;
	}

	return 0;
}

