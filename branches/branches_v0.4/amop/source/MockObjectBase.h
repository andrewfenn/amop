#ifndef __AMOP_MOCKOBJECTBASE_HH
#define __AMOP_MOCKOBJECTBASE_HH

#include "Any.h"
#include "ObjectHolder.h"
#include "DynamicObject.h"

#include <map>
#include <vector>

namespace amop
{

namespace Detail
{
  
	class TMockObjectBase : public Detail::TObjectHolder
	{
	public:
		TMockObjectBase();
		~TMockObjectBase();

		void Clear();
        void Verify();

	protected:       
		typedef std::vector<TComparable> TComparableList;
		typedef std::map<size_t, TComparableList> TParamMap;
		typedef std::map<size_t, TComparable> TParamDefaultMap;

        std::map<size_t, std::pair<bool,any> > mReturnDefaults;
        std::map<size_t, std::vector<std::pair<bool,any> > > mReturns;
		std::map<size_t, size_t> mCallCounter;
        std::map<size_t, size_t> mExpectCallCounter;
		
		std::map<size_t, any> mRedirects;
		std::map<size_t,  TParamMap> mExpects;
		std::map<size_t,  TParamDefaultMap> mExpectDefaults;
        
        std::map<size_t,  TParamMap> mSetters;
        std::map<size_t,  TParamDefaultMap > mSetterDefaults;		

        std::auto_ptr<TDynamicObject> mDynamicObject;

		void AddCallCounter(size_t idx);
		size_t GetCallCounter(size_t idx);
        void SetExpectCallCounter(size_t idx, size_t c);

		any& GetRedirect(size_t idx);
		void SetRedirect(size_t idx, const any& redirect);

        void SetReturnDefault(size_t idx, const std::pair<bool, any>& result);
        void SetReturn(size_t idx, const std::pair<bool, any>& result);
        std::pair<bool, any>& GetReturn(size_t idx);

		void SetActual(size_t idx, size_t paramId, const any& param);
		void SetExpectDefault(size_t idx, size_t paramId, const TComparable& param);
		void SetExpect(size_t idx, size_t paramId, const TComparable& param);

        bool HaveExpectDefault(size_t idx, size_t paramId);
        bool HaveExpect(size_t idx, size_t paramId);

        void SetSetterDefault(size_t idx, size_t paramId, const TComparable& result);
        void SetSetter(size_t idx, size_t paramId, const TComparable& param);

        bool HaveSetterDefault(size_t idx, size_t paramId);
        bool HaveSetter(size_t idx, size_t paramId);
        void ApplySetter(size_t idx, size_t paramId, const any& param);

		void* GetVptr();
		
    private:
        TMockObjectBase( const TMockObjectBase& );
        TMockObjectBase& operator=(const TMockObjectBase& );

	};
}

}

//Local Variables:
//c-basic-offset: 4
//End:
#endif //__AMOP_MOCKOBJECTBASE_HH
