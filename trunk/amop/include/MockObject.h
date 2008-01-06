#ifndef __MOCKPP_MOCKOBJECT_HH
#define __MOCKPP_MOCKOBJECT_HH

#include "Config.h"
#include "MockObjectBase.h"
#include "MockObjectException.h"
#include "ReturnMatchBuilder.h"

namespace MockPP
{

//------------------------------------------------------------------
template <class T>
class TMockObject : public Detail::TMockObjectBase
{
public:
	operator T*()
	{ 
		return (T*)GetVptr();
	}	

	template <class T>
	TReturnMatchBuilder<T> Method(T method)
	{
		typedef int ItIsNotPointerToMemberMethod[
			Detail::IsPointerToMethod<T>::Result ? 1 : -1];
		
		return CreateMatchBuilder(method);
	}

private:
	

};

}


#endif //__MOCKPP_MOCKOBJECT_HH