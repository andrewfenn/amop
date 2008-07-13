#include "CheckOffsetFunc.h"

namespace amop
{

namespace detail
{
    namespace inner
    {
        namespace offset
        {
            //------------------------------------------------------------------
            template<int I>
            static FunctionAddress getIndexFuncRecur(size_t i)
            {
                typedef int (CheckIdx<I>::*CheckFuncPtr)(Check::CheckType&);

                CheckFuncPtr ptr = &CheckIdx<I>::func;
                FunctionAddress p = HorribleCast<void*>(ptr);

                if(i == I)
                    return p;
                else
                    return getIndexFuncRecur<I+1>(i);
            }

            //------------------------------------------------------------------
            template<int I>
            static FunctionAddress getIndexDestructorRecur(size_t i)
            {
                typedef void (CheckIdx<I>::*CheckFuncPtr)(void*);

                CheckFuncPtr ptr = &CheckIdx<I>::destructor;
                FunctionAddress p = HorribleCast<void*>(ptr);

                if(i == I)
                    return p;
                else
                    return getIndexDestructorRecur<I+1>(i);
            }

            //------------------------------------------------------------------
            static FunctionAddress getIndexFunc(size_t i)
            {
                return getIndexFuncRecur<0>(i);
            }

            //------------------------------------------------------------------
            static FunctionAddress getIndexDestructor(size_t i)
            {
                return getIndexDestructorRecur<0>(i);
            }

            //------------------------------------------------------------------
            Check* createCheckObject()
            {
                static FunctionAddress vtable[MAX_NUM_VIRTUAL_FUNCTIONS];
                static FunctionAddress vtbl = &vtable[0];

                for(size_t i = 0 ; i < MAX_NUM_VIRTUAL_FUNCTIONS; ++i)
                    vtable[i] = inner::offset::getIndexFunc(i);

                return (Check*)&vtbl;
            }

            //------------------------------------------------------------------
            Check* createCheckDestroyObject()
            {
                static FunctionAddress vtable[MAX_NUM_VIRTUAL_FUNCTIONS];
                static FunctionAddress vtbl = &vtable[0];

                for(size_t i = 0 ; i < MAX_NUM_VIRTUAL_FUNCTIONS; ++i)
                    vtable[i] = inner::offset::getIndexDestructor(i);

                return (Check*)&vtbl;
            }

            //------------------------------------------------------------------
    	    template<>
    	    inline FunctionAddress getIndexFuncRecur<MAX_NUM_VIRTUAL_FUNCTIONS>(size_t)
    	    {
        	    return 0;
    	    }
    	
    	    //------------------------------------------------------------------
    	    template<>
    	    inline FunctionAddress getIndexDestructorRecur<MAX_NUM_VIRTUAL_FUNCTIONS>(size_t)
    	    {
        	    return 0;
    	    }        
        }
    }

    //------------------------------------------------------------------
    bool inner::Check::m_isCheckCall = false;

    size_t inner::Check::m_destructorOffset = 0xFFFFFFFF;
    }
}

