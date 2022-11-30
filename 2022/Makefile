#21a: STD = gnu++20

STD ?= gnu++17
CXXFLAGS = -std=$(STD)
CXXFLAGS += -I../aoclib/include/

ifdef DEBUG
  CXXFLAGS += -g
endif

ifdef D
  CXXFLAGS += $(D)
endif

# 08a: LINKFLAGS += -L../aoclib -laoc

.SUFFIXES:

%b: %a
	cp -f ./$^ ./$@

%a: %.o
	$(CXX) $(LINKFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -c -o $@

.NOTINTERMEDIATE: %a

