#include "MockFunctionImpl.h"
#include "Comparable.h"
#include "MockObjectException.h"
#include "ExceptionThrower.h"

#ifdef __GNUC__
#include <tr1/memory>
#endif

#include <assert.h>

namespace amop
{
    namespace detail
    {        
        //------------------------------------------------------------------
        MockFunctionImpl::MockFunctionImpl()
            : m_callCounter(0)
        {
        }

        //------------------------------------------------------------------
        void MockFunctionImpl::setExpectCallCounter(size_t counter)
        {
            m_expectCallCounter.reset(new size_t(counter));
        }

        //------------------------------------------------------------------
        void MockFunctionImpl::setRedirect(const any& result, bool isDefault)
        {
            setReturnInternal(ReturnTypePair(DO, result), isDefault);
        }

        //------------------------------------------------------------------
        void MockFunctionImpl::setSetter(size_t paramId, const Comparable& param, bool isDefault)
        {
            if (isDefault)
            {
                if( !m_setterDefault.get() )
                {
                    m_setterDefault.reset(new ParamDefaultMap() );
                }

                (*m_setterDefault)[paramId] = param;
            }
            else
            {
                if( !m_setter.get() )
                {
                    m_setter.reset(new ParamMap());
                }

                (*m_setter)[paramId].push_back(param);
            }
        }

        //------------------------------------------------------------------
        void MockFunctionImpl::setExpect(size_t paramId, const Comparable& param, bool isDefault)
        {
            if(isDefault)
            {
                if( !m_expectDefault.get() )
                {
                    m_expectDefault.reset(new ParamDefaultMap());
                }

                (*m_expectDefault)[paramId] = param;
            }
            else
            {
                if( !m_expect.get() )
                {
                    m_expect.reset(new ParamMap());
                }

                (*m_expect)[paramId].push_back(param);
            }
        }

        //------------------------------------------------------------------
        void MockFunctionImpl::setReturnInternal(const ReturnTypePair& ret, bool isDefault)
        {
            if(isDefault)
            {
                m_returnDefault.reset( new ReturnTypePair(ret) );                    
            }
            else
            {
                if( !m_return.get() )
                {
                    m_return.reset( new std::vector< ReturnTypePair >() );
                }

                m_return->push_back(ret);
            }               
        }

        //------------------------------------------------------------------
        void MockFunctionImpl::setReturn(const std::pair<bool, any>& result, bool isDefault)
        {
            if( result.first )
            {
                setReturnInternal(ReturnTypePair(THROW, result.second), isDefault);
            }
            else
            {
                setReturnInternal(ReturnTypePair(RETURN, result.second), isDefault);
            }
        }

        //------------------------------------------------------------------
        size_t MockFunctionImpl::getCallCounter()
        {
            return m_callCounter;
        }

        //------------------------------------------------------------------
        any& MockFunctionImpl::getRedirect()
        {
            static any empty;
            
            ReturnTypePair* ret = getReturn(false);

            if( !ret || ret->first != DO )
            {
                return empty;
            }

            return ret->second;            
        }

        //------------------------------------------------------------------
        void MockFunctionImpl::addCallCounter()
        {
            m_callCounter++;

            ReturnTypePair* exitValue = getReturn(false);

            if(exitValue && exitValue->first == THROW)
            {
                AmopSharedPtr<ExceptionThrowerBase> thrower;
                try
                {
                    thrower = any_cast<AmopSharedPtr<ExceptionThrowerBase> >(exitValue->second);
                }
                catch(bad_any_cast & /*bac*/)
                {
                    assert(false);
                }
                
                thrower->ThrowTypedException();
            }
        }

        //------------------------------------------------------------------
        void MockFunctionImpl::verify()
        {
            verifyCallCounter();                
            verifyReturn();                                
            verifyExpect();
        }

        //------------------------------------------------------------------
        void MockFunctionImpl::verifyCallCounter()
        {
            if( !m_expectCallCounter.get() )
            {
                return ;
            }

            if( *m_expectCallCounter != m_callCounter )
            {
                throw CallCountException(*m_expectCallCounter, m_callCounter);
            }
        }

        //------------------------------------------------------------------
        void MockFunctionImpl::verifyReturn()
        {
            if( m_returnDefault.get() || !m_return.get() )
            {
                return ;
            }

            if( m_return->size() != m_callCounter )
            {
                throw CallCountException(m_return->size(), m_callCounter);
            }
        }

        //------------------------------------------------------------------
        void MockFunctionImpl::verifyExpect()
        {
            if( !m_expect.get() )
            {
                return;
            }

            size_t maxCount = 0;                

            for( ParamMap::iterator iterator = m_expect->begin()
                ; iterator != m_expect->end()
                ; ++iterator )
            {
                if(m_expectDefault.get() && m_expectDefault->count(iterator->first))
                {
                    continue;
                }

                maxCount = std::max(maxCount, iterator->second.size());
            }                

            if(maxCount != 0 && maxCount != m_callCounter)
            {
                throw CallCountException(maxCount, m_callCounter);
            }
        }

        //------------------------------------------------------------------
        any& MockFunctionImpl::getReturn()
        {
            ReturnTypePair * exitValue = getReturn(true);            

            return exitValue->second;
        }

        //------------------------------------------------------------------
        MockFunctionImpl::ReturnTypePair* MockFunctionImpl::getReturn(
            bool check
            ) const
        {
            size_t callCounter = m_callCounter - 1;

            if( m_return.get() && callCounter < m_return->size())
            {
                return &(*m_return)[callCounter];
            }	            

            if(!m_returnDefault.get())
            {
                if( !check )
                {
                    return NULL;
                }
                
                size_t expect = m_return.get() ? m_return->size() : 0;

                throw CallCountException(expect, callCounter);
            }

            return m_returnDefault.get();
        }

        //------------------------------------------------------------------
        void MockFunctionImpl::setActual(size_t paramId, const any& param)
        {
            applySetter(paramId, param);

            if( !m_expect.get() || !m_expect->count(paramId))
            {
                if( !m_expectDefault.get() || !m_expectDefault->count(paramId) )
                {
                    return ;
                }
                else
                {
                    if( (*m_expectDefault)[paramId].IsEmpty() )
                    {
                        return ;
                    }
                    
                    if( (*m_expectDefault)[paramId] == param )
                    {
                        return ;
                    }

                    throw NotEqualException(paramId, (*m_expectDefault)[paramId], param);
                }
            }

            // Must have expect
            ComparableList& clist = (*m_expect)[paramId] ;

            if( clist.size() <= m_callCounter )
            {
                if(! m_expectDefault.get() ) return ;

                ParamDefaultMap& paramDefaultMap = (*m_expectDefault);

                if( !paramDefaultMap.count(paramId) ) return ;

                if( paramDefaultMap[paramId] != param )
                    throw NotEqualException(paramId, paramDefaultMap[paramId], param);
            }

            if( !clist[m_callCounter].IsEmpty() )
            {
                if( clist[m_callCounter] != param )
                    throw NotEqualException(paramId, clist[m_callCounter], param);
            }
        }

        //------------------------------------------------------------------
        void MockFunctionImpl::applySetter(size_t paramId, const any& param)
        {
            if(!m_setter.get() || !m_setter->count(paramId))
            { 
                if( !m_setterDefault.get() || !m_setterDefault->count(paramId))
                {
                    return ;
                }
                else
                {
                    (*m_setterDefault)[paramId].assign(param);
                    return ;
                }
            }

            // Must have expect
            ComparableList& clist = (*m_setter)[paramId] ;                

            if( clist.size() <= m_callCounter )
            {
                if(!m_setterDefault.get()) return ;

                ParamDefaultMap& paramDefaultMap = *m_setterDefault;

                if( !paramDefaultMap.count(paramId) ) return ;

                paramDefaultMap[paramId].assign(param);
                return ;
            }	

            clist[m_callCounter].assign( param );
        }
    }
}
