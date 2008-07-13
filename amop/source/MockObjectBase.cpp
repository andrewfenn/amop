#include "MockObjectBase.h"
#include "DynamicFunction.h"
#include "MockFunctionImpl.h"

#include <map>

namespace amop
{
    namespace detail
    {        
        //------------------------------------------------------------------
        MockObjectBase::MockObjectBase()
        {
            m_dynamicObject.reset(new DynamicObject());
        }

        //------------------------------------------------------------------
        MockObjectBase::~MockObjectBase()
        {
            clear();
        }		

        //------------------------------------------------------------------
        MockFunction* MockObjectBase::createMockFunction(DynamicFunction* function)
        {
            if( function->GetHandler() )
            {
                std::vector<MockFunctionImpl*>::iterator iter = 
                    std::find(m_functions.begin(), m_functions.end(), function->GetHandler() );
                
                if( iter != m_functions.end() )
                {
                    return *iter;
                }
            }
            
            MockFunctionImpl* func = new MockFunctionImpl();
            m_functions.push_back(func);
            function->setHandler( func );
            return func;
        }

        //------------------------------------------------------------------
        void MockObjectBase::clear()
        {
            m_dynamicObject.reset(new DynamicObject()) ;

            for( std::vector<MockFunctionImpl*>::iterator iter = m_functions.begin()
                ; iter != m_functions.end()
                ; ++iter )
            {
                delete *iter;
            }

            m_functions.clear();
        }

        //------------------------------------------------------------------
        void MockObjectBase::verify()
        {
            // Check the expect call counter first.
            for( std::vector<MockFunctionImpl*>::iterator iter = m_functions.begin()
                ; iter != m_functions.end()
                ; ++iter )
            {
                (*iter)->verify();
            }            
        }

        //------------------------------------------------------------------
        void* MockObjectBase::getVptr()
        {
            return m_dynamicObject->getVptr();
        }

    }
}

//Local Variables:
//c-basic-offset: 4
//End:

