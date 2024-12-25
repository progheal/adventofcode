#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "aoc/aoc.h"
using namespace std;

struct Node
{
	string name;
	string arg1, op, arg2;
	void fill(string line);
};

void Node::fill(string line)
{
	auto v = AOC::readToVector<string>(line);
	if(v[0].back() == ':')
	{
		arg1 = "";
		op = v[1];
		arg2 = "";
		name = v[0].substr(0, v[0].size() - 1);
	}
	else
	{
		arg1 = v[0];
		op = v[1];
		arg2 = v[2];
		name = v[4];
	}
}

struct Circuit
{
	map<string, Node> circuit;
	uint64_t eval(uint64_t x, uint64_t y);
};

uint64_t Circuit::eval(uint64_t x, uint64_t y)
{
	auto evalNode = [x, y, this](auto self, const string& name) -> bool
	{
		if(name[0] == 'x')
		{
			bool ret = (x & (uint64_t(1) << (name[1] * 10 + name[2] - 528))) != 0;
			cerr << name << " =  " << (ret ? '1' : '0') << " \n";
			return ret;
		}
		if(name[0] == 'y')
		{
			bool ret = (y & (uint64_t(1) << (name[1] * 10 + name[2] - 528))) != 0;
			cerr << name << " =  " << (ret ? '1' : '0') << " \n";
			return ret;
		}
		Node& node = circuit[name];
		cerr << name << " = " << node.arg1 << " " << node.op << " " << node.arg2 << "\n";
		switch(node.op[0])
		{
			case 'A': return self(self, node.arg1) && self(self, node.arg2);
			case 'O': return self(self, node.arg1) || self(self, node.arg2);
			case 'X': return self(self, node.arg1) != self(self, node.arg2);
			default: return false;
		}
	};
	uint64_t z = 0;
	for(int bn = 0; ; bn++)
	{
		string zn = string("z") + (char)('0' + bn / 10) + (char)('0' + bn % 10);
		cerr << "Evaluate " << zn << "\n";
		if(circuit.count(zn) == 0) break;
		if(evalNode(evalNode, zn)) z |= (uint64_t(1)) << bn;
	}
	return z;
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	Circuit c;
	uint64_t x = 0, y = 0;
	int maxbit = 0;
	for(auto& line : AOC::LineReader(cin))
	{
		if(line == "") continue;
		if(line[3] == ':')
		{
			int num = line[1] * 10 + line[2] - 528;
			if(line[0] == 'x')
			{
				if(line[5] == '1') 
					x |= (uint64_t)(1) << num;
			}
			else if(line[0] == 'y')
			{
				if(line[5] == '1')
					y |= (uint64_t)(1) << num;
			}
		}
		else
		{
			Node node;
			node.fill(line);
			c.circuit[node.name] = node;
			if(node.name[0] == 'z')
			{
				int bn = node.name[1] * 10 + node.name[2] - 528;
				maxbit = max(maxbit, bn);
			}
		}
	}

	if(!partb)
	{
		cout << c.eval(x, y) << "\n";
	}


	return 0;
}

