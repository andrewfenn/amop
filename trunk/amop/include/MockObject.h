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
        static void verify(T& o)
        {
            o.verify();
        }
    };

    struct ManualVerify
    {
        template <class T>
        static void verify(T&)
        {
        }
    };
    
    //! The main mock object class
    /*!
        MockObject is a template class for mocking your interface.

        \section sec_creation Initialization

        Let's say your want to mock a interface:

        \code
            class Interface
            {
            public:                
                
                virtual void foo1() = 0;
                virtual void foo2(size_t i, const std::string& s) = 0;
                virtual size_t foo3() = 0;
                virtual size_t foo4(size_t i) = 0;
            };  

        \endcode

        To create your mock object:              
        \code
            MockObject<Interface> mock;
        \endcode

        The \c Interface is what interface you want to mock with and
        it should be a pure virtual class
        (No member variables, No non-virtual member functions).
        
        \section sec_usage Usage

        After that, you can treat it as a smart pointer of your interface, 
        putting it into your testing functions directly.        

        \code
            // The functions to test
            void functionToTest(Interface*);           
            void functionToTestByRef(Interface&);
            
            // In your test:          
            functionToTest(mock);
            functionToTestByRef(*mock);
        \endcode

        \section Bindings

        MockObject support 3 types of function binding modes:
        \ref ss_call, \ref ss_everycall and \ref ss_query.
        But all the usage of this modes are the same:        

        \code
            MockObject<Interface> mock;
            
            mock.everyCall(&Interface::foo2).expect(10, "String");
            
            mock.call(&Interface::foo3).returning(20);                       
            
            size_t numberOfCalls = mock.query(&Interface::foo4).count();            
        \endcode

        After you binded your functions to the mock object, you can
        peform verify the arguments, query the number of times the function called
        and redirect it to your own function etc., according the modes you choosed.
        
        And you can mix these modes together by calling it twice:

        \code
            // The Interface::foo4 will return 10 every times
            mock.everyCall(&Interface::foo4).returning(10);
            
            // The &Interface::foo4 expect 10, and then 20
            mock.call(&Interface::foo4)
                .expect(10)
                .expect(20);          
        \endcode        
        
        \subsection ss_call call Mode

        In this mode, your can verify the arguments, setting the return values
        of your function. <b>The number of calls must match exactly, 
        each call has distinct arguments that it shall verify against, 
        and distinct return values it shall mock. </b> When the expectation is not corrected,
        an exception will be thrown:
        
        \code
            mock.call(&Interface::foo4)
                .expect(10).Return(1)
                .expect(20).Return(2);          
            
            // We use UnitTest++ here, but you can use any unit-test library with amop.
            CHECK(1, mock->foo4(10));
            CHECK(2, mock->foo4(20));

            CHECK_THROW(mock->foo4(30), CallCountException);            
        \endcode

        The MockObject::call method will bind your given method to call mode, 
        and it will return an object, which you can call its method to setting the
        behaviors of given function when it is mocked. Please 
        reference detail::ReturnMatchBuilder<F, detail::CallPolicy> for its usage.

        \sa MockObject::call
        \sa detail::ReturnMatchBuilder<F, detail::CallPolicy>                
       
        \subsection ss_everycall everyCall Mode

        The everyCall mode is very similar to \ref ss_call , except
        verify arguments an infinite number of times, 
        and shall not care about number of calls:

        \code
            mock.everyCall(&Interface::foo4)
                .expect(10).Return(1)

            CHECK(1, mock->foo4(10);
            CHECK(1, mock->foo4(10);
            CHECK(1, mock->foo4(10);
            CHECK(1, mock->foo4(10);
        \endcode

        The MockObject::everyCall method will return an object in everyCall mode,
        which you can setting the behaviors like MockObject::call return object does. Please
        reference detail::ReturnMatchBuilder<F, detail::EveryCallPolicy> for its usage.

        \sa MockObject::everyCall
        \sa detail::ReturnMatchBuilder<F, detail::EveryCallPolicy>                

        \subsection ss_query query Mode

        The query Mode is used for getting the function's propeties for mocking
        (Like the number of calls):

        \code
            mock.everyCall(&Inteface::foo4).Return(1)

            mock->foo4(10);
            mock->foo4(10);
            mock->foo4(10);
            mock->foo4(10);
            mock->foo4(10);

            CHECK(5, mock.query(&Interface::foo4).Count())
        \endcode

        Please reference detail::ReturnMatchBuilder<F, detail::QueryPolicy> for its usage.

        \sa MockObject::query
        \sa detail::ReturnMatchBuilder<F, detail::QueryPolicy>                
        
        \subsection ss_destructor Binding to destructor
        
        In some case, your will want to mock the destructor of a interface,
        your can use the trait object Destructor:

        \code
            mock.call(Destructor());
        \endcode

        \remarks
            In fact, it is not binding to your constructor directly, but
            it is binding to the delete operator of your interface.
            However, because it is a pure virutal class, 
            there are only 2 way to call the destructor, 
            one is delete it by using the delete operator, 
            another one is call it directly ( mock->~Interface() ). 
            Normally latter method is rarely happened.

    */
    //------------------------------------------------------------------
    template <class T, typename VerifyPolicy = AutoVerify >
    class MockObject : public detail::MockObjectBase
    {
    public:        
        //! Destructor        
        /*!
            The destructor of MockObject, 
        */        
        ~MockObject()
        {
            VerifyPolicy::verify(*this);
        }        
        
        operator T*()
        { 
            return (T*)getVptr();
        }	

        T& operator *()
        {
            return *((T*)getVptr());
        }

        T* operator ->()
        {
            return (T*)getVptr();
        }

        void clear() { detail::MockObjectBase::clear(); }
        void verify() { detail::MockObjectBase::verify(); }

        //*****************************************
        //     call Mode
        //*****************************************
        //! Create the \ref ss_call binding        
        /*!
            This function create the \ref ss_call binding. The \ref ss_call is used for
            verify and setting arguments for each calls, which the number of calls must match exactly.

            \param method
                The method to be binding with

            \return
                The builder object for setting the behavior the given method.

            \sa \ref ss_call, detail::ReturnMatchBuilder<F, detail::CallPolicy>                
        */
        
        template <class F>
        detail::ReturnMatchBuilder<F, typename detail::CallPolicy> call(F method)
        {
            typedef int ItIsNotPointerToMemberMethod[
                detail::IsPointerToMethod<F>::Result ? 1 : -1];

            detail::DynamicFunction* function = getDynamicObject()->bind(method);

            return detail::ReturnMatchBuilder<F, detail::CallPolicy>(createMockFunction(function));
        }

        //! Create the \ref ss_call binding for destructor        
        /*!
            This function create the \ref ss_call binding for destructor. The \ref ss_call is used for
            verify and setting arguments for each calls, which the number of calls must match exactly.
            To binding a function to destructor, using the Destructor() trait object.

            \return
                The builder object for setting the behavior the given method.

            \sa \ref ss_call, \ref ss_destructor, detail::ReturnMatchBuilder<F, detail::CallPolicy>
        */
        detail::ReturnMatchBuilder<void (T::*)(void*), typename detail::CallPolicy> call(const Destructor&)
        {
            typedef void (T::*TDestructorType)(void*);               
            
            detail::DynamicFunction* function = getDynamicObject()->template bindDestructor<T>();
            
            return detail::ReturnMatchBuilder<TDestructorType, detail::CallPolicy>(createMockFunction(function));            
        }


        //*****************************************
        //     everyCall Mode
        //*****************************************
        //! Create the \ref ss_everycall binding        
        /*!
            This function create the ss_everycall binding. The ss_everycall is used for
            verify arguments an infinite number of times,  and shall not care about number of calls.

            \param method
                The method to be binding with

            \return
                The builder object for setting the behavior the given method.

            \sa \ref ss_everycall, detail::ReturnMatchBuilder<F, detail::EveryCallPolicy>                
        */
        template <class F>
        detail::ReturnMatchBuilder<F, detail::EveryCallPolicy> everyCall(F method)
        {
            typedef int ItIsNotPointerToMemberMethod[
                detail::IsPointerToMethod<F>::Result ? 1 : -1];

            detail::DynamicFunction* function = getDynamicObject()->bind(method);

            return detail::ReturnMatchBuilder<F, detail::EveryCallPolicy>(createMockFunction(function));
        }

        //! Create the \ref ss_everycall binding for destructor        
        /*!
            This function create the ss_everycall binding for destructor. The ss_everycall is used for
            verify and setting arguments for each calls, which the number of calls must match exactly.
            To binding a function to destructor, using the Destructor() trait object.

            \return
                The builder object for setting the behavior the given method.

            \sa \ref ss_everycall, \ref ss_destructor, detail::ReturnMatchBuilder<F, detail::EveryCallPolicy>
        */
        detail::ReturnMatchBuilder<void (T::*)(void*), detail::EveryCallPolicy> everyCall(const Destructor&)
        {
            typedef void (T::*TDestructorType)(void*);               
            
            detail::DynamicFunction* function = getDynamicObject()->template bindDestructor<T>();
            
            return detail::ReturnMatchBuilder<TDestructorType, detail::EveryCallPolicy>(createMockFunction(function));            
        }

        //*****************************************
        //     query Mode
        //*****************************************                
        //! Create the \ref ss_query binding        
        /*!
            This function create the \ref ss_query binding. The \ref ss_query is used for
            getting the property of mocking function (Like the number calls)

            \param method
                The method to be binding with

            \return
                The builder object for getting the behavior the given method.

            \sa \ref ss_query, detail::ReturnMatchBuilder<F, detail::QueryPolicy>                
        */
        template <class F>
        detail::ReturnMatchBuilder<F, detail::QueryPolicy> query(F method)
        {
            typedef int ItIsNotPointerToMemberMethod[
                detail::IsPointerToMethod<F>::Result ? 1 : -1];

            detail::DynamicFunction* function = getDynamicObject()->bind(method);

            return detail::ReturnMatchBuilder<F, detail::QueryPolicy>(createMockFunction(function));
        }

        //! Create the \ref ss_query binding for destructor
        /*!
            This function create the \ref ss_query binding for destructor. The \ref ss_query is used for
            getting the property of mocking function (Like the number calls)
            To binding a function to destructor, using the Destructor() trait object.

            \return
                The builder object for getting the behavior the given method.

            \sa \ref ss_query, detail::ReturnMatchBuilder<F, detail::QueryPolicy>                
        */
        detail::ReturnMatchBuilder<void (T::*)(void*), detail::QueryPolicy> query(const Destructor&)
        {
            typedef void (T::*TDestructorType)(void*);               
            
            detail::DynamicFunction* function = getDynamicObject()->template bindDestructor<T>();
            
            return detail::ReturnMatchBuilder<TDestructorType, detail::QueryPolicy>(createMockFunction(function));            
        }
        
    private:


    };
}


#endif //__AMOP_MOCKOBJECT_HH

