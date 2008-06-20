#ifndef __AMOP_MOCKOBJECT_HH
#define __AMOP_MOCKOBJECT_HH

#include "Config.h"
#include "MockObjectBase.h"
#include "MockObjectException.h"
#include "ReturnMatchBuilder.h"
#include "Destructor.h"
#include "Policy.h"

namespace amop
{

    //------------------------------------------------------------------
    template <class T>
    class TMockObject : public Detail::TMockObjectBase
    {
    public:
        operator T*()
        { 
            return (T*)GetVptr();
        }	

        //*****************************************
        //     Single Version
        //*****************************************
        template <class F>
        TReturnMatchBuilder<F, TSinglePolicy> Single(F method)
        {
            typedef int ItIsNotPointerToMemberMethod[
                Detail::IsPointerToMethod<F>::Result ? 1 : -1];

            Detail::TDynamicFunction* function = GetDynamicObject()->Bind(method);

            return TReturnMatchBuilder<F, TSinglePolicy>(CreateMockFunction(function));
        }

        TReturnMatchBuilder<void (T::*)(void*), TSinglePolicy> Single(const Destructor&)
        {
            typedef void (T::*TDestructorType)(void*);               
            
            Detail::TDynamicFunction* function = GetDynamicObject()->BindDestructor<T>();
            
            return TReturnMatchBuilder<TDestructorType, TSinglePolicy>(CreateMockFunction(function));            
        }


        //*****************************************
        //     All Version
        //*****************************************
        template <class F>
        TReturnMatchBuilder<F, TAllPolicy> All(F method)
        {
            typedef int ItIsNotPointerToMemberMethod[
                Detail::IsPointerToMethod<F>::Result ? 1 : -1];

            Detail::TDynamicFunction* function = GetDynamicObject()->Bind(method);

            return TReturnMatchBuilder<F, TAllPolicy>(CreateMockFunction(function));
        }

        TReturnMatchBuilder<void (T::*)(void*), TAllPolicy> All(const Destructor&)
        {
            typedef void (T::*TDestructorType)(void*);               
            
            Detail::TDynamicFunction* function = GetDynamicObject()->BindDestructor<T>();
            
            return TReturnMatchBuilder<TDestructorType, TAllPolicy>(CreateMockFunction(function));            
        }

        //*****************************************
        //     Query Version
        //*****************************************
        template <class F>
        TReturnMatchBuilder<F, TQueryPolicy> Query(F method)
        {
            typedef int ItIsNotPointerToMemberMethod[
                Detail::IsPointerToMethod<F>::Result ? 1 : -1];

            Detail::TDynamicFunction* function = GetDynamicObject()->Bind(method);

            return TReturnMatchBuilder<F, TQueryPolicy>(CreateMockFunction(function));
        }

        TReturnMatchBuilder<void (T::*)(void*), TQueryPolicy> Query(const Destructor&)
        {
            typedef void (T::*TDestructorType)(void*);               
            
            Detail::TDynamicFunction* function = GetDynamicObject()->BindDestructor<T>();
            
            return TReturnMatchBuilder<TDestructorType, TQueryPolicy>(CreateMockFunction(function));            
        }

        //*****************************************
        //     Query Version
        //*****************************************
        template <class F>
        TReturnMatchBuilder<F, TRedirectPolicy> Redirect(F method)
        {
            typedef int ItIsNotPointerToMemberMethod[
                Detail::IsPointerToMethod<F>::Result ? 1 : -1];

            Detail::TDynamicFunction* function = GetDynamicObject()->Bind(method);

            return TReturnMatchBuilder<F, TRedirectPolicy>(CreateMockFunction(function));
        }

        TReturnMatchBuilder<void (T::*)(void*), TRedirectPolicy> Redirect(const Destructor&)
        {
            typedef void (T::*TDestructorType)(void*);               
            
            Detail::TDynamicFunction* function = GetDynamicObject()->BindDestructor<T>();
            
            return TReturnMatchBuilder<TDestructorType, TRedirectPolicy>(CreateMockFunction(function));            
        }

    private:


    };
}


#endif //__AMOP_MOCKOBJECT_HH

