#ifndef __AMOP_RETURNMATCHBUILDER_HH
#define __AMOP_RETURNMATCHBUILDER_HH

#include "ObjectHolder.h"
#include "Functor.h"
#include "Comparable.h"

namespace amop
{

//------------------------------------------------------------------
template <class F>
class TReturnMatchBuilder
{
public:
	TReturnMatchBuilder(Detail::TObjectHolder* objectHolder,
		size_t offset) : mObjectHolder(objectHolder),
			mOffset(offset)
	{
	}

	template<class T>
	TReturnMatchBuilder Will(T result)
	{
		typedef Detail::RemoveReference<Detail::Functor<F>::ReturnType>::Type ToType;

		//****************************************/
		//** If you see this, that's mean your  **/
		//** expect type is not convertiable to **/
		//** actual type.						**/
		//****************************************/
		typedef int ItIsNotConvertible[
			Detail::IsConvertible<T, ToType>::Result ? 1 : -1];
				
		mObjectHolder->SetReturnDefault(mOffset, (ToType)result);
		return *this;
	}

	template<class T>
	TReturnMatchBuilder Wills(T result)
	{
		typedef Detail::RemoveReference<Detail::Functor<F>::ReturnType>::Type ToType;

		//****************************************/
		//** If you see this, that's mean your  **/
		//** expect type is not convertiable to **/
		//** actual type.						**/
		//****************************************/
		typedef int ItIsNotConvertible[
			Detail::IsConvertible<T, ToType>::Result ? 1 : -1];
				
		mObjectHolder->SetReturn(mOffset, (ToType)result);
		return *this;
	}

	template<int I, class T>
	TReturnMatchBuilder Expect(T expect)
	{
		typedef Detail::Get< Detail::Functor<F>::ParameterTypes, I>::Type ToTypeRef;
		typedef Detail::RemoveReference<ToTypeRef>::Type ToType;

		//****************************************/
		//** If you see this, that's mean your  **/
		//** expect type is not convertiable to **/
		//** actual type.						**/
		//****************************************/
		typedef int ItIsNotConvertible[
			Detail::IsConvertible<T, ToType>::Result ? 1 : -1];
		
		Detail::TComparable compare = Detail::TComparable::MakeCompare<ToType>(expect);

		mObjectHolder->SetExpectDefault(mOffset, I, compare);
		return *this;
	}

	template<int I, class T>
	TReturnMatchBuilder Expects(T expect)
	{
		typedef Detail::Get< Detail::Functor<F>::ParameterTypes, I>::Type ToTypeRef;
		typedef Detail::RemoveReference<ToTypeRef>::Type ToType;

		//****************************************/
		//** If you see this, that's mean your  **/
		//** expect type is not convertiable to **/
		//** actual type.						**/
		//****************************************/
		typedef int ItIsNotConvertible[
			Detail::IsConvertible<T, ToType>::Result ? 1 : -1];
		
		Detail::TComparable compare = Detail::TComparable::MakeCompare<ToType>(expect);

		mObjectHolder->SetExpect(mOffset, I, compare);
		return *this;
	}

private:
	size_t mOffset;
	Detail::TObjectHolder* mObjectHolder;
};

}


#endif //__AMOP_RETURNMATCHBUILDER_HH
