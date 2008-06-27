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
        IMockFunction* TMockObjectBase::CreateMockFunction(TDynamicFunction* function)
        {
            if( function->GetHandler() )
            {
                std::vector<TMockFunctionImpl*>::iterator iter = 
                    std::find(mFunctions.begin(), mFunctions.end(), function->GetHandler() );
                
                if( iter != mFunctions.end() )
                {
                    return *iter;
                }
            }
            
            TMockFunctionImpl* func = new TMockFunctionImpl();
            mFunctions.push_back(func);
            function->SetHandler( func );
            return func;
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

