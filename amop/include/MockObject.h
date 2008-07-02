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
    
    //! The main mock object class
    /*!
        TMockObject is a template class for mocking your interface.

        \section sec_creation Initialization

        Let's say your want to mock a interface:

        \code
            class IInterface
            {
            public:                
                
                virtual void Foo1() = 0;
                virtual void Foo2(size_t i, const std::string& s) = 0;
                virtual size_t Foo3() = 0;
                virtual size_t Foo4(size_t i) = 0;
            };  

        \endcode

        To create your mock object:              
        \code
            TMockObject<IInterface> mock;
        \endcode

        The \c IInterface is what interface you want to mock with and
        it should be a pure virtual class
        (No member variables, No non-virtual member functions).
        
        \section sec_usage Usage

        After that, you can treat it as a smart pointer of your interface, 
        putting it into your testing functions directly.        

        \code
            // The functions to test
            void functionToTest(IInterface*);           
            void functionToTestByRef(IInterface&);
            
            // In your test:          
            functionToTest(mock);
            functionToTestByRef(*mock);
        \endcode

        \section Bindings

        TMockObject support 4 types of function binding modes:
        \ref ss_call, \ref ss_everycall, \ref ss_query, 
        \ref ss_redirect. But all the usage of this modes are the same:        

        \code
            TMockObject<IInterface> mock;
            
            mock.EveryCall(&IInterface::Foo2).Expect(10, "String");
            
            mock.Call(&IInterface::Foo3).Return(20);
            
            mock.Redirect(&IInterface::Foo4).Do(&SomeOtherFunction);          
            
            size_t numberOfCalls = mock.Query(&IInterface::Foo4).Count();            
        \endcode

        After you binded your functions to the mock object, you can
        peform verify the arguments, query the number of times the function called
        and redirect it to your own function etc., according the modes you choosed.
        
        And you can mix these modes together by calling it twice:

        \code
            // The IInterface::Foo4 will return 10 every times
            mock.EveryCall(&IInterface::Foo4).Return(10);
            
            // The &IInterface::Foo4 expect 10, and then 20
            mock.Call(&IInterface::Foo4)
                .Expect(10)
                .Expect(20);          
        \endcode        
        
        \subsection ss_call Call

        In this mode, your can verify the arguments, setting the return values
        of your function. <b>The number of calls must match exactly, 
        each call has distinct arguments that it shall verify against, 
        and distinct return values it shall mock. </b> When the expectation is not corrected,
        an exception will be thrown:
        
        \code
            mock.Call(&IInterface::Foo4)
                .Expect(10).Return(1)
                .Expect(20).Return(2);          
            
            // We use UnitTest++ here, but you can use any unit-test library with amop.
            CHECK(1, mock->Foo4(10));
            CHECK(2, mock->Foo4(20));

            CHECK_THROW(mock->Foo4(30), TCallCountException);            
        \endcode

        The TMockObject::Call method will bind your given method to Call mode, 
        and it will return an object, which you can call its method to setting the
        behaviors of given function when it is mocked. Please 
        reference Detail::TReturnMatchBuilder<F, Detail::TCallPolicy> for its usage.

        \sa TMockObject::Call
        \sa Detail::TReturnMatchBuilder<F, Detail::TCallPolicy>                
       
        \subsection ss_everycall EveryCall

        The Call function of TMockObject is used for 

        \subsection ss_query Query

        The Call function of TMockObject is used for 

        \subsection ss_redirect Redirect

        The Call function of TMockObject is used for            

        \subsection ss_destructor Binding to destructor
        
        In some case, your will want to mock the destructor of a interface,
        your can use the trait object Destructor:

        \code
            mock.Call(Destructor());
        \endcode

        \remarks
            In fact, it is not binding to your constructor directly, but
            it is binding to the delete operator of your interface.
            However, because it is a pure virutal class, 
            there are only 2 way to call the destructor, 
            one is delete it by using the delete operator, 
            another one is call it directly ( mock->~IInterface() ). 
            Normally latter method is rarely happened.

    */
    //------------------------------------------------------------------
    template <class T, typename VerifyPolicy = AutoVerify >
    class TMockObject : public Detail::TMockObjectBase
    {
    public:        
        //! Destructor        
        /*!
            The destructor of TMockObject, 
        */        
        ~TMockObject()
        {
            VerifyPolicy::Verify(*this);
        }        
        
        operator T*()
        { 
            return (T*)GetVptr();
        }	

        T& operator *()
        {
            return *((T*)GetVptr());
        }

        T* operator ->()
        {
            return (T*)GetVptr();
        }

        void Clear() { Detail::TMockObjectBase::Clear(); }
        void Verify() { Detail::TMockObjectBase::Verify(); }

        //*****************************************
        //     Call Mode
        //*****************************************
        //! Create the call mode binding        
        /*!
            This function create the call mode binding. The call mode is used for
            verify and setting arguments for each calls, which the number of calls must match exactly.

            \param method
                The method to be binding with

            \return
                The builder object for setting the behavior the given method.

            \sa \ref ss_call, Detail::TReturnMatchBuilder<F, Detail::TCallPolicy>                
        */
        
        template <class F>
        Detail::TReturnMatchBuilder<F, typename Detail::TCallPolicy> Call(F method)
        {
            typedef int ItIsNotPointerToMemberMethod[
                Detail::IsPointerToMethod<F>::Result ? 1 : -1];

            Detail::TDynamicFunction* function = GetDynamicObject()->Bind(method);

            return Detail::TReturnMatchBuilder<F, Detail::TCallPolicy>(CreateMockFunction(function));
        }

        //! Create the call mode binding for destructor        
        /*!
            This function create the call mode binding for destructor. The call mode is used for
            verify and setting arguments for each calls, which the number of calls must match exactly.
            To binding a function to destructor, using the Destructor() trait object.

            \return
                The builder object for setting the behavior the given method.

            \sa \ref ss_call, \ref ss_destructor, Detail::TReturnMatchBuilder<F, Detail::TCallPolicy>
        */
        Detail::TReturnMatchBuilder<void (T::*)(void*), typename Detail::TCallPolicy> Call(const Destructor&)
        {
            typedef void (T::*TDestructorType)(void*);               
            
            Detail::TDynamicFunction* function = GetDynamicObject()->template BindDestructor<T>();
            
            return Detail::TReturnMatchBuilder<TDestructorType, Detail::TCallPolicy>(CreateMockFunction(function));            
        }


        //*****************************************
        //     EveryCall Mode
        //*****************************************
        template <class F>
        Detail::TReturnMatchBuilder<F, Detail::TEveryCallPolicy> EveryCall(F method)
        {
            typedef int ItIsNotPointerToMemberMethod[
                Detail::IsPointerToMethod<F>::Result ? 1 : -1];

            Detail::TDynamicFunction* function = GetDynamicObject()->Bind(method);

            return Detail::TReturnMatchBuilder<F, Detail::TEveryCallPolicy>(CreateMockFunction(function));
        }

        Detail::TReturnMatchBuilder<void (T::*)(void*), Detail::TEveryCallPolicy> EveryCall(const Destructor&)
        {
            typedef void (T::*TDestructorType)(void*);               
            
            Detail::TDynamicFunction* function = GetDynamicObject()->template BindDestructor<T>();
            
            return Detail::TReturnMatchBuilder<TDestructorType, Detail::TEveryCallPolicy>(CreateMockFunction(function));            
        }

        //*****************************************
        //     Query Mode
        //*****************************************        


        //! Binding the function to Query mode
        /*!
            This function binds a function to query mode. After you 
        */
        template <class F>
        Detail::TReturnMatchBuilder<F, Detail::TQueryPolicy> Query(F method)
        {
            typedef int ItIsNotPointerToMemberMethod[
                Detail::IsPointerToMethod<F>::Result ? 1 : -1];

            Detail::TDynamicFunction* function = GetDynamicObject()->Bind(method);

            return Detail::TReturnMatchBuilder<F, Detail::TQueryPolicy>(CreateMockFunction(function));
        }

        Detail::TReturnMatchBuilder<void (T::*)(void*), Detail::TQueryPolicy> Query(const Destructor&)
        {
            typedef void (T::*TDestructorType)(void*);               
            
            Detail::TDynamicFunction* function = GetDynamicObject()->template BindDestructor<T>();
            
            return Detail::TReturnMatchBuilder<TDestructorType, Detail::TQueryPolicy>(CreateMockFunction(function));            
        }

        //*****************************************
        //     Redirect Mode
        //*****************************************
        template <class F>
        Detail::TReturnMatchBuilder<F, Detail::TRedirectPolicy> Redirect(F method)
        {
            typedef int ItIsNotPointerToMemberMethod[
                Detail::IsPointerToMethod<F>::Result ? 1 : -1];

            Detail::TDynamicFunction* function = GetDynamicObject()->Bind(method);

            return Detail::TReturnMatchBuilder<F, Detail::TRedirectPolicy>(CreateMockFunction(function));
        }

        Detail::TReturnMatchBuilder<void (T::*)(void*), Detail::TRedirectPolicy> Redirect(const Destructor&)
        {
            typedef void (T::*TDestructorType)(void*);               
            
            Detail::TDynamicFunction* function = GetDynamicObject()->template BindDestructor<T>();
            
            return Detail::TReturnMatchBuilder<TDestructorType, Detail::TRedirectPolicy>(CreateMockFunction(function));            
        }

    private:


    };
}


#endif //__AMOP_MOCKOBJECT_HH

