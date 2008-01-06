#ifndef __AMOP_CHECKOFFSETFUNC_HH
#define __AMOP_CHECKOFFSETFUNC_HH

#include "MockObjectException.h"
#include "Config.h"

namespace amop
{

namespace Detail
{

//------------------------------------------------------------------
class TCheck 
{
public:
	static bool mIsCheckCall;
};

//------------------------------------------------------------------
typedef int (TCheck::*TCheckFuncPtr) ();

//------------------------------------------------------------------
template<int I>
class TCheckIdx
{
public:
	int Func() { TCheck::mIsCheckCall = true; return I; }
};

//------------------------------------------------------------------
template<int I>
static void* GetIndexFuncRecur(size_t i)
{
	typedef int (TCheckIdx<I>::*TCheckFuncPtr)();
	
	TCheckFuncPtr _ptr = &TCheckIdx<I>::Func;
	void* p = HorribleCast<void*>(_ptr);
	
	if(i == I)
		return p;
	else
		return GetIndexFuncRecur<I+1>(i);
}

//------------------------------------------------------------------
template<>
static void* GetIndexFuncRecur<MAX_NUM_VIRTUAL_FUNCTIONS>(size_t i)
{
	return 0;
}

//------------------------------------------------------------------
static void* GetIndexFunc(size_t i)
{
	return GetIndexFuncRecur<0>(i);
}

//------------------------------------------------------------------
static TCheck* CreateCheckObject()
{
	static void* vtable[MAX_NUM_VIRTUAL_FUNCTIONS];
	static void* vtbl = &vtable[0];
		
	for(size_t i = 0 ; i < MAX_NUM_VIRTUAL_FUNCTIONS; ++i)
		vtable[i] = GetIndexFunc(i);
			
	return (TCheck*)&vtbl;
}

//------------------------------------------------------------------
template <class T>
size_t GetOffset(T memFuncPtr)
{
	TCheckFuncPtr check = 
		 reinterpret_cast<TCheckFuncPtr>(memFuncPtr);

	TCheck::mIsCheckCall = false; 
	TCheck* checkObject = CreateCheckObject();
	
	size_t offset = (checkObject->*check)();	

	if(!TCheck::mIsCheckCall)
	{
		throw TNotPureVirtualException();
	}

	return offset;
}

}

}

#endif //__AMOP_CHECKOFFSETFUNC_HH