#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include "aoc/aoc.h"
using namespace std;

struct Signal
{
	string from;
	bool pulse;
};

struct Module
{
	string name;
	vector<size_t> history;
	size_t lastPeriod;
	size_t periodLen;
	Module(const string& name = {}) : name(name), history(), lastPeriod() {}
	void endTick();
	pair<size_t, size_t> possiblePeriod();
	virtual Signal signal(const Signal&) {return Signal{};}
	virtual size_t hash() const {return 0;}
	virtual ~Module() = default;
};

void Module::endTick()
{
	history.push_back(hash());
	size_t hlen = history.size();
	if(lastPeriod && history[hlen-1] == history[hlen-1-lastPeriod])
	{
		++periodLen;
		return;
	}
	lastPeriod = periodLen = 0;
	for(size_t p = 1; p <= hlen / 2; p++)
	{
		auto ri = history.rbegin();
		auto rip = next(ri, p);
		size_t thislen = mismatch(rip, history.rend(), ri).first - rip;
		if(thislen >= p*5 && thislen >= 20 && thislen > periodLen)
		{
			periodLen = thislen;
			lastPeriod = p;
		}
	}
}

pair<size_t, size_t> Module::possiblePeriod()
{
	return {lastPeriod, periodLen};
}

class FlipFlop : public Module
{
	bool state;
public:
	FlipFlop(const string& name) : Module(name), state(false) {}
	virtual Signal signal(const Signal& s) override
	{
		if(s.pulse)
			return Signal{};
		else
		{
			state = !state;
			return Signal{name, state};
		}
	}
	virtual size_t hash() const override {return state?1:0;}
};

class Conjunction : public Module
{
	map<string, bool> state;
public:
	Conjunction(const string& name) : Module(name), state() {}
	void addInput(const string& in)
	{
		state[in] = false;
	}
	virtual Signal signal(const Signal& s) override
	{
		state[s.from] = s.pulse;
		bool result = !all_of(begin(state), end(state), [](const auto& info){return info.second;});
		return Signal{name, result};
	}
	virtual size_t hash() const override
	{
		size_t h = 0;
		for(auto& [name, input] : state) h = h * 2 + (input?1:0);
		return h;
	}
};

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	map<string, unique_ptr<Module>> machine;
	map<string, vector<string>> wires;

	AOC::readPerLine(cin, [&](const string& desc) {
		size_t arrow = desc.find(" -> ");
		string name = desc.substr(0, arrow);
		if(name[0] == '%')
		{
			name = name.substr(1);
			machine[name] = make_unique<FlipFlop>(name);
		}
		else if(name[0] == '&')
		{
			name = name.substr(1);
			machine[name] = make_unique<Conjunction>(name);
		}
		vector<string> dest;
		size_t pos = arrow + 4, comma;
		while((comma = desc.find(", ", pos)) != string::npos)
		{
			dest.push_back(desc.substr(pos, comma - pos));
			pos = comma + 2;
		}
		dest.push_back(desc.substr(pos));
		wires[name].swap(dest);
	});

	for(auto& [name, dest] : wires)
	{
		for(auto& dn : dest)
		{
			if(machine.count(dn) > 0)
			{
				auto p = dynamic_cast<Conjunction*>(machine[dn].get());
				if(p) p->addInput(name);
			}
		}
	}

	bool activate = false;
	int64_t tick = 0;

	auto button = [&]() -> pair<int64_t, int64_t>
	{
		int64_t low = 0, high = 0;
		queue<pair<string, Signal>> sig;
		low = wires["broadcaster"].size() + 1;
		for(auto& dest : wires["broadcaster"])
		{
			sig.push({dest, {"broadcaster", false}});
		}
		while(!sig.empty())
		{
			auto [dest, signal] = sig.front();
			sig.pop();
			//cerr << signal.from << " -" << (signal.pulse ? "high" : "low") << "-> " << dest << endl;
			if(machine.count(dest) == 0) continue;
			auto result = machine[dest]->signal(signal);
			if(result.from != "")
			{
				(result.pulse ? high : low) += wires[dest].size();
				for(auto& target : wires[dest])
				{
					sig.push({target, result});
					if(target == "rx" && !result.pulse) activate = true;
				}
			}
		}
		for(auto& [name, module] : machine)
		{
			module->endTick();
		}
		++tick;
		return {low, high};
	};

	auto snapshot = [&]() -> string
	{
		stringstream ss;
		for(auto& [name, module] : machine)
		{
			ss << name << module->hash();
		}
		return ss.str();
	};

	int64_t rep = 1000;
	int64_t lowAll = 0, highAll = 0;
	int64_t count;
	int lastPeriodCount = 0;
	map<string, pair<int, int>> periodStatus;
	for(count = 1; partb ? !activate : count <= rep ; count++)
	{
		auto [l, h] = button();
		string snap = snapshot();
		lowAll += l;
		highAll += h;
		if(count % 1000 == 0)
			cerr << count << " " << snap << " " << l << " " << h << "     " << endl;
		int nocycle = 0;
		for(auto& [name, module] : machine)
		{
			auto current = module->possiblePeriod();
			if(current.first == 0) ++nocycle;
			if(current.first != periodStatus[name].first)
			{
				cerr << "Module " << name << " thinks it";
				if(current.first == 0)
					cerr << " no longer in a cycle." << endl;
				else
					cerr << " is now in a cycle of length " << current.first << " holding for " << current.second << " ticks." << endl;
			}
			periodStatus[name] = current;
		}
		if(nocycle == 0)
		{
			cerr << "It is tick " << tick << " now, everyone think they are in a cycle. Here are what they think:" << endl;
			for(auto& [name, module] : machine)
			{
				auto [len, run] = module->possiblePeriod();
				cerr << "Module " << name << " thinks the cycle has length " << len << " and already have run length " << run << endl;
			}
			break;
		}
	}
	if(!partb)
		cout << (lowAll * highAll) << endl;
	else
		;

	return 0;
}

