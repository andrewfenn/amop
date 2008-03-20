#ifndef __AMOP_COMPARABLE_HH
#define __AMOP_COMPARABLE_HH

#include "Any.h"

namespace amop
{
namespace Detail
{

template <class From, class To>
struct IsConvertible
{
	struct Yes { char c[10]; };;
	typedef char No;

	static From MakeFrom();
	static No Convertor(...);
	static Yes Convertor(To);

	enum { Result = (sizeof (Convertor(MakeFrom())) == sizeof(Yes)) };
};

template <bool, class T1, class T2>
struct Selector
{
    typedef T1 Type;
};

template <class T1, class T2>
struct Selector<false, T1, T2>
{
    typedef T2 Type;
};
	
class TComparable;

//------------------------------------------------------------------
class TComparableBase
{
public:
	virtual ~TComparableBase(){}

	virtual bool Compare(const any& other) const = 0;	
    virtual void Assign(const any& other) = 0;
	virtual TComparableBase* Clone() = 0;
};

//------------------------------------------------------------------
template<class From, class To>
class TComparableImp : public TComparableBase
{
public:
	TComparableImp(const any& data) : mData(data) {}

	virtual bool Compare(const any& other) const
	{
		return (*(any_cast<const To*>(other))) == (To)(any_cast<From>(mData));
	}

    virtual void Assign(const any& other) 
    {
        *const_cast<To*>(any_cast<const To*>(other)) = (To)any_cast<From>(mData);
    }

	virtual TComparableBase* Clone() { return new TComparableImp<From, To>(mData); }

protected:
	any mData;
};

//------------------------------------------------------------------
template<class P, class To>
class TComparablePolicyImp : public TComparableBase
{
public:
    TComparablePolicyImp(P& policy) : mPolicy(policy) {}

	virtual bool Compare(const any& other) const
	{
		return mPolicy.Compare((*(any_cast<const To*>(other))));
	}

    virtual void Assign(const any& other) 
    {
        return mPolicy.Assign(*const_cast<To*>(any_cast<const To*>(other)));
    }

	virtual TComparableBase* Clone() { return new TComparablePolicyImp<P, To>(mPolicy); }

protected:
	P mPolicy;
};


//------------------------------------------------------------------
class TComparable
{
public:
	TComparable() : mHolder(NULL) {}

	template<class To, class From>
	static TComparable MakeCompare(From value)
	{
		TComparable compare;
		compare.mHolder = new TComparableImp<From, To>(value);
		return compare;
	}

    template<class To, class P>
    static TComparable MakeCompareByPolicy(P& policy)
    {
        TComparable compare;
        compare.mHolder = new TComparablePolicyImp<P, To>(policy);
        return compare;
    }

	TComparable(const TComparable& other)
	{
		mHolder = other.mHolder ? other.mHolder->Clone() : NULL;
	}

	~TComparable()
	{
		delete mHolder;
	}

	TComparable& operator= (const TComparable& other)
	{
		delete mHolder;
		mHolder = other.mHolder ? other.mHolder->Clone() : NULL;

		return *this;
	}	

	bool operator == (const any& other) const
	{
		return mHolder->Compare(other);
	}	

	bool operator != (const any& other) const
	{
		return !(*this == other);
	}

    TComparable& Assign (const any& other)
    {
        mHolder->Assign(other);

        return *this;
    }

	TComparableBase* mHolder;
};

}
}

#endif //__AMOP_COMPARABLE_HH

