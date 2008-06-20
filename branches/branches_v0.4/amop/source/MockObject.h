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

            if(Detail::TDynamicFunction* func = GetDynamicObject()->GetBinded(method))
            {
                return TReturnMatchBuilder<F>(GetMockFunction(func));
            }
            else
            {
                std::pair<Detail::IMockFunction*, Detail::IDynamicFunctionHandler*> ptr = CreateMockFunction();
                
                GetDynamicObject()->Bind(method, ptr.second);

                return TReturnMatchBuilder<F>(ptr.first);
            }
        }

        TReturnMatchBuilder<void (T::*)(void*)> Method(const Destructor&)
        {
            typedef void (T::*TDestructorType)(void*);               
            
            if(Detail::TDynamicFunction* func = GetDynamicObject()->GetBindedDestructor<T>())
            {
                return TReturnMatchBuilder<TDestructorType>(GetMockFunction(func));
            }
            else
            {
                std::pair<Detail::IMockFunction*, Detail::IDynamicFunctionHandler*> ptr = CreateMockFunction();
                
                GetDynamicObject()->BindDestructor<T>(ptr.second);

                return TReturnMatchBuilder<TDestructorType>(ptr.first);
            }
        }
    private:


    };
}


#endif //__AMOP_MOCKOBJECT_HH

