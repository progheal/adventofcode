#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include "aoc/aoc.h"
#include "aoc/aocmachine.h"
#include "aoc/ocr.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	char crt[6][41] = {};

	vector<int64_t> intercept;
	int cycleCount = 0;
	auto cycle = [&](int64_t reg){
		++cycleCount;
		// cerr << cycleCount << " " << reg << endl;
		if(cycleCount % 40 == 20)
			intercept.push_back(reg * cycleCount);
		if(cycleCount < 240)
		{
			int crtcol = (cycleCount-1) % 40;
			int crtrow = (cycleCount-1) / 40;
			if(abs(reg - crtcol) <= 1)
				crt[crtrow][crtcol] = '#';
			else
				crt[crtrow][crtcol] = '.';
		}
	};

	AOC::Assembly::ISA64 isa;
	isa.addInstruction<1>("addx",
		[&](int64_t v, auto& m, auto& p)->int {
			int64_t r = m["x"];
			cycle(r);
			cycle(r);
			m["x"] = r+v;
			return 0;
		});
	isa.addInstruction<0>("noop",
		[&](auto& m, auto& p)->int {
			cycle(m["x"]);
			return 0;
		});

	auto prog = AOC::readToMatrix<string>(cin);
	auto p = isa.assemble(prog);

	AOC::Assembly::Machine64 m;
	m["x"] = 1;

	p.execute(m);
	if(!partb)
		cout << accumulate(intercept.begin(), intercept.begin()+6, 0) << endl;
	else
	{
		vector<string> vs;
		for(int i = 0; i < 6; i++)
			vs.push_back(crt[i]);
		cout << AOC::ocr(vs) << endl;
	}

	return 0;
}

