#ifndef __AMOP_EXPECTSMAKER__HH
#define __AMOP_EXPECTSMAKER__HH

namespace amop
{
    namespace detail
    {
        template <DETAIL_TPARAMS_DEF(15, Empty)> 
        struct ExpectAll
        {
            ExpectAll(DETAIL_ARGS_DEFAULT(15))
                : p0(t1)
                , p1(t2)
                , p2(t3)
                , p3(t4)
                , p4(t5)
                , p5(t6)
                , p6(t7)
                , p7(t8)
                , p8(t9)
                , p9(t10)
                , p10(t11)
                , p11(t12)
                , p12(t13)
                , p13(t14)
                , p14(t15)
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
            T9 p8;
            T10 p9;
            T11 p10;
            T12 p11;
            T13 p12;
            T14 p13;
            T15 p14;
        };


        namespace 
        {

            detail::ExpectAll<> all()
            {                                 
                return detail::ExpectAll<>();
            }

#define DETAIL_ALL_MAKER_BUILD(i)       \
    template <DETAIL_TPARAMS(i)>                                            \
    static detail::ExpectAll< DETAIL_ARGS(i) > all( DETAIL_FUNC_PARAMS(i,t) )           \
    {                                                                       \
        return detail::ExpectAll<DETAIL_ARGS(i)>(DETAIL_ARGS_P(i));              \
    }

DETAIL_ALL_MAKER_BUILD(1);
DETAIL_ALL_MAKER_BUILD(2);
DETAIL_ALL_MAKER_BUILD(3);
DETAIL_ALL_MAKER_BUILD(4);
DETAIL_ALL_MAKER_BUILD(5);
DETAIL_ALL_MAKER_BUILD(6);
DETAIL_ALL_MAKER_BUILD(7);
DETAIL_ALL_MAKER_BUILD(8);
DETAIL_ALL_MAKER_BUILD(9);
DETAIL_ALL_MAKER_BUILD(10);
DETAIL_ALL_MAKER_BUILD(11);
DETAIL_ALL_MAKER_BUILD(12);
DETAIL_ALL_MAKER_BUILD(13);
DETAIL_ALL_MAKER_BUILD(14);
DETAIL_ALL_MAKER_BUILD(15);

        }

        template <typename F, int L, typename S>
        class ExpectMaker;

        template <typename F, typename S>
        class ExpectMakerBase
        {
        public:
            ExpectMakerBase()
            {
            }

            template <int I>
            struct P
            {
                typedef typename detail::Get< typename detail::Functor<F>::ParameterTypes, I >::Type Type;
            };            

            S* getSelf()
            {
                return ((S*)this);
            }
        };

        template <typename F, typename S>
        class ExpectMaker<F, 0, S> : public ExpectMakerBase<F,S>
        {
        public:
            S expect()            
            {
                return ExpectMakerBase<F,S>::getSelf()->expectInternal(all());
            }        
        };

        // Marco Debug only 
        /*template <typename F, typename S>
        class ExpectMaker<F, 3, S> : public ExpectMakerBase<F,S>
        {                                                   
        public:                                             
            template<DETAIL_TPARAMS(3)>                     
            S Expect(DETAIL_FUNC_PARAMS(3,t))               
            {                                               
                return ExpectMakerBase<F,S>::Self->_Expect(all(DETAIL_ARGS_P(3))); 
            }                                               
        };*/

    #define DETAIL_MAKE_EXPECT_MAKER(i)     \
        template <typename F, typename S>   \
        class ExpectMaker<F, i, S> : public ExpectMakerBase<F,S>    \
        {                                                   \
        public:                                             \
            template<DETAIL_TPARAMS(i)>                     \
            S expect(DETAIL_FUNC_PARAMS(i,t))               \
            {                                               \
                return ExpectMakerBase<F,S>::getSelf()->expectInternal(all(DETAIL_ARGS_P(i))); \
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
    DETAIL_MAKE_EXPECT_MAKER(9);
    DETAIL_MAKE_EXPECT_MAKER(10);
    DETAIL_MAKE_EXPECT_MAKER(11);
    DETAIL_MAKE_EXPECT_MAKER(12);
    DETAIL_MAKE_EXPECT_MAKER(13);
    DETAIL_MAKE_EXPECT_MAKER(14);
    DETAIL_MAKE_EXPECT_MAKER(15);
    }
}

#endif //__AMOP_EXPECTSMAKER__HH


