CC=g++

SRCS=MockObjectBase.cpp DynamicObject.cpp CheckOffsetFunc.cpp DynamicFunction.cpp MockFunctionImpl.cpp VirtualTable.cpp

TESTSRCS=./Test/Main.cpp ./Test/TestMockObject.cpp ./Test/TestTemplateHelppers.cpp ./Test/TestVirtualFunctionRedirect.cpp 

INCPATH=-I./UnitTest++/src -I../Include

LIBPATH=-L./ -L./UnitTest++/

OBJS=$(patsubst %.cpp, %.o, $(SRCS))
TESTOBJS=$(patsubst %.cpp, %.o, $(TESTSRCS))

main:$(OBJS)
	ar rcs libAmop.a $(OBJS)

$(OBJS):
	$(CC) -Wall -g -c $(INCPATH) $(SRCS) 

test: main
	$(CC) -Wall -g $(INCPATH) $(LIBPATH) $(TESTSRCS) -lAmop -lUnitTest++ -o runTest
	@./runTest

clean:
	@rm -f $(TESTOBJS) $(OBJS) libAmop.a runTest 2> /dev/null

all: test

