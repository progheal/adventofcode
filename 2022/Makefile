#21a: STD = gnu++20

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

10a: LINKFLAGS += -L../aoclib/build -laoc

15s: 15s.o
	$(CXX) $^ $(LINKFLAGS) -o $@

.SUFFIXES:

%b: %a
	cp -f ./$^ ./$@

%a: %.o
	$(CXX) $^ $(LINKFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -c -o $@

.NOTINTERMEDIATE: %a

