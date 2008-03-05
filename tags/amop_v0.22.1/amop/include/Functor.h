#ifndef __AMOP_FUNCTOR__HH
#define __AMOP_FUNCTOR__HH

#include "Config.h"
#include "Tuple.h"
#include "MacroRepeat.h"
#include "FastDelegate.h"

namespace amop
{
namespace Detail
{

template< typename T>
struct Functor;

template< typename R, typename C >
struct Functor< R (C::*)()>
{
	typedef typename R ReturnType;
	typename typename typedef Tuple<> ParameterTypes;
	typename typename typedef fastdelegate::FastDelegate0<R> FunctorType;
};

template< typename R, typename C >
struct Functor< R (C::*)() const>
{
	typedef typename R ReturnType;
	typename typename typedef Tuple<> ParameterTypes;
	typename typename typedef fastdelegate::FastDelegate0<R> FunctorType;
};


//now specialize
#define DETAIL_FUNCTION_BUILD(n)					\
template< typename R, typename C, DETAIL_TPARAMS(n) >		\
struct Functor< R (C::*)(DETAIL_TPARAMS(n))>			\
{												\
	typedef typename R ReturnType;								\
	typedef typename Tuple<DETAIL_TPARAMS(n)> ParameterTypes;	\
	typename typename typedef fastdelegate::FastDelegate##n<DETAIL_TPARAMS(n), R> FunctorType; \
};			\
template< typename R, typename C, DETAIL_TPARAMS(n) >		\
struct Functor< R (C::*)(DETAIL_TPARAMS(n)) const>			\
{												\
	typedef typename R ReturnType;								\
	typedef typename Tuple<DETAIL_TPARAMS(n)> ParameterTypes;	\
	typename typename typedef fastdelegate::FastDelegate##n<DETAIL_TPARAMS(n), R> FunctorType; \
};


DETAIL_FUNCTION_BUILD(1);
DETAIL_FUNCTION_BUILD(2);
DETAIL_FUNCTION_BUILD(3);
DETAIL_FUNCTION_BUILD(4);
DETAIL_FUNCTION_BUILD(5);
DETAIL_FUNCTION_BUILD(6);
DETAIL_FUNCTION_BUILD(7);
DETAIL_FUNCTION_BUILD(8);
/*
DETAIL_FUNCTION_BUILD(9);
DETAIL_FUNCTION_BUILD(10);
DETAIL_FUNCTION_BUILD(11);
DETAIL_FUNCTION_BUILD(12);
DETAIL_FUNCTION_BUILD(13);
DETAIL_FUNCTION_BUILD(14);
DETAIL_FUNCTION_BUILD(15);
*/

}
}

#endif //__FUNCTOR__HH
