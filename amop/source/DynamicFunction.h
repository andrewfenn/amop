#ifndef __AMOP_DYNAMICFUNCTION_HH
#define __AMOP_DYNAMICFUNCTION_HH

namespace amop
{
    namespace Detail
    {        
        //------------------------------------------------------------------
        class IDynamicFunctionHandler
        {
        public:
            virtual ~IDynamicFunctionHandler(){}

            // Inheritent from TObjectHolder        
            virtual any& GetRedirect() = 0;
            
            virtual std::pair<bool, any>& GetReturn() = 0;
            
            virtual void SetActual(size_t paramId, const any& param) = 0;
            
            virtual void AddCallCounter() = 0;
        };        
        
        //------------------------------------------------------------------
        class TDynamicFunction
        {
        public:
            TDynamicFunction(IDynamicFunctionHandler* handler)
                : mHandler(handler)
            {
            }

            IDynamicFunctionHandler* GetHandler()
            {
                return mHandler;
            }
        
        protected:
            IDynamicFunctionHandler* mHandler;
        };
    }
}

#endif //__AMOP_DYNAMICFUNCTION_HH


