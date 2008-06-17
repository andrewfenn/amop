#ifndef __AMOP_MOCKOBJECTEXECEPTION__HH
#define __AMOP_MOCKOBJECTEXECEPTION__HH

#include <exception>
#include "Any.h"

namespace amop
{

    //------------------------------------------------------------------
    class TNotImplementedException : public std::exception        
    {
    public:
        TNotImplementedException()
            : std::exception("Not Implemented")
        {
        }
    };

    //------------------------------------------------------------------
    class TNotPureVirtualException : public std::exception        
    {
    public:        
        TNotPureVirtualException()
            : std::exception("It is not a pure virtual function")
        {
        }
    };

    //------------------------------------------------------------------
    class TNotEqualException : public std::exception
    {
    public:
        TNotEqualException(size_t param, const any& expect, const any& actual) 
            : std::exception("Not Equal")
            , mParam(param)
            , mExpect(expect)
            , mActual(actual)
          {
          }

          any& GetExpect() { return mExpect; }
          any& GetActual() { return mActual; }

          size_t GetParam() { return mParam; }

    protected:
        size_t mParam;
        any mExpect;
        any mActual;

    };

    //------------------------------------------------------------------
    class TCallCountException : public std::exception
    {
    public:
        TCallCountException(size_t expect, size_t actual) 
            : std::exception("Call Count Fail")
            , mExpect(expect)
            , mActual(actual)
          {
          };

          size_t GetExpect() { return mExpect; }
          size_t GetActual() { return mActual; }

    protected:
        size_t mExpect;
        size_t mActual;
    };

}

#endif //__AMOP_MOCKOBJECTEXECEPTION__HH
