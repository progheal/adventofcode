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

${BUILD}/libaoc.a: ${BUILD}/ocr.o ${BUILD}/util.o
	$(AR) cr $@ $^

${BUILD}/%.o: ${SRC}/%.cpp | ${BUILD}
	$(CXX) $(CXXFLAGS) $^ -c -o $@

LINKFLAGS = -L${BUILD}
TESTS = ocrmain rotate tokenize
LIBRARIES = ${BUILD}/libaoc.a
LIBARGS = -laoc

test: $(foreach target, ${TESTS}, ${BUILD}/test/${target})

${BUILD}/test:
	mkdir ${BUILD}/test

${BUILD}/test/%.o: ${TEST}/%.cpp | ${BUILD}/test
	$(CXX) $(CXXFLAGS) $^ -c -o $@

${BUILD}/test/%: ${BUILD}/test/%.o $(LIBRARIES)
	$(CXX) $(LINKFLAGS) $< $(LIBARGS) -o $@

