#include "DynamicObject.h"
#include "VirtualTable.h"

namespace amop
{
    namespace Detail
    {        
        //------------------------------------------------------------------
        TDynamicObject::TDynamicObject(TObjectHolder* object)            
        {
            mVirtualTable = Detail::TVirtualTable::CreateVirtualTable(object);
        }

        //------------------------------------------------------------------
        void* TDynamicObject::GetVptr()
        {
            return &mVirtualTable->mVtpr;
        }            

        //------------------------------------------------------------------
        void TDynamicObject::_BindFunction(size_t offset, TFunctionAddress data)
        {
            assert(offset < MAX_NUM_VIRTUAL_FUNCTIONS);
            mVirtualTable->mVtable[offset] = data;
        }          
    }
}



