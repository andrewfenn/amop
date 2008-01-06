#include <UnitTest++.h>
#include "../Any.h"
#include "../Tuple.h"
#include "../Functor.h"

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
	typedef Detail::Tuple<float , int , double > TestTuple;
	typedef Detail::Tuple<> EmptyTuple;

	CHECK_EQUAL(TestTuple::Length, 3);
	
	bool result = Equal< Detail::Get<TestTuple, 1>::Type, int >::Result;
	CHECK( result );
}

template <class T>
static void MakeFunctor(T f)
{
	Detail::Functor<T>();
}

//------------------------------------------------------------------
TEST(TestFunctorParams)
{
	using namespace Detail;
	
	struct Temp
	{
		void Foo1() {};
		int Foo2(float, int, double) { return 0; }
	};

	typedef Detail::Functor<void (Temp::*)()> F1; //(&Temp::Foo1); 
	typedef Detail::Functor<int (Temp::*)(float, int, double) > F2; //(&Temp::Foo2); 

	MakeFunctor(&Temp::Foo1);
	MakeFunctor(&Temp::Foo2);

	bool result = Equal< Get<F2::ParameterTypes, 1>::Type, int >::Result;
	CHECK(result);
}
