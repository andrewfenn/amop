#include "VirtualTable.h"
#include "Config.h"
#include "MockObjectException.h"
#include "ObjectHolder.h"

#include <assert.h>

namespace amop
{
    namespace detail
    {
        //------------------------------------------------------------------
        ObjectHolder* getHolderFromThis(void* pointer)
        {
            return VirtualTable::getThis(pointer);
        }

        //------------------------------------------------------------------
        class NotImplemented
        {
        public:
            void func()
            {
                throw NotImplementedException();
            }
        };

        //------------------------------------------------------------------
        static FunctionAddress getNotImplementedFunc()
        {
            typedef void (NotImplemented::*NotImplementedFuncPtr)();

            NotImplementedFuncPtr ptr = &NotImplemented::func;
            FunctionAddress p = HorribleCast<FunctionAddress>(ptr);

            return p; 
        }

        //------------------------------------------------------------------
        VirtualTable::VirtualTable(ObjectHolder* object)
        {
            m_vtpr = &m_vtable[0];
            m_this = object;
        }

        //------------------------------------------------------------------
        ObjectHolder* VirtualTable::getThis(void* callerThisAddres)
        {
            void* callerThis = *((void**)callerThisAddres);

            // _this pointer should point to &mVtable[0]
            VirtualTable* this_ = (VirtualTable*)callerThis;

            assert(callerThis == &this_->m_vtable[0]);

            return this_->m_this;        
        }

        //------------------------------------------------------------------
        VirtualTable* VirtualTable::createVirtualTable(ObjectHolder* object)
        {
            VirtualTable* vtable = new VirtualTable(object);

            for(size_t i = 0 ; i < MAX_NUM_VIRTUAL_FUNCTIONS; ++i)
                vtable->m_vtable[i] = getNotImplementedFunc();

            return vtable;
        }
    }
}





