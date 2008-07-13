#ifndef __AMOP_DYNAMICFUNCTION_HH
#define __AMOP_DYNAMICFUNCTION_HH

#include "Config.h"
#include "Any.h"

namespace amop
{
    namespace detail
    {        
        //------------------------------------------------------------------
        class DynamicFunctionHandler
        {
        public:
            virtual ~DynamicFunctionHandler(){}

            virtual any& getRedirect() = 0;
            
            virtual any& getReturn() = 0;
            
            virtual void setActual(size_t paramId, const any& param) = 0;
            
            virtual void addCallCounter() = 0;
        };        
        
        //------------------------------------------------------------------
        class DynamicFunction
        {
        public:
            DynamicFunction();               

            DynamicFunctionHandler* GetHandler()
            {
                return m_handler;
            }

            void setHandler(DynamicFunctionHandler* handler)
            {
                m_handler = handler;
            }

            any& getRedirect();
            
            any& getReturn();
            
            void setActual(size_t paramId, const any& param);
            
            void addCallCounter();

        protected:
            DynamicFunctionHandler* m_handler;
        };
    }
}

#endif //__AMOP_DYNAMICFUNCTION_HH


