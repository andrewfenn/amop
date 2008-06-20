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
    namespace Detail
    {        
        class TVirtualTable;
        class TDynamicFunction;        
        
        //------------------------------------------------------------------
        class TDynamicObject : public Detail::TObjectHolder
        {
        public:
            TDynamicObject();
            ~TDynamicObject();

            void* GetVptr();
            
            template <class F>
            TDynamicFunction* Bind(F method)
            {
                size_t offset = Detail::Inner::TCheckOffset::GetOffset(method);

                return _BindFunction(offset, Detail::CallHandler::Create<F>(offset));
            }

            template <class T>
            TDynamicFunction*  BindDestructor()
            {
                typedef void (T::*TDestructorType)(void*);
                
                size_t offset = Detail::Inner::TCheckOffset::GetOffsetDestructor<T>();

                return _BindFunction(offset, Detail::CallHandler::Create<TDestructorType>(offset));
            }            

        public:
            // Inheritent from TObjectHolder        
            any& GetRedirect(size_t idx);
            any& GetReturn(size_t idx);
            void SetActual(size_t idx, size_t paramId, const any& param);
            void AddCallCounter(size_t idx);

            TDynamicFunction* _BindFunction(size_t offset, TFunctionAddress data);
            
            typedef std::map<size_t, TDynamicFunction*> TDynamicFunctionMap;

            Detail::TVirtualTable* mVirtualTable;	
            TDynamicFunctionMap mFunctions;            
        };
    }
}

#endif //__AMOP_DYNAMICOBJECT_HH

