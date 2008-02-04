#include <UnitTest++.h>
#include "../MockObject.h"
#include <string>

using namespace amop;

//------------------------------------------------------------------
class IInterface
{
public:
	virtual ~IInterface(){}

	virtual void SimpleFunction() = 0;
	virtual void SimpleFunctionWithParams(float, std::string, const char*) = 0;
	
	virtual void SimpleFunctionWithAlotParams(float, int, float, int, std::string) = 0;

	virtual int SimpleFunctionWithReturn() = 0;
	virtual int& SimpleFunctionWithRefReturn() = 0;

	virtual void SimpleConstFunction() const = 0;

	virtual std::string ComplexFunction(const std::string& crs,
		std::string& rs, std::string s) = 0;

	virtual std::string ComplexConstFunction(const std::string& crs,
		std::string& rs, std::string s) const = 0;
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

	mock.Method(&IInterface::SimpleFunction);
	mock.Method(&IInterface::SimpleFunctionWithAlotParams);

	((IInterface*)mock)->SimpleFunction();
	((IInterface*)mock)->SimpleFunctionWithAlotParams(0, 0, 0, 0, std::string());
}

//------------------------------------------------------------------
TEST(MockObjectMethodComplex)
{
	TMockObject<IInterface> mock;

	mock.Method(&IInterface::ComplexFunction)
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

	mock.Method(&IInterface::SimpleFunctionWithReturn)
		.Will(22);
	mock.Method(&IInterface::SimpleFunctionWithRefReturn)
		.Will(1262);

	CHECK_EQUAL(22, ((IInterface*)mock)->SimpleFunctionWithReturn());
	CHECK_EQUAL(1262, ((IInterface*)mock)->SimpleFunctionWithRefReturn());
}

//------------------------------------------------------------------
TEST(MockObjectMethodConstSimple)
{
	TMockObject<IInterface> mock;

	mock.Method(&IInterface::SimpleConstFunction);
			
	((IInterface*)mock)->SimpleConstFunction();	
}

//------------------------------------------------------------------
TEST(MockObjectMethodConstComplex)
{
	TMockObject<IInterface> mock;

	mock.Method(&IInterface::ComplexConstFunction)
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

	mock.Method(&IInterface::SimpleFunctionWithReturn)
		.Wills(22).Wills(11);
	
	CHECK_EQUAL(22, ((IInterface*)mock)->SimpleFunctionWithReturn());
	CHECK_EQUAL(11, ((IInterface*)mock)->SimpleFunctionWithReturn());
}


//------------------------------------------------------------------
TEST(MockObjectMethodSimpleExpect)
{
	TMockObject<IInterface> mock;

	mock.Method(&IInterface::SimpleFunctionWithParams)
		.Expect<0>(21.0f)
		.Expect<1>("Hello World")
		.Expect<2>("SomeText");

	((IInterface*)mock)->SimpleFunctionWithParams(21.0f, "Hello World", "SomeText");
}

//------------------------------------------------------------------
TEST(MockObjectMethodMultipleExpect)
{
	TMockObject<IInterface> mock;

	mock.Method(&IInterface::SimpleFunctionWithParams)
		.Expects<0>(1.0f)		.Expects<0>(2.0f)			.Expects<0>(3.0f)
		.Expects<1>("Hello 1")	.Expects<1>("Hello 2")		.Expects<1>("Hello 3")
		.Expects<2>("SomeText1").Expects<2>("SomeText2")	.Expects<2>("SomeText3");

	((IInterface*)mock)->SimpleFunctionWithParams(1.0f, "Hello 1", "SomeText1");
	((IInterface*)mock)->SimpleFunctionWithParams(2.0f, "Hello 2", "SomeText2");
	((IInterface*)mock)->SimpleFunctionWithParams(3.0f, "Hello 3", "SomeText3");
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

	mock.Method(&IInterface::ComplexFunction)
		.Redirect(&Local::HandleRedirect);

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
TEST(MockObjectMethodRedirectMethod)
{	
	struct Local
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

	Local local;
	
	TMockObject<IInterface> mock;

	mock.Method(&IInterface::ComplexFunction)
		.Redirect(&local, &Local::HandleRedirect);

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
TEST(MockObjectMethodAddCallCount)
{		
	TMockObject<IInterface> mock;

	mock.Method(&IInterface::ComplexFunction).Will(std::string());

	std::string second = "Second";

	((IInterface*)mock)->ComplexFunction("First", second, "Third");
	((IInterface*)mock)->ComplexFunction("First", second, "Third");
	((IInterface*)mock)->ComplexFunction("First", second, "Third");

	CHECK_EQUAL(3, mock.Method(&IInterface::ComplexFunction).Count());
}
