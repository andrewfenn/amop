#ifndef __MOCKPP_MOCKOBJECTEXECEPTION__HH
#define __MOCKPP_MOCKOBJECTEXECEPTION__HH

#include "Any.h"

namespace MockPP
{

//------------------------------------------------------------------
class TNotImplementedException
{
};

//------------------------------------------------------------------
class TNotPureVirtualException
{
};

//------------------------------------------------------------------
class TNotEqualException
{
public:
	TNotEqualException(size_t param, const any& expect, const any& actual) :
	  mParam(param), mExpect(expect), mActual(actual){};

	any& GetExpect() { return mExpect; }
	any& GetActual() { return mActual; }

	size_t GetParam() { return mParam; }

protected:
	any mExpect;
	any mActual;
	size_t mParam;

};

}

#endif //__MOCKPP_MOCKOBJECTEXECEPTION__HH