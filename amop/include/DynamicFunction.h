#ifndef __AMOP_DYNAMICFUNCTION_HH
#define __AMOP_DYNAMICFUNCTION_HH

#include "Config.h"
#include "Any.h"

namespace amop
{
    namespace Detail
    {        
        //------------------------------------------------------------------
        class IDynamicFunctionHandler
        {
        public:
            virtual ~IDynamicFunctionHandler(){}

            virtual any& GetRedirect() = 0;
            
            virtual any& GetReturn() = 0;
            
            virtual void SetActual(size_t paramId, const any& param) = 0;
            
            virtual void AddCallCounter() = 0;
        };        
        
        //------------------------------------------------------------------
        class TDynamicFunction
        {
        public:
            TDynamicFunction();               

            IDynamicFunctionHandler* GetHandler()
            {
                return mHandler;
            }

            void SetHandler(IDynamicFunctionHandler* handler)
            {
                mHandler = handler;
            }

            any& GetRedirect();
            
            any& GetReturn();
            
            void SetActual(size_t paramId, const any& param);
            
            void AddCallCounter();

        protected:
            IDynamicFunctionHandler* mHandler;
        };
    }
}

#endif //__AMOP_DYNAMICFUNCTION_HH


