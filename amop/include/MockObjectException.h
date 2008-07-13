#ifndef __AMOP_MOCKOBJECTEXECEPTION__HH
#define __AMOP_MOCKOBJECTEXECEPTION__HH

#include <stdexcept>
#include "Any.h"

namespace amop
{

    //------------------------------------------------------------------
    class NotImplementedException : public std::runtime_error        
    {
    public:
        NotImplementedException()
            : std::runtime_error("Not Implemented")
        {
        }
    };

    //------------------------------------------------------------------
    class NotPureVirtualException : public std::runtime_error        
    {
    public:        
        NotPureVirtualException()
            : std::runtime_error("It is not a pure virtual function")
        {
        }
    };

    //------------------------------------------------------------------
    class NotEqualException : public std::runtime_error
    {
    public:
        NotEqualException(size_t param, const any& expect, const any& actual) 
            : std::runtime_error("Not Equal")
            , m_param(param)
            , m_expect(expect)
            , m_actual(actual)
          {
          }

          any& GetExpect() { return m_expect; }
          any& GetActual() { return m_actual; }

          size_t GetParam() { return m_param; }

    protected:
        size_t m_param;
        any m_expect;
        any m_actual;

    };

    //------------------------------------------------------------------
    class CallCountException : public std::runtime_error
    {
    public:
        CallCountException(size_t expect, size_t actual) 
            : std::runtime_error("Call Count Fail")
            , m_expect(expect)
            , m_actual(actual)
          {
          };

          size_t GetExpect() { return m_expect; }
          size_t GetActual() { return m_actual; }

    protected:
        size_t m_expect;
        size_t m_actual;
    };

}

#endif //__AMOP_MOCKOBJECTEXECEPTION__HH
