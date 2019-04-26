#include <cstring>
#include "test_types.h"
#include "../pod_inspector.h"
#include "../seq_tuple.h"
#include "pod_ops.h"
#include "tests.h"

void run_all_tests()
{
    field_count_test();
    type_decode_test();
    user_def_struct1 uds1{ nullptr,1,2,3,{ 4, 5 },6,7};
    test_type(uds1);
    //test_value_holder_cpp14();
    test_pod_ops();
}

void test_pod_ops()
{
#if USE_CPP_11_WAY
    user_def_struct1 uds1 {nullptr,1,2,'3',{4.4f,5.5},'6',7};
    simple_enclosure se{11,uds1,0xffff,1.2f,2.3f};
    simple_enclosure2 se2{11,uds1,1,{1.2f,2.3},1};
#ifdef CPP_11
    auto counter_se = get_type_total_count<decltype(se)>();
    auto counter_se2 = get_type_total_count<decltype(se2)>();
	for (int i = 0; i < 100; i++)
	{
        if (i % 2 == 0)
        {
            auto info = get_pod_meta_infos<decltype(se)>();
            (void)info;
        }
        else
        {
            auto info2 = get_pod_meta_infos<decltype(se2)>();
            (void)info2;
        }
	}
#else
    dump_struct(uds1);
    dump_struct(se);
    dump_struct(se2);
    test_pod_ops_2<5,6>(se);
    test_pod_ops_2<5,6>(se2);
    constexpr auto counter_se = get_type_total_count<decltype(se)>();
    test_pod_ops_2<counter_se-2,counter_se-1>(se);
    constexpr auto counter_se2 = get_type_total_count<decltype(se2)>();
    test_pod_ops_2<counter_se2-2,counter_se2-1>(se2);
#endif
#endif
}
void type_decode_test()
{

    using test_type0 = short* ;
    using test_type1 = short const* ;
    using test_type2 = short const* const volatile* ;
    using test_type3 = short const * const volatile* volatile *;
    using test_type4 = const short * const volatile* volatile * const*;
    using test_type5 = short const * const volatile* volatile * const*
            const * const volatile* volatile const* ;

    //test_type_cyphers<test_type0, test_type1, test_type2,
      //      test_type3, test_type4, test_type5>();
    constexpr auto tt5 = encode_type<test_type5>();
    using decoded_type = decode_cypher<tt5>::type;

    static_assert(std::is_same<decoded_type, test_type5 >::value, "");
    test_type_cyphers_type<test_type0, test_type1, test_type2,
            test_type3, test_type4, test_type5>();

}

void type_naming_probe()
{
    using t1 = long long const*; using t2 = long volatile const*;
    using t3 = int const*; using t10 = long long *const;
    using t20 = long volatile *const; using t30 = const int*const;
    using t11 = long*const; using t12 = const long *;
    using t13 = const char volatile *; using t111 = const long*const*;
    using t121 = long *const*volatile const*const*; using t131 = const double*const*;
    using usd = user_def_struct const*;
    using usd1 =  const volatile user_def_struct*;
    using usd2 =  user_def_struct volatile * volatile const*;
    dump_types<t1, t2, t3, t10, t20, t30, t11, t12, t13,
            t111, t121, t131, usd, usd1, usd2>();
    test_type_cyphers_type<t1, t2, t3, t10, t20, t30, t11, t12, t13,
            t111, t121, t131, usd, usd1, usd2>();
}
void field_count_test()
{
    test_count<user_def_struct0, user_def_struct1,
            user_def_struct, ms, almost_empty,
            TYPE_KOEFF_4, TYPE_TEMPERATURE,
        #ifdef TEST_BIT_FIELDS_TOO
        #ifndef __clang__

            STATUS_BITS,
        #endif
        #endif
            compose_rppt>();
}

size_t calc_offset_padding(size_t & offset, size_t & padding, size_t align)
{
    padding = (-offset) & (align - 1);
    offset = offset + padding;
    return offset;
}
size_t adjust_offset(size_t offset, size_t align)
{
    size_t padding = (-offset) & (align - 1);
    offset = offset + padding;
    return offset;
}
