#ifndef __AMOP_CALLHANDLER_HH
#define __AMOP_CALLHANDLER_HH

#include "Config.h"
#include "VirtualTable.h"
#include "FunctionHolder.h"
#include <vector>

namespace amop
{

namespace Detail
{

struct CallHandler
{
	/*
    template <int I>
	struct SelectID
	{
		template <class T>
		struct Get
		{	
			static TFunctionAddress Select(size_t offset)
			{			
				if(I == offset)
					return HorribleCast<TFunctionAddress>(&FunctionHolder<I,T>::Func);
				else
					return SelectID<I+1>::template Get<T>::Select(offset);
			}
		};
	};*/

		
	template <class T>
	static TFunctionAddress Select(size_t offset)
	{
        std::vector<TFunctionAddress> funcs;

#define DETAIL_FUNC_ITEM(n, t) funcs.push_back(     \
    HorribleCast<TFunctionAddress>(&FunctionHolder<n-1,T>::Func) );    
#define DETAIL_FUNC_ITEMS(n) DETAIL_REPEAT(n,DETAIL_FUNC_ITEM,DETAIL_FUNC_ITEM,t)
        DETAIL_FUNC_ITEMS(MAX_NUM_VIRTUAL_FUNCTIONS);

        return funcs[offset];
        //return SelectID<0>::Get<T>::Select(offset);
	}

	template<class T>
	static TFunctionAddress Create(size_t offset)
	{
		TFunctionAddress result = Select<T>(offset);
		assert(result);
		
		return result;
	}
};

//template <>
//struct CallHandler::SelectID<MAX_NUM_VIRTUAL_FUNCTIONS>
//{
//	template <class T>
//	struct Get
//	{		
//		static TFunctionAddress Select(size_t offset)
//		{
//			return 0;
//		}
//	};
//};

}

}

#endif //__AMOP_CALLHANDLER_HH

