#ifndef __AMOP_CONFIG__HH
#define __AMOP_CONFIG__HH

#include <exception>

namespace amop
{

#define MAX_NUM_VIRTUAL_FUNCTIONS 50

#define DETAIL_TYPELIST_WITHOUT_HEAD typename T2,typename T3, typename T4, \
    typename T5, typename T6, typename T7, typename T8, typename T9, \
    typename T10, typename T11, typename T12, typename T13, typename T14, \
    typename T15

#define DETAIL_TYPELIST typename T1, DETAIL_TYPELIST_WITHOUT_HEAD

#define DETAIL_TYPELIST_DEFAULT typename T1=Empty, typename T2=Empty,typename T3=Empty, typename T4=Empty, \
    typename T5=Empty, typename T6=Empty, typename T7=Empty, typename T8=Empty, typename T9=Empty, \
    typename T10=Empty, typename T11=Empty, typename T12=Empty, typename T13=Empty, typename T14=Empty, \
    typename T15=Empty

#define DETAIL_TYPELIST_EMPTY Empty, Empty, Empty, Empty, \
    Empty, Empty, Empty, Empty, Empty, \
    Empty, Empty, Empty, Empty, Empty, \
    Empty


namespace detail
{

        struct Empty {};

        template <class T1, class T2>
        struct IsEqual { enum { Result = false }; };

        template <class T>
        struct IsEqual<T,T> { enum { Result = true }; };

        template <class T>
        struct IsPointerToMethod
        {
            enum {Result = 1 };
        };

        template <class T>
        struct RemoveReference
        {
            typedef T Type;
            typedef const T ConstType;
        };

        template <class T>
        struct RemoveReference<T&>
        {
            typedef T Type;
            typedef const T ConstType;
        };

        template <class T>
        struct RemoveReference<const T&>
        {
            typedef T Type;
            typedef const T ConstType;
        };

        // This union is declared outside the horrible_cast because BCC 5.5.1
        // can't inline a function with a nested class, and gives a warning.
        template <class OutputClass, class InputClass>
        union HorribleUnion{
            OutputClass out;
            InputClass in;
        };

#ifdef _MSC_VER

	#ifndef _WIN64
		#define DETAIL_HAVE_THIS_CALL 1
	#endif

	#define DETAIL_CDECL __cdecl

        template <class OutputClass, class InputClass>
        inline OutputClass HorribleCast(const InputClass input){
            HorribleUnion<OutputClass, InputClass> u;
            // Cause a compile-time error if in, out and u are not the same size.
            // If the compile fails here, it means the compiler has peculiar
            // unions which would prevent the cast from working.
            typedef int ERROR_CantUseHorrible_cast[sizeof(InputClass)==sizeof(u) 
                && sizeof(InputClass)==sizeof(OutputClass) ? 1 : -1];
            u.in = input;
            return u.out;
        }

        //*************************************************************************
        // Because some compiler still do not support TR1, so i create one for it
        //*************************************************************************
        template <class T> class SmartPtr
        {
        public:
            // Constructors
            SmartPtr()
            {
                m_ptr = NULL;
                m_count = new int;
                *m_count = 1;
            }

            SmartPtr(const SmartPtr<T> &original)
            {
                m_ptr = original.m_ptr;
                m_count = original.m_count;
                (*m_count)++;
            }

            SmartPtr(T* original)
            {
                m_ptr = original;
                m_count = new int;
                *m_count = 1;
            }

            ~SmartPtr()
            {
                release();
            }


            //assignment
            SmartPtr<T> &operator=(const SmartPtr &rhs)
            {
                if (this == &rhs)
                    return *this;

                release();
                m_ptr = rhs.m_ptr;
                m_count = rhs.m_count;
                acquire();

                return *this;
            }

            SmartPtr<T> &operator=(const T* rhs)
            {
                if (ptr == rhs)
                    return *this;

                release();
                m_ptr = m_rhs;
                m_count = new int;
                *m_count = 1;
            }
            //Use
            T* operator->()
            {
                return m_ptr;
            }

        private:
            T* m_ptr;
            int* m_count;

            void acquire()
            {
                (*m_count)++;
            }

            void release()
            {
                (*m_count)--;
                if (!(*m_count))
                {
                    delete m_count;

                    if (m_ptr)
                        delete m_ptr;

                    m_ptr = NULL;
                }

            }
            // Comparison
            bool operator>(const SmartPtr<T> &rhs)
            {
                return this>&rhs;
            }
        };

	#define AmopSharedPtr detail::SmartPtr
#endif


#ifdef __BORLANDC__

	// BCB already uses default C calling convention
	#define DETAIL_CDECL

	struct BorlandMFP
	{
		void* funcadr;
		int delta;
		int vtable; // or 0 if no virtual inheritance
	};

        template <class OutputClass, class InputClass>
	inline OutputClass HorribleCast(const InputClass input){
	    HorribleUnion<BorlandMFP, InputClass> u;
	    // Cause a compile-time error if in, out and u are not the same size.
	    // If the compile fails here, it means the compiler has peculiar
	    // unions which would prevent the cast from working.
	    typedef int ERROR_CantUseHorrible_cast[sizeof(InputClass)==sizeof(u)
		&& sizeof(InputClass)==sizeof(BorlandMFP) ? 1 : -1];
	    u.in = input;
	    return u.out.funcadr;
	}

	// Version 2009+ comes with boost
	#define AmopSharedPtr std::tr1::shared_ptr

#endif


#ifdef __GNUC__

	#define DETAIL_CDECL

	struct GnuMFP
	{
	    union
	    {
		void* funcadr;
		int vtable_index_2; // = vindex*2+1, always odd
	    };
	    int delta;
	};

	template <class OutputClass, class InputClass>
	inline OutputClass HorribleCast(const InputClass input){
	    HorribleUnion<GnuMFP, InputClass> u;
	    // Cause a compile-time error if in, out and u are not the same size.
	    // If the compile fails here, it means the compiler has peculiar
	    // unions which would prevent the cast from working.
	    typedef int ERROR_CantUseHorrible_cast[sizeof(InputClass)==sizeof(u)
		&& sizeof(InputClass)==sizeof(GnuMFP) ? 1 : -1];
	    u.in = input;
	    return u.out.funcadr;
	}

	#define AmopSharedPtr std::tr1::shared_ptr
#endif


typedef void* FunctionAddress;

} // namespace detail

} // namespace amop


#endif //__AMOP_CONFIG__HH

