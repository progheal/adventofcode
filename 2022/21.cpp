#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <limits>
#include "aoc/aoc.h"
using namespace std;

constexpr int64_t NOTHING = std::numeric_limits<int64_t>::min();

struct Monkey
{
	string name;
	int64_t value;
	string arg1, op, arg2;
	int64_t call(bool partb);
	int64_t root();
	int64_t expect(int64_t expectation);
};

map<string, Monkey> monkeys;

int64_t Monkey::call(bool partb)
{
	if(name == "humn" && partb)
	{
		return NOTHING;
	}
	if(value != NOTHING)
	{
		return value;
	}
	else
	{
		int64_t v1 = monkeys[arg1].call(partb), v2 = monkeys[arg2].call(partb);
		if(v1 == NOTHING || v2 == NOTHING) return NOTHING;
		switch(op[0])
		{
		case '+': return v1+v2;
		case '-': return v1-v2;
		case '*': return v1*v2;
		case '/': return v1/v2;
		default: return NOTHING;
		}
	}
}

int64_t Monkey::expect(int64_t expectation)
{
	if(name == "humn")
		return expectation;
	else if(value != NOTHING)
		return value;
	else
	{
		int64_t v1 = monkeys[arg1].call(true);
		if(v1 == NOTHING)
		{
			int64_t v2 = monkeys[arg2].call(true);
			//cerr << name << " expect " << arg1 << " = " << v2 << endl;
			switch(op[0])
			{
			case '+': return monkeys[arg1].expect(expectation - v2);
			case '-': return monkeys[arg1].expect(expectation + v2);
			case '*': return monkeys[arg1].expect(expectation / v2);
			case '/': return monkeys[arg1].expect(expectation * v2);
			}
		}
		else
		{
			//cerr << name << " expect " << arg2 << " = " << v1 << endl;
			switch(op[0])
			{
			case '+': return monkeys[arg2].expect(expectation - v1);
			case '-': return monkeys[arg2].expect(v1 - expectation);
			case '*': return monkeys[arg2].expect(expectation / v1);
			case '/': return monkeys[arg2].expect(v1 / expectation);
			}
		}
		return NOTHING;
	}
}

int64_t Monkey::root()
{
	int64_t v1 = monkeys[arg1].call(true);
	if(v1 == NOTHING)
		return monkeys[arg1].expect(monkeys[arg2].call(true));
	else
		return monkeys[arg2].expect(v1);
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	AOC::readPerLineTokenized(cin,
		[&](const vector<string>& vs)
		{
			string name = vs[0].substr(0, vs[0].size()-1);
			if(vs.size() == 2)
				monkeys[name] = Monkey{name, stoi(vs[1])};
			else
				monkeys[name] = Monkey{name, NOTHING, vs[1], vs[2], vs[3]};
		});

	if(!partb)
	{
		cout << monkeys["root"].call(false) << endl;
	}
	else
	{
		cout << monkeys["root"].root() << endl;
	}

	return 0;
}

