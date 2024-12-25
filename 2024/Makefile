#00a: STD = gnu++20

STD ?= gnu++17
CXXFLAGS = -std=$(STD)
CXXFLAGS += -I../aoclib/include/

ifdef DEBUG
  CXXFLAGS += -g
endif

ifdef OPT
  CXXFLAGS += -O3
endif

ifdef D
  CXXFLAGS += $(D)
endif

19a: LINKFLAGS += -L../aoclib/build -laoc

.SUFFIXES:

%b: %a
	cp -f ./$^ ./$@

%a: %.o
	$(CXX) $^ $(LINKFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -c -o $@

09b: 09b.o
	$(CXX) $^ $(LINKFLAGS) -o $@

24.dot: 24.in 24.sed
	(echo digraph {; sed -Ef 24.sed 24.in; echo }) > 24.dot

24.svg: 24.dot
	dot -Tsvg 24.dot -o 24.svg

.NOTINTERMEDIATE: %a
