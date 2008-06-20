#include <UnitTest++.h>
#include "../MockObject.h"
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

    CHECK_THROW(((IInterface*)mock)->SimpleFunction(), TNotImplementedException);
}

//------------------------------------------------------------------
TEST(MockObjectNotImplementedThrowComplexFunction)
{
    TMockObject<IInterface> mock;

    CHECK((IInterface*)mock);

    std::string crs, rs, s;

    CHECK_THROW(
        ((IInterface*)mock)->ComplexFunction(crs, rs, s), 
        TNotImplementedException);
}

//mockSubscriber.method("receive").expects(once()).with( eq(message) );
//------------------------------------------------------------------
TEST(MockObjectMethodSimple)
{
    TMockObject<IInterface> mock;

    mock.All(&IInterface::SimpleFunction);
    mock.All(&IInterface::SimpleFunctionWithAlotParams);

    ((IInterface*)mock)->SimpleFunction();
    ((IInterface*)mock)->SimpleFunctionWithAlotParams(0, 0, 0, 0, std::string());

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodComplex)
{
    TMockObject<IInterface> mock;

    mock.All(&IInterface::ComplexFunction)
        .Will("Test Result");
    std::string crs, rs, s;

    std::string result = 
        ((IInterface*)mock)->ComplexFunction(crs, rs, s);

    CHECK_EQUAL("Test Result", result.c_str());
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleWithReturn)
{
    TMockObject<IInterface> mock;

    mock.All(&IInterface::SimpleFunctionWithReturn)
        .Will(22);
    mock.All(&IInterface::SimpleFunctionWithRefReturn)
        .Will(1262);

    CHECK_EQUAL(22, ((IInterface*)mock)->SimpleFunctionWithReturn());
    CHECK_EQUAL(1262, ((IInterface*)mock)->SimpleFunctionWithRefReturn());
}

//------------------------------------------------------------------
TEST(MockObjectMethodConstSimple)
{
    TMockObject<IInterface> mock;

    mock.All(&IInterface::SimpleConstFunction);

    ((IInterface*)mock)->SimpleConstFunction();	

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

void ThisThrows(){
  throw SimpleException(42);
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleWithThrow)
{
    TMockObject<IInterface> mock;

    mock.All(&IInterface::SimpleFunctionWithReturn)
      .Throw(std::exception());

    CHECK_THROW(((IInterface*)mock)->SimpleFunctionWithReturn(), std::exception );
    
    mock.Verify();
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleWithInheritedThrow)
{
    TMockObject<IInterface> mock;

    mock.All(&IInterface::SimpleFunctionWithReturn)
      .Throw(SimpleException(22));

    CHECK_THROW(((IInterface*)mock)->SimpleFunctionWithReturn(), std::exception);
    
    mock.Verify();
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleWithManyThrow)
{
    TMockObject<IInterface> mock;

    mock.Single(&IInterface::SimpleFunctionWithReturn)
      .Throws(SimpleException(22));
    mock.Single(&IInterface::SimpleFunctionWithReturn)
      .Wills(42);
    mock.Single(&IInterface::SimpleFunctionWithReturn)
      .Throws(SimpleException(142));
    
    try{
        ((IInterface*)mock)->SimpleFunctionWithReturn();
        CHECK(false);
    }catch(SimpleException & se){
        CHECK_EQUAL(22, se.value);
    }
    CHECK_EQUAL(42, ((IInterface*)mock)->SimpleFunctionWithReturn());
    try{
        ((IInterface*)mock)->SimpleFunctionWithReturn();
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

    mock.All(&IInterface::ComplexConstFunction)
        .Will("Test Result");
    std::string crs, rs, s;

    std::string result = 
        ((IInterface*)mock)->ComplexConstFunction(crs, rs, s);

    CHECK_EQUAL("Test Result", result.c_str());
}


//------------------------------------------------------------------
TEST(MockObjectMethodMultiWithReturn)
{
    TMockObject<IInterface> mock;

    mock.Single(&IInterface::SimpleFunctionWithReturn)
        .Wills(22).Wills(11);

    CHECK_EQUAL(22, ((IInterface*)mock)->SimpleFunctionWithReturn());
    CHECK_EQUAL(11, ((IInterface*)mock)->SimpleFunctionWithReturn());
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpect)
{
    TMockObject<IInterface> mock;

    mock.All(&IInterface::SimpleFunctionWithParams)
        .Expect<0>(21.0f)
        .Expect<1>("Hello World")
        .Expect<2>("SomeText");

    ((IInterface*)mock)->SimpleFunctionWithParams(21.0f, "Hello World", "SomeText");

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpectPolicy)
{
    TMockObject<IInterface> mock;

    mock.All(&IInterface::PolicyTestFunction)
        .Expect<0>("First")
        .Expect<1>(Policy::Pointer("Second"))
        .Expect<2>(Policy::Array("Third", strlen("Third") + 1) );

    std::string first = "First";
    std::string second = "Second";
    char buf[] = "Third";
    
    ((IInterface*)mock)->PolicyTestFunction(first, &second, buf) ;

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpectThrow)
{
    TMockObject<IInterface> mock;

    mock.All(&IInterface::SimpleFunctionWithParams)
        .Expect<0>(21.0f)
        .Expect<1>("Hello World")
        .Expect<2>("SomeText");

    CHECK_THROW( ((IInterface*)mock)->SimpleFunctionWithParams(2.0f, "Hllo World", "SomeTet")
        , TNotEqualException );
}


//------------------------------------------------------------------
TEST(MockObjectMethodMultipleExpect)
{
    TMockObject<IInterface> mock;

    mock.Single(&IInterface::SimpleFunctionWithParams)
        .Expects<0>(1.0f)		.Expects<0>(2.0f)			.Expects<0>(3.0f)
        .Expects<1>("Hello 1")	.Expects<1>("Hello 2")		.Expects<1>("Hello 3")
        .Expects<2>("SomeText1").Expects<2>("SomeText2")	.Expects<2>("SomeText3");

    ((IInterface*)mock)->SimpleFunctionWithParams(1.0f, "Hello 1", "SomeText1");
    ((IInterface*)mock)->SimpleFunctionWithParams(2.0f, "Hello 2", "SomeText2");
    ((IInterface*)mock)->SimpleFunctionWithParams(3.0f, "Hello 3", "SomeText3");\

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodMultipleExpectThrow)
{
    TMockObject<IInterface> mock;

    mock.Single(&IInterface::SimpleFunctionWithParams)
        .Expects<0>(1.0f)		.Expects<0>(2.0f)			.Expects<0>(3.0f)
        .Expects<1>("Hello 1")	.Expects<1>("Hello 2")		.Expects<1>("Hello 3")
        .Expects<2>("SomeText1").Expects<2>("SomeText2")	.Expects<2>("SomeText3");

    CHECK_THROW( ((IInterface*)mock)->SimpleFunctionWithParams(2.0f, "Hllo 1", "SmeText1")
        , TNotEqualException );
}


//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpectAndReturn)
{
    TMockObject<IInterface> mock;

    mock.All(&IInterface::SimpleFunctionWithParamsAndReturn)
        .Expect<0>(21.0f)
        .Expect<1>("Hello World")
        .Expect<2>("SomeText")
        .Will(2);

    CHECK_EQUAL(2, 
        ((IInterface*)mock)->SimpleFunctionWithParamsAndReturn(21.0f, "Hello World", "SomeText") );

    CHECK_THROW(
        ((IInterface*)mock)->SimpleFunctionWithParamsAndReturn(1.0f, "Heo World", "SoText")
        , TNotEqualException );
}

//------------------------------------------------------------------
TEST(MockObjectMethodRedirectFreeFunc)
{
    static std::string firstRecv;
    static std::string secondRecv;
    static std::string thirdRecv;

    struct Local
    {
        static std::string HandleRedirect(const std::string& crs,
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

    mock.Redirect(&IInterface::ComplexFunction)
        .Do(&Local::HandleRedirect);

    std::string second = "Second";

    std::string result = 
        ((IInterface*)mock)->ComplexFunction("First", second, "Third");

    CHECK_EQUAL("RESULT", result.c_str());

    CHECK_EQUAL("First", firstRecv.c_str());
    CHECK_EQUAL("Second", secondRecv.c_str());		
    CHECK_EQUAL("Third",  thirdRecv.c_str());

    CHECK_EQUAL("CHANGED", second.c_str());	
}

//------------------------------------------------------------------
struct TestRedirectLocal
{
    std::string HandleRedirect(const std::string& crs,
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
TEST(MockObjectMethodRedirectMethod)
{	

    TestRedirectLocal local;

    TMockObject<IInterface> mock;

    mock.Redirect(&IInterface::ComplexFunction)
        .Do(&local, &TestRedirectLocal::HandleRedirect);

    std::string second = "Second";

    std::string result = 
        ((IInterface*)mock)->ComplexFunction("First", second, "Third");

    CHECK_EQUAL("RESULT", result.c_str());

    CHECK_EQUAL("First", local.firstRecv.c_str());
    CHECK_EQUAL("Second", local.secondRecv.c_str());		
    CHECK_EQUAL("Third",  local.thirdRecv.c_str());

    CHECK_EQUAL("CHANGED", second.c_str());	
}

//------------------------------------------------------------------
TEST(MockObjectMethodSet)
{	
    TestRedirectLocal local;

    TMockObject<IInterface> mock;

    mock.All(&IInterface::ComplexFunction)
        .Set<1>("CHANGED")
        .Will("");        

    std::string second = "Second";

    std::string result = 
        ((IInterface*)mock)->ComplexFunction("First", second, "Third");

    CHECK_EQUAL("CHANGED", second.c_str());	
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleSetPolicy)
{
    TMockObject<IInterface> mock;

    mock.All(&IInterface::PolicyTestFunction)
        .Set<0>("First")
        .Set<1>(Policy::Pointer("Second"))
        .Set<2>(Policy::Array("Third", strlen("Third") + 1) );

    std::string first;
    std::string second;
    char buf[sizeof("Third")];

    ((IInterface*)mock)->PolicyTestFunction(first, &second, buf) ;

    CHECK_EQUAL( "First", first.c_str() );
    CHECK_EQUAL( "Second", second.c_str()  );
    CHECK( memcmp("Third", buf, sizeof("Third") )== 0  );

}

//------------------------------------------------------------------
TEST(MockObjectMethodSetMultiple)
{	
    TestRedirectLocal local;

    TMockObject<IInterface> mock;

    mock.Single(&IInterface::ComplexFunction)
        .Sets<1>("C1").Sets<1>("C2").Sets<1>("C3");

    mock.All(&IInterface::ComplexFunction)
        .Will("");        

    std::string second = "Second";

    ((IInterface*)mock)->ComplexFunction("First", second, "Third");
    CHECK_EQUAL("C1", second.c_str());	
    ((IInterface*)mock)->ComplexFunction("First", second, "Third");
    CHECK_EQUAL("C2", second.c_str());	
    ((IInterface*)mock)->ComplexFunction("First", second, "Third");
    CHECK_EQUAL("C3", second.c_str());	

}

//------------------------------------------------------------------
TEST(MockObjectMethodAddCallCount)
{		
    TMockObject<IInterface> mock;

    mock.All(&IInterface::ComplexFunction).Will(std::string());

    std::string second = "Second";

    ((IInterface*)mock)->ComplexFunction("First", second, "Third");
    ((IInterface*)mock)->ComplexFunction("First", second, "Third");
    ((IInterface*)mock)->ComplexFunction("First", second, "Third");

    CHECK_EQUAL(3u, mock.Query(&IInterface::ComplexFunction).Count());
}

//------------------------------------------------------------------
TEST(MockObjectMethodReset)
{		
    TMockObject<IInterface> mock;

    mock.All(&IInterface::SimpleFunction);

    mock.Clear();

    CHECK_THROW(((IInterface*)mock)->SimpleFunction(), TNotImplementedException);
}

//------------------------------------------------------------------
TEST(MockObjectMethodDestructor)
{		
    TMockObject<IInterface> mock;

    mock.All(Destructor());

    delete ((IInterface*)mock);

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodVerifyCallCount)
{		
    TMockObject<IInterface> mock;

    mock.All(&IInterface::SimpleFunction)
        .Count(3);

    ((IInterface*)mock)->SimpleFunction();
    ((IInterface*)mock)->SimpleFunction();
    ((IInterface*)mock)->SimpleFunction();

    // It will not throw
    mock.Verify();

    ((IInterface*)mock)->SimpleFunction();
    CHECK_THROW(mock.Verify(), TCallCountException);
}

//------------------------------------------------------------------
TEST(MockObjectMethodVerifyExpects)
{		
    TMockObject<IInterface> mock;

    mock.Single(&IInterface::SimpleFunctionWithParams)
        .Expects<0>(1.0f).Expects<0>(2.0f).Expects<0>(3.0f);
        
    mock.All(&IInterface::SimpleFunctionWithParams)        
        .Expect<1>("")
        .Expect<2>("");


    ((IInterface*)mock)->SimpleFunctionWithParams(1.0f, "", "");
    ((IInterface*)mock)->SimpleFunctionWithParams(2.0f, "", "");
    ((IInterface*)mock)->SimpleFunctionWithParams(3.0f, "", "");

    // It will not throw
    mock.Verify();

    ((IInterface*)mock)->SimpleFunctionWithParams(4.0f, "", "");
    CHECK_THROW(mock.Verify(), TCallCountException);
}

//------------------------------------------------------------------
TEST(MockObjectMethodWillsCallCountException)
{		
    TMockObject<IInterface> mock;

    mock.Single(&IInterface::SimpleFunctionWithReturn)
        .Wills(1)
        .Wills(2)
        .Wills(3);

    ((IInterface*)mock)->SimpleFunctionWithReturn();
    ((IInterface*)mock)->SimpleFunctionWithReturn();
    ((IInterface*)mock)->SimpleFunctionWithReturn();

    CHECK_THROW(((IInterface*)mock)->SimpleFunctionWithReturn()
        , TCallCountException);
}

//------------------------------------------------------------------
TEST(MockObjectMethodWillsCallCountVerify)
{		
    TMockObject<IInterface> mock;

    mock.Single(&IInterface::SimpleFunctionWithReturn)
        .Wills(1)
        .Wills(2)
        .Wills(3);

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

    mock1.All(&ISimple1::SimpleFunction);

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

    mock1.All(&ISimple1::SimpleFunction);
    mock2.All(&ISimple2::SimpleFunction);

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
    mock1.All(&ISimple1::SimpleFunctionWithReturn)
        .Will(expected1);

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

    mock1.All(&ISimple1::SimpleFunctionWithReturn)
        .Will(expected1);
    mock2.All(&ISimple2::SimpleFunctionWithReturn)
        .Will(expected2);

    CHECK_THROW(((ISimple1*)mock1)->SimpleFunction(), TNotImplementedException);
    CHECK_THROW(((ISimple2*)mock2)->SimpleFunction(), TNotImplementedException);

    int got;
    got = ((ISimple1*)mock1)->SimpleFunctionWithReturn();
    CHECK_EQUAL(expected1, got);
    got = ((ISimple2*)mock2)->SimpleFunctionWithReturn();
    CHECK_EQUAL(expected2, got);
}


