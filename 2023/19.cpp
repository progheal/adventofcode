#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <optional>
#include "aoc/aoc.h"
using namespace std;

int partdummy;

struct Part
{
	int x, m, a, s;
	int& operator [](char c)
	{
		switch(c)
		{
		case 'x': return x;
		case 'm': return m;
		case 'a': return a;
		case 's': return s;
		default:  return partdummy;
		}
	}
	int operator [](char c) const
	{
		switch(c)
		{
		case 'x': return x;
		case 'm': return m;
		case 'a': return a;
		case 's': return s;
		default:  return 0;
		}
	}
};

istream& operator >> (istream& in, Part& p)
{
	char dummy, key;
	in >> dummy; // '{'
	do
	{
		in >> key;
		in >> dummy >> p[key] >> dummy;
	} while(dummy != '}');
	return in;
}

struct PartRange
{
	Part min, max;
	int64_t size()
	{
		return (int64_t)(max.x - min.x) *
		       (int64_t)(max.m - min.m) *
		       (int64_t)(max.a - min.a) *
		       (int64_t)(max.s - min.s);

	}
};

ostream& operator << (ostream& out, const PartRange& pr)
{
	out <<  "{X(" << pr.min.x << "," << pr.max.x
	    << "),M(" << pr.min.m << "," << pr.max.m
	    << "),A(" << pr.min.a << "," << pr.max.a
	    << "),S(" << pr.min.s << "," << pr.max.s << ")}";
	return out;
}

struct Branch
{
	char src;
	char op;
	int value;
	string target;
	bool operator() (const Part& p)
	{
		if(src == 0) return true;
		return op == '<' ? p[src] < value : p[src] > value;
	}
	pair<optional<PartRange>, optional<PartRange>> SplitRange(const PartRange&);
};

ostream& operator << (ostream& out, const Branch& b)
{
	if(b.src != 0)
		out << b.src << b.op << b.value << ':' << b.target;
	else
		out << b.target;
	return out;
}

struct Workflow
{
	vector<Branch> br;
};

ostream& operator << (ostream& out, const Workflow& w)
{
	bool first = true;
	for(auto& b : w.br)
	{
		if(first) out << '{'; else out << ',';
		out << b;
		first = false;
	}
	out << '}';
	return out;
}

map<string, Workflow> process;
vector<Part> parts;

pair<optional<PartRange>, optional<PartRange>> Branch::SplitRange(const PartRange& r)
{
	if(src == 0)
		return {r, nullopt};
	bool minok = operator()(r.min), maxok = operator()(r.max);
	if(minok && maxok)
		return {r, nullopt};
	else if(!minok && !maxok)
		return {nullopt, r};
	else
	{
		PartRange prT = r, prF = r;
		if(minok && !maxok)
		{
			prT.max[src] = value;
			prF.min[src] = value;
		}
		else
		{
			prT.min[src] = value+1;
			prF.max[src] = value+1;
		}
		return {prT, prF};
	}
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	bool readProcess = true;
	AOC::readPerLine(cin, [&](const string& s){
		if(s == "") {readProcess = false; return;}
		if(readProcess)
		{
			size_t br = s.find('{');
			string label = s.substr(0, br);
			auto& w = process[label];
			AOC::read<string>(s.substr(br+1, s.length() - br - 2),
				[&](const string sbr)
				{
					if(sbr.size() == 1 || (sbr[1] != '<' && sbr[1] != '>'))
					{
						w.br.push_back({0, 0, 0, sbr});
					}
					else
					{
						size_t col = sbr.find(':');
						w.br.push_back({sbr[0], sbr[1], stoi(sbr.substr(2)), sbr.substr(col+1)});
					}
				}, ',');
		}
		else
		{
			Part p;
			stringstream ss(s);
			ss >> p;
			parts.push_back(p);
		}
	}, true);

	if(!partb)
	{
		int64_t sum = 0;
		for(const auto& p : parts)
		{
			string curr = "in";
			while(curr != "A" && curr != "R")
			{
				auto& w = process[curr];
				for(auto& b : w.br)
				{
					if(b(p))
					{
						curr = b.target;
						break;
					}
				}
			}
			if(curr == "A")
			{
				sum += p.x + p.m + p.a + p.s;
			}
		}
		cout << sum << endl;
	}
	else
	{
		map<string, vector<PartRange>> pipeline;
		queue<string> stage;
		pipeline["in"].push_back(PartRange{Part{1,1,1,1}, Part{4001,4001,4001,4001}});
		stage.push("in");
		while(!stage.empty())
		{
			string site = stage.front();
			stage.pop();
			auto& vb = process[site].br;
			//cerr << "Running " << site << process[site] << " on: " << endl;
			for(auto pr : pipeline[site])
			{
				//cerr << "    " << pr << endl;
				for(auto& v : vb)
				{
					auto [prT, prF] = v.SplitRange(pr);
					if(prT)
					{
						//cerr << "    => split " << *prT << " to " << v.target << endl;
						pipeline[v.target].push_back(*prT);
						if(v.target != "A" && v.target != "R")
							stage.push(v.target);
					}
					if(!prF) break;
					pr = *prF;
				}
			}
			pipeline[site].clear();
		}
		size_t A = 0;
		for(auto& pr : pipeline["A"])
		{
			A += pr.size();
			/*
			cerr
				<< "[" << pr.min.x << "-" << (pr.max.x-1) << "]x"
				<< "[" << pr.min.m << "-" << (pr.max.m-1) << "]x"
				<< "[" << pr.min.a << "-" << (pr.max.a-1) << "]x"
				<< "[" << pr.min.s << "-" << (pr.max.s-1) << "]:"
				<< pr.size() << " parts" << endl;
			*/
		}
		cout << A << endl;
	}

	return 0;
}

