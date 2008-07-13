#ifndef __AMOP_MOCKFUNCTION_HH
#define __AMOP_MOCKFUNCTION_HH

#include "Config.h"
#include "Any.h"

namespace amop
{
    namespace detail
    {        
        class Comparable;
        
        //------------------------------------------------------------------
        class MockFunction
        {
        public:
            virtual ~MockFunction(){}

            virtual void setExpectCallCounter(size_t counter) = 0;

            virtual void setRedirect(const any& result, bool isDefault) = 0;

            virtual void setSetter(size_t paramId, const Comparable& param, bool isDefault) = 0;

            virtual void setExpect(size_t paramId, const Comparable& param, bool isDefault) = 0;

            virtual void setReturn(const std::pair<bool, any>& result, bool isDefault) = 0;

            virtual size_t getCallCounter() = 0;
        };
    }
}

#endif //__AMOP_MOCKFUNCTION_HH


