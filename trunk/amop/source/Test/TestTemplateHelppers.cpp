#include <UnitTest++.h>
#include "Any.h"
#include "Tuple.h"
#include "Functor.h"

using namespace amop;

template<class T1, class T2>
struct Equal
{
	enum { Result = false };
};

template<class T>
struct Equal<T,T>
{
	enum { Result = true };
};

//------------------------------------------------------------------
TEST(TestTuple)
{
	typedef detail::Tuple<float , int , double > TestTuple;
	typedef detail::Tuple<> EmptyTuple;

	int len = TestTuple::Length;
	CHECK_EQUAL(len, 3);
	
	bool result = Equal< detail::Get<TestTuple, 1>::Type, int >::Result;
	CHECK( result );
}

template <class T>
static void MakeFunctor(T /*t*/)
{
	detail::Functor<T>();
}

struct TestFunctorClass
{
    void DETAIL_CDECL Foo0() {};
    void Foo1() {};
	int Foo2(float, int, double) { return 0; }
};

//------------------------------------------------------------------
TEST(TestFunctorParams)
{
	using namespace detail;
	
	typedef detail::Functor<void (DETAIL_CDECL TestFunctorClass::*)()> F0; //(&TestFunctorClass::Foo1); 
	typedef detail::Functor<void (TestFunctorClass::*)()> F1; //(&TestFunctorClass::Foo1); 
	typedef detail::Functor<int (TestFunctorClass::*)(float, int, double) > F2; //(&TestFunctorClass::Foo2); 

    MakeFunctor(&TestFunctorClass::Foo0);
	MakeFunctor(&TestFunctorClass::Foo1);
	MakeFunctor(&TestFunctorClass::Foo2);

	bool result = Equal< Get<F2::ParameterTypes, 1>::Type, int >::Result;
	CHECK(result);
}
