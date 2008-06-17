#ifndef __AMOP_DYNAMICOBJECT_HH
#define __AMOP_DYNAMICOBJECT_HH

#include "Config.h"
#include "CheckOffsetFunc.h"
#include "CallHandler.h"

namespace amop
{
    namespace Detail
    {        
        class TVirtualTable;
        
        //------------------------------------------------------------------
        class TDynamicObject
        {
        public:
            TDynamicObject(TObjectHolder* object);

            void* GetVptr();

            template <class F>
            size_t Bind(F method)
            {
                size_t offset = Detail::Inner::TCheckOffset::GetOffset(method);

                _BindFunction(offset, Detail::CallHandler::Create<F>(offset));

                return offset;
            }

            template <class T>
            size_t BindDestructor()
            {
                typedef void (T::*TDestructorType)(void*);               
                
                size_t offset = Detail::Inner::TCheckOffset::GetOffsetDestructor<T>();

                _BindFunction(offset, Detail::CallHandler::Create<TDestructorType>(offset));

                return offset;
            }

        public:
            void _BindFunction(size_t offset, TFunctionAddress data);
            
            Detail::TVirtualTable* mVirtualTable;	
        };
    }
}

#endif //__AMOP_DYNAMICOBJECT_HH

