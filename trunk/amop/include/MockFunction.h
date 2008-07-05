#ifndef __AMOP_MOCKFUNCTION_HH
#define __AMOP_MOCKFUNCTION_HH

#include "Config.h"
#include "Any.h"

namespace amop
{
    namespace Detail
    {        
        class TComparable;
        
        //------------------------------------------------------------------
        class IMockFunction
        {
        public:
            virtual ~IMockFunction(){}

            virtual void SetExpectCallCounter(size_t counter) = 0;

            virtual void SetRedirect(const any& result, bool isDefault) = 0;

            virtual void SetSetter(size_t paramId, const TComparable& param, bool isDefault) = 0;

            virtual void SetExpect(size_t paramId, const TComparable& param, bool isDefault) = 0;

            virtual void SetReturn(const std::pair<bool, any>& result, bool isDefault) = 0;

            virtual size_t GetCallCounter() = 0;
        };
    }
}

#endif //__AMOP_MOCKFUNCTION_HH


