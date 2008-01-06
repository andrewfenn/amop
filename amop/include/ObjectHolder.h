#ifndef __MOCKPP_OBJECTHOLDER_HH
#define __MOCKPP_OBJECTHOLDER_HH

#include "Any.h"

namespace MockPP
{
namespace Detail
{

class TComparable;

//------------------------------------------------------------------
class TObjectHolder
{
public:
	TObjectHolder()
	{
	}

	virtual ~TObjectHolder(){}

	template <class R>
	R Return(size_t idx)
	{
		return any_cast<R>(GetReturn(idx));
	}

	template <class T>
	void Actual(size_t funcIdx, size_t idx, T t)
	{
		SetActual(funcIdx, idx, t);
	}

	virtual void SetReturn(size_t idx, const any& result) = 0;
	virtual void SetReturnDefault(size_t idx, const any& result) = 0;
	
	virtual any& GetReturn(size_t idx) = 0;
	virtual void SetActual(size_t idx, size_t paramId, const any& param) = 0;
	
	virtual void SetExpectDefault(size_t idx, size_t paramId, const TComparable& param) = 0;
	virtual void SetExpect(size_t idx, size_t paramId, const TComparable& param) = 0;


	virtual void AddCallCounter(size_t idx) = 0;
};

}

}

#endif //__MOCKPP_OBJECTHOLDER_HH