#ifndef __AMOP_CHECKOFFSETFUNC_HH
#define __AMOP_CHECKOFFSETFUNC_HH

#include "Config.h"
#include "MockObjectException.h"
#include "Functor.h"

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
#ifdef DETAIL_HAVE_THIS_CALL        
        typedef int (__thiscall Check::*CheckFuncPtrThisCall) (Check::CheckType&);        
#endif

        typedef int (DETAIL_CDECL Check::*CheckFuncPtrCdecl) (Check::CheckType&);        

        //------------------------------------------------------------------
        template<int I>
        class CheckIdx
        {
        public:
#ifdef DETAIL_HAVE_THIS_CALL        
            int __thiscall funcThisCall( Check::CheckType& checkCode) 
            { 
                checkCode = &Check::m_isCheckCall; 
                return I; 
            }
#endif
            int DETAIL_CDECL funcCDecl( Check::CheckType& checkCode) 
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
        Check* createCheckObject(AmopCallingConventionType type);

        //------------------------------------------------------------------
        Check* createCheckDestroyObject();        

        //------------------------------------------------------------------
        template <class T>
        static size_t getOffset(T memFuncPtr)
        {
            Check::m_isCheckCall = false;             
            Check::CheckType checkCode = 0;
            size_t offset = 0;
            
            

            if (Functor<T>::CallingConvention == (int)AMOP_CDECL)
            {
                Check* checkObject = createCheckObject(AMOP_CDECL);
                
                inner::offset::CheckFuncPtrCdecl check =
                    reinterpret_cast<inner::offset::CheckFuncPtrCdecl>(memFuncPtr);
                
                offset = (checkObject->*check)(checkCode);	
            }
#ifdef DETAIL_HAVE_THIS_CALL
            else
            {                                
                Check* checkObject = createCheckObject(AMOP_THISCALL);
                
                inner::offset::CheckFuncPtrThisCall check =
                    reinterpret_cast<inner::offset::CheckFuncPtrThisCall>(memFuncPtr);

                offset = (checkObject->*check)(checkCode);	
            }
#endif

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

