#ifndef __AMOP_RETURNMATCHBUILDER_HH
#define __AMOP_RETURNMATCHBUILDER_HH


#include "Functor.h"
#include "Comparable.h"
#include "Policy.h"
#include "MockFunction.h"
#include "ExceptionThrower.h"
#include "ExpectsMaker.h"

#include <utility>

#ifdef __GNUC__
#include <tr1/memory>
#endif 

namespace amop
{    
    namespace detail
    {
        class CallPolicy{};
        class EveryCallPolicy{};
        class QueryPolicy{};        

#define DETAIL_APPLY_ALL(action, param) \
    action<0>(param.p0); \
    action<1>(param.p1); \
    action<2>(param.p2); \
    action<3>(param.p3); \
    action<4>(param.p4); \
    action<5>(param.p5); \
    action<6>(param.p6); \
    action<7>(param.p7); \

    template <typename F, typename Policy>
    class ReturnMatchBuilder;

        template <class F>
        struct SetNormal
        {
            template<int I, class T, bool ReadOnly>
            static detail::Comparable make(T result)
            {
                typedef typename detail::Get< typename detail::Functor<F>::ParameterTypes, I>::Type ToTypeRef;
                typedef typename detail::RemoveReference<ToTypeRef>::Type ToType;

                //****************************************/
                //** If you see this, that's mean your  **/
                //** expect type is not convertiable to **/
                //** actual type.						**/
                //****************************************/
                typedef int ItIsNotConvertible[
                    detail::IsConvertible<T, ToType>::Result ? 1 : -1];
                    return detail::Comparable::makeCompare<ToType, ReadOnly>(result);
            }
        };

        template <class P, class F>
        struct SetPolicy
        {
            template<int I, class T, bool ReadOnly>
            static detail::Comparable make(P policy)
            {
                typedef typename detail::Get< typename detail::Functor<F>::ParameterTypes, I>::Type ToTypeRef;
                typedef typename detail::RemoveReference<ToTypeRef>::Type ToType;

                return detail::Comparable::makeCompareByPolicy<ToType, ReadOnly>(policy);
            }
        };


    //------------------------------------------------------------------
    class ReturnMatchBuilderBase
    {
    public:
        ReturnMatchBuilderBase(
            detail::MockFunction* function
            ) 
            : m_function(function)
        {
        }   	        


    protected:
        detail::MockFunction* m_function;
    };
    

    //! The builder object for Query mode 
    /*!
        This template class defined the query mode return object from 
        \ref MockObject::query.
    */
    //------------------------------------------------------------------
    template<typename F>
    class ReturnMatchBuilder<F, detail::QueryPolicy>
        : public ReturnMatchBuilderBase
    {
    public:
        typedef ReturnMatchBuilderBase Base;


    public:
        ReturnMatchBuilder(
            detail::MockFunction* function
            ) 
            : Base(function)
        {
        }   	

        //! Get the number of calls this method called
        /*!
            \return
                The number of calls this method called
        */
            
        size_t count()
        {
            return Base::m_function->getCallCounter();
        }
    };    
    
    //! The builder object for call mode 
    /*!
        This template class defined the call mode return object from 
        \ref MockObject::call.
    */
    //------------------------------------------------------------------
    template <typename F>
    class ReturnMatchBuilder<F, detail::CallPolicy>
        : public ReturnMatchBuilderBase
        , public detail::ExpectMaker<F, detail::Length< typename detail::Functor<F>::ParameterTypes >::Value, 
            ReturnMatchBuilder<F, detail::CallPolicy> 
            >
    {
    public:
        typedef ReturnMatchBuilderBase Base;

    public:
        typedef ExpectMaker<F, detail::Length< typename detail::Functor<F>::ParameterTypes >::Value , 
            ReturnMatchBuilder<F, detail::CallPolicy> 
            >
        ExpectMakersType;                
        
        ReturnMatchBuilder(
            detail::MockFunction* function
            ) 
            : Base(function)            
        {            
        }   	

        //! Redirect the method to the given free function
        /*!
            Redirect all call to the given free function,
            When you did that, all expectations you set will not affected.

            \param freeFunc
                The given free function for redirecting

            \return The match builder of current method
        */
        template <typename T>
        ReturnMatchBuilder doing(T freeFunc)
        {
            typename detail::Functor<F>::FunctorType functor(freeFunc);

            Base::m_function->setRedirect(functor, false);

            return *this;
        }

        //! Redirect the method to the given bounded function
        /*!
            Redirect all call to the given bounded function,
            When you did that, all expectations you set will not affected.

            \param pointer
                The object pointer of the function you want to bound

            \param func
                The member function you want to redirect to

            \return The match builder of current method
        */
        template <typename C, typename T>
        ReturnMatchBuilder doing(C* pointer, T func)
        {
            typename detail::Functor<F>::FunctorType functor(pointer, func);

            Base::m_function->setRedirect(functor, false);

            return *this;
        }

        //! Setting the return value of this method
        /*!
            Set the return value when this method was called.
            The return value should be copyable.

            \param result
                The return value it should return

            \return The match builder of current method
        */
        template<class T>
        ReturnMatchBuilder returning(T result)
        {
            typedef typename detail::RemoveReference<typename detail::Functor<F>::ReturnType>::Type ToType;

            //****************************************/
            //** If you see this, that's mean your  **/
            //** expect type is not convertiable to **/
            //** actual type.						**/
            //****************************************/
            typedef int ItIsNotConvertible[
                detail::IsConvertible<T, ToType>::Result ? 1 : -1];

                Base::m_function->setReturn(std::make_pair(false,(ToType)result), false);
                return *this;
        }    

        //! Set this method will thrown the given exception
        /*!
            Set the exception when this method is called.
            The excpetion should be copyable.

            \param exception
                The exception to throw

            \return The match builder of current method
        */
        template<class T>
        ReturnMatchBuilder throwing(T exception)
        {
            //Can we verify that it is convertible to throw specifier.
            Base::m_function->setReturn(std::make_pair(true,AmopSharedPtr<detail::ExceptionThrowerBase>(new detail::ExceptionThrower<T>(exception))), false);
            return *this;
        }

        //! Set the single expect value by given its index
        /*!
            Set the single expect value by given its index, for example:

            \code
                class Interface
                {
                public:
                    virtual void Foo(int a, int b) = 0;
                };
                
                mock.EveryCall(&Interface::Foo).expectOne<0>(100).expectOne<1>(200);

            \endcode

            Then the expect value of Foo will be 100 for a, 200 for b.

            \param expect
                The expect value

            \return The match builder of current method
        */
        template<int I, class T>
        ReturnMatchBuilder expectOne(T expect)
        {
            typedef typename detail::Selector<
                detail::IsConvertible<T, policy::Policy>::Result
                , SetPolicy<T, F>
                , SetNormal<F>
            >::Type Setter;

            Base::m_function->setExpect(I, Setter::template make<I, T, true>(expect), false);
            return *this;
        }    

        /*! 
            \fn expect
            \brief Set multiple expect values
        */

        template<int I>
        void expectOne(detail::Empty)
        {
            Base::m_function->setExpect(I, detail::Comparable(), false);
        };                
        
        //! Set the incoming parameter by given its index
        /*!
            Set the single incoming value by given its index, for example:

            \code
                class Interface
                {
                public:
                    virtual void Foo(int& a, int& b) = 0;
                };
                
                mock.EveryCall(&Interface::Foo).setOne<0>(100).setOne<1>(200);

                int test_a = 0;
                int test_b = 0;

                ((Interface*)mock)->Foo(test_a, test_b);

                CHECK(100, test_a);
                CHECK(200, test_b);               
            \endcode

            \param result
                The value to set 

            \return The match builder of current method
        */
        template<int I, class T>
        ReturnMatchBuilder setOne(T result)
        {
            typedef typename detail::Selector<
                detail::IsConvertible<T, policy::Policy>::Result
                , SetPolicy<T, F>
                , SetNormal<F>
            >::Type Setter;

            Base::m_function->setSetter(I, Setter::template make<I, T, false>(result), false);
            return *this;
        }                        

        template<DETAIL_TPARAMS(8)>
        ReturnMatchBuilder expectInternal(const detail::ExpectAll<DETAIL_ARGS(8)>& expect)
        {
            DETAIL_APPLY_ALL(expectOne, expect);
            return *this;
        }

    };


    //! The builder object for EveryCall mode 
    /*!
        This template class defined the every call mode return object from 
        \ref MockObject::everyCall.
    */
    //------------------------------------------------------------------
    template <typename F>
    class ReturnMatchBuilder<F, detail::EveryCallPolicy>
        : public ReturnMatchBuilderBase
        , public detail::ExpectMaker<F, detail::Length< typename detail::Functor<F>::ParameterTypes >::Value ,
            ReturnMatchBuilder<F, detail::EveryCallPolicy> 
            >
    {
    public:
        typedef ReturnMatchBuilderBase Base;


    public:
        ReturnMatchBuilder(
            detail::MockFunction* function
            ) 
            : Base(function)

        {
        }   	


        //! Redirect the method to the given free function
        /*!
            Redirect all call to the given free function,
            When you did that, all expectations you set will not affected.

            \param freeFunc
                The given free function for redirecting

            \return The match builder of current method
        */
        template <typename T>
        ReturnMatchBuilder doing(T freeFunc)
        {
            typename detail::Functor<F>::FunctorType functor(freeFunc);

            Base::m_function->setRedirect(functor, true);

            return *this;
        }

        //! Redirect the method to the given bounded function
        /*!
            Redirect all call to the given bounded function,
            When you did that, all expectations you set will not affected.

            \param pointer
                The object pointer of the function you want to bound

            \param func
                The member function you want to redirect to

            \return The match builder of current method
        */
        template <typename C, typename T>
        ReturnMatchBuilder doing(C* pointer, T func)
        {
            typename detail::Functor<F>::FunctorType functor(pointer, func);

            Base::m_function->setRedirect(functor, true);

            return *this;
        }

        //! Setting the return value of this method
        /*!
            Set the return value when this method was called.
            The return value should be copyable.

            \param result
                The return value it should return

            \return The match builder of current method
        */
        template<class T>
        ReturnMatchBuilder returning(T result)
        {
            typedef typename detail::Functor<F>::ReturnType R;
            typedef typename detail::RemoveReference<R>::Type ToType;

            //****************************************/
            //** If you see this, that's mean your  **/
            //** expect type is not convertiable to **/
            //** actual type.						**/
            //****************************************/
            typedef int ItIsNotConvertible[
                detail::IsConvertible<T, ToType>::Result ? 1 : -1];

                Base::m_function->setReturn(std::make_pair(false,(ToType)result), true);
                return *this;
        }

        //! Set this method will thrown the given exception
        /*!
            Set the exception when this method is called.
            The excpetion should be copyable.

            \param exception
                The exception to throw

            \return The match builder of current method
        */
        template<class T>
        ReturnMatchBuilder throwing(T exception)
        {
            //Can we verify that it is convertible to throw specifier.
            Base::m_function->setReturn(std::make_pair(true,AmopSharedPtr<detail::ExceptionThrowerBase>(new detail::ExceptionThrower<T>(exception))), true);
            return *this;
        }

        //! Set the single expect value by given its index
        /*!
            Set the single expect value by given its index, for example:

            \code
                class Interface
                {
                public:
                    virtual void Foo(int a, int b) = 0;
                };
                
                mock.EveryCall(&Interface::Foo).expectOne<0>(100).expectOne<1>(200);

            \endcode

            Then the expect value of Foo will be 100 for a, 200 for b.

            \param expect
                The expect value

            \return The match builder of current method
        */
        template<int I, class T>
        ReturnMatchBuilder expectOne(T expect)
        {
            typedef typename detail::Selector<
                detail::IsConvertible<T, policy::Policy>::Result
                , SetPolicy<T, F>
                , SetNormal<F>
            >::Type Setter;

            detail::Comparable c = Setter::template make<I, T, true>(expect);
            
            Base::m_function->setExpect(I, c, true);
            return *this;
        }

        template<int I>
        void expectOne(detail::Empty)
        {
            Base::m_function->setExpect(I, detail::Comparable() , true);
        }

        template<DETAIL_TPARAMS(8)>
        ReturnMatchBuilder expectInternal(const detail::ExpectAll<DETAIL_ARGS(8)>& expect)
        {
            DETAIL_APPLY_ALL(expectOne, expect);
            return *this;
        }

        //! Set the incoming parameter by given its index
        /*!
            Set the single incoming value by given its index, for example:

            \code
                class Interface
                {
                public:
                    virtual void Foo(int& a, int& b) = 0;
                };
                
                mock.EveryCall(&Interface::Foo).setOne<0>(100).setOne<1>(200);

                int test_a = 0;
                int test_b = 0;

                ((Interface*)mock)->Foo(test_a, test_b);

                CHECK(100, test_a);
                CHECK(200, test_b);               
            \endcode

            \param result
                The value to set 

            \return The match builder of current method
        */        
        template<int I, class T>
        ReturnMatchBuilder setOne(T result)
        {
            typedef typename detail::Selector<
                detail::IsConvertible<T, policy::Policy>::Result
                , SetPolicy<T,F>
                , SetNormal<F>
            >::Type Setter;

            Base::m_function->setSetter(I, Setter::template make<I, T, false>(result), true);
            return *this;
        }

        //! Set the expect call counter
        /*!
            \param counter
                the number of call this method should be called
                

            \return The match builder of current method
        */
        void count(size_t counter)
        {
            Base::m_function->setExpectCallCounter(counter);
        }    
    };
}

    static detail::Empty ignore()
    {
        return detail::Empty();
    }

}


//Local Variables:
//c-basic-offset: 4
//End:
#endif //__AMOP_RETURNMATCHBUILDER_HH
