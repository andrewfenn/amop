#include <UnitTest++.h>
#include "MockObject.h"
#include <string>

using namespace amop;

//------------------------------------------------------------------
class IInterface
{
public:
    virtual void SimpleFunction() = 0;
    virtual void SimpleFunctionWithParams(float, std::string, const char*) = 0;

    virtual int SimpleFunctionWithParamsAndReturn(float, std::string, const char*) = 0;

    virtual void SimpleFunctionWithAlotParams(float, int, float, int, std::string) = 0;

    virtual int SimpleFunctionWithReturn() = 0;
    virtual int& SimpleFunctionWithRefReturn() = 0;

    virtual void SimpleConstFunction() const = 0;

    virtual std::string ComplexFunction(const std::string& crs,
        std::string& rs, std::string s) = 0;

    virtual std::string ComplexConstFunction(const std::string& crs,
        std::string& rs, std::string s) const = 0;

    virtual void PolicyTestFunction(
        std::string& str
        , std::string* pointedStr
        , char* const buffer
        );


    virtual ~IInterface(){}
};

//------------------------------------------------------------------
TEST(CreateMockObject)
{
    TMockObject<IInterface> mock;

    CHECK( (IInterface*)mock );	
}

//------------------------------------------------------------------
TEST(MockObjectNotImplementedThrowSimpleFunction)
{
    TMockObject<IInterface> mock;

    CHECK((IInterface*)mock);

    CHECK_THROW(mock->SimpleFunction(), TNotImplementedException);
    CHECK_THROW((*mock).SimpleFunction(), TNotImplementedException);
}

//------------------------------------------------------------------
TEST(MockObjectNotImplementedThrowComplexFunction)
{
    TMockObject<IInterface> mock;

    CHECK((IInterface*)mock);

    std::string crs, rs, s;

    CHECK_THROW(
        mock->ComplexFunction(crs, rs, s), 
        TNotImplementedException);
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimple)
{
    TMockObject<IInterface> mock;

    mock.EveryCall(&IInterface::SimpleFunction);
    mock.EveryCall(&IInterface::SimpleFunctionWithAlotParams);

    mock->SimpleFunction();
    mock->SimpleFunctionWithAlotParams(0, 0, 0, 0, std::string());

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodComplex)
{
    TMockObject<IInterface> mock;

    mock.EveryCall(&IInterface::ComplexFunction)
        .Return("Test Result");
    std::string crs, rs, s;

    std::string result = 
        mock->ComplexFunction(crs, rs, s);

    CHECK_EQUAL("Test Result", result.c_str());
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleWithReturn)
{
    TMockObject<IInterface> mock;

    mock.EveryCall(&IInterface::SimpleFunctionWithReturn)
        .Return(22);
    mock.EveryCall(&IInterface::SimpleFunctionWithRefReturn)
        .Return(1262);

    CHECK_EQUAL(22, mock->SimpleFunctionWithReturn());
    CHECK_EQUAL(1262, mock->SimpleFunctionWithRefReturn());
}

//------------------------------------------------------------------
TEST(MockObjectMethodConstSimple)
{
    TMockObject<IInterface> mock;

    mock.EveryCall(&IInterface::SimpleConstFunction);

    mock->SimpleConstFunction();	

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

struct SimpleException : public std::exception
{
  int value;
  SimpleException(int v)
    : std::exception()
    , value(v){
  }
};

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleWithThrow)
{
    TMockObject<IInterface> mock;

    mock.EveryCall(&IInterface::SimpleFunctionWithReturn)
      .Throw(std::exception());

    CHECK_THROW(mock->SimpleFunctionWithReturn(), std::exception );
    
    mock.Verify();
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleWithThrowNoReturn)
{
    TMockObject<IInterface> mock;

    mock.EveryCall(&IInterface::SimpleFunction)
      .Throw(std::exception());

    CHECK_THROW(mock->SimpleFunction(), std::exception );
    
    mock.Verify();
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleWithInheritedThrow)
{
    TMockObject<IInterface> mock;

    mock.EveryCall(&IInterface::SimpleFunctionWithReturn)
      .Throw(SimpleException(22));

    CHECK_THROW(mock->SimpleFunctionWithReturn(), std::exception);
    
    mock.Verify();
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleWithManyThrow)
{
    TMockObject<IInterface> mock;

    mock.Call(&IInterface::SimpleFunctionWithReturn)
      .Throw(SimpleException(22));
    mock.Call(&IInterface::SimpleFunctionWithReturn)
      .Return(42);
    mock.Call(&IInterface::SimpleFunctionWithReturn)
      .Throw(SimpleException(142));
    
    try{
        mock->SimpleFunctionWithReturn();
        CHECK(false);
    }catch(SimpleException & se){
        CHECK_EQUAL(22, se.value);
    }
    CHECK_EQUAL(42, mock->SimpleFunctionWithReturn());
    try{
        mock->SimpleFunctionWithReturn();
        CHECK(false);
    }catch(SimpleException & se){
        CHECK_EQUAL(142, se.value);
    }
    
    mock.Verify();
}


//------------------------------------------------------------------
TEST(MockObjectMethodConstComplex)
{
    TMockObject<IInterface> mock;

    mock.EveryCall(&IInterface::ComplexConstFunction)
        .Return("Test Result");
    std::string crs, rs, s;

    std::string result = 
        mock->ComplexConstFunction(crs, rs, s);

    CHECK_EQUAL("Test Result", result.c_str());
}


//------------------------------------------------------------------
TEST(MockObjectMethodMultiWithReturn)
{
    TMockObject<IInterface> mock;

    mock.Call(&IInterface::SimpleFunctionWithReturn)
        .Return(22).Return(11);

    CHECK_EQUAL(22, mock->SimpleFunctionWithReturn());
    CHECK_EQUAL(11, mock->SimpleFunctionWithReturn());
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpect)
{
    TMockObject<IInterface> mock;

    mock.EveryCall(&IInterface::SimpleFunctionWithParams)
        .ExpectOne<0>(21.0f)
        .ExpectOne<1>("Hello World")
        .ExpectOne<2>("SomeText");

    mock->SimpleFunctionWithParams(21.0f, "Hello World", "SomeText");

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpectAll)
{
    TMockObject<IInterface> mock;

    mock.EveryCall(&IInterface::SimpleFunctionWithParams)
        .Expect(21.0f, "Hello World", "SomeText");

    mock->SimpleFunctionWithParams(21.0f, "Hello World", "SomeText");

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpectAllIgnore)
{
    TMockObject<IInterface> mock;

    mock.EveryCall(&IInterface::SimpleFunctionWithParams)
        .Expect(21.0f, Ignore, "SomeText");

    mock->SimpleFunctionWithParams(21.0f, "Hello World", "SomeText");

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpectPolicy)
{
    TMockObject<IInterface> mock;

    mock.EveryCall(&IInterface::PolicyTestFunction)
        .ExpectOne<0>("First")
        .ExpectOne<1>(Policy::Pointer("Second"))
        .ExpectOne<2>(Policy::Array("Third", strlen("Third") + 1) );

    std::string first = "First";
    std::string second = "Second";
    char buf[] = "Third";
    
    mock->PolicyTestFunction(first, &second, buf) ;

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpectPolicyAll)
{
    TMockObject<IInterface> mock;

    mock.EveryCall(&IInterface::PolicyTestFunction)
        .Expect("First", Policy::Pointer("Second"), Policy::Array("Third", strlen("Third") + 1) );

    std::string first = "First";
    std::string second = "Second";
    char buf[] = "Third";
    
    mock->PolicyTestFunction(first, &second, buf) ;

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpectThrow)
{
    TMockObject<IInterface> mock;

    mock.EveryCall(&IInterface::SimpleFunctionWithParams)
        .ExpectOne<0>(21.0f)
        .ExpectOne<1>("Hello World")
        .ExpectOne<2>("SomeText");

    CHECK_THROW( mock->SimpleFunctionWithParams(2.0f, "Hllo World", "SomeTet")
        , TNotEqualException );
}


//------------------------------------------------------------------
TEST(MockObjectMethodMultipleExpectOne)
{
    TMockObject<IInterface> mock;

    mock.Call(&IInterface::SimpleFunctionWithParams)
        .ExpectOne<0>(1.0f).ExpectOne<1>("Hello 1").ExpectOne<2>("SomeText1") 
        .ExpectOne<0>(2.0f).ExpectOne<1>("Hello 2").ExpectOne<2>("SomeText2")	
        .ExpectOne<0>(3.0f).ExpectOne<1>("Hello 3").ExpectOne<2>("SomeText3");

    mock->SimpleFunctionWithParams(1.0f, "Hello 1", "SomeText1");
    mock->SimpleFunctionWithParams(2.0f, "Hello 2", "SomeText2");
    mock->SimpleFunctionWithParams(3.0f, "Hello 3", "SomeText3");

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodMultipleExpect)
{
    TMockObject<IInterface> mock;

    mock.Call(&IInterface::SimpleFunctionWithParams)
        .Expect(1.0f, "Hello 1", "SomeText1")
        .Expect(2.0f, "Hello 2", "SomeText2")	
        .Expect(3.0f, "Hello 3", "SomeText3");

    mock->SimpleFunctionWithParams(1.0f, "Hello 1", "SomeText1");
    mock->SimpleFunctionWithParams(2.0f, "Hello 2", "SomeText2");
    mock->SimpleFunctionWithParams(3.0f, "Hello 3", "SomeText3");

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodMultipleExpectgnoreMultiple)
{
    TMockObject<IInterface> mock;

    mock.Call(&IInterface::SimpleFunctionWithParams)
        .Expect(Ignore, Ignore, "SomeText")
        .Expect(Ignore, "Hello World", Ignore)
        .Expect(3.0f,  Ignore, Ignore);

    mock->SimpleFunctionWithParams(1.0f, "asfdlkjsf",  "SomeText");
    mock->SimpleFunctionWithParams(2.0f, "Hello World", "fsda");
    mock->SimpleFunctionWithParams(3.0f, "asdfjdskjfl", "fdsafjl");

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}



//------------------------------------------------------------------
TEST(MockObjectMethodMultipleExpectThrow)
{
    TMockObject<IInterface, ManualVerify> mock;

    mock.Call(&IInterface::SimpleFunctionWithParams)
        .ExpectOne<0>(1.0f)		.ExpectOne<0>(2.0f)			.ExpectOne<0>(3.0f)
        .ExpectOne<1>("Hello 1")	.ExpectOne<1>("Hello 2")		.ExpectOne<1>("Hello 3")
        .ExpectOne<2>("SomeText1").ExpectOne<2>("SomeText2")	.ExpectOne<2>("SomeText3");

    CHECK_THROW( mock->SimpleFunctionWithParams(2.0f, "Hllo 1", "SmeText1")
        , TNotEqualException );
}


//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpectAndReturn)
{
    TMockObject<IInterface> mock;

    mock.EveryCall(&IInterface::SimpleFunctionWithParamsAndReturn)
        .ExpectOne<0>(21.0f)
        .ExpectOne<1>("Hello World")
        .ExpectOne<2>("SomeText")
        .Return(2);

    CHECK_EQUAL(2, 
        mock->SimpleFunctionWithParamsAndReturn(21.0f, "Hello World", "SomeText") );

    CHECK_THROW(
        mock->SimpleFunctionWithParamsAndReturn(1.0f, "Heo World", "SoText")
        , TNotEqualException );
}

//------------------------------------------------------------------
TEST(MockObjectMethodDoFreeFunc)
{
    static std::string firstRecv;
    static std::string secondRecv;
    static std::string thirdRecv;

    struct Local
    {
        static std::string HandleDo(const std::string& crs,
            std::string& rs, std::string s)
        {
            firstRecv = crs;
            secondRecv = rs;
            thirdRecv = s;

            rs = "CHANGED";

            return "RESULT";
        }
    };

    TMockObject<IInterface> mock;

    mock.Call(&IInterface::ComplexFunction).Do(&Local::HandleDo);

    std::string second = "Second";

    std::string result = 
        mock->ComplexFunction("First", second, "Third");

    CHECK_EQUAL("RESULT", result.c_str());

    CHECK_EQUAL("First", firstRecv.c_str());
    CHECK_EQUAL("Second", secondRecv.c_str());		
    CHECK_EQUAL("Third",  thirdRecv.c_str());

    CHECK_EQUAL("CHANGED", second.c_str());	
}

// TODO:
//struct TTestDoor
//{
//    std::string operator() (const std::string& crs,
//        std::string& rs, std::string s)
//    {
//        firstRecv = crs;
//        secondRecv = rs;
//        thirdRecv = s;
//
//        rs = "CHANGED";
//
//        return "RESULT";
//    }
//
//    std::string firstRecv;
//    std::string secondRecv;
//    std::string thirdRecv;
//};
//
////------------------------------------------------------------------
//TEST(MockObjectMethodDoObject)
//{
//    TTestDoor rd;
//
//    TMockObject<IInterface> mock;
//
//    mock.Do(&IInterface::ComplexFunction)
//        .Do(rd);
//
//    std::string second = "Second";
//
//    std::string result = 
//        mock->ComplexFunction("First", second, "Third");
//
//    CHECK_EQUAL("RESULT", result.c_str());
//
//    CHECK_EQUAL("First", rd.firstRecv.c_str());
//    CHECK_EQUAL("Second", rd.secondRecv.c_str());		
//    CHECK_EQUAL("Third",  rd.thirdRecv.c_str());
//
//    CHECK_EQUAL("CHANGED", second.c_str());	
//}
//

//------------------------------------------------------------------
struct TestDoLocal
{
    std::string HandleDo(const std::string& crs,
        std::string& rs, std::string s)
    {
        firstRecv = crs;
        secondRecv = rs;
        thirdRecv = s;

        rs = "CHANGED";

        return "RESULT";
    }

    std::string firstRecv;
    std::string secondRecv;
    std::string thirdRecv;
};

//------------------------------------------------------------------
TEST(MockObjectMethodDoMethod)
{	
    TestDoLocal local;

    TMockObject<IInterface> mock;

    mock.Call(&IInterface::ComplexFunction)
        .Do(&local, &TestDoLocal::HandleDo);

    std::string second = "Second";

    std::string result = 
        mock->ComplexFunction("First", second, "Third");

    CHECK_EQUAL("RESULT", result.c_str());

    CHECK_EQUAL("First", local.firstRecv.c_str());
    CHECK_EQUAL("Second", local.secondRecv.c_str());		
    CHECK_EQUAL("Third",  local.thirdRecv.c_str());

    CHECK_EQUAL("CHANGED", second.c_str());	
}


//------------------------------------------------------------------
TEST(MockObjectMethodMixMethod)
{	
    TestDoLocal local;

    TMockObject<IInterface> mock;

    mock.Call(&IInterface::ComplexFunction)
        .Do(&local, &TestDoLocal::HandleDo)
        .Return("SECOND_RESULT")   
        .Throw(SimpleException(22));

    std::string second = "Second";

    std::string dump;

    CHECK_EQUAL("RESULT",  mock->ComplexFunction("First", second, "Third").c_str());
    CHECK_EQUAL("SECOND_RESULT", mock->ComplexFunction("First", dump, "Third").c_str());
    CHECK_THROW(mock->ComplexFunction("First", dump, "Third"), SimpleException);


    CHECK_EQUAL("First", local.firstRecv.c_str());
    CHECK_EQUAL("Second", local.secondRecv.c_str());		
    CHECK_EQUAL("Third",  local.thirdRecv.c_str());

    CHECK_EQUAL("CHANGED", second.c_str());	
}


//------------------------------------------------------------------
TEST(MockObjectMethodSet)
{	
    TestDoLocal local;

    TMockObject<IInterface> mock;

    mock.EveryCall(&IInterface::ComplexFunction)
        .Set<1>("CHANGED")
        .Return("");        

    std::string second = "Second";

    std::string result = 
        mock->ComplexFunction("First", second, "Third");

    CHECK_EQUAL("CHANGED", second.c_str());	
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleSetPolicy)
{
    TMockObject<IInterface> mock;

    mock.EveryCall(&IInterface::PolicyTestFunction)
        .Set<0>("First")
        .Set<1>(Policy::Pointer("Second"))
        .Set<2>(Policy::Array("Third", strlen("Third") + 1) );

    std::string first;
    std::string second;
    char buf[sizeof("Third")];

    mock->PolicyTestFunction(first, &second, buf) ;

    CHECK_EQUAL( "First", first.c_str() );
    CHECK_EQUAL( "Second", second.c_str()  );
    CHECK( memcmp("Third", buf, sizeof("Third") )== 0  );

}

//------------------------------------------------------------------
TEST(MockObjectMethodSetMultiple)
{	
    TMockObject<IInterface> mock;

    mock.Call(&IInterface::ComplexFunction)
        .Set<1>("C1").Set<1>("C2").Set<1>("C3");

    mock.EveryCall(&IInterface::ComplexFunction)
        .Return("");        

    std::string second = "Second";

    mock->ComplexFunction("First", second, "Third");
    CHECK_EQUAL("C1", second.c_str());	
    mock->ComplexFunction("First", second, "Third");
    CHECK_EQUAL("C2", second.c_str());	
    mock->ComplexFunction("First", second, "Third");
    CHECK_EQUAL("C3", second.c_str());	

}

//------------------------------------------------------------------
TEST(MockObjectMethodAddCallCount)
{		
    TMockObject<IInterface> mock;

    mock.EveryCall(&IInterface::ComplexFunction).Return(std::string());

    std::string second = "Second";

    mock->ComplexFunction("First", second, "Third");
    mock->ComplexFunction("First", second, "Third");
    mock->ComplexFunction("First", second, "Third");

    CHECK_EQUAL(3u, mock.Query(&IInterface::ComplexFunction).Count());
}

//------------------------------------------------------------------
TEST(MockObjectMethodReset)
{		
    TMockObject<IInterface> mock;

    mock.EveryCall(&IInterface::SimpleFunction);

    mock.Clear();

    CHECK_THROW(mock->SimpleFunction(), TNotImplementedException);
}

//------------------------------------------------------------------
TEST(MockObjectMethodDestructor)
{		
    TMockObject<IInterface> mock;

    mock.EveryCall(Destructor());

    delete ((IInterface*)mock);

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodVerifyCallCount)
{		
    TMockObject<IInterface, ManualVerify> mock;

    mock.EveryCall(&IInterface::SimpleFunction)
        .Count(3);

    mock->SimpleFunction();
    mock->SimpleFunction();
    mock->SimpleFunction();

    // It will not throw
    mock.Verify();

    mock->SimpleFunction();
    CHECK_THROW(mock.Verify(), TCallCountException);
}

//------------------------------------------------------------------
TEST(MockObjectMethodVerifyExpect)
{		
    TMockObject<IInterface, ManualVerify> mock;

    mock.Call(&IInterface::SimpleFunctionWithParams)
        .ExpectOne<0>(1.0f).ExpectOne<0>(2.0f).ExpectOne<0>(3.0f);
        
    mock.EveryCall(&IInterface::SimpleFunctionWithParams)        
        .ExpectOne<1>("")
        .ExpectOne<2>("");


    mock->SimpleFunctionWithParams(1.0f, "", "");
    mock->SimpleFunctionWithParams(2.0f, "", "");
    mock->SimpleFunctionWithParams(3.0f, "", "");

    // It will not throw
    mock.Verify();

    mock->SimpleFunctionWithParams(4.0f, "", "");
    CHECK_THROW(mock.Verify(), TCallCountException);
}

//------------------------------------------------------------------
TEST(MockObjectMethodReturnCallCountException)
{		
    TMockObject<IInterface, ManualVerify> mock;

    mock.Call(&IInterface::SimpleFunctionWithReturn)
        .Return(1)
        .Return(2)
        .Return(3);

    mock->SimpleFunctionWithReturn();
    mock->SimpleFunctionWithReturn();
    mock->SimpleFunctionWithReturn();

    CHECK_THROW(mock->SimpleFunctionWithReturn()
        , TCallCountException);
}

//------------------------------------------------------------------
TEST(MockObjectMethodReturnCallCountVerify)
{		
    TMockObject<IInterface, ManualVerify> mock;

    mock.Call(&IInterface::SimpleFunctionWithReturn)
        .Return(1)
        .Return(2)
        .Return(3);

    CHECK_THROW(mock.Verify(), TCallCountException);
}

//------------------------------------------------------------------
class ISimple1
{
public:
    virtual void SimpleFunction() = 0;
    virtual int SimpleFunctionWithReturn() = 0;
    virtual ~ISimple1();
};

//------------------------------------------------------------------
class ISimple2
{
public:
    virtual void SimpleFunction() = 0;
    virtual int SimpleFunctionWithReturn() = 0;
    virtual ~ISimple2();
};

//------------------------------------------------------------------
TEST(TwoInterfaces_noneImplemented)
{
    TMockObject<ISimple1> mock1;
    TMockObject<ISimple2> mock2;

    CHECK_THROW(((ISimple1*)mock1)->SimpleFunction(), TNotImplementedException);
    CHECK_THROW(((ISimple2*)mock2)->SimpleFunction(), TNotImplementedException);

    CHECK_THROW(((ISimple1*)mock1)->SimpleFunctionWithReturn(), TNotImplementedException);
    CHECK_THROW(((ISimple2*)mock2)->SimpleFunctionWithReturn(), TNotImplementedException);
}

//------------------------------------------------------------------
TEST(TwoInterfaces_oneSimpleImplemented)
{
    TMockObject<ISimple1> mock1;
    TMockObject<ISimple2> mock2;

    mock1.EveryCall(&ISimple1::SimpleFunction);

    ((ISimple1*)mock1)->SimpleFunction();
    CHECK_THROW(((ISimple2*)mock2)->SimpleFunction(), TNotImplementedException);

    CHECK_THROW(((ISimple1*)mock1)->SimpleFunctionWithReturn(), TNotImplementedException);
    CHECK_THROW(((ISimple2*)mock2)->SimpleFunctionWithReturn(), TNotImplementedException);
}

//------------------------------------------------------------------
TEST(TwoInterfaces_bothSimpleImplemented)
{
    TMockObject<ISimple1> mock1;
    TMockObject<ISimple2> mock2;

    mock1.EveryCall(&ISimple1::SimpleFunction);
    mock2.EveryCall(&ISimple2::SimpleFunction);

    ((ISimple1*)mock1)->SimpleFunction();
    ((ISimple2*)mock2)->SimpleFunction();

    CHECK_THROW(((ISimple1*)mock1)->SimpleFunctionWithReturn(), TNotImplementedException);
    CHECK_THROW(((ISimple2*)mock2)->SimpleFunctionWithReturn(), TNotImplementedException);
}

//------------------------------------------------------------------
TEST(TwoInterfaces_oneReturnImplemented)
{
    TMockObject<ISimple1> mock1;
    TMockObject<ISimple2> mock2;

    int expected1 = 1;
    mock1.EveryCall(&ISimple1::SimpleFunctionWithReturn)
        .Return(expected1);

    CHECK_THROW(((ISimple1*)mock1)->SimpleFunction(), TNotImplementedException);
    CHECK_THROW(((ISimple2*)mock2)->SimpleFunction(), TNotImplementedException);

    int got = ((ISimple1*)mock1)->SimpleFunctionWithReturn();
    CHECK_EQUAL(expected1, got);
    CHECK_THROW(((ISimple2*)mock2)->SimpleFunctionWithReturn(), TNotImplementedException);
}

//------------------------------------------------------------------
TEST(TwoInterfaces_bothReturnImplemented)
{
    TMockObject<ISimple1> mock1;
    TMockObject<ISimple2> mock2;

    int expected1 = 1;
    int expected2 = 2;

    mock1.EveryCall(&ISimple1::SimpleFunctionWithReturn)
        .Return(expected1);
    mock2.EveryCall(&ISimple2::SimpleFunctionWithReturn)
        .Return(expected2);

    CHECK_THROW(((ISimple1*)mock1)->SimpleFunction(), TNotImplementedException);
    CHECK_THROW(((ISimple2*)mock2)->SimpleFunction(), TNotImplementedException);

    int got;
    got = ((ISimple1*)mock1)->SimpleFunctionWithReturn();
    CHECK_EQUAL(expected1, got);
    got = ((ISimple2*)mock2)->SimpleFunctionWithReturn();
    CHECK_EQUAL(expected2, got);
}

struct IOverloads
{
    virtual ~IOverloads(){}
    virtual int SimpleFunction() = 0;
    virtual int SimpleFunction(int x) = 0;
};

TEST(Overloads1)
{
    // An example of an interface with method overrides
    

    // The method with no parameters
    typedef int (IOverloads::*PMF0)();
    PMF0 pmf0 = &IOverloads::SimpleFunction;

    // The method with a single parameter
    typedef int (IOverloads::*PMF1)(int);
    PMF1 pmf1 = &IOverloads::SimpleFunction;

    TMockObject<IOverloads> mock;

    mock.EveryCall(pmf0).Return(1);
    mock.EveryCall(pmf1).Return(2);

    IOverloads * pI = mock;

    CHECK_EQUAL(1, pI->SimpleFunction());
    CHECK_EQUAL(2, pI->SimpleFunction(3)); 
};


