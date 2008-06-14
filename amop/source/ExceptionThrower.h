#ifndef __AMOP_EXCEPTIONTHROWER_HH
#define __AMOP_EXCEPTIONTHROWER_HH

#include "Config.h"

namespace amop{
  
    namespace Detail{
        class ExceptionThrowerBase{
        public:
            virtual void ThrowTypedException() = 0;
            virtual ~ExceptionThrowerBase(){}
        };
        
        template<class E>
        class ExceptionThrower : public ExceptionThrowerBase{
        private:
            E const _exception;

            ExceptionThrower& operator=(const ExceptionThrower&);
            
        public:
            ExceptionThrower(E const & exception)
                : ExceptionThrowerBase()
                , _exception(exception)
            {
            }
            virtual ~ExceptionThrower(){}
            virtual void ThrowTypedException(){
                throw _exception;
            }
        };
 
    }
}

//Local Variables:
//c-basic-offset: 4
//End:

#endif
