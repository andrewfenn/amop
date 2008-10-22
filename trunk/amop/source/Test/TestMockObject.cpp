#include <UnitTest++.h>
#include "MockObject.h"
#include <string>

using namespace amop;

//------------------------------------------------------------------
class Interface
{
public:
    virtual void __cdecl simpleFunction() = 0;
    virtual void simpleFunctionWithParams(float, std::string, const std::string&) = 0;

    virtual int simpleFunctionWithParamsAndReturn(float, std::string, const std::string&) = 0;

    virtual void simpleFunctionWithAlotParams(float, int, float, int, std::string) = 0;

    virtual int simpleFunctionWithReturn() = 0;
    virtual int& simpleFunctionWithRefReturn() = 0;

    virtual void simpleConstFunction() const = 0;

    virtual std::string complexFunction(const std::string& crs,
        std::string& rs, std::string s) = 0;

    virtual std::string ComplexConstFunction(const std::string& crs,
        std::string& rs, std::string s) const = 0;

    virtual void PolicyTestFunction(
        std::string& str
        , std::string* pointedStr
        , char* const buffer
        );


    virtual ~Interface(){}
};

//------------------------------------------------------------------
TEST(CreateMockObject)
{
    MockObject<Interface> mock;

    CHECK( (Interface*)mock );	
}

//------------------------------------------------------------------
TEST(MockObjectNotImplementedThrowSimpleFunction)
{
    MockObject<Interface> mock;

    CHECK((Interface*)mock);

    CHECK_THROW(mock->simpleFunction(), NotImplementedException);
    CHECK_THROW((*mock).simpleFunction(), NotImplementedException);
}

//------------------------------------------------------------------
TEST(MockObjectNotImplementedThrowComplexFunction)
{
    MockObject<Interface> mock;

    CHECK((Interface*)mock);

    std::string crs, rs, s;

    CHECK_THROW(
        mock->complexFunction(crs, rs, s), 
        NotImplementedException);
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimple)
{
    MockObject<Interface> mock;

    mock.everyCall(&Interface::simpleFunction);
    mock.everyCall(&Interface::simpleFunctionWithAlotParams);

    mock->simpleFunction();
    mock->simpleFunctionWithAlotParams(0, 0, 0, 0, std::string());

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodComplex)
{
    MockObject<Interface> mock;

    mock.everyCall(&Interface::complexFunction)
        .returning("Test Result");
    std::string crs, rs, s;

    std::string result = 
        mock->complexFunction(crs, rs, s);

    CHECK_EQUAL("Test Result", result.c_str());
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleWithReturn)
{
    MockObject<Interface> mock;

    mock.everyCall(&Interface::simpleFunctionWithReturn)
        .returning(22);
    mock.everyCall(&Interface::simpleFunctionWithRefReturn)
        .returning(1262);

    CHECK_EQUAL(22, mock->simpleFunctionWithReturn());
    CHECK_EQUAL(1262, mock->simpleFunctionWithRefReturn());
}

//------------------------------------------------------------------
TEST(MockObjectMethodConstSimple)
{
    MockObject<Interface> mock;

    mock.everyCall(&Interface::simpleConstFunction);

    mock->simpleConstFunction();	

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
    MockObject<Interface> mock;

    mock.everyCall(&Interface::simpleFunctionWithReturn)
      .throwing(std::exception());

    CHECK_THROW(mock->simpleFunctionWithReturn(), std::exception );
    
    mock.verify();
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleWithThrowNoReturn)
{
    MockObject<Interface> mock;

    mock.everyCall(&Interface::simpleFunction)
      .throwing(std::exception());

    CHECK_THROW(mock->simpleFunction(), std::exception );
    
    mock.verify();
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleWithInheritedThrow)
{
    MockObject<Interface> mock;

    mock.everyCall(&Interface::simpleFunctionWithReturn)
      .throwing(SimpleException(22));

    CHECK_THROW(mock->simpleFunctionWithReturn(), std::exception);
    
    mock.verify();
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleWithManyThrow)
{
    MockObject<Interface> mock;

    mock.call(&Interface::simpleFunctionWithReturn)
      .throwing(SimpleException(22));
    mock.call(&Interface::simpleFunctionWithReturn)
      .returning(42);
    mock.call(&Interface::simpleFunctionWithReturn)
      .throwing(SimpleException(142));
    
    try{
        mock->simpleFunctionWithReturn();
        CHECK(false);
    }catch(SimpleException & se){
        CHECK_EQUAL(22, se.value);
    }
    CHECK_EQUAL(42, mock->simpleFunctionWithReturn());
    try{
        mock->simpleFunctionWithReturn();
        CHECK(false);
    }catch(SimpleException & se){
        CHECK_EQUAL(142, se.value);
    }
    
    mock.verify();
}


//------------------------------------------------------------------
TEST(MockObjectMethodConstComplex)
{
    MockObject<Interface> mock;

    mock.everyCall(&Interface::ComplexConstFunction)
        .returning("Test Result");
    std::string crs, rs, s;

    std::string result = 
        mock->ComplexConstFunction(crs, rs, s);

    CHECK_EQUAL("Test Result", result.c_str());
}


//------------------------------------------------------------------
TEST(MockObjectMethodMultiWithReturn)
{
    MockObject<Interface> mock;

    mock.call(&Interface::simpleFunctionWithReturn)
        .returning(22).returning(11);

    CHECK_EQUAL(22, mock->simpleFunctionWithReturn());
    CHECK_EQUAL(11, mock->simpleFunctionWithReturn());
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpect)
{
    MockObject<Interface> mock;

    mock.everyCall(&Interface::simpleFunctionWithParams)
        .expectOne<0>(21.0f)
        .expectOne<1>("Hello World")
        .expectOne<2>("SomeText");

    mock->simpleFunctionWithParams(21.0f, "Hello World", "SomeText");

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpectAll)
{
    MockObject<Interface> mock;

    mock.everyCall(&Interface::simpleFunctionWithParams)
        .expect(21.0f, "Hello World", "SomeText");

    mock->simpleFunctionWithParams(21.0f, "Hello World", "SomeText");

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpectAllIgnore)
{
    MockObject<Interface> mock;

    mock.everyCall(&Interface::simpleFunctionWithParams)
        .expect(21.0f, ignore(), "SomeText");

    mock->simpleFunctionWithParams(21.0f, "Hello World", "SomeText");

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpectPolicy)
{
    MockObject<Interface> mock;

    mock.everyCall(&Interface::PolicyTestFunction)
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
    MockObject<Interface> mock;

    mock.everyCall(&Interface::PolicyTestFunction)
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
    MockObject<Interface> mock;

    mock.everyCall(&Interface::simpleFunctionWithParams)
        .expectOne<0>(21.0f)
        .expectOne<1>("Hello World")
        .expectOne<2>("SomeText");

    CHECK_THROW( mock->simpleFunctionWithParams(2.0f, "Hllo World", "SomeTet")
        , NotEqualException );
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpectThrowNotEqual)
{
    MockObject<Interface, ManualVerify> mock;

    mock.call(&Interface::simpleFunctionWithParams)
      .expect(21.0f, "Hello World", "SomeText");

    CHECK_THROW( mock->simpleFunctionWithParams(2.0f, "Hllo World", "SomeTet")
        , NotEqualException );
}


//------------------------------------------------------------------ 

//------------------------------------------------------------------
TEST(MockObjectMethodMultipleExpectOne)
{
    MockObject<Interface> mock;

    mock.call(&Interface::simpleFunctionWithParams)
        .expectOne<0>(1.0f).expectOne<1>("Hello 1").expectOne<2>("SomeText1") 
        .expectOne<0>(2.0f).expectOne<1>("Hello 2").expectOne<2>("SomeText2")	
        .expectOne<0>(3.0f).expectOne<1>("Hello 3").expectOne<2>("SomeText3");

    mock->simpleFunctionWithParams(1.0f, "Hello 1", "SomeText1");
    mock->simpleFunctionWithParams(2.0f, "Hello 2", "SomeText2");
    mock->simpleFunctionWithParams(3.0f, "Hello 3", "SomeText3");

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodMultipleExpect)
{
    MockObject<Interface> mock;

    mock.call(&Interface::simpleFunctionWithParams)
        .expect(1.0f, "Hello 1", "SomeText1")
        .expect(2.0f, "Hello 2", "SomeText2")	
        .expect(3.0f, "Hello 3", "SomeText3");

    mock->simpleFunctionWithParams(1.0f, "Hello 1", "SomeText1");
    mock->simpleFunctionWithParams(2.0f, "Hello 2", "SomeText2");
    mock->simpleFunctionWithParams(3.0f, "Hello 3", "SomeText3");

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodMultipleExpectgnoreMultiple)
{
    MockObject<Interface> mock;

    mock.call(&Interface::simpleFunctionWithParams)
        .expect(ignore(), ignore(), "SomeText")
        .expect(ignore(), "Hello World", ignore())
        .expect(3.0f,  ignore(), ignore());

    mock->simpleFunctionWithParams(1.0f, "asfdlkjsf",  "SomeText");
    mock->simpleFunctionWithParams(2.0f, "Hello World", "fsda");
    mock->simpleFunctionWithParams(3.0f, "asdfjdskjfl", "fdsafjl");

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}



//------------------------------------------------------------------
TEST(MockObjectMethodMultipleExpectThrow)
{
    MockObject<Interface, ManualVerify> mock;

    mock.call(&Interface::simpleFunctionWithParams)
        .expectOne<0>(1.0f)		.expectOne<0>(2.0f)			.expectOne<0>(3.0f)
        .expectOne<1>("Hello 1")	.expectOne<1>("Hello 2")		.expectOne<1>("Hello 3")
        .expectOne<2>("SomeText1").expectOne<2>("SomeText2")	.expectOne<2>("SomeText3");

    CHECK_THROW( mock->simpleFunctionWithParams(2.0f, "Hllo 1", "SmeText1")
        , NotEqualException );
}


//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpectAndReturn)
{
    MockObject<Interface> mock;

    mock.everyCall(&Interface::simpleFunctionWithParamsAndReturn)
        .expectOne<0>(21.0f)
        .expectOne<1>("Hello World")
        .expectOne<2>("SomeText")
        .returning(2);

    CHECK_EQUAL(2, 
        mock->simpleFunctionWithParamsAndReturn(21.0f, "Hello World", "SomeText") );

    CHECK_THROW(
        mock->simpleFunctionWithParamsAndReturn(1.0f, "Heo World", "SoText")
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

    MockObject<Interface> mock;

    mock.call(&Interface::complexFunction).doing(&Local::HandleDo);

    std::string second = "Second";

    std::string result = 
        mock->complexFunction("First", second, "Third");

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
//    MockObject<Interface> mock;
//
//    mock.doing(&Interface::complexFunction)
//        .doing(rd);
//
//    std::string second = "Second";
//
//    std::string result = 
//        mock->complexFunction("First", second, "Third");
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

    MockObject<Interface> mock;

    mock.call(&Interface::complexFunction)
        .doing(&local, &TestDoLocal::HandleDo);

    std::string second = "Second";

    std::string result = 
        mock->complexFunction("First", second, "Third");

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

    MockObject<Interface> mock;

    mock.call(&Interface::complexFunction)
        .doing(&local, &TestDoLocal::HandleDo)
        .returning("SECOND_RESULT")   
        .throwing(SimpleException(22));

    std::string second = "Second";

    std::string dump;

    CHECK_EQUAL("RESULT",  mock->complexFunction("First", second, "Third").c_str());
    CHECK_EQUAL("SECOND_RESULT", mock->complexFunction("First", dump, "Third").c_str());
    CHECK_THROW(mock->complexFunction("First", dump, "Third"), SimpleException);


    CHECK_EQUAL("First", local.firstRecv.c_str());
    CHECK_EQUAL("Second", local.secondRecv.c_str());		
    CHECK_EQUAL("Third",  local.thirdRecv.c_str());

    CHECK_EQUAL("CHANGED", second.c_str());	
}


//------------------------------------------------------------------
TEST(MockObjectMethodSet)
{	
    TestDoLocal local;

    MockObject<Interface> mock;

    mock.everyCall(&Interface::complexFunction)
        .setOne<1>("CHANGED")
        .returning("");        

    std::string second = "Second";

    std::string result = 
        mock->complexFunction("First", second, "Third");

    CHECK_EQUAL("CHANGED", second.c_str());	
}

//------------------------------------------------------------------
TEST(MockObjectMethodSimpleSetPolicy)
{
    MockObject<Interface> mock;

    mock.everyCall(&Interface::PolicyTestFunction)
        .setOne<0>("First")
        .setOne<1>(policy::pointer("Second"))
        .setOne<2>(policy::array("Third", strlen("Third") + 1) );

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
    MockObject<Interface> mock;

    mock.call(&Interface::complexFunction)
        .setOne<1>("C1").setOne<1>("C2").setOne<1>("C3");

    mock.everyCall(&Interface::complexFunction)
        .returning("");        

    std::string second = "Second";

    mock->complexFunction("First", second, "Third");
    CHECK_EQUAL("C1", second.c_str());	
    mock->complexFunction("First", second, "Third");
    CHECK_EQUAL("C2", second.c_str());	
    mock->complexFunction("First", second, "Third");
    CHECK_EQUAL("C3", second.c_str());	

}

//------------------------------------------------------------------
TEST(MockObjectMethodAddCallCount)
{		
    MockObject<Interface> mock;

    mock.everyCall(&Interface::complexFunction).returning(std::string());

    std::string second = "Second";

    mock->complexFunction("First", second, "Third");
    mock->complexFunction("First", second, "Third");
    mock->complexFunction("First", second, "Third");

    CHECK_EQUAL(3u, mock.query(&Interface::complexFunction).count());
}

//------------------------------------------------------------------
TEST(MockObjectMethodReset)
{		
    MockObject<Interface> mock;

    mock.everyCall(&Interface::simpleFunction);

    mock.clear();

    CHECK_THROW(mock->simpleFunction(), NotImplementedException);
}

//------------------------------------------------------------------
TEST(MockObjectMethodDestructor)
{		
    MockObject<Interface> mock;

    mock.everyCall(destructor());

    delete ((Interface*)mock);

    // To disable  unreferenced formal parameter in UnitTest++
    CHECK(true);
}

//------------------------------------------------------------------
TEST(MockObjectMethodVerifyCallCount)
{		
    MockObject<Interface, ManualVerify> mock;

    mock.everyCall(&Interface::simpleFunction)
        .count(3);

    mock->simpleFunction();
    mock->simpleFunction();
    mock->simpleFunction();

    // It will not throw
    mock.verify();

    mock->simpleFunction();
    CHECK_THROW(mock.verify(), CallCountException);
}

//------------------------------------------------------------------
TEST(MockObjectMethodVerifyExpect)
{		
    MockObject<Interface, ManualVerify> mock;

    mock.call(&Interface::simpleFunctionWithParams)
        .expectOne<0>(1.0f).expectOne<0>(2.0f).expectOne<0>(3.0f);
        
    mock.everyCall(&Interface::simpleFunctionWithParams)        
        .expectOne<1>("")
        .expectOne<2>("");


    mock->simpleFunctionWithParams(1.0f, "", "");
    mock->simpleFunctionWithParams(2.0f, "", "");
    mock->simpleFunctionWithParams(3.0f, "", "");

    // It will not throw
    mock.verify();

    mock->simpleFunctionWithParams(4.0f, "", "");
    CHECK_THROW(mock.verify(), CallCountException);
}

//------------------------------------------------------------------
TEST(MockObjectMethodReturnCallCountException)
{		
    MockObject<Interface, ManualVerify> mock;

    mock.call(&Interface::simpleFunctionWithReturn)
        .returning(1)
        .returning(2)
        .returning(3);

    mock->simpleFunctionWithReturn();
    mock->simpleFunctionWithReturn();
    mock->simpleFunctionWithReturn();

    CHECK_THROW(mock->simpleFunctionWithReturn()
        , CallCountException);
}

//------------------------------------------------------------------
TEST(MockObjectMethodReturnCallCountVerify)
{		
    MockObject<Interface, ManualVerify> mock;

    mock.call(&Interface::simpleFunctionWithReturn)
        .returning(1)
        .returning(2)
        .returning(3);

    CHECK_THROW(mock.verify(), CallCountException);
}

//------------------------------------------------------------------
class Simple1
{
public:
    virtual void simpleFunction() = 0;
    virtual int simpleFunctionWithReturn() = 0;
    virtual ~Simple1();
};

//------------------------------------------------------------------
class Simple2
{
public:
    virtual void simpleFunction() = 0;
    virtual int simpleFunctionWithReturn() = 0;
    virtual ~Simple2();
};

//------------------------------------------------------------------
TEST(TwoInterfaces_noneImplemented)
{
    MockObject<Simple1> mock1;
    MockObject<Simple2> mock2;

    CHECK_THROW(((Simple1*)mock1)->simpleFunction(), NotImplementedException);
    CHECK_THROW(((Simple2*)mock2)->simpleFunction(), NotImplementedException);

    CHECK_THROW(((Simple1*)mock1)->simpleFunctionWithReturn(), NotImplementedException);
    CHECK_THROW(((Simple2*)mock2)->simpleFunctionWithReturn(), NotImplementedException);
}

//------------------------------------------------------------------
TEST(TwoInterfaces_oneSimpleImplemented)
{
    MockObject<Simple1> mock1;
    MockObject<Simple2> mock2;

    mock1.everyCall(&Simple1::simpleFunction);

    ((Simple1*)mock1)->simpleFunction();
    CHECK_THROW(((Simple2*)mock2)->simpleFunction(), NotImplementedException);

    CHECK_THROW(((Simple1*)mock1)->simpleFunctionWithReturn(), NotImplementedException);
    CHECK_THROW(((Simple2*)mock2)->simpleFunctionWithReturn(), NotImplementedException);
}

//------------------------------------------------------------------
TEST(TwoInterfaces_bothSimpleImplemented)
{
    MockObject<Simple1> mock1;
    MockObject<Simple2> mock2;

    mock1.everyCall(&Simple1::simpleFunction);
    mock2.everyCall(&Simple2::simpleFunction);

    ((Simple1*)mock1)->simpleFunction();
    ((Simple2*)mock2)->simpleFunction();

    CHECK_THROW(((Simple1*)mock1)->simpleFunctionWithReturn(), NotImplementedException);
    CHECK_THROW(((Simple2*)mock2)->simpleFunctionWithReturn(), NotImplementedException);
}

//------------------------------------------------------------------
TEST(TwoInterfaces_oneReturnImplemented)
{
    MockObject<Simple1> mock1;
    MockObject<Simple2> mock2;

    int expected1 = 1;
    mock1.everyCall(&Simple1::simpleFunctionWithReturn)
        .returning(expected1);

    CHECK_THROW(((Simple1*)mock1)->simpleFunction(), NotImplementedException);
    CHECK_THROW(((Simple2*)mock2)->simpleFunction(), NotImplementedException);

    int got = ((Simple1*)mock1)->simpleFunctionWithReturn();
    CHECK_EQUAL(expected1, got);
    CHECK_THROW(((Simple2*)mock2)->simpleFunctionWithReturn(), NotImplementedException);
}

//------------------------------------------------------------------
TEST(TwoInterfaces_bothReturnImplemented)
{
    MockObject<Simple1> mock1;
    MockObject<Simple2> mock2;

    int expected1 = 1;
    int expected2 = 2;

    mock1.everyCall(&Simple1::simpleFunctionWithReturn)
        .returning(expected1);
    mock2.everyCall(&Simple2::simpleFunctionWithReturn)
        .returning(expected2);

    CHECK_THROW(((Simple1*)mock1)->simpleFunction(), NotImplementedException);
    CHECK_THROW(((Simple2*)mock2)->simpleFunction(), NotImplementedException);

    int got;
    got = ((Simple1*)mock1)->simpleFunctionWithReturn();
    CHECK_EQUAL(expected1, got);
    got = ((Simple2*)mock2)->simpleFunctionWithReturn();
    CHECK_EQUAL(expected2, got);
}

struct Overloads
{
    virtual ~Overloads(){}
    virtual int simpleFunction() = 0;
    virtual int simpleFunction(int x) = 0;
};

TEST(Overloads1)
{
    // An example of an interface with method overrides
    

    // The method with no parameters
    typedef int (Overloads::*PMF0)();
    PMF0 pmf0 = &Overloads::simpleFunction;

    // The method with a single parameter
    typedef int (Overloads::*PMF1)(int);
    PMF1 pmf1 = &Overloads::simpleFunction;

    MockObject<Overloads> mock;

    mock.everyCall(pmf0).returning(1);
    mock.everyCall(pmf1).returning(2);

    Overloads * pI = mock;

    CHECK_EQUAL(1, pI->simpleFunction());
    CHECK_EQUAL(2, pI->simpleFunction(3)); 
};


