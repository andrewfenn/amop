#ifndef __AMOP_VIRTUALTABLE_HH
#define __AMOP_VIRTUALTABLE_HH

#include "Config.h"
#include <assert.h>

namespace amop
{
    namespace Detail
    {
        class TObjectHolder;
        
        //------------------------------------------------------------------
        class TVirtualTable
        {
        public:
            void* mVtable[MAX_NUM_VIRTUAL_FUNCTIONS];
            void* mVtpr;
            TObjectHolder* mThis;            

            static TObjectHolder* GetThis(void* callerThisAddres);

            static TVirtualTable* CreateVirtualTable(TObjectHolder* object);

        private:
            TVirtualTable(TObjectHolder* object);
        };

    }

}

#endif //__AMOP_VIRTUALTABLE_HH

