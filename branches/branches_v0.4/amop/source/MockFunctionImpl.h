#ifndef __AMOP_MOCKFUNCTIONIMPL_HH
#define __AMOP_MOCKFUNCTIONIMPL_HH

#include "MockFunction.h"
#include "DynamicFunction.h"
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

            virtual void SetExpectCallCounter(size_t counter);            

            virtual void SetRedirect(const any& result);

            virtual void SetSetter(size_t paramId, const TComparable& param, bool isDefault);

            virtual void SetExpect(size_t paramId, const TComparable& param, bool isDefault);

            virtual void SetReturn(const std::pair<bool, any>& result, bool isDefault);

            virtual size_t GetCallCounter();

            any& GetRedirect();

            void AddCallCounter();

            void Verify();

            std::pair<bool, any>& GetReturn();

            void SetActual(size_t paramId, const any& param);

        protected:              
            void VerifyCallCounter();

            void VerifyReturn();

            void VerifyExpect();

            void ApplySetter(size_t paramId, const any& param);


            any mRedirect;
            size_t mCallCounter;
            std::auto_ptr<size_t> mExpectCallCounter;

            typedef std::vector<TComparable> TComparableList;
		    typedef std::map<size_t, TComparableList> TParamMap;
		    typedef std::map<size_t, TComparable> TParamDefaultMap;
            
            std::auto_ptr< std::vector<std::pair<bool,any> > > mReturn;	
            std::auto_ptr< std::pair<bool,any> > mReturnDefault;

            std::auto_ptr<TParamMap> mExpect;
		    std::auto_ptr<TParamDefaultMap> mExpectDefault;

            std::auto_ptr<TParamMap> mSetter;
            std::auto_ptr<TParamDefaultMap > mSetterDefault;
        };        
    }
}

#endif //__AMOP_MOCKFUNCTIONIMPL_HH


