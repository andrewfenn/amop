#ifndef __AMOP_CHECKOFFSETFUNC_HH
#define __AMOP_CHECKOFFSETFUNC_HH

#include "MockObjectException.h"
#include "Config.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

namespace amop
{

namespace Detail
{

//------------------------------------------------------------------
namespace Inner
{
    class TCheck 
    {
    public:
        static bool mIsCheckCall;
        static size_t mDestructorOffset;

        typedef bool* TCheckType;        
    };

    namespace TCheckOffset
    {
        //------------------------------------------------------------------
        typedef int (TCheck::*TCheckFuncPtr) (TCheck::TCheckType&);


        

        //------------------------------------------------------------------
        template<int I>
        class TCheckIdx
        {
        public:
            int Func( TCheck::TCheckType& checkCode) 
            { 
                checkCode = &TCheck::mIsCheckCall; 
                return I; 
            }

            void Destructor( void* )
            {
                TCheck::mDestructorOffset = I;
            }
        };

        //------------------------------------------------------------------
        TCheck* CreateCheckObject();

        //------------------------------------------------------------------
        TCheck* CreateCheckDestroyObject();        

        //------------------------------------------------------------------
        template <class T>
        static size_t GetOffset(T memFuncPtr)
        {
            Inner::TCheckOffset::TCheckFuncPtr check = 
                reinterpret_cast<Inner::TCheckOffset::TCheckFuncPtr>(memFuncPtr);

            TCheck::mIsCheckCall = false; 
            TCheck* checkObject = CreateCheckObject();

            TCheck::TCheckType checkCode = 0;
            size_t offset = (checkObject->*check)(checkCode);	

            if(&TCheck::mIsCheckCall != checkCode)
            {
                throw TNotPureVirtualException();
            }

            return offset;
        }

        //------------------------------------------------------------------
        template <class T>
        static size_t GetOffsetDestructor()
        {
            TCheck::mDestructorOffset = 0xFFFFFFFF;
            
            TCheck* checkObject = CreateCheckDestroyObject();

            // Force delete it..
            delete (T*)checkObject;

            if(0xFFFFFFFF == TCheck::mDestructorOffset)
            {
                throw TNotPureVirtualException();
            }

            return TCheck::mDestructorOffset;
        }
            

    	
    };

}

}

}

#endif //__AMOP_CHECKOFFSETFUNC_HH

