#ifndef __AMOP_CHECKOFFSETFUNC_HH
#define __AMOP_CHECKOFFSETFUNC_HH

#include "MockObjectException.h"
#include "Config.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

namespace amop
{

namespace detail
{

//------------------------------------------------------------------
namespace inner
{
    class Check 
    {
    public:
        static bool m_isCheckCall;
        static size_t m_destructorOffset;

        typedef bool* CheckType;        
    };

    namespace offset
    {
        //------------------------------------------------------------------
        typedef int (Check::*CheckFuncPtr) (Check::CheckType&);        

        //------------------------------------------------------------------
        template<int I>
        class CheckIdx
        {
        public:
            int func( Check::CheckType& checkCode) 
            { 
                checkCode = &Check::m_isCheckCall; 
                return I; 
            }

            void destructor( void* )
            {
                Check::m_destructorOffset = I;
            }
        };

        //------------------------------------------------------------------
        Check* createCheckObject();

        //------------------------------------------------------------------
        Check* createCheckDestroyObject();        

        //------------------------------------------------------------------
        template <class T>
        static size_t getOffset(T memFuncPtr)
        {
            inner::offset::CheckFuncPtr check = 
                reinterpret_cast<inner::offset::CheckFuncPtr>(memFuncPtr);

            Check::m_isCheckCall = false; 
            Check* checkObject = createCheckObject();

            Check::CheckType checkCode = 0;
            size_t offset = (checkObject->*check)(checkCode);	

            if(&Check::m_isCheckCall != checkCode)
            {
                throw NotPureVirtualException();
            }

            return offset;
        }

        //------------------------------------------------------------------
        template <class T>
        static size_t getOffsetDestructor()
        {
            Check::m_destructorOffset = 0xFFFFFFFF;
            
            Check* checkObject = createCheckDestroyObject();

            // Force delete it..
            delete (T*)checkObject;

            if(0xFFFFFFFF == Check::m_destructorOffset)
            {
                throw NotPureVirtualException();
            }

            return Check::m_destructorOffset;
        }
            

    	
    };

}

}

}

#endif //__AMOP_CHECKOFFSETFUNC_HH

