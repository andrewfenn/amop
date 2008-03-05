#ifndef __AMOP_CALLHANDLER_HH
#define __AMOP_CALLHANDLER_HH

#include "Config.h"
#include "VirtualTable.h"
#include "FunctionHolder.h"

namespace amop
{

namespace Detail
{

struct CallHandler
{
	template <int I>
	struct SelectID
	{
		template <class T>
		struct Get
		{	
			static void* Select(T method, size_t offset)
			{			
				if(I == offset)
					return HorribleCast<void*>(&FunctionHolder<I,T>::Func);
				else
					return SelectID<I+1>::Get<T>::Select(method, offset);
			}
		};
	};

	template <>
	struct SelectID<MAX_NUM_VIRTUAL_FUNCTIONS>
	{
		template <class T>
		struct Get
		{		
			static void* Select(T method, size_t offset)
			{
				return 0;
			}
		};
	};
	
	template <class T>
	static void* Select(T method, 
		size_t offset)
	{
		return SelectID<0>::Get<T>::Select(method, offset);
	}

	template<class T>
	static void* Create(T method, size_t offset)
	{
		void* result = Select(method, offset);
		assert(result);
		
		return result;
	}
};

}

}

#endif //__AMOP_CALLHANDLER_HH