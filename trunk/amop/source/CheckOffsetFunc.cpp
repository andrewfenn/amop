#include "CheckOffsetFunc.h"

namespace amop
{

namespace Detail
{
    namespace Inner
    {
    
        namespace TCheckOffset
        {
            //------------------------------------------------------------------
            template<int I>
            static TFunctionAddress GetIndexFuncRecur(size_t i)
            {
                typedef int (TCheckIdx<I>::*TCheckFuncPtr)(TCheck::TCheckType&);

                TCheckFuncPtr _ptr = &TCheckIdx<I>::Func;
                TFunctionAddress p = HorribleCast<void*>(_ptr);

                if(i == I)
                    return p;
                else
                    return GetIndexFuncRecur<I+1>(i);
            }

            //------------------------------------------------------------------
            template<int I>
            static TFunctionAddress GetIndexDestructorRecur(size_t i)
            {
                typedef void (TCheckIdx<I>::*TCheckFuncPtr)(void*);

                TCheckFuncPtr _ptr = &TCheckIdx<I>::Destructor;
                TFunctionAddress p = HorribleCast<void*>(_ptr);

                if(i == I)
                    return p;
                else
                    return GetIndexDestructorRecur<I+1>(i);
            }

            //------------------------------------------------------------------
            static TFunctionAddress GetIndexFunc(size_t i)
            {
                return GetIndexFuncRecur<0>(i);
            }

            //------------------------------------------------------------------
            static TFunctionAddress GetIndexDestructor(size_t i)
            {
                return GetIndexDestructorRecur<0>(i);
            }

            //------------------------------------------------------------------
            TCheck* CreateCheckObject()
            {
                static TFunctionAddress vtable[MAX_NUM_VIRTUAL_FUNCTIONS];
                static TFunctionAddress vtbl = &vtable[0];

                for(size_t i = 0 ; i < MAX_NUM_VIRTUAL_FUNCTIONS; ++i)
                    vtable[i] = Inner::TCheckOffset::GetIndexFunc(i);

                return (TCheck*)&vtbl;
            }

            //------------------------------------------------------------------
            TCheck* CreateCheckDestroyObject()
            {
                static TFunctionAddress vtable[MAX_NUM_VIRTUAL_FUNCTIONS];
                static TFunctionAddress vtbl = &vtable[0];

                for(size_t i = 0 ; i < MAX_NUM_VIRTUAL_FUNCTIONS; ++i)
                    vtable[i] = Inner::TCheckOffset::GetIndexDestructor(i);

                return (TCheck*)&vtbl;
            }

            //------------------------------------------------------------------
    	    template<>
    	    inline TFunctionAddress GetIndexFuncRecur<MAX_NUM_VIRTUAL_FUNCTIONS>(size_t)
    	    {
        	    return 0;
    	    }
    	
    	    //------------------------------------------------------------------
    	    template<>
    	    inline TFunctionAddress GetIndexDestructorRecur<MAX_NUM_VIRTUAL_FUNCTIONS>(size_t)
    	    {
        	    return 0;
    	    }        
        }
    }

    //------------------------------------------------------------------
    bool Inner::TCheck::mIsCheckCall = false;

    size_t Inner::TCheck::mDestructorOffset = 0xFFFFFFFF;
    }
}

