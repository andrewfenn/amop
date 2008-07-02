#ifndef __AMOP_EXPECTSMAKER__HH
#define __AMOP_EXPECTSMAKER__HH

namespace amop
{
    namespace Detail
    {
        template <DETAIL_TPARAMS_DEF(8, Empty)> 
        struct TExpectAll
        {
            TExpectAll(DETAIL_ARGS_DEFAULT(8))
                : p0(t1)
                , p1(t2)
                , p2(t3)
                , p3(t4)
                , p4(t5)
                , p5(t6)
                , p6(t7)
                , p7(t8)
            {
            }
            
            T1 p0;
            T2 p1;
            T3 p2;
            T4 p3;
            
            T5 p4;
            T6 p5;
            T7 p6;
            T8 p7;
        };

        Detail::TExpectAll<> All()
        {                                 
            return Detail::TExpectAll<>();
        }

#define DETAIL_ALL_MAKER_BUILD(i)       \
    template <DETAIL_TPARAMS(i)>                                            \
    Detail::TExpectAll< DETAIL_ARGS(i) > All( DETAIL_FUNC_PARAMS(i,t) )           \
    {                                                                       \
        return Detail::TExpectAll<DETAIL_ARGS(i)>(DETAIL_ARGS_P(i));              \
    }

DETAIL_ALL_MAKER_BUILD(1);
DETAIL_ALL_MAKER_BUILD(2);
DETAIL_ALL_MAKER_BUILD(3);
DETAIL_ALL_MAKER_BUILD(4);
DETAIL_ALL_MAKER_BUILD(5);
DETAIL_ALL_MAKER_BUILD(6);
DETAIL_ALL_MAKER_BUILD(7);
DETAIL_ALL_MAKER_BUILD(8);


        template <typename F, int L, typename S>
        class TExpectMaker;

        template <typename F, typename S>
        class TExpectMakerBase
        {
        public:
            TExpectMakerBase()
            {
            }

            template <int I>
            struct P
            {
                typedef typename Detail::Get< typename Detail::Functor<F>::ParameterTypes, I >::Type Type;
            };            

            S* GetSelf()
            {
                return ((S*)this);
            }
        };

        template <typename F, typename S>
        class TExpectMaker<F, 0, S> : public TExpectMakerBase<F,S>
        {
        public:
            S Expect()            
            {
                return TExpectMakerBase<F,S>::GetSelf()->_Expect(All());
            }        
        };

        // Marco Debug only 
        /*template <typename F, typename S>
        class TExpectMaker<F, 3, S> : public TExpectMakerBase<F,S>
        {                                                   
        public:                                             
            template<DETAIL_TPARAMS(3)>                     
            S Expect(DETAIL_FUNC_PARAMS(3,t))               
            {                                               
                return TExpectMakerBase<F,S>::Self->_Expect(All(DETAIL_ARGS_P(3))); 
            }                                               
        };*/

    #define DETAIL_MAKE_EXPECT_MAKER(i)     \
        template <typename F, typename S>   \
        class TExpectMaker<F, i, S> : public TExpectMakerBase<F,S>    \
        {                                                   \
        public:                                             \
            template<DETAIL_TPARAMS(i)>                     \
            S Expect(DETAIL_FUNC_PARAMS(i,t))               \
            {                                               \
                return TExpectMakerBase<F,S>::GetSelf()->_Expect(All(DETAIL_ARGS_P(i))); \
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
}

#endif //__AMOP_EXPECTSMAKER__HH


