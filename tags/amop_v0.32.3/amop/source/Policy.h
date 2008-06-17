#ifndef __AMOP_POLICY_HH
#define __AMOP_POLICY_HH

namespace amop
{
    namespace Policy
    {
        //------------------------------------------------------------------
        class TPolicy
        {
        };

        //------------------------------------------------------------------
        template<class T>
        class TPointer : public TPolicy
        {
        public:
            typedef T Type;

            TPointer(T value)
                : mValue(value)
            {
            }

            template <class P>
            bool Compare(P* pointer) const
            {
                return *pointer == mValue;
            }

            template <class P>
            void Assign(P& pointer) const
            {
                *pointer = mValue;
            }

        protected:
            T mValue;
        };

        //------------------------------------------------------------------
        template <class T>
        TPointer<T> Pointer(T value)
        {
            return TPointer<T>(value);
        }

        //------------------------------------------------------------------
        template<class T>
        class TArray : public TPolicy
        {
        public:
            typedef T* Type;

            TArray(T* value, size_t len)
                : mValue(value)
                , mLen(len)
            {
            }

            template <class P>
            bool Compare(P in) const
            {
                for( size_t i = 0; i < mLen; ++ i)
                {
                    if( *(in+i) != *(mValue+i) )
                    {
                        return false;
                    }
                }

                return true;
            }

            template <class P>
            void Assign(P& in) const
            {
                for( size_t i = 0; i < mLen; ++ i )
                {
                    in[i] = mValue[i];
                }
            }

        protected:
            T* mValue;
            size_t mLen;
        };

        //------------------------------------------------------------------
        template <class T>
        TArray<T> Array(T* value, size_t len)
        {
            return TArray<T>(value, len);
        }
    }
}

#endif //__AMOP_POLICY_HH

