#ifndef __AMOP_MOCKFUNCTIONIMPL_HH
#define __AMOP_MOCKFUNCTIONIMPL_HH

#include "MockFunction.h"
#include "DynamicFunction.h"
#include "Comparable.h"
#include <vector>
#include <map>

namespace amop
{
    namespace detail
    {        
        //------------------------------------------------------------------
        class MockFunctionImpl 
            : public MockFunction
            , public DynamicFunctionHandler
        {
        public:
            MockFunctionImpl();            

            // MockFunction Interface
            void setExpectCallCounter(size_t counter);            

            void setRedirect(const any& result, bool isDefault);

            void setSetter(size_t paramId, const Comparable& param, bool isDefault);

            void setExpect(size_t paramId, const Comparable& param, bool isDefault);

            void setReturn(const std::pair<bool, any>& result, bool isDefault);

            virtual size_t getCallCounter();

            // DynamicFunctionHandler Interface         
            any& getRedirect();

            void addCallCounter();

            any& getReturn();

            void setActual(size_t paramId, const any& param);

            // Its own public interface
            void verify();

        protected:              
            enum ReturnType { RETURN, THROW, DO };

            typedef std::pair<ReturnType, any> ReturnTypePair;
            
            void verifyCallCounter();

            void verifyReturn();

            void verifyExpect();

            void applySetter(size_t paramId, const any& param);

            ReturnTypePair* getReturn(bool check) const;

            void setReturnInternal(const ReturnTypePair& ret, bool isDefault);
            
            size_t m_callCounter;
            std::auto_ptr<size_t> m_expectCallCounter;

            typedef std::vector<Comparable> ComparableList;
		    typedef std::map<size_t, ComparableList> ParamMap;
		    typedef std::map<size_t, Comparable> ParamDefaultMap;
            
            std::auto_ptr< std::vector< ReturnTypePair > > m_return;	
            std::auto_ptr< ReturnTypePair > m_returnDefault;

            std::auto_ptr<ParamMap> m_expect;
		    std::auto_ptr<ParamDefaultMap> m_expectDefault;

            std::auto_ptr<ParamMap> m_setter;
            std::auto_ptr<ParamDefaultMap > m_setterDefault;
        };        
    }
}

#endif //__AMOP_MOCKFUNCTIONIMPL_HH


