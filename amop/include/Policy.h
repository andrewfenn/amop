#ifndef __AMOP_POLICY_HH
#define __AMOP_POLICY_HH

namespace amop
{
    namespace policy
    {
        //------------------------------------------------------------------
        class Policy
        {
        };

        //------------------------------------------------------------------
        template<class T>
        class PointerPolicy : public Policy
        {
        public:
            typedef T Type;

            PointerPolicy(T value)
                : m_value(value)
            {
            }

            template <class P>
            bool compare(P* pointer) const
            {
                return *pointer == m_value;
            }

            template <class P>
            void assign(P& pointer) const
            {
                *pointer = m_value;
            }

        protected:
            T m_value;
        };

        //------------------------------------------------------------------
        template <class T>
        PointerPolicy<T> pointer(T value)
        {
            return PointerPolicy<T>(value);
        }

        //------------------------------------------------------------------
        template<class T>
        class ArrayPolicy : public Policy
        {
        public:
            typedef T* Type;

            ArrayPolicy(T* value, size_t len)
                : m_value(value)
                , m_len(len)
            {
            }

            template <class P>
            bool compare(P in) const
            {
                for( size_t i = 0; i < m_len; ++ i)
                {
                    if( *(in+i) != *(m_value+i) )
                    {
                        return false;
                    }
                }

                return true;
            }

            template <class P>
            void assign(P& in) const
            {
                for( size_t i = 0; i < m_len; ++ i )
                {
                    in[i] = m_value[i];
                }
            }

        protected:
            T* m_value;
            size_t m_len;
        };

        //------------------------------------------------------------------
        template <class T>
        ArrayPolicy<T> array(T* value, size_t len)
        {
            return ArrayPolicy<T>(value, len);
        }
    }
}

#endif //__AMOP_POLICY_HH

