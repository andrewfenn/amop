#ifndef __AMOP_VIRTUALTABLE_HH
#define __AMOP_VIRTUALTABLE_HH

#include "Config.h"
#include <assert.h>

namespace amop
{
    namespace detail
    {
        class ObjectHolder;
        
        //------------------------------------------------------------------
        class VirtualTable
        {
        public:
            void* m_vtable[MAX_NUM_VIRTUAL_FUNCTIONS];
            void* m_vtpr;
            ObjectHolder* m_this;            

            static ObjectHolder* getThis(void* callerThisAddres);

            static VirtualTable* createVirtualTable(ObjectHolder* object);

        private:
            VirtualTable(ObjectHolder* object);
        };

    }

}

#endif //__AMOP_VIRTUALTABLE_HH

