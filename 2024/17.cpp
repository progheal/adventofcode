#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include "aoc/aoc.h"
using namespace std;

vector<uint64_t> run(vector<uint64_t> program, uint64_t reg[])
{
	vector<uint64_t> output;
	int ip;

	auto combo = [&](uint64_t v)
	{
		switch(v)
		{
		case 0: case 1: case 2: case 3: return v;
		case 4: case 5: case 6: return reg[v-4];
		default: assert("Invalid combo arg"); exit(1);
		}
	};

	ip = 0;
	while(ip < program.size())
	{
		unsigned int op = program[ip];
		unsigned int arg = program[ip+1];
		switch(op)
		{
		case 0: reg[0] >>= combo(arg); break;
		case 1: reg[1] ^= arg; break;
		case 2: reg[1] = combo(arg) % 8; break;
		case 3: if(reg[0]) ip = arg - 2; break;
		case 4: reg[1] ^= reg[2]; break;
		case 5: output.push_back(combo(arg) % 8); break;
		case 6: reg[1] = reg[0] >> combo(arg); break;
		case 7: reg[2] = reg[0] >> combo(arg); break;
		}
		ip += 2;
	}

	return output;
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	uint64_t oa, ob, oc;
	uint64_t reg[3];

	string line;
	getline(cin, line);
	reg[0] = oa = AOC::readNumbers<uint64_t>(line)[0];
	getline(cin, line);
	reg[1] = ob = AOC::readNumbers<uint64_t>(line)[0];
	getline(cin, line);
	reg[2] = oc = AOC::readNumbers<uint64_t>(line)[0];
	getline(cin, line);
	getline(cin, line);
	auto program = AOC::readNumbers<uint64_t>(line);

	if(!partb)
	{
		stringstream output;
		for(auto v : run(program, reg))
			output << "," << v;
		string s = output.str();
		cout << s.substr(1) << "\n";
	}
	else
	{
		vector<uint64_t> candidate, extended;
		candidate.push_back(0);
		for(int sz = program.size() - 1; sz >= 0; sz--)
		{
			for(auto a : candidate)
			{
				cerr << "Extend " << oct << a << dec << " to get " << program[sz] << "\n";
				for(uint64_t w = 0; w <= 7; w++)
				{
					reg[0] = (a << 3) + w;
					reg[1] = ob;
					reg[2] = oc;
					auto res = run(program, reg);
					cerr << w << "->" << res[0] << "\n";
					if(res[0] == program[sz])
					{
						extended.push_back((a << 3) + w);
						reg[0] = (a << 3) + w;
						reg[1] = ob;
						reg[2] = oc;
						auto check = run(program, reg);
						cerr << "check " << oct << ((a << 3) + w) << dec << ": ";
						if(!std::equal(check.begin(), check.end(),
							program.begin() + sz))
						{
							cerr << "failed.\n";
						}
						else
						{
							cerr << "ok!\n";
						}
					}
				}
			}
			swap(candidate, extended);
			extended.clear();
			for(auto v : candidate) cerr << oct << v << dec << " ";
			cerr << "\n";
		}
		
		cout << candidate[0] << "\n";
	}
	return 0;
}

