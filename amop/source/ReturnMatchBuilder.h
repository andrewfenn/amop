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

        template <DETAIL_TPARAMS_DEF(8, Empty)> 
        struct TExpectsAll
        {
            TExpectsAll(DETAIL_ARGS_DEFAULT(8))
                : p0(t1)
                , p1(t2)
                , p2(t3)
                , p3(t4)
                , p4(t5)
                , p5(t6)
                , p6(t7)
                , p7(t8)
            {
            }
            
            T1 p0;
            T2 p1;
            T3 p2;
            T4 p3;
            
            T5 p4;
            T6 p5;
            T7 p6;
            T8 p7;
        };
    }

#define DETAIL_ALL_MAKER_BUILD(i)       \
    template <DETAIL_TPARAMS(i)>                                            \
    Detail::TExpectsAll< DETAIL_ARGS(i) > All( DETAIL_FUNC_PARAMS(i,t) )           \
    {                                                                       \
        return Detail::TExpectsAll<DETAIL_ARGS(i)>(DETAIL_ARGS_P(i));              \
    }

DETAIL_ALL_MAKER_BUILD(1);
DETAIL_ALL_MAKER_BUILD(2);
DETAIL_ALL_MAKER_BUILD(3);
DETAIL_ALL_MAKER_BUILD(4);
DETAIL_ALL_MAKER_BUILD(5);
DETAIL_ALL_MAKER_BUILD(6);
DETAIL_ALL_MAKER_BUILD(7);
DETAIL_ALL_MAKER_BUILD(8);

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

    //------------------------------------------------------------------
    template <typename F, typename Policy>
    class TReturnMatchBuilderBase
    {
    public:
        TReturnMatchBuilderBase(
            Detail::IMockFunction* function
            ) 
            : mFunction(function)
        {
        }   	        

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

        template <class P>
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


    protected:
        Detail::IMockFunction* mFunction;
    };

    //------------------------------------------------------------------
    template<typename F>
    class TReturnMatchBuilder<F, Detail::TRedirectPolicy>
        : public TReturnMatchBuilderBase<F, Detail::TQueryPolicy>
    {
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

            mFunction->SetRedirect(functor);

            return TReturnMatchBuilder<F,Detail::TRedirectPolicy>(mFunction);
        }

        template <typename C, typename T>
        TReturnMatchBuilder<F,Detail::TRedirectPolicy> Do(C* pointer, T func)
        {
            typename Detail::Functor<F>::FunctorType functor(pointer, func);

            mFunction->SetRedirect(functor);

            return TReturnMatchBuilder<F,Detail::TRedirectPolicy>(mFunction);
        }
    };

    //------------------------------------------------------------------
    template<typename F>
    class TReturnMatchBuilder<F, Detail::TQueryPolicy>
        : public TReturnMatchBuilderBase<F, Detail::TQueryPolicy>
    {
    public:
        TReturnMatchBuilder(
            Detail::IMockFunction* function
            ) 
            : TReturnMatchBuilderBase(function)
        {
        }   	

        size_t Count()
        {
            return mFunction->GetCallCounter();
        }
    };    
    
    //------------------------------------------------------------------
    template <typename F>
    class TReturnMatchBuilder<F, Detail::TCallPolicy>
        : public TReturnMatchBuilderBase<F, Detail::TCallPolicy>
        , public TExpectsMaker<F, Detail::Length< typename Detail::Functor<F>::ParameterTypes >::Value , 
            typename TReturnMatchBuilder<F, Detail::TCallPolicy> 
            >
    {
    public:
        typedef TExpectsMaker<F, Detail::Length< typename Detail::Functor<F>::ParameterTypes >::Value , 
            typename TReturnMatchBuilder<F, Detail::TCallPolicy> 
            >
        TExpectMakersType;                
        
        TReturnMatchBuilder(
            Detail::IMockFunction* function
            ) 
            : TReturnMatchBuilderBase(function)            
        {
            SetSelf(this);
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

                mFunction->SetReturn(std::make_pair(false,(ToType)result), false);
                return *this;
        }    

        template<class T>
        TReturnMatchBuilder Throws(T exception)
        {
            //Can we verify that it is convertible to throw specifier.
            mFunction->SetReturn(std::make_pair(true,AmopSharedPtr<Detail::ExceptionThrowerBase>(new Detail::ExceptionThrower<T>(exception))), false);
            return *this;
        }

        template<int I, class T>
        TReturnMatchBuilder ExpectOne(T expect)
        {
            typedef typename Detail::Selector<
                Detail::IsConvertible<T, Policy::TPolicy>::Result
                , SetPolicy<T>
                , SetNormal
            >::Type Setter;

            mFunction->SetExpect(I, Setter::template Make<I, T, true>(expect), false);
            return *this;
        }    

        template<int I>
        void ExpectOne(Detail::Empty)
        {
            mFunction->SetExpect(I, Detail::TComparable(), false);
        };                
        
        template<int I, class T>
        TReturnMatchBuilder Sets(T result)
        {
            typedef typename Detail::Selector<
                Detail::IsConvertible<T, Policy::TPolicy>::Result
                , SetPolicy<T>
                , SetNormal
            >::Type Setter;

            mFunction->SetSetter(I, Setter::template Make<I, T, false>(result), false);
            return *this;
        }                        

        template<DETAIL_TPARAMS(8)>
        TReturnMatchBuilder _Expects(const Detail::TExpectsAll<DETAIL_ARGS(8)>& expect)
        {
            DETAIL_APPLY_ALL(ExpectOne, expect);
            return *this;
        }

    };


    //------------------------------------------------------------------
    template <typename F>
    class TReturnMatchBuilder<F, Detail::TEveryCallPolicy>
        : public TReturnMatchBuilderBase<F, Detail::TEveryCallPolicy>
        , public TExpectsMaker<F, Detail::Length< typename Detail::Functor<F>::ParameterTypes >::Value , 
            typename TReturnMatchBuilder<F, Detail::TEveryCallPolicy> 
            >
    {
    public:
        TReturnMatchBuilder(
            Detail::IMockFunction* function
            ) 
            : TReturnMatchBuilderBase(function)

        {
            SetSelf(this);
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

                mFunction->SetReturn(std::make_pair(false,(ToType)result), true);
                return *this;
        }

        template<class T>
        TReturnMatchBuilder Throw(T exception)
        {
            //Can we verify that it is convertible to throw specifier.
            mFunction->SetReturn(std::make_pair(true,AmopSharedPtr<Detail::ExceptionThrowerBase>(new Detail::ExceptionThrower<T>(exception))), true);
            return *this;
        }

        template<int I, class T>
        TReturnMatchBuilder ExpectOne(T expect)
        {
            typedef typename Detail::Selector<
                Detail::IsConvertible<T, Policy::TPolicy>::Result
                , SetPolicy<T>
                , SetNormal
            >::Type Setter;

            mFunction->SetExpect(I, Setter::template Make<I, T, true>(expect), true);
            return *this;
        }

        template<int I>
        void ExpectOne(Detail::Empty)
        {
            mFunction->SetExpect(I, Detail::TComparable() , true);
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
                , SetPolicy<T>
                , SetNormal
            >::Type Setter;

            mFunction->SetSetter(I, Setter::template Make<I, T, false>(result), true);
            return *this;
        }

        void Count(size_t counter)
        {
            mFunction->SetExpectCallCounter(counter);
        }    
    };

    static Detail::Empty Ignore;
}


//Local Variables:
//c-basic-offset: 4
//End:
#endif //__AMOP_RETURNMATCHBUILDER_HH
