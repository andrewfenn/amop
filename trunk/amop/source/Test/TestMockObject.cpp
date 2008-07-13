#include <UnitTest++.h>
#include "MockObject.h"
#include <string>

using namespace amop;

//------------------------------------------------------------------
class IInterface
{
public:
    virtual void SimpleFunction() = 0;
    virtual void SimpleFunctionWithParams(float, std::string, const std::string&) = 0;

    virtual int SimpleFunctionWithParamsAndReturn(float, std::string, const std::string&) = 0;

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
    MockObject<IInterface> mock;

    CHECK( (IInterface*)mock );	
}

//------------------------------------------------------------------
TEST(MockObjectNotImplementedThrowSimpleFunction)
{
    MockObject<IInterface> mock;

    CHECK((IInterface*)mock);

    CHECK_THROW(mock->SimpleFunction(), NotImplementedException);
    CHECK_THROW((*mock).SimpleFunction(), NotImplementedException);
}

//------------------------------------------------------------------
TEST(MockObjectNotImplementedThrowComplexFunction)
{
    MockObject<IInterface> mock;

    CHECK((IInterface*)mock);

    std::string crs, rs, s;

    CHECK_THROW(
        mock->ComplexFunction(crs, rs, s), 
        NotImplementedException);
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimple)
{
    MockObject<IInterface> mock;

    mock.everyCall(&IInterface::SimpleFunction);
    mock.everyCall(&IInterface::SimpleFunctionWithAlotParams);

    mock->SimpleFunction();
    mock->SimpleFunctionWithAlotParams(0, 0, 0, 0, std::string());

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodComplex)
{
    MockObject<IInterface> mock;

    mock.everyCall(&IInterface::ComplexFunction)
        .returning("Test Result");
    std::string crs, rs, s;

    std::string result = 
        mock->ComplexFunction(crs, rs, s);

    CHECK_EQUAL("Test Result", result.c_str());
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleWithReturn)
{
    MockObject<IInterface> mock;

    mock.everyCall(&IInterface::SimpleFunctionWithReturn)
        .returning(22);
    mock.everyCall(&IInterface::SimpleFunctionWithRefReturn)
        .returning(1262);

    CHECK_EQUAL(22, mock->SimpleFunctionWithReturn());
    CHECK_EQUAL(1262, mock->SimpleFunctionWithRefReturn());
}

//------------------------------------------------------------------
TEST(MockObjectMethodConstSimple)
{
    MockObject<IInterface> mock;

    mock.everyCall(&IInterface::SimpleConstFunction);

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
    MockObject<IInterface> mock;

    mock.everyCall(&IInterface::SimpleFunctionWithReturn)
      .throwing(std::exception());

    CHECK_THROW(mock->SimpleFunctionWithReturn(), std::exception );
    
    mock.verify();
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleWithThrowNoReturn)
{
    MockObject<IInterface> mock;

    mock.everyCall(&IInterface::SimpleFunction)
      .throwing(std::exception());

    CHECK_THROW(mock->SimpleFunction(), std::exception );
    
    mock.verify();
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleWithInheritedThrow)
{
    MockObject<IInterface> mock;

    mock.everyCall(&IInterface::SimpleFunctionWithReturn)
      .throwing(SimpleException(22));

    CHECK_THROW(mock->SimpleFunctionWithReturn(), std::exception);
    
    mock.verify();
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleWithManyThrow)
{
    MockObject<IInterface> mock;

    mock.call(&IInterface::SimpleFunctionWithReturn)
      .throwing(SimpleException(22));
    mock.call(&IInterface::SimpleFunctionWithReturn)
      .returning(42);
    mock.call(&IInterface::SimpleFunctionWithReturn)
      .throwing(SimpleException(142));
    
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
    
    mock.verify();
}


//------------------------------------------------------------------
TEST(MockObjectMethodConstComplex)
{
    MockObject<IInterface> mock;

    mock.everyCall(&IInterface::ComplexConstFunction)
        .returning("Test Result");
    std::string crs, rs, s;

    std::string result = 
        mock->ComplexConstFunction(crs, rs, s);

    CHECK_EQUAL("Test Result", result.c_str());
}


//------------------------------------------------------------------
TEST(MockObjectMethodMultiWithReturn)
{
    MockObject<IInterface> mock;

    mock.call(&IInterface::SimpleFunctionWithReturn)
        .returning(22).returning(11);

    CHECK_EQUAL(22, mock->SimpleFunctionWithReturn());
    CHECK_EQUAL(11, mock->SimpleFunctionWithReturn());
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpect)
{
    MockObject<IInterface> mock;

    mock.everyCall(&IInterface::SimpleFunctionWithParams)
        .expectOne<0>(21.0f)
        .expectOne<1>("Hello World")
        .expectOne<2>("SomeText");

    mock->SimpleFunctionWithParams(21.0f, "Hello World", "SomeText");

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpectAll)
{
    MockObject<IInterface> mock;

    mock.everyCall(&IInterface::SimpleFunctionWithParams)
        .expect(21.0f, "Hello World", "SomeText");

    mock->SimpleFunctionWithParams(21.0f, "Hello World", "SomeText");

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpectAllIgnore)
{
    MockObject<IInterface> mock;

    mock.everyCall(&IInterface::SimpleFunctionWithParams)
        .expect(21.0f, Ignore, "SomeText");

    mock->SimpleFunctionWithParams(21.0f, "Hello World", "SomeText");

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpectPolicy)
{
    MockObject<IInterface> mock;

    mock.everyCall(&IInterface::PolicyTestFunction)
        .expectOne<0>("First")
        .expectOne<1>(policy::pointer("Second"))
        .expectOne<2>(policy::array("Third", strlen("Third") + 1) );

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
    MockObject<IInterface> mock;

    mock.everyCall(&IInterface::PolicyTestFunction)
        .expect("First", policy::pointer("Second"), policy::array("Third", strlen("Third") + 1) );

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
    MockObject<IInterface> mock;

    mock.everyCall(&IInterface::SimpleFunctionWithParams)
        .expectOne<0>(21.0f)
        .expectOne<1>("Hello World")
        .expectOne<2>("SomeText");

    CHECK_THROW( mock->SimpleFunctionWithParams(2.0f, "Hllo World", "SomeTet")
        , NotEqualException );
}


//------------------------------------------------------------------
TEST(MockObjectMethodMultipleExpectOne)
{
    MockObject<IInterface> mock;

    mock.call(&IInterface::SimpleFunctionWithParams)
        .expectOne<0>(1.0f).expectOne<1>("Hello 1").expectOne<2>("SomeText1") 
        .expectOne<0>(2.0f).expectOne<1>("Hello 2").expectOne<2>("SomeText2")	
        .expectOne<0>(3.0f).expectOne<1>("Hello 3").expectOne<2>("SomeText3");

    mock->SimpleFunctionWithParams(1.0f, "Hello 1", "SomeText1");
    mock->SimpleFunctionWithParams(2.0f, "Hello 2", "SomeText2");
    mock->SimpleFunctionWithParams(3.0f, "Hello 3", "SomeText3");

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodMultipleExpect)
{
    MockObject<IInterface> mock;

    mock.call(&IInterface::SimpleFunctionWithParams)
        .expect(1.0f, "Hello 1", "SomeText1")
        .expect(2.0f, "Hello 2", "SomeText2")	
        .expect(3.0f, "Hello 3", "SomeText3");

    mock->SimpleFunctionWithParams(1.0f, "Hello 1", "SomeText1");
    mock->SimpleFunctionWithParams(2.0f, "Hello 2", "SomeText2");
    mock->SimpleFunctionWithParams(3.0f, "Hello 3", "SomeText3");

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodMultipleExpectgnoreMultiple)
{
    MockObject<IInterface> mock;

    mock.call(&IInterface::SimpleFunctionWithParams)
        .expect(Ignore, Ignore, "SomeText")
        .expect(Ignore, "Hello World", Ignore)
        .expect(3.0f,  Ignore, Ignore);

    mock->SimpleFunctionWithParams(1.0f, "asfdlkjsf",  "SomeText");
    mock->SimpleFunctionWithParams(2.0f, "Hello World", "fsda");
    mock->SimpleFunctionWithParams(3.0f, "asdfjdskjfl", "fdsafjl");

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}



//------------------------------------------------------------------
TEST(MockObjectMethodMultipleExpectThrow)
{
    MockObject<IInterface, ManualVerify> mock;

    mock.call(&IInterface::SimpleFunctionWithParams)
        .expectOne<0>(1.0f)		.expectOne<0>(2.0f)			.expectOne<0>(3.0f)
        .expectOne<1>("Hello 1")	.expectOne<1>("Hello 2")		.expectOne<1>("Hello 3")
        .expectOne<2>("SomeText1").expectOne<2>("SomeText2")	.expectOne<2>("SomeText3");

    CHECK_THROW( mock->SimpleFunctionWithParams(2.0f, "Hllo 1", "SmeText1")
        , NotEqualException );
}


//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpectAndReturn)
{
    MockObject<IInterface> mock;

    mock.everyCall(&IInterface::SimpleFunctionWithParamsAndReturn)
        .expectOne<0>(21.0f)
        .expectOne<1>("Hello World")
        .expectOne<2>("SomeText")
        .returning(2);

    CHECK_EQUAL(2, 
        mock->SimpleFunctionWithParamsAndReturn(21.0f, "Hello World", "SomeText") );

    CHECK_THROW(
        mock->SimpleFunctionWithParamsAndReturn(1.0f, "Heo World", "SoText")
        , NotEqualException );
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

    MockObject<IInterface> mock;

    mock.call(&IInterface::ComplexFunction).doing(&Local::HandleDo);

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
//    MockObject<IInterface> mock;
//
//    mock.doing(&IInterface::ComplexFunction)
//        .doing(rd);
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

    MockObject<IInterface> mock;

    mock.call(&IInterface::ComplexFunction)
        .doing(&local, &TestDoLocal::HandleDo);

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

    MockObject<IInterface> mock;

    mock.call(&IInterface::ComplexFunction)
        .doing(&local, &TestDoLocal::HandleDo)
        .returning("SECOND_RESULT")   
        .throwing(SimpleException(22));

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

    MockObject<IInterface> mock;

    mock.everyCall(&IInterface::ComplexFunction)
        .set<1>("CHANGED")
        .returning("");        

    std::string second = "Second";

    std::string result = 
        mock->ComplexFunction("First", second, "Third");

    CHECK_EQUAL("CHANGED", second.c_str());	
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleSetPolicy)
{
    MockObject<IInterface> mock;

    mock.everyCall(&IInterface::PolicyTestFunction)
        .set<0>("First")
        .set<1>(policy::pointer("Second"))
        .set<2>(policy::array("Third", strlen("Third") + 1) );

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
    MockObject<IInterface> mock;

    mock.call(&IInterface::ComplexFunction)
        .set<1>("C1").set<1>("C2").set<1>("C3");

    mock.everyCall(&IInterface::ComplexFunction)
        .returning("");        

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
    MockObject<IInterface> mock;

    mock.everyCall(&IInterface::ComplexFunction).returning(std::string());

    std::string second = "Second";

    mock->ComplexFunction("First", second, "Third");
    mock->ComplexFunction("First", second, "Third");
    mock->ComplexFunction("First", second, "Third");

    CHECK_EQUAL(3u, mock.query(&IInterface::ComplexFunction).count());
}

//------------------------------------------------------------------
TEST(MockObjectMethodReset)
{		
    MockObject<IInterface> mock;

    mock.everyCall(&IInterface::SimpleFunction);

    mock.clear();

    CHECK_THROW(mock->SimpleFunction(), NotImplementedException);
}

//------------------------------------------------------------------
TEST(MockObjectMethodDestructor)
{		
    MockObject<IInterface> mock;

    mock.everyCall(Destructor());

    delete ((IInterface*)mock);

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodVerifyCallCount)
{		
    MockObject<IInterface, ManualVerify> mock;

    mock.everyCall(&IInterface::SimpleFunction)
        .count(3);

    mock->SimpleFunction();
    mock->SimpleFunction();
    mock->SimpleFunction();

    // It will not throw
    mock.verify();

    mock->SimpleFunction();
    CHECK_THROW(mock.verify(), CallCountException);
}

//------------------------------------------------------------------
TEST(MockObjectMethodVerifyExpect)
{		
    MockObject<IInterface, ManualVerify> mock;

    mock.call(&IInterface::SimpleFunctionWithParams)
        .expectOne<0>(1.0f).expectOne<0>(2.0f).expectOne<0>(3.0f);
        
    mock.everyCall(&IInterface::SimpleFunctionWithParams)        
        .expectOne<1>("")
        .expectOne<2>("");


    mock->SimpleFunctionWithParams(1.0f, "", "");
    mock->SimpleFunctionWithParams(2.0f, "", "");
    mock->SimpleFunctionWithParams(3.0f, "", "");

    // It will not throw
    mock.verify();

    mock->SimpleFunctionWithParams(4.0f, "", "");
    CHECK_THROW(mock.verify(), CallCountException);
}

//------------------------------------------------------------------
TEST(MockObjectMethodReturnCallCountException)
{		
    MockObject<IInterface, ManualVerify> mock;

    mock.call(&IInterface::SimpleFunctionWithReturn)
        .returning(1)
        .returning(2)
        .returning(3);

    mock->SimpleFunctionWithReturn();
    mock->SimpleFunctionWithReturn();
    mock->SimpleFunctionWithReturn();

    CHECK_THROW(mock->SimpleFunctionWithReturn()
        , CallCountException);
}

//------------------------------------------------------------------
TEST(MockObjectMethodReturnCallCountVerify)
{		
    MockObject<IInterface, ManualVerify> mock;

    mock.call(&IInterface::SimpleFunctionWithReturn)
        .returning(1)
        .returning(2)
        .returning(3);

    CHECK_THROW(mock.verify(), CallCountException);
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
    MockObject<ISimple1> mock1;
    MockObject<ISimple2> mock2;

    CHECK_THROW(((ISimple1*)mock1)->SimpleFunction(), NotImplementedException);
    CHECK_THROW(((ISimple2*)mock2)->SimpleFunction(), NotImplementedException);

    CHECK_THROW(((ISimple1*)mock1)->SimpleFunctionWithReturn(), NotImplementedException);
    CHECK_THROW(((ISimple2*)mock2)->SimpleFunctionWithReturn(), NotImplementedException);
}

//------------------------------------------------------------------
TEST(TwoInterfaces_oneSimpleImplemented)
{
    MockObject<ISimple1> mock1;
    MockObject<ISimple2> mock2;

    mock1.everyCall(&ISimple1::SimpleFunction);

    ((ISimple1*)mock1)->SimpleFunction();
    CHECK_THROW(((ISimple2*)mock2)->SimpleFunction(), NotImplementedException);

    CHECK_THROW(((ISimple1*)mock1)->SimpleFunctionWithReturn(), NotImplementedException);
    CHECK_THROW(((ISimple2*)mock2)->SimpleFunctionWithReturn(), NotImplementedException);
}

//------------------------------------------------------------------
TEST(TwoInterfaces_bothSimpleImplemented)
{
    MockObject<ISimple1> mock1;
    MockObject<ISimple2> mock2;

    mock1.everyCall(&ISimple1::SimpleFunction);
    mock2.everyCall(&ISimple2::SimpleFunction);

    ((ISimple1*)mock1)->SimpleFunction();
    ((ISimple2*)mock2)->SimpleFunction();

    CHECK_THROW(((ISimple1*)mock1)->SimpleFunctionWithReturn(), NotImplementedException);
    CHECK_THROW(((ISimple2*)mock2)->SimpleFunctionWithReturn(), NotImplementedException);
}

//------------------------------------------------------------------
TEST(TwoInterfaces_oneReturnImplemented)
{
    MockObject<ISimple1> mock1;
    MockObject<ISimple2> mock2;

    int expected1 = 1;
    mock1.everyCall(&ISimple1::SimpleFunctionWithReturn)
        .returning(expected1);

    CHECK_THROW(((ISimple1*)mock1)->SimpleFunction(), NotImplementedException);
    CHECK_THROW(((ISimple2*)mock2)->SimpleFunction(), NotImplementedException);

    int got = ((ISimple1*)mock1)->SimpleFunctionWithReturn();
    CHECK_EQUAL(expected1, got);
    CHECK_THROW(((ISimple2*)mock2)->SimpleFunctionWithReturn(), NotImplementedException);
}

//------------------------------------------------------------------
TEST(TwoInterfaces_bothReturnImplemented)
{
    MockObject<ISimple1> mock1;
    MockObject<ISimple2> mock2;

    int expected1 = 1;
    int expected2 = 2;

    mock1.everyCall(&ISimple1::SimpleFunctionWithReturn)
        .returning(expected1);
    mock2.everyCall(&ISimple2::SimpleFunctionWithReturn)
        .returning(expected2);

    CHECK_THROW(((ISimple1*)mock1)->SimpleFunction(), NotImplementedException);
    CHECK_THROW(((ISimple2*)mock2)->SimpleFunction(), NotImplementedException);

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

    MockObject<IOverloads> mock;

    mock.everyCall(pmf0).returning(1);
    mock.everyCall(pmf1).returning(2);

    IOverloads * pI = mock;

    CHECK_EQUAL(1, pI->SimpleFunction());
    CHECK_EQUAL(2, pI->SimpleFunction(3)); 
};


