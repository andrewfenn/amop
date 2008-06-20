#include "MockObjectBase.h"
#include "DynamicFunction.h"
#include "MockFunctionImpl.h"

#include <map>

namespace amop
{
    namespace Detail
    {        
        //------------------------------------------------------------------
        TMockObjectBase::TMockObjectBase()
        {
            mDynamicObject.reset(new TDynamicObject());
        }

        //------------------------------------------------------------------
        TMockObjectBase::~TMockObjectBase()
        {
            Clear();
        }		

        //------------------------------------------------------------------
        std::pair<IMockFunction*, IDynamicFunctionHandler*> TMockObjectBase::CreateMockFunction()
        {
            TMockFunctionImpl* func = new TMockFunctionImpl();
            mFunctions.push_back(func);

            return std::pair<IMockFunction*, IDynamicFunctionHandler*>(func, func);
        }

        //------------------------------------------------------------------
        IMockFunction* TMockObjectBase::GetMockFunction(TDynamicFunction* func)
        {
            return dynamic_cast<TMockFunctionImpl*>( func->GetHandler() );
        }

        //------------------------------------------------------------------
        void TMockObjectBase::Clear()
        {
            mDynamicObject.reset(new TDynamicObject()) ;

            for( std::vector<TMockFunctionImpl*>::iterator iter = mFunctions.begin()
                ; iter != mFunctions.end()
                ; ++iter )
            {
                delete *iter;
            }

            mFunctions.clear();
        }

        //------------------------------------------------------------------
        void TMockObjectBase::Verify()
        {
            // Check the expect call counter first.
            for( std::vector<TMockFunctionImpl*>::iterator iter = mFunctions.begin()
                ; iter != mFunctions.end()
                ; ++iter )
            {
                (*iter)->Verify();
            }            
        }

        //------------------------------------------------------------------
        void* TMockObjectBase::GetVptr()
        {
            return mDynamicObject->GetVptr();
        }

    }
}

//Local Variables:
//c-basic-offset: 4
//End:

