#include "DynamicFunction.h"

namespace amop
{
    namespace detail
    {                
        //------------------------------------------------------------------
        DynamicFunction::DynamicFunction()                
        {
        }
        
        //------------------------------------------------------------------
        any& DynamicFunction::getRedirect()
        {
            return GetHandler()->getRedirect();
        }
            
        //------------------------------------------------------------------
        any& DynamicFunction::getReturn()
        {
            return GetHandler()->getReturn();
        }
            
        //------------------------------------------------------------------
        void DynamicFunction::setActual(size_t paramId, const any& param)
        {
            return GetHandler()->setActual(paramId, param);
        }
            
        //------------------------------------------------------------------
        void DynamicFunction::addCallCounter()
        {
            return GetHandler()->addCallCounter();
        }
    }
}


