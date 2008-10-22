#ifndef __AMOP_FUNCTIONHOLDER_HH
#define __AMOP_FUNCTIONHOLDER_HH

#include "Config.h"
#include "Functor.h"

namespace amop
{
namespace detail
{
	class ObjectHolder;
    
    ObjectHolder* getHolderFromThis(void* pointer);    
    
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
#define VT_THIS getHolderFromThis(this)
#endif


#ifndef HANDLE_REDIRECT_R
#define HANDLE_REDIRECT_R			\
	do { \
	if(VT_THIS->template redirector<FunctorType>(I)){ \
		return VT_THIS->template redirector<FunctorType>(I)(DETAIL_FUNC_PARAMS(n, t));	\
		}	\
	}while(false)
#endif

#ifndef HANDLE_REDIRECT
#define HANDLE_REDIRECT			\
	do { \
	if(VT_THIS->template redirector(I)){ \
		VT_THIS->template redirector<FunctorType>(I)(DETAIL_FUNC_PARAMS(n, t));	\
		}	\
	}while(false)
#endif
	
	template <int I, class C>
	struct FunctionHolderBase<I, void (DETAIL_CDECL C::*)(), false>
	{			
		typedef void (DETAIL_CDECL C::*FunctorType)();
        
        void DETAIL_CDECL func() { 
			VT_THIS->addCallCounter(I); 
			if(VT_THIS->template redirector<FunctorType>(I))
				VT_THIS->template redirector<FunctorType>(I)();
		}
	};

	template <int I, class C>
	struct FunctionHolderBase<I, void (DETAIL_CDECL C::*)() const, false>
	{			
		typedef void (DETAIL_CDECL C::*FunctorType)();
		void DETAIL_CDECL func() 
		{ 
			VT_THIS->addCallCounter(I); 
			
			if(VT_THIS->template redirector<FunctorType>(I))
			{
				VT_THIS->template redirector<FunctorType>(I)();
			}
		}
	};

	//------------------------------------------------------------------
	template <int I, class R, class C>
	struct FunctionHolderBase<I, R (DETAIL_CDECL C::*)(), true>
	{			
		typedef R (DETAIL_CDECL C::*FunctorType)();
		R DETAIL_CDECL func() 
		{ 
			VT_THIS->addCallCounter(I);

			if(VT_THIS->template redirector<FunctorType>(I))
			{
				return VT_THIS->template redirector<FunctorType>(I)();
			}

			return VT_THIS->template returning<R>(I);
		}
	};

	//------------------------------------------------------------------
	template <int I, class R, class C>
	struct FunctionHolderBase<I, R (DETAIL_CDECL C::*)() const, true>
	{			
		typedef R (DETAIL_CDECL C::*FunctorType)();
		R DETAIL_CDECL func() 
		{ 
			VT_THIS->addCallCounter(I);

			if(VT_THIS->template redirector<FunctorType>(I))
			{
				return VT_THIS->template redirector<FunctorType>(I)();
			}

			return VT_THIS->template returning<R>(I);
		}
	};

#ifdef DETAIL_HAVE_THIS_CALL
    //---------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------
    // THIS CALL VERSIONS
    //---------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------

    template <int I, class C>
	struct FunctionHolderBase<I, void (__thiscall C::*)(), false>
	{			
		typedef void (__thiscall C::*FunctorType)();
        
        void __thiscall func() { 
			VT_THIS->addCallCounter(I); 
			if(VT_THIS->template redirector<FunctorType>(I))
				VT_THIS->template redirector<FunctorType>(I)();
		}
	};

	template <int I, class C>
	struct FunctionHolderBase<I, void (__thiscall C::*)() const, false>
	{			
		typedef void (__thiscall C::*FunctorType)();
		void __thiscall func() 
		{ 
			VT_THIS->addCallCounter(I); 
			
			if(VT_THIS->template redirector<FunctorType>(I))
			{
				VT_THIS->template redirector<FunctorType>(I)();
			}
		}
	};

	//------------------------------------------------------------------
	template <int I, class R, class C>
	struct FunctionHolderBase<I, R (__thiscall C::*)(), true>
	{			
		typedef R (__thiscall C::*FunctorType)();
		R __thiscall func() 
		{ 
			VT_THIS->addCallCounter(I);

			if(VT_THIS->template redirector<FunctorType>(I))
			{
				return VT_THIS->template redirector<FunctorType>(I)();
			}

			return VT_THIS->template returning<R>(I);
		}
	};

	//------------------------------------------------------------------
	template <int I, class R, class C>
	struct FunctionHolderBase<I, R (__thiscall C::*)() const, true>
	{			
		typedef R (__thiscall C::*FunctorType)();
		R __thiscall func() 
		{ 
			VT_THIS->addCallCounter(I);

			if(VT_THIS->template redirector<FunctorType>(I))
			{
				return VT_THIS->template redirector<FunctorType>(I)();
			}

			return VT_THIS->template returning<R>(I);
		}
	};
#endif 


#define DETAIL_ACTUALCALL_ITEM(n, t)		VT_THIS->template actual<T##n>(I,DETAIL_DEC(n),t##n);
#define DETAIL_ACTUALCALL_ITEM_END(n, t)	VT_THIS->template actual<T##n>(I,DETAIL_DEC(n),t##n);


	//------------------------------------------------------------------
#define	DETAIL_FUNCTIONHOLDER_BUILD_O(n)				\
	template <int I, class C, DETAIL_TPARAMS(n)>	\
	struct FunctionHolderBase<I, void (DETAIL_CALLING_CONVENTION C::*)(DETAIL_ARGS(n)), false>   \
	{																  \
		typedef void (DETAIL_CALLING_CONVENTION C::*FunctorType)(DETAIL_FUNC_PARAMS(n, t));		\
		void func(DETAIL_FUNC_PARAMS(n, t))								  \
		{															  \
			DETAIL_REPEAT(n,DETAIL_ACTUALCALL_ITEM,DETAIL_ACTUALCALL_ITEM_END,t);  \
			VT_THIS->addCallCounter(I);								  \
			if(VT_THIS->template redirector<FunctorType>(I))					\
				VT_THIS->template redirector<FunctorType>(I)(DETAIL_ARGS_P(n));	\
		}															  \
	};																\
	template <int I, class C, DETAIL_TPARAMS(n)>						\
	struct FunctionHolderBase<I, void (DETAIL_CALLING_CONVENTION C::*)(DETAIL_ARGS(n)) const, false>   \
	{																  \
		typedef void (DETAIL_CALLING_CONVENTION C::*FunctorType)(DETAIL_FUNC_PARAMS(n, t));		\
		void DETAIL_CALLING_CONVENTION func(DETAIL_FUNC_PARAMS(n, t))								  \
		{															  \
			DETAIL_REPEAT(n,DETAIL_ACTUALCALL_ITEM,DETAIL_ACTUALCALL_ITEM_END,t);  \
			VT_THIS->addCallCounter(I);								  \
			if(VT_THIS->template redirector<FunctorType>(I))					\
				VT_THIS->template redirector<FunctorType>(I)(DETAIL_ARGS_P(n));	\
		}															  \
	};																	

#define	DETAIL_FUNCTIONHOLDER_BUILD_R(n)				\
	template <int I, class R, class C, DETAIL_TPARAMS(n)>				\
	struct FunctionHolderBase<I, R (DETAIL_CALLING_CONVENTION C::*)(DETAIL_ARGS(n)), true>		\
	{																	\
		typedef R (DETAIL_CALLING_CONVENTION C::*FunctorType)(DETAIL_FUNC_PARAMS(n, t));		\
		R DETAIL_CALLING_CONVENTION func(DETAIL_FUNC_PARAMS(n,t))									\
		{																\
			DETAIL_REPEAT(n,DETAIL_ACTUALCALL_ITEM,DETAIL_ACTUALCALL_ITEM_END,t);  \
			VT_THIS->addCallCounter(I);									\
			if(VT_THIS->template redirector<FunctorType>(I))					\
				return VT_THIS->template redirector<FunctorType>(I)(DETAIL_ARGS_P(n));	\
			return VT_THIS->template returning<R>(I);								\
		}																\
	};			\
	template <int I, class R, class C, DETAIL_TPARAMS(n)>				\
	struct FunctionHolderBase<I, R (DETAIL_CALLING_CONVENTION C::*)(DETAIL_ARGS(n)) const, true>		\
	{																	\
		typedef R (DETAIL_CALLING_CONVENTION C::*FunctorType)(DETAIL_FUNC_PARAMS(n, t));		\
		R DETAIL_CALLING_CONVENTION func(DETAIL_FUNC_PARAMS(n,t))									\
		{																\
			DETAIL_REPEAT(n,DETAIL_ACTUALCALL_ITEM,DETAIL_ACTUALCALL_ITEM_END,t);  \
			VT_THIS->addCallCounter(I);									\
			if(VT_THIS->template redirector<FunctorType>(I))					\
				return VT_THIS->template redirector<FunctorType>(I)(DETAIL_ARGS_P(n));	\
			return VT_THIS->template returning<R>(I);								\
		}																\
	};

#define DETAIL_FUNCTIONHOLDER_BUILD(n) DETAIL_FUNCTIONHOLDER_BUILD_O(n) \
	DETAIL_FUNCTIONHOLDER_BUILD_R(n)

#define DETAIL_CALLING_CONVENTION DETAIL_CDECL

DETAIL_FUNCTIONHOLDER_BUILD(1);
DETAIL_FUNCTIONHOLDER_BUILD(2);
DETAIL_FUNCTIONHOLDER_BUILD(3);
DETAIL_FUNCTIONHOLDER_BUILD(4);
DETAIL_FUNCTIONHOLDER_BUILD(5);
DETAIL_FUNCTIONHOLDER_BUILD(6);
DETAIL_FUNCTIONHOLDER_BUILD(7);
DETAIL_FUNCTIONHOLDER_BUILD(8);

#ifdef DETAIL_HAVE_THIS_CALL
#undef DETAIL_CALLING_CONVENTION 
#define DETAIL_CALLING_CONVENTION __thiscall 

DETAIL_FUNCTIONHOLDER_BUILD(1);
DETAIL_FUNCTIONHOLDER_BUILD(2);
DETAIL_FUNCTIONHOLDER_BUILD(3);
DETAIL_FUNCTIONHOLDER_BUILD(4);
DETAIL_FUNCTIONHOLDER_BUILD(5);
DETAIL_FUNCTIONHOLDER_BUILD(6);
DETAIL_FUNCTIONHOLDER_BUILD(7);
DETAIL_FUNCTIONHOLDER_BUILD(8);

#endif

#undef DETAIL_CALLING_CONVENTION

/*
DETAIL_FUNCTIONHOLDER_BUILD(9);
DETAIL_FUNCTIONHOLDER_BUILD(10);
DETAIL_FUNCTIONHOLDER_BUILD(11);
DETAIL_FUNCTIONHOLDER_BUILD(12);
DETAIL_FUNCTIONHOLDER_BUILD(13);
DETAIL_FUNCTIONHOLDER_BUILD(14);
DETAIL_FUNCTIONHOLDER_BUILD(15);
*/

#undef VT_THIS
#undef HANDLE_REDIRECT
#undef HANDLE_REDIRECT_R
}

}

#endif //__FUNCTIONHOLDER_HH
