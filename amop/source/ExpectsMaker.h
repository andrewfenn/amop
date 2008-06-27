#ifndef __AMOP_EXPECTSMAKER__HH
#define __AMOP_EXPECTSMAKER__HH

namespace amop
{
    template <typename F, int L, typename S>
    class TExpectsMaker;

    template <typename F, typename S>
    class TExpectsMakerBase
    {
    public:
        template <int I>
        struct P
        {
            typedef typename Detail::Get< typename Detail::Functor<F>::ParameterTypes, I >::Type Type;
        };

        void SetSelf(S* p)
        {
            Self = p;
        }

        S* Self;
    };

    template <typename F, typename S>
    class TExpectsMaker<F, 0, S> : public TExpectsMakerBase<F,S>
    {
    public:
        S Expects()            
        {
            return Self->_Expects(All());
        }        
    };

    // Marco Debug only 
    /*template <typename F, typename S>
    class TExpectsMaker<F, 3, S> : public TExpectsMakerBase<F,S>
    {                                                   
    public:                                             
        template<DETAIL_TPARAMS(3)>                     
        S Expects(DETAIL_FUNC_PARAMS(3,t))               
        {                                               
            return Self->_Expects(All(DETAIL_ARGS_P(3))); 
        }                                               
    };*/

#define DETAIL_MAKE_EXPECT_MAKER(i)     \
    template <typename F, typename S>   \
    class TExpectsMaker<F, i, S> : public TExpectsMakerBase<F,S>    \
    {                                                   \
    public:                                             \
        template<DETAIL_TPARAMS(i)>                     \
        S Expects(DETAIL_FUNC_PARAMS(i,t))               \
        {                                               \
            return Self->_Expects(All(DETAIL_ARGS_P(i))); \
        }                                               \
    }

DETAIL_MAKE_EXPECT_MAKER(1);
DETAIL_MAKE_EXPECT_MAKER(2);
DETAIL_MAKE_EXPECT_MAKER(3);
DETAIL_MAKE_EXPECT_MAKER(4);
DETAIL_MAKE_EXPECT_MAKER(5);
DETAIL_MAKE_EXPECT_MAKER(6);
DETAIL_MAKE_EXPECT_MAKER(7);
DETAIL_MAKE_EXPECT_MAKER(8);

}

#endif //__AMOP_EXPECTSMAKER__HH