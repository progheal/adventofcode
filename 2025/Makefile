
STD ?= gnu++20
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

#19a: LINKFLAGS += -L../aoclib/build -laoc

.SUFFIXES:

%b: %a
	cp -f ./$^ ./$@

%a: %.o
	$(CXX) $^ $(LINKFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -c -o $@

.NOTINTERMEDIATE: %a
