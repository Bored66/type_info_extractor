#include <stdio.h>
#include <cstring>
#include "test_types.h"
#ifdef CPP_14
#include "../mimic_type.h"
#include "../seq_tuple.h"
#include "pod_ops.h"
#endif

void test_value_holder_cpp14()
{
#ifdef CPP_14
    constexpr
            auto info = get_pod_meta_infos<ms>();
    auto mimic_uds = mimic_type<user_def_struct1>();
    puts(decltype(mimic_uds)::name());
    auto &uds0 = dynamic_cast<holder<4,seq_tuple<float,double>>&>(mimic_uds);
    uds0.value = {1.1f, 2.3};
    user_def_struct1 uds1;
    memcpy(&uds1, &mimic_uds, sizeof(uds1));
    using ts = build_type<get_type< info.type_ids[0] >::type,
        get_type< info.type_ids[1] >::type,
        get_type< info.type_ids[2] >::type >::result;
    puts(ts::name());
    ts t{0,1,2};
    auto &ts0 = dynamic_cast<holder<0,get_type< info.type_ids[0] >::type>&>(t);
    auto &ts1 = dynamic_cast<holder<1,get_type< info.type_ids[1] >::type>&>(t);
    auto &ts2 = dynamic_cast<holder<2,get_type< info.type_ids[2] >::type>&>(t);
    printf("%d=%d:%d=%d:%d=%d\n", 0, ts0.value, 1, ts1.value, 2, ts2.value);
    printf("%g=%g:%g=%g\n", 1.1, uds1.st.f, 2.3, uds1.st.i);
    const//expr
    auto cnt121 = get_type_total_count<user_def_struct>();(void)cnt121;
    user_def_struct0 UDS0{1.2, 2.3};
    auto f = getNth<0>(mimic_uds);
    printf("getNth<4> %g", f);
    std::cout << getNth<1>(mimic_uds) << " " << getNth<2>(mimic_uds)
               << " " << getNth<3>(mimic_uds)
               //<< " " << getNth<4>(mimic_uds)
               << std::endl;
    //std::cout << "++++" <<
      //           dump_tuple_to_string<0,decltype(mimic_uds)>(mimic_uds);

#endif
}

void print_member_offset(void *st, void *member)
{
#if defined(__code_model_32__) || defined (FORCE_32)
    printf("fact offset: %d ",
#elif defined(WIN64)
    printf("fact offset: %lld ",
#else
    printf("fact offset: %ld ",
#endif
        size_t(member) - size_t(st));
}
template <int I> struct Wrapper
{static constexpr int i = I;};

template <int I> struct Lookup {
    template <typename V> struct Helper
    {  using Type = V; };

    using TypeSzT = typename Helper<std::size_t>::Type;
    using Type = typename Helper<Wrapper<I>>::Type;
};

template <int I> template <int J>
struct Lookup<I>::Helper<Wrapper<J>> {
  using Type = void;
};

using A = typename Lookup<12>::Type; // should be void

void test_offsets()
{
    /*    return;
    size_t offset = 0, padding = 0, adjusted = 0, offset_of = 0, off = 0;
    puts("Offsets:");
    ms ms1;
    calc_dump_memebr_data(ms, ms::in);
    calc_dump_memebr_data(ms, ms::i2);
    calc_dump_memebr_data(ms, ms::i3);
    calc_offset(&ms1, &ms1.ch); calc_dump_memebr_data(ms, ms::ch);
    calc_offset(&ms1, &ms1.sh); calc_dump_memebr_data(ms, ms::sh);
    calc_dump_memebr_data(ms, ms::ch1);
    calc_dump_memebr_data(ms, ms::l);
    calc_dump_memebr_data(ms, ms::d);
    calc_dump_memebr_data(ms, ms::ld);
    calc_dump_memebr_data(ms, ms::fl);
    calc_dump_memebr_data(ms, ms::st);
    off = offset;offset = adjusted = offset_of;
    print_member_offset(&ms1, &ms1.st);calc_dump_memebr_data(user_def_struct, user_def_struct::i);off = 0;
    print_member_offset(&ms1, &ms1.ch2); calc_dump_memebr_data(ms, ms::ch2);
    print_member_offset(&ms1, &ms1.sh1); calc_dump_memebr_data(ms, ms::sh1);
    print_member_offset(&ms1, &ms1.st); calc_dump_memebr_data(user_def_struct, user_def_struct::i);
    print_member_offset(&ms1, &ms1.st.st); calc_dump_memebr_data(user_def_struct, user_def_struct::st);
    calc_dump_memebr_data(user_def_struct, user_def_struct::st2);*/
}
