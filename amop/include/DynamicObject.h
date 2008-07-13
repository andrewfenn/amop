#ifndef __AMOP_DYNAMICOBJECT_HH
#define __AMOP_DYNAMICOBJECT_HH

#include "Config.h"
#include "CheckOffsetFunc.h"
#include "CallHandler.h"
#include "ObjectHolder.h"
#include "DynamicFunction.h"

#include <map>

namespace amop
{
    namespace detail
    {        
        class VirtualTable;
        class DynamicFunction;        
        
        //------------------------------------------------------------------
        class DynamicObject : public detail::ObjectHolder
        {
        public:
            DynamicObject();
            ~DynamicObject();

            void* getVptr();
            
            template <class F>
            DynamicFunction* bind(F method)
            {
                size_t offset = detail::inner::offset::getOffset(method);

                return bindFunction(offset, detail::CallHandler::create<F>(offset));
            }

            template <class T>
            DynamicFunction*  bindDestructor()
            {
                typedef void (T::*TDestructorType)(void*);
                
                size_t offset = detail::inner::offset::getOffsetDestructor<T>();

                return bindFunction(offset, detail::CallHandler::create<TDestructorType>(offset));
            }            

        public:
            // Inheritent from ObjectHolder        
            any& getRedirect(size_t idx);
            any& getReturn(size_t idx);
            void setActual(size_t idx, size_t paramId, const any& param);
            void addCallCounter(size_t idx);

            DynamicFunction* bindFunction(size_t offset, FunctionAddress data);
            
            typedef std::map<size_t, DynamicFunction*> DynamicFunctionMap;

            detail::VirtualTable* m_virtualTable;	
            DynamicFunctionMap m_functions;            
        };
    }
}

#endif //__AMOP_DYNAMICOBJECT_HH

