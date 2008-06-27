#include "DynamicFunction.h"

namespace amop
{
    namespace Detail
    {                
        //------------------------------------------------------------------
        TDynamicFunction::TDynamicFunction()                
        {
        }
        
        //------------------------------------------------------------------
        any& TDynamicFunction::GetRedirect()
        {
            return GetHandler()->GetRedirect();
        }
            
        //------------------------------------------------------------------
        any& TDynamicFunction::GetReturn()
        {
            return GetHandler()->GetReturn();
        }
            
        //------------------------------------------------------------------
        void TDynamicFunction::SetActual(size_t paramId, const any& param)
        {
            return GetHandler()->SetActual(paramId, param);
        }
            
        //------------------------------------------------------------------
        void TDynamicFunction::AddCallCounter()
        {
            return GetHandler()->AddCallCounter();
        }
    }
}


