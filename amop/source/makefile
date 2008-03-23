CC=g++

SRCS=MockObject.cpp MockObjectBase.cpp CheckOffsetFunc.cpp 

TESTSRCS=./Test/Main.cpp ./Test/TestMockObject.cpp ./Test/TestTemplateHelppers.cpp ./Test/TestVirtualFunctionRedirect.cpp 

INCPATH=./UnitTest++/src

LIBPATH=-L./ -L./UnitTest++/

OBJS=$(patsubst %.cpp, %.o, $(SRCS))
TESTOBJS=$(patsubst %.cpp, %.o, $(TESTSRCS))

main:$(OBJS)
	ar rcs libAmop.a $(OBJS)

$(OBJS):
	$(CC) -c $(SRCS) 

test: main
	$(CC) -I$(INCPATH) $(LIBPATH) $(TESTSRCS) -lAmop -lUnitTest++ -o runTest
	@./runTest

clean:
	@rm -f $(TESTOBJS) $(OBJS) libAmop.a runTest 2> /dev/null
	
all: test
	
