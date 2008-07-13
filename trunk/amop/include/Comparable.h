#ifndef __AMOP_COMPARABLE_HH
#define __AMOP_COMPARABLE_HH

#include "Any.h"

namespace amop
{
    namespace detail
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

        class Comparable;

        //------------------------------------------------------------------
        class ComparableBase
        {
        public:
            virtual ~ComparableBase(){}

            virtual bool compare(const any& other) const = 0;	
            virtual void assign(const any& other) = 0;
            virtual ComparableBase* clone() = 0;
        };

        //------------------------------------------------------------------
        template<class From, class To, bool ReadOnly>
        class ComparableImp;

        //------------------------------------------------------------------
        template<class From, class To>
        class ComparableImp<From, To, true> : public ComparableBase
        {
        public:
            ComparableImp(const any& data) : m_data(data) {}

            virtual bool compare(const any& other) const
            {
                return (*(any_cast<const To*>(other))) == (To)(any_cast<From>(m_data));
            }

            virtual void assign(const any& /*other*/) 
            {
            }

            virtual ComparableBase* clone() { return new ComparableImp<From, To, true>(m_data); }

        protected:
            any m_data;
        };

        //------------------------------------------------------------------
        template<class From, class To>
        class ComparableImp<From, To, false> : public ComparableBase
        {
        public:
            ComparableImp(const any& data) : m_data(data) {}

            virtual bool compare(const any& other) const
            {
                return (*(any_cast<const To*>(other))) == (To)(any_cast<From>(m_data));
            }

            virtual void assign(const any& other) 
            {
                *const_cast<To*>(any_cast<const To*>(other)) = (To)any_cast<From>(m_data);
            }

            virtual ComparableBase* clone() { return new ComparableImp<From, To, false>(m_data); }

        protected:
            any m_data;
        };

        template<class P, class To, bool ReadOnly>
        class ComparablePolicyImp;

        //------------------------------------------------------------------
        template<class P, class To>
        class ComparablePolicyImp<P, To, true> : public ComparableBase
        {
        public:
            ComparablePolicyImp(P& policy) : m_policy(policy) {}

            virtual bool compare(const any& other) const
            {
                return m_policy.compare((*(any_cast<const To*>(other))));
            }

            virtual void assign(const any&) 
            {
            }

            virtual ComparableBase* clone() { return new ComparablePolicyImp<P, To, true>(m_policy); }

        protected:
            P m_policy;
        };

        //------------------------------------------------------------------
        template<class P, class To>
        class ComparablePolicyImp<P, To, false> : public ComparableBase
        {
        public:
            ComparablePolicyImp(P& policy) : m_policy(policy) {}

            virtual bool compare(const any& other) const
            {
                return m_policy.compare((*(any_cast<const To*>(other))));
            }

            virtual void assign(const any& other) 
            {
                m_policy.assign(*const_cast<To*>(any_cast<const To*>(other)));
            }

            virtual ComparableBase* clone() { return new ComparablePolicyImp<P, To, false>(m_policy); }

        protected:
            P m_policy;
        };


        //------------------------------------------------------------------
        class Comparable
        {
        public:
            Comparable() : m_holder(NULL) {}

            bool IsEmpty()
            {
                return m_holder == NULL;
            }

            template<class To, bool ReadOnly, class From>
            static Comparable makeCompare(From value)
            {
                Comparable compare;
                compare.m_holder = new ComparableImp<From, To, ReadOnly>(value);
                return compare;
            }

            template<class To, bool ReadOnly, class P >
            static Comparable makeCompareByPolicy(P& policy)
            {
                Comparable compare;
                compare.m_holder = new ComparablePolicyImp<P, To, ReadOnly>(policy);
                return compare;
            }

            Comparable(const Comparable& other)
            {
                m_holder = other.m_holder ? other.m_holder->clone() : NULL;
            }

            ~Comparable()
            {
                delete m_holder;
            }

            Comparable& operator= (const Comparable& other)
            {
                delete m_holder;
                m_holder = other.m_holder ? other.m_holder->clone() : NULL;

                return *this;
            }	

            bool operator == (const any& other) const
            {
                return m_holder->compare(other);
            }	

            bool operator != (const any& other) const
            {
                return !(*this == other);
            }

            Comparable& assign (const any& other)
            {
                m_holder->assign(other);

                return *this;
            }

            ComparableBase* m_holder;
        };

    }
}

#endif //__AMOP_COMPARABLE_HH

