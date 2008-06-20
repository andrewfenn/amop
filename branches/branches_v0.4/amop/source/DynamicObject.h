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

            template<class F>
            TDynamicFunction* GetBinded(F method)
            {
                size_t offset = Detail::Inner::TCheckOffset::GetOffset(method);

                return !mFunctions.count(offset) ? NULL : mFunctions[offset];
            }

            template<class T>
            TDynamicFunction* GetBindedDestructor()
            {
                size_t offset = Detail::Inner::TCheckOffset::GetOffsetDestructor<T>();

                return !mFunctions.count(offset) ? NULL : mFunctions[offset];
            }

            template <class F>
            void Bind(F method, IDynamicFunctionHandler* handler)
            {
                size_t offset = Detail::Inner::TCheckOffset::GetOffset(method);

                _BindFunction(offset, Detail::CallHandler::Create<F>(offset), handler);
            }

            template <class T>
            void BindDestructor(IDynamicFunctionHandler* handler)
            {
                typedef void (T::*TDestructorType)(void*);
                
                size_t offset = Detail::Inner::TCheckOffset::GetOffsetDestructor<T>();

                _BindFunction(offset, Detail::CallHandler::Create<TDestructorType>(offset), handler);
            }            

        public:
            // Inheritent from TObjectHolder        
            any& GetRedirect(size_t idx);
            std::pair<bool, any>& GetReturn(size_t idx);
            void SetActual(size_t idx, size_t paramId, const any& param);
            void AddCallCounter(size_t idx);

            void _BindFunction(size_t offset, TFunctionAddress data, IDynamicFunctionHandler* handler);
            
            typedef std::map<size_t, TDynamicFunction*> TDynamicFunctionMap;

            Detail::TVirtualTable* mVirtualTable;	
            TDynamicFunctionMap mFunctions;            
        };
    }
}

#endif //__AMOP_DYNAMICOBJECT_HH

