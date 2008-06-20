#include "DynamicObject.h"
#include "VirtualTable.h"
#include "DynamicFunction.h"

namespace amop
{
    namespace Detail
    {        
        //------------------------------------------------------------------
        TDynamicObject::TDynamicObject()            
        {
            mVirtualTable = Detail::TVirtualTable::CreateVirtualTable(this);
        }

        //------------------------------------------------------------------
        TDynamicObject::~TDynamicObject()
        {
            delete mVirtualTable;

            for( TDynamicFunctionMap::iterator iter = mFunctions.begin()
                ; iter != mFunctions.end()
                ; ++iter )
            {
                delete iter->second;
            }
        }

        //------------------------------------------------------------------
        void* TDynamicObject::GetVptr()
        {
            return &mVirtualTable->mVtpr;
        }            

        //------------------------------------------------------------------
        void TDynamicObject::_BindFunction(
            size_t offset
            , TFunctionAddress data
            , IDynamicFunctionHandler* handler
            )
        {
            assert(offset < MAX_NUM_VIRTUAL_FUNCTIONS);
            mVirtualTable->mVtable[offset] = data;

            if( mFunctions.count(offset) )
            {
                return;
            }

            mFunctions[offset] = new TDynamicFunction(handler);
        }          

        //------------------------------------------------------------------
        any& TDynamicObject::GetRedirect(size_t idx)
        {
            assert(mFunctions.count(idx));
            
            return mFunctions[idx]->GetHandler()->GetRedirect();
        }

        //------------------------------------------------------------------
        std::pair<bool, any>& TDynamicObject::GetReturn(size_t idx)
        {
            assert(mFunctions.count(idx));
            
            return mFunctions[idx]->GetHandler()->GetReturn();
        }

        //------------------------------------------------------------------
        void TDynamicObject::SetActual(size_t idx, size_t paramId, const any& param)
        {
            assert(mFunctions.count(idx));
            
            mFunctions[idx]->GetHandler()->SetActual(paramId, param);
        }

        //------------------------------------------------------------------
        void TDynamicObject::AddCallCounter(size_t idx)
        {
            assert(mFunctions.count(idx));
            
            mFunctions[idx]->GetHandler()->AddCallCounter();
        }
    }
}



