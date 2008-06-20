#ifndef __AMOP_RETURNMATCHBUILDER_HH
#define __AMOP_RETURNMATCHBUILDER_HH


#include "Functor.h"
#include "Comparable.h"
#include "Policy.h"
#include "MockFunction.h"
#include "ExceptionThrower.h"

#include <utility>

#ifdef __GNUC__
#include <tr1/memory>
#endif 

namespace amop
{
    template <typename F, typename Policy>
    class TReturnMatchBuilder;

    class TSinglePolicy{};
    class TAllPolicy{};
    class TQueryPolicy{};
    class TRedirectPolicy{};      

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
    class TReturnMatchBuilder<F, TRedirectPolicy>
        : public TReturnMatchBuilderBase<F, TQueryPolicy>
    {
    public:
        TReturnMatchBuilder(
            Detail::IMockFunction* function
            ) 
            : TReturnMatchBuilderBase(function)
        {
        }        
        
        template <typename T>
        TReturnMatchBuilder<F,TRedirectPolicy> Do(T freeFunc)
        {
            typename Detail::Functor<F>::FunctorType functor(freeFunc);

            mFunction->SetRedirect(functor);

            return TReturnMatchBuilder<F,TRedirectPolicy>(mFunction);
        }

        template <typename C, typename T>
        TReturnMatchBuilder<F,TRedirectPolicy> Do(C* pointer, T func)
        {
            typename Detail::Functor<F>::FunctorType functor(pointer, func);

            mFunction->SetRedirect(functor);

            return TReturnMatchBuilder<F,TRedirectPolicy>(mFunction);
        }
    };

    //------------------------------------------------------------------
    template<typename F>
    class TReturnMatchBuilder<F, TQueryPolicy>
        : public TReturnMatchBuilderBase<F, TQueryPolicy>
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
    class TReturnMatchBuilder<F, TSinglePolicy>
        : public TReturnMatchBuilderBase<F, TSinglePolicy>
    {
    public:
        TReturnMatchBuilder(
            Detail::IMockFunction* function
            ) 
            : TReturnMatchBuilderBase(function)

        {
        }   	

        template<class T>
        TReturnMatchBuilder Wills(T result)
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
        TReturnMatchBuilder Expects(T expect)
        {
            typedef typename Detail::Selector<
                Detail::IsConvertible<T, Policy::TPolicy>::Result
                , SetPolicy<T>
                , SetNormal
            >::Type Setter;

            mFunction->SetExpect(I, Setter::template Make<I, T, true>(expect), false);
            return *this;
        }    

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
    };


    //------------------------------------------------------------------
    template <typename F>
    class TReturnMatchBuilder<F, TAllPolicy>
        : public TReturnMatchBuilderBase<F, TAllPolicy>
    {
    public:
        TReturnMatchBuilder(
            Detail::IMockFunction* function
            ) 
            : TReturnMatchBuilderBase(function)

        {
        }   	

        template<class T>
        TReturnMatchBuilder Will(T result)
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
        TReturnMatchBuilder Expect(T expect)
        {
            typedef typename Detail::Selector<
                Detail::IsConvertible<T, Policy::TPolicy>::Result
                , SetPolicy<T>
                , SetNormal
            >::Type Setter;

            mFunction->SetExpect(I, Setter::template Make<I, T, true>(expect), true);
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

}


//Local Variables:
//c-basic-offset: 4
//End:
#endif //__AMOP_RETURNMATCHBUILDER_HH
