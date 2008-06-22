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
    struct AutoVerify
    {
        template <class T>
        static void Verify(T& o)
        {
            o.Verify();
        }
    };

    struct ManualVerify
    {
        template <class T>
        static void Verify(T&)
        {
        }
    };
    
    //------------------------------------------------------------------
    template <class T, typename VerifyPolicy = AutoVerify >
    class TMockObject : public Detail::TMockObjectBase
    {
    public:
        ~TMockObject()
        {
            VerifyPolicy::Verify(*this);
        }        
        
        operator T*()
        { 
            return (T*)GetVptr();
        }	

        //*****************************************
        //     Single Version
        //*****************************************
        template <class F>
        TReturnMatchBuilder<F, Detail::TSinglePolicy> Single(F method)
        {
            typedef int ItIsNotPointerToMemberMethod[
                Detail::IsPointerToMethod<F>::Result ? 1 : -1];

            Detail::TDynamicFunction* function = GetDynamicObject()->Bind(method);

            return TReturnMatchBuilder<F, Detail::TSinglePolicy>(CreateMockFunction(function));
        }

        TReturnMatchBuilder<void (T::*)(void*), Detail::TSinglePolicy> Single(const Destructor&)
        {
            typedef void (T::*TDestructorType)(void*);               
            
            Detail::TDynamicFunction* function = GetDynamicObject()->BindDestructor<T>();
            
            return TReturnMatchBuilder<TDestructorType, Detail::TSinglePolicy>(CreateMockFunction(function));            
        }


        //*****************************************
        //     All Version
        //*****************************************
        template <class F>
        TReturnMatchBuilder<F, Detail::TAllPolicy> All(F method)
        {
            typedef int ItIsNotPointerToMemberMethod[
                Detail::IsPointerToMethod<F>::Result ? 1 : -1];

            Detail::TDynamicFunction* function = GetDynamicObject()->Bind(method);

            return TReturnMatchBuilder<F, Detail::TAllPolicy>(CreateMockFunction(function));
        }

        TReturnMatchBuilder<void (T::*)(void*), Detail::TAllPolicy> All(const Destructor&)
        {
            typedef void (T::*TDestructorType)(void*);               
            
            Detail::TDynamicFunction* function = GetDynamicObject()->BindDestructor<T>();
            
            return TReturnMatchBuilder<TDestructorType, Detail::TAllPolicy>(CreateMockFunction(function));            
        }

        //*****************************************
        //     Query Version
        //*****************************************
        template <class F>
        TReturnMatchBuilder<F, Detail::TQueryPolicy> Query(F method)
        {
            typedef int ItIsNotPointerToMemberMethod[
                Detail::IsPointerToMethod<F>::Result ? 1 : -1];

            Detail::TDynamicFunction* function = GetDynamicObject()->Bind(method);

            return TReturnMatchBuilder<F, Detail::TQueryPolicy>(CreateMockFunction(function));
        }

        TReturnMatchBuilder<void (T::*)(void*), Detail::TQueryPolicy> Query(const Destructor&)
        {
            typedef void (T::*TDestructorType)(void*);               
            
            Detail::TDynamicFunction* function = GetDynamicObject()->BindDestructor<T>();
            
            return TReturnMatchBuilder<TDestructorType, Detail::TQueryPolicy>(CreateMockFunction(function));            
        }

        //*****************************************
        //     Redirect Version
        //*****************************************
        template <class F>
        TReturnMatchBuilder<F, Detail::TRedirectPolicy> Redirect(F method)
        {
            typedef int ItIsNotPointerToMemberMethod[
                Detail::IsPointerToMethod<F>::Result ? 1 : -1];

            Detail::TDynamicFunction* function = GetDynamicObject()->Bind(method);

            return TReturnMatchBuilder<F, Detail::TRedirectPolicy>(CreateMockFunction(function));
        }

        TReturnMatchBuilder<void (T::*)(void*), Detail::TRedirectPolicy> Redirect(const Destructor&)
        {
            typedef void (T::*TDestructorType)(void*);               
            
            Detail::TDynamicFunction* function = GetDynamicObject()->BindDestructor<T>();
            
            return TReturnMatchBuilder<TDestructorType, Detail::TRedirectPolicy>(CreateMockFunction(function));            
        }

    private:


    };
}


#endif //__AMOP_MOCKOBJECT_HH

