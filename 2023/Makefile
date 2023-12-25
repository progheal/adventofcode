05a: STD = gnu++20

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

08a 13a: LINKFLAGS += -L../aoclib/build -laoc

.SUFFIXES:

%b: %a
	cp -f ./$^ ./$@

%a: %.o
	$(CXX) $^ $(LINKFLAGS) -o $@

11m: 11m.o
	$(CXX) $^ $(LINKFLAGS) -o $@

17large_a: 17large.o
	$(CXX) $^ $(LINKFLAGS) -o $@

20b: 20b.o
	$(CXX) $^ $(LINKFLAGS) -o $@

21b: 21b.o
	$(CXX) $^ $(LINKFLAGS) -o $@

23b: 23b.o
	$(CXX) $^ $(LINKFLAGS) -o $@

24b: 24b.o
	$(CXX) $^ $(LINKFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -c -o $@

.NOTINTERMEDIATE: %a
