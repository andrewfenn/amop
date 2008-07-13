#include "DynamicObject.h"
#include "VirtualTable.h"
#include "DynamicFunction.h"

namespace amop
{
    namespace detail
    {        
        //------------------------------------------------------------------
        DynamicObject::DynamicObject()            
        {
            m_virtualTable = detail::VirtualTable::createVirtualTable(this);
        }

        //------------------------------------------------------------------
        DynamicObject::~DynamicObject()
        {
            delete m_virtualTable;

            for( DynamicFunctionMap::iterator iter = m_functions.begin()
                ; iter != m_functions.end()
                ; ++iter )
            {
                delete iter->second;
            }
        }

        //------------------------------------------------------------------
        void* DynamicObject::getVptr()
        {
            return &m_virtualTable->m_vtpr;
        }            

        //------------------------------------------------------------------
        DynamicFunction* DynamicObject::bindFunction(
            size_t offset
            , FunctionAddress data            
            )
        {
            assert(offset < MAX_NUM_VIRTUAL_FUNCTIONS);
            m_virtualTable->m_vtable[offset] = data;

            if( m_functions.count(offset) )
            {
                return m_functions[offset];
            }

            DynamicFunction* p = new DynamicFunction();
            m_functions[offset] = p;
            
            return p;
        }          

        //------------------------------------------------------------------
        any& DynamicObject::getRedirect(size_t idx)
        {
            assert(m_functions.count(idx));
            
            return m_functions[idx]->getRedirect();
        }

        //------------------------------------------------------------------
        any& DynamicObject::getReturn(size_t idx)
        {
            assert(m_functions.count(idx));
            
            return m_functions[idx]->getReturn();
        }

        //------------------------------------------------------------------
        void DynamicObject::setActual(size_t idx, size_t paramId, const any& param)
        {
            assert(m_functions.count(idx));
            
            m_functions[idx]->setActual(paramId, param);
        }

        //------------------------------------------------------------------
        void DynamicObject::addCallCounter(size_t idx)
        {
            assert(m_functions.count(idx));
            
            m_functions[idx]->addCallCounter();
        }
    }
}



