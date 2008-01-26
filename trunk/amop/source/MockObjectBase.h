#ifndef __AMOP_MOCKOBJECTBASE_HH
#define __AMOP_MOCKOBJECTBASE_HH

#include "any.h"
#include "ObjectHolder.h"
#include "ReturnMatchBuilder.h"
#include "CheckOffsetFunc.h"
#include "CallHandler.h"
#include <map>
#include <vector>

namespace amop
{

namespace Detail
{
	class TVirtualTable;


	//------------------------------------------------------------------
	class TMockObjectBase : public Detail::TObjectHolder
	{
	public:
		TMockObjectBase();
		~TMockObjectBase();

	protected:
		std::map<size_t, any> mReturnDefaults;
		std::map<size_t, std::vector<any> > mReturns;
		std::map<size_t, size_t> mCallCounter;
		
		std::map<size_t, any> mRedirects;

		Detail::TVirtualTable* mVirtualTable;

		typedef std::vector<TComparable> TComparableList;
		typedef std::map<size_t, TComparableList> TParamMap;
		typedef std::map<size_t, TComparable> TParamDefaultMap;

		std::map<size_t,  TParamMap> mExpects;
		std::map<size_t,  TParamDefaultMap> mExpectDefaults;

		void AddCallCounter(size_t idx);

		any& GetRedirect(size_t idx);
		void SetRedirect(size_t idx, const any& redirect);

		void SetReturnDefault(size_t idx, const any& result);
		void SetReturn(size_t idx, const any& result);
		any& GetReturn(size_t idx);

		void SetActual(size_t idx, size_t paramId, const any& param);
		void SetExpectDefault(size_t idx, size_t paramId, const TComparable& param);
		void SetExpect(size_t idx, size_t paramId, const TComparable& param);

		void* GetVptr();

		template <class T>
		TReturnMatchBuilder<T> CreateMatchBuilder(size_t offset, void* data)
		{
			assert(offset < MAX_NUM_VIRTUAL_FUNCTIONS);
			mVirtualTable->mVtable[offset] = data;

			return TReturnMatchBuilder<T>(this, offset);
		}

		template <class T>
		TReturnMatchBuilder<T> CreateMatchBuilder(T method)
		{
			size_t offset = Detail::GetOffset(method);

			return CreateMatchBuilder<T>(offset, 
				Detail::CallHandler::Create(method, offset) );


		}

	};
}

}

#endif //__AMOP_MOCKOBJECTBASE_HH