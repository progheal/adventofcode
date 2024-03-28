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

.PHONY: all test

all: ${BUILD}/libaoc.a test

${BUILD}:
	mkdir ${BUILD}

${BUILD}/libaoc.a: ${BUILD}/ocr.o ${BUILD}/util.o ${BUILD}/ap.o
	$(AR) cr $@ $^

${BUILD}/%.o: ${SRC}/%.cpp | ${BUILD}
	$(CXX) $(CXXFLAGS) $^ -c -o $@

LINKFLAGS = -L${BUILD}
TESTS = ap grid input ocr rotate tokenize
LIBRARIES = ${BUILD}/libaoc.a
LIBARGS = -laoc -lgtest -lgmock -lgtest_main

TESTOBJ = $(foreach target, ${TESTS}, ${BUILD}/test/${target}.o)

test: ${BUILD}/test/testall

${BUILD}/test:
	mkdir ${BUILD}/test

${BUILD}/test/testall: ${TESTOBJ} ${BUILD}/libaoc.a
	$(CXX) $(LINKFLAGS) ${TESTOBJ} $(LIBARGS) -o $@

${BUILD}/test/%.o: ${TEST}/%.cpp | ${BUILD}/test
	$(CXX) $(CXXFLAGS) $^ -c -o $@
