#ifndef __MOCKPP_FUNCTIONHOLDER_HH
#define __MOCKPP_FUNCTIONHOLDER_HH

#include "Config.h"
#include "Functor.h"

namespace MockPP
{
namespace Detail
{
	//------------------------------------------------------------------
	template <int I, class T, bool>
	struct FunctionHolderBase;
	
	//------------------------------------------------------------------
	template <int I, class T>
	struct FunctionHolder : public FunctionHolderBase<I, T, 
		!IsEqual< typename Functor<T>::ReturnType, void >::Result  >
	{
	};
	
#ifndef VT_THIS 
#define VT_THIS TVirtualTable::GetThis(this)
#endif
	
	template <int I, class C>
	struct FunctionHolderBase<I, void (C::*)(), false>
	{			
		typedef typename void (FunctionHolderBase::*TFuncPtrType)();
		void Func() { VT_THIS->AddCallCounter(I); }
	};

	template <int I, class C>
	struct FunctionHolderBase<I, void (C::*)() const, false>
	{			
		typedef typename void (FunctionHolderBase::*TFuncPtrType)();
		void Func() { VT_THIS->AddCallCounter(I); }
	};

	//------------------------------------------------------------------
	template <int I, class R, class C>
	struct FunctionHolderBase<I, R (C::*)(), true>
	{			
		typedef typename R (FunctionHolderBase::*TFuncPtrType)();
		R Func() 
		{ 
			VT_THIS->AddCallCounter(I);
			return VT_THIS->Return<R>(I);
		}
	};

	//------------------------------------------------------------------
	template <int I, class R, class C>
	struct FunctionHolderBase<I, R (C::*)() const, true>
	{			
		typedef typename R (FunctionHolderBase::*TFuncPtrType)();
		R Func() 
		{ 
			VT_THIS->AddCallCounter(I);
			return VT_THIS->Return<R>(I);
		}
	};

#define DETAIL_ACTUALCALL_ITEM(n, t)		VT_THIS->Actual<T##n>(I,DETAIL_DEC(n),t##n);
#define DETAIL_ACTUALCALL_ITEM_END(n, t)	VT_THIS->Actual<T##n>(I,DETAIL_DEC(n),t##n);


	//------------------------------------------------------------------
#define	DETAIL_FUNCTIONHOLDER_BUILD_O(n)				\
	template <int I, class C, DETAIL_TPARAMS(n)>	\
	struct FunctionHolderBase<I, void (C::*)(DETAIL_TPARAMS(n)), false>   \
	{																  \
		typedef typename void (FunctionHolderBase::*TFuncPtrType)();     \
		void Func(DETAIL_FUNC_PARAMS(n, t))								  \
		{															  \
			DETAIL_REPEAT(n,DETAIL_ACTUALCALL_ITEM,DETAIL_ACTUALCALL_ITEM_END,t);  \
			VT_THIS->AddCallCounter(I);								  \
		}															  \
	};																\
	template <int I, class C, DETAIL_TPARAMS(n)>						\
	struct FunctionHolderBase<I, void (C::*)(DETAIL_TPARAMS(n)) const, false>   \
	{																  \
		typedef typename void (FunctionHolderBase::*TFuncPtrType)();     \
		void Func(DETAIL_FUNC_PARAMS(n, t))								  \
		{															  \
			DETAIL_REPEAT(n,DETAIL_ACTUALCALL_ITEM,DETAIL_ACTUALCALL_ITEM_END,t);  \
			VT_THIS->AddCallCounter(I);								  \
		}															  \
	};																	

#define	DETAIL_FUNCTIONHOLDER_BUILD_R(n)				\
	template <int I, class R, class C, DETAIL_TPARAMS(n)>				\
	struct FunctionHolderBase<I, R (C::*)(DETAIL_TPARAMS(n)), true>		\
	{																	\
		typedef typename R (FunctionHolderBase::*TFuncPtrType)();		\
		R Func(DETAIL_FUNC_PARAMS(n,t))									\
		{																\
			DETAIL_REPEAT(n,DETAIL_ACTUALCALL_ITEM,DETAIL_ACTUALCALL_ITEM_END,t);  \
			VT_THIS->AddCallCounter(I);									\
			return VT_THIS->Return<R>(I);								\
		}																\
	};			\
	template <int I, class R, class C, DETAIL_TPARAMS(n)>				\
	struct FunctionHolderBase<I, R (C::*)(DETAIL_TPARAMS(n)) const, true>		\
	{																	\
		typedef typename R (FunctionHolderBase::*TFuncPtrType)();		\
		R Func(DETAIL_FUNC_PARAMS(n,t))									\
		{																\
			DETAIL_REPEAT(n,DETAIL_ACTUALCALL_ITEM,DETAIL_ACTUALCALL_ITEM_END,t);  \
			VT_THIS->AddCallCounter(I);									\
			return VT_THIS->Return<R>(I);								\
		}																\
	};

#define DETAIL_FUNCTIONHOLDER_BUILD(n) DETAIL_FUNCTIONHOLDER_BUILD_O(n) \
	DETAIL_FUNCTIONHOLDER_BUILD_R(n)

DETAIL_FUNCTIONHOLDER_BUILD(1);
DETAIL_FUNCTIONHOLDER_BUILD(2);
DETAIL_FUNCTIONHOLDER_BUILD(3);
DETAIL_FUNCTIONHOLDER_BUILD(4);
DETAIL_FUNCTIONHOLDER_BUILD(5);
DETAIL_FUNCTIONHOLDER_BUILD(6);
DETAIL_FUNCTIONHOLDER_BUILD(7);
DETAIL_FUNCTIONHOLDER_BUILD(8);
DETAIL_FUNCTIONHOLDER_BUILD(9);
DETAIL_FUNCTIONHOLDER_BUILD(10);
DETAIL_FUNCTIONHOLDER_BUILD(11);
DETAIL_FUNCTIONHOLDER_BUILD(12);
DETAIL_FUNCTIONHOLDER_BUILD(13);
DETAIL_FUNCTIONHOLDER_BUILD(14);
DETAIL_FUNCTIONHOLDER_BUILD(15);

#undef VT_THIS

}

}

#endif __FUNCTIONHOLDER_HH