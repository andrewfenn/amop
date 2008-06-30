#ifndef __AMOP_MOCKOBJECTBASE_HH
#define __AMOP_MOCKOBJECTBASE_HH

#include "Any.h"
#include "DynamicObject.h"

#include <map>
#include <vector>
#include <set>

namespace amop
{

namespace Detail
{
    class TDynamicFunction;
    class IMockFunction;
    class TMockFunctionImpl;

    //------------------------------------------------------------------
    class TMockObjectBase         
	{
	public:
		TMockObjectBase();
		virtual ~TMockObjectBase();

		void Clear();
        void Verify();

    protected:
        void* GetVptr();

        TDynamicObject* GetDynamicObject()
        {
            return mDynamicObject.get();
        }		

        IMockFunction* CreateMockFunction(TDynamicFunction* function);        

	private:       
        // Inheritent from IDynamicObjectHandler
        any& GetRedirect(TDynamicFunction* dynamicFunction);

        std::pair<bool, any>& GetReturn(TDynamicFunction* dynamicFunction);
        void SetActual(TDynamicFunction* dynamicFunction, size_t paramId, const any& param);
        void AddCallCounter(TDynamicFunction* dynamicFunction);

        friend class TMockFunctionImpl;        
        std::vector<TMockFunctionImpl*> mFunctions;

        std::auto_ptr<TDynamicObject> mDynamicObject;              
		    
        TMockObjectBase( const TMockObjectBase& );
        TMockObjectBase& operator=(const TMockObjectBase& );
	};
}

}

//Local Variables:
//c-basic-offset: 4
//End:
#endif //__AMOP_MOCKOBJECTBASE_HH
