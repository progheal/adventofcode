#pragma once

#include <iosfwd>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <cctype>
#include <exception>

namespace AOC
{
namespace Assembly
{

// 執行的機器，儲存暫存器之值
// 提供界面存取其暫存器
// 有一個可改名的特殊暫存器做為程式指標 IP，預設為 "_ip" 一般輸入應該用不到
// 若題目有指定程式指標名可另作設定
template<class RegType = int>
class Machine
{
	std::map<std::string, RegType> registerFile;
	std::string ipName = "_ip";
public:
	typedef RegType RegisterType;
	void setIPRegisterName(const std::string& s) {ipName = s;}
	RegType& getIP()
	{
		return registerFile[ipName];
	}
	RegType& getRegister(const std::string& name)
	{
		return registerFile[name];
	}
	RegType& operator[] (const std::string& name)
	{
		return registerFile[name];
	}
	void print(std::ostream& out)
	{
		for(auto& [name, value] : registerFile)
		{
			if(name == ipName) continue;
			out << name << "=" << value << " ";
		}
	}
};

// 指定機器的單字長度
typedef Machine<int> Machine32;
typedef Machine<int64_t> Machine64;

// 前置宣告程式類別
template<class RegType> class Program;

// 程式中的命令抽象類別
template<class RegType = int>
struct Command
{
	virtual int execute(Machine<RegType>& m, Program<RegType>& p) = 0;
	virtual std::string operator [] (int i) = 0;
	virtual void print(std::ostream&) = 0;
	virtual ~Command() {}
};

// 指命令實作，支援最多三個參數
// Callback 為命令實際內容，並回傳跳轉偏移量 (相對於*下一個命令*，即回傳 0 表示不跳轉)
template<class RegType, int ArgCount, class Callback>
class ConcreteCommand : public Command<RegType>
{
	Callback callback;
	std::string op, arg1, arg2, arg3;
	void strip()
	{
		if(arg1.size() > 1 && arg1.back() == ',') arg1.pop_back();
		if(arg2.size() > 1 && arg2.back() == ',') arg2.pop_back();
		if(arg3.size() > 1 && arg3.back() == ',') arg3.pop_back();
	}
public:
	template<int AC = ArgCount, std::enable_if_t<AC == 0, int> = 0>
	ConcreteCommand(Callback cb, std::string op) : callback(cb), op(op), arg1(), arg2(), arg3() {}
	template<int AC = ArgCount, std::enable_if_t<AC == 1, int> = 0>
	ConcreteCommand(Callback cb, std::string op, std::string a1) : callback(cb), op(op), arg1(a1), arg2(), arg3() {strip();}
	template<int AC = ArgCount, std::enable_if_t<AC == 2, int> = 0>
	ConcreteCommand(Callback cb, std::string op, std::string a1, std::string a2) : callback(cb), op(op), arg1(a1), arg2(a2), arg3() {strip();}
	template<int AC = ArgCount, std::enable_if_t<AC == 3, int> = 0>
	ConcreteCommand(Callback cb, std::string op, std::string a1, std::string a2, std::string a3) : callback(cb), op(op), arg1(a1), arg2(a2), arg3(a3) {strip();}
	// 執行
	virtual int execute(Machine<RegType>& m, Program<RegType>& p) override;
	// 存取指令本身的原始參數
	virtual std::string operator [] (int arg) override
	{
		switch(arg)
		{
		case 0: return op;
		case 1: return arg1;
		case 2: return arg2;
		case 3: return arg3;
		default: return "";
		}
	}
	// 印出指令
	virtual void print(std::ostream& out) override
	{
		out << op;
		if(arg1 != "") out << " " << arg1;
		if(arg2 != "") out << " " << arg2;
		if(arg3 != "") out << " " << arg3;
	}
};

// (不帶參數的)單個指令
// 使用 assemble 帶入參數產生實際執行的命令
template<class RegType>
struct Instruction
{
	virtual std::unique_ptr<Command<RegType>> assemble(const std::vector<std::string>& inst) = 0;
	virtual ~Instruction() {}
};

template<class RegType, int ArgCount, class Callback>
class ConcreteInstruction : public Instruction<RegType>
{
	std::string name;
	Callback callback;
public:
	ConcreteInstruction(std::string n, Callback cb) : name(n), callback(cb) {}
	virtual std::unique_ptr<Command<RegType>> assemble(const std::vector<std::string>& inst) override;
};

template<class RegType, int ArgCount, class Callback>
std::unique_ptr<Command<RegType>> ConcreteInstruction<RegType, ArgCount, Callback>::assemble(const std::vector<std::string>& inst)
{
	if constexpr (ArgCount == 0)
		return std::unique_ptr<Command<RegType>>(new ConcreteCommand<RegType, ArgCount, Callback>(callback, name));
	else if constexpr (ArgCount == 1)
		return std::unique_ptr<Command<RegType>>(new ConcreteCommand<RegType, ArgCount, Callback>(callback, name, inst[1]));
	else if constexpr (ArgCount == 2)
		return std::unique_ptr<Command<RegType>>(new ConcreteCommand<RegType, ArgCount, Callback>(callback, name, inst[1], inst[2]));
	else if constexpr (ArgCount == 3)
		return std::unique_ptr<Command<RegType>>(new ConcreteCommand<RegType, ArgCount, Callback>(callback, name, inst[1], inst[2], inst[3]));
	else
		return nullptr;
}

// 程式，包含指令串列
template<class RegType>
class Program
{
	std::vector<std::unique_ptr<Command<RegType>>> p;
public:
	Program(Program&& rhs) : p(std::move(rhs.p)) {};
	Program(std::vector<std::unique_ptr<Command<RegType>>>&& pp) : p(std::move(pp)) {} 
	// 執行
	// 命令的回傳值表相對*下一指令*的跳躍量，即 0 不跳躍
	void execute(Machine<RegType>& m)
	{
		RegType& ip = m.getIP();
		ip = 0;
		while(ip >= 0 && ip < p.size())
		{
			int jmp = p[ip]->execute(m, *this);
			ip += jmp + 1;
		}
	}
	// 執行並印出現在執行到的命令及機器狀態
	template<class Ostream>
	void execute(Machine<RegType>& m, Ostream& debug)
	{
		RegType& ip = m.getIP();
		ip = 0;
		m.print(debug);
		while(ip >= 0 && ip < p.size())
		{
			debug << ip << " ";
			p[ip]->print(debug);
			int jmp = p[ip]->execute(m, *this);
			ip += jmp + 1;
			debug << "; ip=" << ip << " ";
			m.print(debug);
			debug << std::endl;
		}
	}
	// 更換單個命令
	void setCommand(int pos, std::unique_ptr<Command<RegType>> c)
	{
		if(pos < 0 || pos >= p.size()) return;
		p[pos].swap(c);
	}
	// 取得單個指令
	std::vector<std::string> getCommand(int pos)
	{
		if(pos < 0 || pos >= p.size()) return {};
		std::vector<std::string> s;
		for(int i = 0; i < 4; i++)
		{
			std::string t = (*p[pos])[i];
			if(t != "") s.push_back(t);
		}
		return s;
	}
	// 印出程式
	friend std::ostream& operator << (std::ostream& out, const Program& p)
	{
		for(auto& pc : p.p)
		{
			pc->print(out);
			out << std::endl;
		}
		return out;
	}
};

// 指令集
template<class RegType>
class InstructionSet
{
	std::map<std::string, std::unique_ptr<Instruction<RegType>>> iset;
public:
	template<int ArgCount, class Callback>
	void addInstruction(std::string name, Callback callback);
	std::unique_ptr<Command<RegType>> assembleOne(const std::vector<std::string>& inst);
	Program<RegType> assemble(const std::vector<std::vector<std::string>>& program);
};

// 增加指令到指令集中
template<class RegType> template<int ArgCount, class Callback>
void InstructionSet<RegType>::addInstruction(std::string name, Callback callback)
{
	iset[name].reset(new ConcreteInstruction<RegType, ArgCount, Callback>(name, callback));
}

// 組譯一條指令
template<class RegType>
std::unique_ptr<Command<RegType>> InstructionSet<RegType>::assembleOne(const std::vector<std::string>& line)
{
	if(iset.count(line[0]) == 0)
		throw std::invalid_argument("Unknown instruction: " + line[0]);
	return iset[line[0]]->assemble(line);
}

// 組譯整隻程式
template<class RegType>
Program<RegType> InstructionSet<RegType>::assemble(const std::vector<std::vector<std::string>>& program)
{
	std::vector<std::unique_ptr<Command<RegType>>> list;
	for(const auto& line : program)
	{
		list.push_back(assembleOne(line));
	}
	return Program<RegType>{std::move(list)};
}

// 指定長度的指令集
typedef InstructionSet<int> ISA32;
typedef InstructionSet<int64_t> ISA64;

// (C++17) 編譯期判斷某指令是否修改程式；其 callback 多收兩個參數為執行中機器及執行中程式
// 使用例參照 2016 Day 23, Day 25
// 利用 SFINAE 判斷 invoke_result 是否為合法表達式
// https://stackoverflow.com/a/57452225
template<class RegType, int ArgCount, class Callback, class = int>
struct IsModifyingInstruction : public std::false_type {};

template<class RegType, class Callback>
struct IsModifyingInstruction<RegType, 0, Callback,
		typename std::invoke_result<Callback, Machine<RegType>&, Program<RegType>&>::type
	> : public std::true_type {};

template<class RegType, class Callback>
struct IsModifyingInstruction<RegType, 1, Callback,
		typename std::invoke_result<Callback, int&, Machine<RegType>&, Program<RegType>&>::type
	> : public std::true_type {};

template<class RegType, class Callback>
struct IsModifyingInstruction<RegType, 2, Callback,
		typename std::invoke_result<Callback, int&, int&, Machine<RegType>&, Program<RegType>&>::type
	> : public std::true_type {};

template<class RegType, class Callback>
struct IsModifyingInstruction<RegType, 3, Callback,
		typename std::invoke_result<Callback, int&, int&, int&, Machine<RegType>&, Program<RegType>&>::type
	> : public std::true_type {};

// 執行命令
template<class RegType, int ArgCount, class Callback>
int ConcreteCommand<RegType, ArgCount, Callback>::execute(Machine<RegType>& m, Program<RegType>& p)
{
	RegType v1, v2, v3;
	RegType *r1 = &v1, *r2 = &v2, *r3 = &v3;
	// 判斷參數，若需存取暫存器則以 r1 等間接連結
	if constexpr (ArgCount >= 1) { if(isdigit(arg1[0]) || arg1[0] == '-' || arg1[0] == '+') v1 = stoi(arg1); else r1 = &m.getRegister(arg1); }
	if constexpr (ArgCount >= 2) { if(isdigit(arg2[0]) || arg2[0] == '-' || arg2[0] == '+') v2 = stoi(arg2); else r2 = &m.getRegister(arg2); }
	if constexpr (ArgCount >= 3) { if(isdigit(arg3[0]) || arg3[0] == '-' || arg3[0] == '+') v3 = stoi(arg3); else r3 = &m.getRegister(arg3); }

	// 若指令想更改程式則傳入之
	// 這一大塊都是 if constexpr，編譯後只有 return callback(...) 留存
	if constexpr (IsModifyingInstruction<RegType, ArgCount, Callback>::value)
	{
		if constexpr (ArgCount == 0) return callback(m, p);
		else if constexpr (ArgCount == 1) { return callback(*r1, m, p); }
		else if constexpr (ArgCount == 2) { return callback(*r1, *r2, m, p); }
		else if constexpr (ArgCount == 3) { return callback(*r1, *r2, *r3, m, p); }
		else return 0;
	}
	else
	{
		if constexpr (ArgCount == 0) return callback();
		else if constexpr (ArgCount == 1) { return callback(*r1); }
		else if constexpr (ArgCount == 2) { return callback(*r1, *r2); }
		else if constexpr (ArgCount == 3) { return callback(*r1, *r2, *r3); }
		else return 0;
	}
}

// 預設常用的指令函數，若不用特別處理則可以直接放入指令中
// 因為我比較熟 Intel ASM 順序，所以預設是目標在前
// 如果是目標在後的則照慣例加上 r 字尾
// 64 位元單字的再加上 64 字尾
namespace Functions
{

auto nop = []()->int { return 0; };

auto mov = [](int& dst, int src)->int { dst = src; return 0; };
auto movr = [](int src, int& dst)->int { dst = src; return 0; };

auto add = [](int& dst, int src)->int { dst += src; return 0; };
auto sub = [](int& dst, int src)->int { dst -= src; return 0; };
auto mul = [](int& dst, int src)->int { dst *= src; return 0; };
auto idiv = [](int& dst, int src)->int { dst /= src; return 0; };
auto rem = [](int& dst, int src)->int { dst %= src; return 0; };

auto addr = [](int src, int& dst)->int { dst += src; return 0; };
auto subr = [](int src, int& dst)->int { dst -= src; return 0; };
auto mulr = [](int src, int& dst)->int { dst *= src; return 0; };
auto idivr = [](int src, int& dst)->int { dst /= src; return 0; };
auto remr = [](int src, int& dst)->int { dst %= src; return 0; };

auto inc = [](int& dst)->int { ++dst; return 0; };
auto dec = [](int& dst)->int { --dst; return 0; };

// 傳入參數預設是相對於**本身**的跳躍量，這是由於題目大多設定如此
// 但機器內部預設回傳相對於**下一指令**的跳躍量以優先保持「不跳躍的指令回傳 0」的設定
// 故預設函數全部都是回傳參數減一
auto jmp = [](int off)->int { return off - 1; };
auto jgz = [](int value, int off)->int { return value > 0 ? off - 1 : 0; };
auto jge = [](int value, int off)->int { return value >= 0 ? off - 1 : 0; };
auto jlz = [](int value, int off)->int { return value < 0 ? off - 1 : 0; };
auto jle = [](int value, int off)->int { return value <= 0 ? off - 1 : 0; };
auto jez = [](int value, int off)->int { return value == 0 ? off - 1 : 0; };
auto jnz = [](int value, int off)->int { return value != 0 ? off - 1 : 0; };

auto mov64 = [](int64_t& dst, int64_t src)->int { dst = src; return 0; };
auto movr64 = [](int64_t src, int64_t& dst)->int { dst = src; return 0; };

auto add64 = [](int64_t& dst, int64_t src)->int { dst += src; return 0; };
auto sub64 = [](int64_t& dst, int64_t src)->int { dst -= src; return 0; };
auto mul64 = [](int64_t& dst, int64_t src)->int { dst *= src; return 0; };
auto idiv64 = [](int64_t& dst, int64_t src)->int { dst /= src; return 0; };
auto rem64 = [](int64_t& dst, int64_t src)->int { dst %= src; return 0; };

auto addr64 = [](int64_t src, int64_t& dst)->int { dst += src; return 0; };
auto subr64 = [](int64_t src, int64_t& dst)->int { dst -= src; return 0; };
auto mulr64 = [](int64_t src, int64_t& dst)->int { dst *= src; return 0; };
auto idivr64 = [](int64_t src, int64_t& dst)->int { dst /= src; return 0; };
auto remr64 = [](int64_t src, int64_t& dst)->int { dst %= src; return 0; };

auto inc64 = [](int64_t& dst)->int { ++dst; return 0; };
auto dec64 = [](int64_t& dst)->int { --dst; return 0; };

auto jmp64 = [](int64_t off)->int { return off - 1; };
auto jgz64 = [](int64_t value, int64_t off)->int { return value > 0 ? off - 1 : 0; };
auto jge64 = [](int64_t value, int64_t off)->int { return value >= 0 ? off - 1 : 0; };
auto jlz64 = [](int64_t value, int64_t off)->int { return value < 0 ? off - 1 : 0; };
auto jle64 = [](int64_t value, int64_t off)->int { return value <= 0 ? off - 1 : 0; };
auto jez64 = [](int64_t value, int64_t off)->int { return value == 0 ? off - 1 : 0; };
auto jnz64 = [](int64_t value, int64_t off)->int { return value != 0 ? off - 1 : 0; };

} // namespace Functions

} // namespace Assembly
} // namespace AOC
