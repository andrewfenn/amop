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

        template <class F>
        TReturnMatchBuilder<F> Method(F method)
        {
            typedef int ItIsNotPointerToMemberMethod[
                Detail::IsPointerToMethod<F>::Result ? 1 : -1];

            return TReturnMatchBuilder<F>(this, mDynamicObject->Bind(method));
        }

        TReturnMatchBuilder<void (T::*)(void*)> Method(const Destructor&)
        {
            typedef void (T::*TDestructorType)(void*);               
            
            return TReturnMatchBuilder<TDestructorType>(this, 
                mDynamicObject->BindDestructor<T>() );
        }
    private:


    };
}


#endif //__AMOP_MOCKOBJECT_HH

