#ifndef __AMOP_MOCKFUNCTIONIMPL_HH
#define __AMOP_MOCKFUNCTIONIMPL_HH

#include "MockFunction.h"
#include "DynamicFunction.h"
#include "Comparable.h"
#include <vector>
#include <map>

namespace amop
{
    namespace Detail
    {        
        //------------------------------------------------------------------
        class TMockFunctionImpl 
            : public IMockFunction
            , public IDynamicFunctionHandler
        {
        public:
            TMockFunctionImpl();            

            // IMockFunction Interface
            void SetExpectCallCounter(size_t counter);            

            void SetRedirect(const any& result, bool isDefault);

            void SetSetter(size_t paramId, const TComparable& param, bool isDefault);

            void SetExpect(size_t paramId, const TComparable& param, bool isDefault);

            void SetReturn(const std::pair<bool, any>& result, bool isDefault);

            virtual size_t GetCallCounter();

            // IDynamicFunctionHandler Interface         
            any& GetRedirect();

            void AddCallCounter();

            any& GetReturn();

            void SetActual(size_t paramId, const any& param);

            // Its own public interface
            void Verify();

        protected:              
            enum TReturnType { RETURN, THROW, DO };

            typedef std::pair<TReturnType, any> TReturnTypePair;
            
            void VerifyCallCounter();

            void VerifyReturn();

            void VerifyExpect();

            void ApplySetter(size_t paramId, const any& param);

            TReturnTypePair* _GetReturn(bool check) const;

            void _SetReturn(const TReturnTypePair& ret, bool isDefault);
            
            size_t mCallCounter;
            std::auto_ptr<size_t> mExpectCallCounter;

            typedef std::vector<TComparable> TComparableList;
		    typedef std::map<size_t, TComparableList> TParamMap;
		    typedef std::map<size_t, TComparable> TParamDefaultMap;
            
            std::auto_ptr< std::vector< TReturnTypePair > > mReturn;	
            std::auto_ptr< TReturnTypePair > mReturnDefault;

            std::auto_ptr<TParamMap> mExpect;
		    std::auto_ptr<TParamDefaultMap> mExpectDefault;

            std::auto_ptr<TParamMap> mSetter;
            std::auto_ptr<TParamDefaultMap > mSetterDefault;
        };        
    }
}

#endif //__AMOP_MOCKFUNCTIONIMPL_HH


