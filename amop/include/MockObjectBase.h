#ifndef __AMOP_MOCKOBJECTBASE_HH
#define __AMOP_MOCKOBJECTBASE_HH

#include "Any.h"
#include "DynamicObject.h"

#include <map>
#include <vector>
#include <set>

namespace amop
{

namespace detail
{
    class DynamicFunction;
    class MockFunction;
    class MockFunctionImpl;

    //------------------------------------------------------------------
    class MockObjectBase         
	{
	public:
		MockObjectBase();
		virtual ~MockObjectBase();

		void clear();
        void verify();

    protected:
        void* getVptr();

        DynamicObject* getDynamicObject()
        {
            return m_dynamicObject.get();
        }		

        MockFunction* createMockFunction(DynamicFunction* function);        

	private:       
        // Inheritent from IDynamicObjectHandler
        any& getRedirect(DynamicFunction* dynamicFunction);

        std::pair<bool, any>& getReturn(DynamicFunction* dynamicFunction);
        void setActual(DynamicFunction* dynamicFunction, size_t paramId, const any& param);
        void addCallCounter(DynamicFunction* dynamicFunction);

        friend class MockFunctionImpl;        
        std::vector<MockFunctionImpl*> m_functions;

        std::auto_ptr<DynamicObject> m_dynamicObject;              
		    
        MockObjectBase( const MockObjectBase& );
        MockObjectBase& operator=(const MockObjectBase& );
	};
}

}

//Local Variables:
//c-basic-offset: 4
//End:
#endif //__AMOP_MOCKOBJECTBASE_HH
