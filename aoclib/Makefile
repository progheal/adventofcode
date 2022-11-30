TOP := $(dir $(firstword $(MAKEFILE_LIST)))

INCLUDE = ${TOP}include
BUILD = ${TOP}build
SRC = ${TOP}src
TEST = ${TOP}test

CXXFLAGS = -std=gnu++17
CXXFLAGS += -I${INCLUDE}

ifdef DEBUG
	CXXFLAGS += -g -DDEBUG=$(DEBUG)
endif

all: ${BUILD}/libaoc.a ${BUILD}/test/aococr

${BUILD}:
	mkdir ${BUILD}

${BUILD}/libaoc.a: ${BUILD}/ocr.o ${BUILD}/util.o
	$(AR) cr $@ $^

${BUILD}/%.o: ${SRC}/%.cpp | ${BUILD}
	$(CXX) $(CXXFLAGS) $^ -c -o $@

LINKFLAGS = -L${BUILD}
TESTOBJECTS = ${BUILD}/test/ocrmain.o
LIBRARIES = ${BUILD}/libaoc.a
LIBARGS = -laoc

${BUILD}/test:
	mkdir ${BUILD}/test

${BUILD}/test/%.o: ${TEST}/%.cpp | ${BUILD}/test
	$(CXX) $(CXXFLAGS) $^ -c -o $@

${BUILD}/test/aococr: $(TESTOBJECTS) $(LIBRARIES)
	$(CXX) $(LINKFLAGS) $(TESTOBJECTS) $(LIBARGS) -o $@

