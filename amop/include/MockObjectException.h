#ifndef __AMOP_MOCKOBJECTEXECEPTION__HH
#define __AMOP_MOCKOBJECTEXECEPTION__HH

#include <stdexcept>
#include "Any.h"

namespace amop
{

    //------------------------------------------------------------------
    class TNotImplementedException : public std::runtime_error        
    {
    public:
        TNotImplementedException()
            : std::runtime_error("Not Implemented")
        {
        }
    };

    //------------------------------------------------------------------
    class TNotPureVirtualException : public std::runtime_error        
    {
    public:        
        TNotPureVirtualException()
            : std::runtime_error("It is not a pure virtual function")
        {
        }
    };

    //------------------------------------------------------------------
    class TNotEqualException : public std::runtime_error
    {
    public:
        TNotEqualException(size_t param, const any& expect, const any& actual) 
            : std::runtime_error("Not Equal")
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
    class TCallCountException : public std::runtime_error
    {
    public:
        TCallCountException(size_t expect, size_t actual) 
            : std::runtime_error("Call Count Fail")
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
