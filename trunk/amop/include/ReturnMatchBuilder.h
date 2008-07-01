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
    namespace Detail
    {
        class TCallPolicy{};
        class TEveryCallPolicy{};
        class TQueryPolicy{};
        class TRedirectPolicy{};      

#define DETAIL_APPLY_ALL(Action, param) \
    Action<0>(param.p0); \
    Action<1>(param.p1); \
    Action<2>(param.p2); \
    Action<3>(param.p3); \
    Action<4>(param.p4); \
    Action<5>(param.p5); \
    Action<6>(param.p6); \
    Action<7>(param.p7); \

    template <typename F, typename Policy>
    class TReturnMatchBuilder;

        template <class F>
        struct SetNormal
        {
            template<int I, class T, bool ReadOnly>
            static Detail::TComparable Make(T result)
            {
                typedef typename Detail::Get< typename Detail::Functor<F>::ParameterTypes, I>::Type ToTypeRef;
                typedef typename Detail::RemoveReference<ToTypeRef>::Type ToType;

                //****************************************/
                //** If you see this, that's mean your  **/
                //** expect type is not convertiable to **/
                //** actual type.						**/
                //****************************************/
                typedef int ItIsNotConvertible[
                    Detail::IsConvertible<T, ToType>::Result ? 1 : -1];
                    return Detail::TComparable::MakeCompare<ToType, ReadOnly>(result);
            }
        };

        template <class P, class F>
        struct SetPolicy
        {
            template<int I, class T, bool ReadOnly>
            static Detail::TComparable Make(P policy)
            {
                typedef typename Detail::Get< typename Detail::Functor<F>::ParameterTypes, I>::Type ToTypeRef;
                typedef typename Detail::RemoveReference<ToTypeRef>::Type ToType;

                return Detail::TComparable::MakeCompareByPolicy<ToType, ReadOnly>(policy);
            }
        };


    //------------------------------------------------------------------
    class TReturnMatchBuilderBase
    {
    public:
        TReturnMatchBuilderBase(
            Detail::IMockFunction* function
            ) 
            : mFunction(function)
        {
        }   	        


    protected:
        Detail::IMockFunction* mFunction;
    };

    //! The builder object for query mode
    /*!
        This template class defined the query mode return object from 
        \ref TMockObject::Query
    */
    //------------------------------------------------------------------
    template<typename F>
    class TReturnMatchBuilder<F, Detail::TRedirectPolicy>
        : public TReturnMatchBuilderBase
    {
    public:
        typedef TReturnMatchBuilderBase TBase;

    public:
        TReturnMatchBuilder(
            Detail::IMockFunction* function
            ) 
            : TReturnMatchBuilderBase(function)
        {
        }        
        
        template <typename T>
        TReturnMatchBuilder<F,Detail::TRedirectPolicy> Do(T freeFunc)
        {
            typename Detail::Functor<F>::FunctorType functor(freeFunc);

            TBase::mFunction->SetRedirect(functor);

            return TReturnMatchBuilder<F,Detail::TRedirectPolicy>(TBase::mFunction);
        }

        template <typename C, typename T>
        TReturnMatchBuilder<F,Detail::TRedirectPolicy> Do(C* pointer, T func)
        {
            typename Detail::Functor<F>::FunctorType functor(pointer, func);

            TBase::mFunction->SetRedirect(functor);

            return TReturnMatchBuilder<F,Detail::TRedirectPolicy>(TBase::mFunction);
        }
    };

    /*!
    */
    //------------------------------------------------------------------
    template<typename F>
    class TReturnMatchBuilder<F, Detail::TQueryPolicy>
        : public TReturnMatchBuilderBase
    {
    public:
        typedef TReturnMatchBuilderBase TBase;


    public:
        TReturnMatchBuilder(
            Detail::IMockFunction* function
            ) 
            : TBase(function)
        {
        }   	

        size_t Count()
        {
            return TBase::mFunction->GetCallCounter();
        }
    };    
    
    /*!
    */
    //------------------------------------------------------------------
    template <typename F>
    class TReturnMatchBuilder<F, Detail::TCallPolicy>
        : public TReturnMatchBuilderBase
        , public Detail::TExpectsMaker<F, Detail::Length< typename Detail::Functor<F>::ParameterTypes >::Value, 
            TReturnMatchBuilder<F, Detail::TCallPolicy> 
            >
    {
    public:
        typedef TReturnMatchBuilderBase TBase;

    public:
        typedef TExpectsMaker<F, Detail::Length< typename Detail::Functor<F>::ParameterTypes >::Value , 
            TReturnMatchBuilder<F, Detail::TCallPolicy> 
            >
        TExpectMakersType;                
        
        TReturnMatchBuilder(
            Detail::IMockFunction* function
            ) 
            : TBase(function)            
        {            
        }   	

        template<class T>
        TReturnMatchBuilder Returns(T result)
        {
            typedef typename Detail::RemoveReference<typename Detail::Functor<F>::ReturnType>::Type ToType;

            //****************************************/
            //** If you see this, that's mean your  **/
            //** expect type is not convertiable to **/
            //** actual type.						**/
            //****************************************/
            typedef int ItIsNotConvertible[
                Detail::IsConvertible<T, ToType>::Result ? 1 : -1];

                TBase::mFunction->SetReturn(std::make_pair(false,(ToType)result), false);
                return *this;
        }    

        template<class T>
        TReturnMatchBuilder Throws(T exception)
        {
            //Can we verify that it is convertible to throw specifier.
            TBase::mFunction->SetReturn(std::make_pair(true,AmopSharedPtr<Detail::ExceptionThrowerBase>(new Detail::ExceptionThrower<T>(exception))), false);
            return *this;
        }

        template<int I, class T>
        TReturnMatchBuilder ExpectOne(T expect)
        {
            typedef typename Detail::Selector<
                Detail::IsConvertible<T, Policy::TPolicy>::Result
                , SetPolicy<T, F>
                , SetNormal<F>
            >::Type Setter;

            TBase::mFunction->SetExpect(I, Setter::template Make<I, T, true>(expect), false);
            return *this;
        }    

        template<int I>
        void ExpectOne(Detail::Empty)
        {
            TBase::mFunction->SetExpect(I, Detail::TComparable(), false);
        };                
        
        template<int I, class T>
        TReturnMatchBuilder Sets(T result)
        {
            typedef typename Detail::Selector<
                Detail::IsConvertible<T, Policy::TPolicy>::Result
                , SetPolicy<T, F>
                , SetNormal<F>
            >::Type Setter;

            TBase::mFunction->SetSetter(I, Setter::template Make<I, T, false>(result), false);
            return *this;
        }                        

        template<DETAIL_TPARAMS(8)>
        TReturnMatchBuilder _Expects(const Detail::TExpectsAll<DETAIL_ARGS(8)>& expect)
        {
            DETAIL_APPLY_ALL(ExpectOne, expect);
            return *this;
        }

    };


    /*!
    */
    //------------------------------------------------------------------
    template <typename F>
    class TReturnMatchBuilder<F, Detail::TEveryCallPolicy>
        : public TReturnMatchBuilderBase
        , public Detail::TExpectsMaker<F, Detail::Length< typename Detail::Functor<F>::ParameterTypes >::Value ,
            TReturnMatchBuilder<F, Detail::TEveryCallPolicy> 
            >
    {
    public:
        typedef TReturnMatchBuilderBase TBase;


    public:
        TReturnMatchBuilder(
            Detail::IMockFunction* function
            ) 
            : TBase(function)

        {
        }   	

        template<class T>
        TReturnMatchBuilder Return(T result)
        {
            typedef typename Detail::Functor<F>::ReturnType R;
            typedef typename Detail::RemoveReference<R>::Type ToType;

            //****************************************/
            //** If you see this, that's mean your  **/
            //** expect type is not convertiable to **/
            //** actual type.						**/
            //****************************************/
            typedef int ItIsNotConvertible[
                Detail::IsConvertible<T, ToType>::Result ? 1 : -1];

                TBase::mFunction->SetReturn(std::make_pair(false,(ToType)result), true);
                return *this;
        }

        template<class T>
        TReturnMatchBuilder Throw(T exception)
        {
            //Can we verify that it is convertible to throw specifier.
            TBase::mFunction->SetReturn(std::make_pair(true,AmopSharedPtr<Detail::ExceptionThrowerBase>(new Detail::ExceptionThrower<T>(exception))), true);
            return *this;
        }

        template<int I, class T>
        TReturnMatchBuilder ExpectOne(T expect)
        {
            typedef typename Detail::Selector<
                Detail::IsConvertible<T, Policy::TPolicy>::Result
                , SetPolicy<T, F>
                , SetNormal<F>
            >::Type Setter;

            Detail::TComparable c = Setter::template Make<I, T, true>(expect);
            
            TBase::mFunction->SetExpect(I, c, true);
            return *this;
        }

        template<int I>
        void ExpectOne(Detail::Empty)
        {
            TBase::mFunction->SetExpect(I, Detail::TComparable() , true);
        }

        template<DETAIL_TPARAMS(8)>
        TReturnMatchBuilder _Expects(const Detail::TExpectsAll<DETAIL_ARGS(8)>& expect)
        {
            DETAIL_APPLY_ALL(ExpectOne, expect);
            return *this;
        }

        
        template<int I, class T>
        TReturnMatchBuilder Set(T result)
        {
            typedef typename Detail::Selector<
                Detail::IsConvertible<T, Policy::TPolicy>::Result
                , SetPolicy<T,F>
                , SetNormal<F>
            >::Type Setter;

            TBase::mFunction->SetSetter(I, Setter::template Make<I, T, false>(result), true);
            return *this;
        }

        void Count(size_t counter)
        {
            TBase::mFunction->SetExpectCallCounter(counter);
        }    
    };
}

    static Detail::Empty Ignore;
}


//Local Variables:
//c-basic-offset: 4
//End:
#endif //__AMOP_RETURNMATCHBUILDER_HH
