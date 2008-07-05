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
    namespace Detail
    {        
        //------------------------------------------------------------------
        TMockFunctionImpl::TMockFunctionImpl()
            : mCallCounter(0)
        {
        }

        //------------------------------------------------------------------
        void TMockFunctionImpl::SetExpectCallCounter(size_t counter)
        {
            mExpectCallCounter.reset(new size_t(counter));
        }

        //------------------------------------------------------------------
        void TMockFunctionImpl::SetRedirect(const any& result, bool isDefault)
        {
            _SetReturn(TReturnTypePair(DO, result), isDefault);
        }

        //------------------------------------------------------------------
        void TMockFunctionImpl::SetSetter(size_t paramId, const TComparable& param, bool isDefault)
        {
            if (isDefault)
            {
                if( !mSetterDefault.get() )
                {
                    mSetterDefault.reset(new TParamDefaultMap() );
                }

                (*mSetterDefault)[paramId] = param;
            }
            else
            {
                if( !mSetter.get() )
                {
                    mSetter.reset(new TParamMap());
                }

                (*mSetter)[paramId].push_back(param);
            }
        }

        //------------------------------------------------------------------
        void TMockFunctionImpl::SetExpect(size_t paramId, const TComparable& param, bool isDefault)
        {
            if(isDefault)
            {
                if( !mExpectDefault.get() )
                {
                    mExpectDefault.reset(new TParamDefaultMap());
                }

                (*mExpectDefault)[paramId] = param;
            }
            else
            {
                if( !mExpect.get() )
                {
                    mExpect.reset(new TParamMap());
                }

                (*mExpect)[paramId].push_back(param);
            }
        }

        //------------------------------------------------------------------
        void TMockFunctionImpl::_SetReturn(const TReturnTypePair& ret, bool isDefault)
        {
            if(isDefault)
            {
                mReturnDefault.reset( new TReturnTypePair(ret) );                    
            }
            else
            {
                if( !mReturn.get() )
                {
                    mReturn.reset( new std::vector< TReturnTypePair >() );
                }

                mReturn->push_back(ret);
            }               
        }

        //------------------------------------------------------------------
        void TMockFunctionImpl::SetReturn(const std::pair<bool, any>& result, bool isDefault)
        {
            if( result.first )
            {
                _SetReturn(TReturnTypePair(THROW, result.second), isDefault);
            }
            else
            {
                _SetReturn(TReturnTypePair(RETURN, result.second), isDefault);
            }
        }

        //------------------------------------------------------------------
        size_t TMockFunctionImpl::GetCallCounter()
        {
            return mCallCounter;
        }

        //------------------------------------------------------------------
        any& TMockFunctionImpl::GetRedirect()
        {
            static any empty;
            
            TReturnTypePair* ret = _GetReturn(false);

            if( !ret || ret->first != DO )
            {
                return empty;
            }

            return ret->second;            
        }

        //------------------------------------------------------------------
        void TMockFunctionImpl::AddCallCounter()
        {
            mCallCounter++;

            TReturnTypePair* exitValue = _GetReturn(false);

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
        void TMockFunctionImpl::Verify()
        {
            VerifyCallCounter();                
            VerifyReturn();                                
            VerifyExpect();
        }

        //------------------------------------------------------------------
        void TMockFunctionImpl::VerifyCallCounter()
        {
            if( !mExpectCallCounter.get() )
            {
                return ;
            }

            if( *mExpectCallCounter != mCallCounter )
            {
                throw TCallCountException(*mExpectCallCounter, mCallCounter);
            }
        }

        //------------------------------------------------------------------
        void TMockFunctionImpl::VerifyReturn()
        {
            if( mReturnDefault.get() || !mReturn.get() )
            {
                return ;
            }

            if( mReturn->size() != mCallCounter )
            {
                throw TCallCountException(mReturn->size(), mCallCounter);
            }
        }

        //------------------------------------------------------------------
        void TMockFunctionImpl::VerifyExpect()
        {
            if( !mExpect.get() )
            {
                return;
            }

            size_t maxCount = 0;                

            for( TParamMap::iterator iterator = mExpect->begin()
                ; iterator != mExpect->end()
                ; ++iterator )
            {
                if(mExpectDefault.get() && mExpectDefault->count(iterator->first))
                {
                    continue;
                }

                maxCount = std::max(maxCount, iterator->second.size());
            }                

            if(maxCount != 0 && maxCount != mCallCounter)
            {
                throw TCallCountException(maxCount, mCallCounter);
            }
        }

        //------------------------------------------------------------------
        any& TMockFunctionImpl::GetReturn()
        {
            TReturnTypePair * exitValue = _GetReturn(true);            

            return exitValue->second;
        }

        //------------------------------------------------------------------
        TMockFunctionImpl::TReturnTypePair* TMockFunctionImpl::_GetReturn(
            bool check
            ) const
        {
            size_t callCounter = mCallCounter - 1;

            if( mReturn.get() && callCounter < mReturn->size())
            {
                return &(*mReturn)[callCounter];
            }	            

            if(!mReturnDefault.get())
            {
                if( !check )
                {
                    return NULL;
                }
                
                size_t expect = mReturn.get() ? mReturn->size() : 0;

                throw TCallCountException(expect, callCounter);
            }

            return mReturnDefault.get();
        }

        //------------------------------------------------------------------
        void TMockFunctionImpl::SetActual(size_t paramId, const any& param)
        {
            ApplySetter(paramId, param);

            if( !mExpect.get() || !mExpect->count(paramId))
            {
                if( !mExpectDefault.get() || !mExpectDefault->count(paramId) )
                {
                    return ;
                }
                else
                {
                    if( (*mExpectDefault)[paramId].IsEmpty() )
                    {
                        return ;
                    }
                    
                    if( (*mExpectDefault)[paramId] == param )
                    {
                        return ;
                    }

                    throw TNotEqualException(paramId, (*mExpectDefault)[paramId], param);
                }
            }

            // Must have expect
            TComparableList& clist = (*mExpect)[paramId] ;

            if( clist.size() <= mCallCounter )
            {
                if(! mExpectDefault.get() ) return ;

                TParamDefaultMap& paramDefaultMap = (*mExpectDefault);

                if( !paramDefaultMap.count(paramId) ) return ;

                if( paramDefaultMap[paramId] != param )
                    throw TNotEqualException(paramId, paramDefaultMap[paramId], param);
            }

            if( !clist[mCallCounter].IsEmpty() )
            {
                if( clist[mCallCounter] != param )
                    throw TNotEqualException(paramId, clist[mCallCounter], param);
            }
        }

        //------------------------------------------------------------------
        void TMockFunctionImpl::ApplySetter(size_t paramId, const any& param)
        {
            if(!mSetter.get() || !mSetter->count(paramId))
            { 
                if( !mSetterDefault.get() || !mSetterDefault->count(paramId))
                {
                    return ;
                }
                else
                {
                    (*mSetterDefault)[paramId].Assign(param);
                    return ;
                }
            }

            // Must have expect
            TComparableList& clist = (*mSetter)[paramId] ;                

            if( clist.size() <= mCallCounter )
            {
                if(!mSetterDefault.get()) return ;

                TParamDefaultMap& paramDefaultMap = *mSetterDefault;

                if( !paramDefaultMap.count(paramId) ) return ;

                paramDefaultMap[paramId].Assign(param);
                return ;
            }	

            clist[mCallCounter].Assign( param );
        }
    }
}
