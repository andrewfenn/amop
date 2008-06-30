CC=g++

SRCS=MockObjectBase.cpp DynamicObject.cpp CheckOffsetFunc.cpp DynamicFunction.cpp MockFunctionImpl.cpp VirtualTable.cpp

TESTSRCS=./Test/Main.cpp ./Test/TestMockObject.cpp ./Test/TestTemplateHelppers.cpp ./Test/TestVirtualFunctionRedirect.cpp 

INCPATH=./UnitTest++/src

LIBPATH=-L./ -L./UnitTest++/

OBJS=$(patsubst %.cpp, %.o, $(SRCS))
TESTOBJS=$(patsubst %.cpp, %.o, $(TESTSRCS))

main:$(OBJS)
	ar rcs libAmop.a $(OBJS)

$(OBJS):
	$(CC) -Wall -g -O0 -c $(SRCS) 

test: main
	$(CC) -Wall -g -O0 -I$(INCPATH) $(LIBPATH) $(TESTSRCS) -lAmop -lUnitTest++ -o runTest
	@./runTest

clean:
	@rm -f $(TESTOBJS) $(OBJS) libAmop.a runTest 2> /dev/null

all: test

