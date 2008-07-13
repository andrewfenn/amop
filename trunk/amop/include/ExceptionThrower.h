#ifndef __AMOP_EXCEPTIONTHROWER_HH
#define __AMOP_EXCEPTIONTHROWER_HH

#include "Config.h"

namespace amop{

    namespace detail{
        class ExceptionThrowerBase{
        public:
            virtual void ThrowTypedException() = 0;
            virtual ~ExceptionThrowerBase(){}
        };

        template<class E>
        class ExceptionThrower : public ExceptionThrowerBase
        {
        private:
            E const m_exception;

            ExceptionThrower& operator=(const ExceptionThrower&);

        public:
            ExceptionThrower(E const & exception)
                : ExceptionThrowerBase()
                , m_exception(exception)
            {
            }
            
            virtual ~ExceptionThrower(){}
            
            virtual void ThrowTypedException()
            {
                throw m_exception;
            }
        };

    }
}

//Local Variables:
//c-basic-offset: 4
//End:

#endif
