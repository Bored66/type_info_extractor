#pragma once

#include <iostream>
#include <typeinfo>
#include <memory.h>
//#define USE_QT_OUTPUT
#ifdef USE_QT_OUTPUT
#include <QDebug>
#endif

#if defined(CPP_14) || defined(TRY_CPP_14_TESTS)
#include "../mimic_type.h"
#endif
//#include "total_count_obsolete.h"
#include "pod_ops.h"

#include "../flip_bytes_bits.h"
#include "../seq_tuple.h"
template<class...PODs>
void test_count()
{
    size_t szs[] = { fields_count<PODs>()... };
#if !defined(__clang__) && defined(CPP_14)
constexpr
#endif
    size_t szs1[] = { get_type_total_count<PODs>()... };
    printf("Field count TEST:\n");
    const char*type_names[] = { typeid(PODs).name()... };
#ifdef __clang_major__
#warning __clang_major__
#endif
#if defined(__clang__) && defined(CPP_14)
    for (size_t i = 0; i < sizeof...(PODs); i++)
        printf("name: %s - upper:%ld full:%ld \n",
               type_names[i], szs[i],  szs1[i]);
#else
    for (size_t i = 0; i < sizeof...(PODs); i++)
        std::cout << "name: " << type_names[i]
                     << " - upper: " << szs[i]
                        << " full: " << szs1[i] << std::endl;
#endif
}
template<class...Ts>
void dump_types()
{
    const char*type_names[] = { typeid(Ts).name()... };
    for (size_t i = 0; i < sizeof...(Ts); i++)
        printf("%s%s", type_names[i], (i % 2? "\n" : ", "));
}
template<class...Ts>
void test_type_cyphers()
{

    const char*type_names[] = { typeid(Ts).name()... };
    constexpr std::size_t syphers[] = { encode_type<Ts>()... };
    constexpr std::size_t flipped[] = { flip_bytes_bits(encode_type<Ts>())...};
    puts("");
    for (size_t i = 0; i < sizeof...(Ts); i++)
    {
#if defined(__code_model_32__) || defined (FORCE_32)
        printf("0x%.8X flipped:0x%.8X %.12d %s\n",
#else
       printf("0x%.8lX flipped:0x%.8lX %.12ld %s\n",
#endif
               syphers[i],
               flipped[i], syphers[i], type_names[i]);
    }
}
template<class...Ts>
void test_type_cyphers_type(bool print_results=true)
{
    constexpr bool cnvts [] =
    {
        std::is_same<typename decode_cypher<encode_type<Ts>()>::type,Ts>::value...
    };
    const char*type_names[] = { typeid(Ts).name()... };
    const char*type_names2[] = { typeid(typename decode_cypher<encode_type<Ts>()>::type).name()... };
    if (print_results)
    {
        printf("Type decode/encode test match>>>\n");
        for (size_t i = 1; i < sizeof...(Ts); i++)
            printf("%s: encode: %s type names: %s\n",
               type_names[i],
               cnvts[i]? "match":"no match",
               strcmp(type_names[i], type_names2[i])==0? "match":"no match");
        puts("End of encode/decode test>>");
    }
}
void run_all_tests();
void field_count_test();
void type_decode_test();
void type_naming_probe();
void pod_ops_demo();
void test_offsets();
void test_value_holder_cpp14();
void field_count_test();
void print_member_offset(void *st, void *member);

#define calc_dump_memebr_data(TYPE, MEMBER)\
    adjusted = calc_offset_padding(offset, padding, alignof(MEMBER));\
    offset_of = offsetof(TYPE, MEMBER) + off;\
    printf ("%s: offset: %d ==(%s) offset(adjusted): %d, size: %d, align: %d prev_padding: %d\n", #MEMBER, \
        offset_of,(offset_of == adjusted? "TRUE":"FALSE"),\
        adjusted,\
        sizeof(MEMBER),alignof(MEMBER),\
        padding);\
    offset += (sizeof(MEMBER));printf("offset + size: %d ", offset);

template<typename TypePOD>
void test_type(TypePOD & pod)
{
#if (defined(CPP_11)  || defined(__clang__)) && !defined(TRY_CPP_14_TESTS)
    (void)pod;
#elif defined(_MSC_VER)
#else
    auto mimic = mimic_type<TypePOD>();
    memcpy(&mimic, &pod, sizeof(pod));
//#if CPP_14
//    std::string str = dump_tuple_to_string(mimic); qDebug() << str.c_str();
#endif
}
