#ifndef __AMOP_OBJECTHOLDER_HH
#define __AMOP_OBJECTHOLDER_HH

#include "Any.h"
#include "Functor.h"
#include <cassert>

#ifdef __GNUC__
#include <tr1/memory>
#endif

namespace amop
{
    namespace detail
    {

        class Comparable;

        //------------------------------------------------------------------
        class ObjectHolder
        {
        public:
            ObjectHolder()
            {
            }

            virtual ~ObjectHolder(){}

            template <class R>
            R returning(size_t idx)
            {                                                
                return any_cast<R>(getReturn(idx));
            }

            template <class T>
            void actual(size_t funcIdx, size_t idx
                , typename detail::RemoveReference<T>::ConstType & t)
            {
                setActual(funcIdx, idx, &t);
            }

            template <class F>
            typename detail::Functor<F>::FunctorType redirector(size_t i)
            {
                any& redirector = getRedirect(i);
                typedef typename detail::Functor<F>::FunctorType FunctorType;

                if(!redirector.empty())
                {
                    return any_cast<FunctorType>(redirector);
                }

                return FunctorType();
            }

            virtual any& getRedirect(size_t idx) = 0;
            
            virtual any& getReturn(size_t idx) = 0;
            virtual void setActual(size_t idx, size_t paramId, const any& param) = 0;	

            virtual void addCallCounter(size_t idx) = 0;	
        };

    }

}

//Local Variables:
//c-basic-offset: 4
//End:

#endif //__AMOP_OBJECTHOLDER_HH

