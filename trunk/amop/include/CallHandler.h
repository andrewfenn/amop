#ifndef __AMOP_CALLHANDLER_HH
#define __AMOP_CALLHANDLER_HH

#include "Config.h"
#include "FunctionHolder.h"
#include <vector>
#include <assert.h>

namespace amop
{

namespace detail
{

struct CallHandler
{
	template <class T>
	static FunctionAddress select(size_t offset)
	{
        std::vector<FunctionAddress> funcs;

#define DETAIL_FUNC_ITEM(n, t) funcs.push_back(     \
    HorribleCast<FunctionAddress>(&FunctionHolder<n-1,T>::func) );    
#define DETAIL_FUNC_ITEMS(n) DETAIL_REPEAT(n,DETAIL_FUNC_ITEM,DETAIL_FUNC_ITEM,t)
        DETAIL_FUNC_ITEMS(MAX_NUM_VIRTUAL_FUNCTIONS);

        return funcs[offset];        
	}

	template<class T>
	static FunctionAddress create(size_t offset)
	{
		FunctionAddress result = select<T>(offset);
		assert(result);
		
		return result;
	}
};

}

}

#endif //__AMOP_CALLHANDLER_HH

