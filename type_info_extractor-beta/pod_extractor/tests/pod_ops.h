#pragma once
#include <typeinfo>
#include <iostream>

#include "pod_extractor/mp.h"
size_t calc_offset_padding(size_t & offset, size_t & padding, size_t align);
size_t adjust_offset(size_t offset, size_t align);

struct FI
{ size_t offset, size, type_id; };
template<typename FromArray>
void extract_field_info
(
    const FromArray & info,
    FI *offsets,
    size_t & currentOffset,
    size_t & currentInfo,
    size_t & currentOffsets
)
{
    while (currentInfo < info.index)
    {
        if (info.type_ids[currentInfo] == meta_prog::mp_type_close_brace_value)
        {
            currentOffset = adjust_offset(currentOffset, info.type_aligns[currentInfo]);
            ++currentInfo; return;
        }
        else
        {
            currentOffset = adjust_offset(currentOffset, info.type_aligns[currentInfo]);
            if (info.type_ids[currentInfo] == meta_prog::mp_type_open_brace_value)
                extract_field_info(info, offsets, currentOffset, ++currentInfo, currentOffsets);
            else
            {
                offsets[currentOffsets++] =
                {
                  currentOffset,
                  info.type_sizes[currentInfo],
                  info.type_ids[currentInfo]
                };
                currentOffset += info.type_sizes[currentInfo];
                ++currentInfo;
            }
        }
    }
}
#ifdef CPP_14
template<std::size_t N, typename T>
constexpr std::size_t get_flat_type_index()
{
    constexpr auto info = get_pod_meta_infos<T>();
    size_t counter = 0;
    size_t realTypeIndex = 0;
    for (;realTypeIndex < info.index && info.index > N;realTypeIndex++)
    {
        if (counter == N &&
                (info.type_ids[realTypeIndex] != meta_prog::mp_type_open_brace_value &&
                 info.type_ids[realTypeIndex] != meta_prog::mp_type_close_brace_value))
            return realTypeIndex;
        if (info.type_ids[realTypeIndex] != meta_prog::mp_type_open_brace_value &&
              info.type_ids[realTypeIndex] != meta_prog::mp_type_close_brace_value)
            counter++;
    }
    return counter;
}
template<typename FromArray>
constexpr std::size_t get_flat_count(const FromArray & info)
{
    size_t counter = 0;
    for (size_t i = 0; i < info.index; i++)
    {
        if (info.type_ids[i] != meta_prog::mp_type_open_brace_value &&
              info.type_ids[i] != meta_prog::mp_type_close_brace_value)
            counter++;
    }
    return counter;
}
template<typename T>
constexpr std::size_t get_type_total_count() noexcept
{
    constexpr auto info = get_pod_meta_infos<T>();
    return get_flat_count(info);
}
template<std::size_t N, typename T>
decltype(auto) get_nth_type_from()
{
    constexpr auto info = get_pod_meta_infos<T>();
    constexpr auto flatIndex = get_flat_type_index<N, T>();
    using target_type = typename get_type<info.type_ids[flatIndex]>::type;
    return target_type{};
}

template<typename T>
constexpr auto fill_field_info(FI *fi) -> decltype(get_pod_meta_infos<T>())
{
    constexpr auto info = get_pod_meta_infos<T>();
    size_t currentOffset = 0;
    size_t currentInfo = 0;
    size_t currentOffsets = 0;
    extract_field_info(info, fi, currentOffset, currentInfo, currentOffsets);
    return info;
}
template<std::size_t N, typename T>
auto get_nth_lvalue_ref_from(T & pod)
    -> typename std::add_lvalue_reference<decltype(get_nth_type_from<N,T>())>::type
{
    FI fi[get_type_total_count<T>()];
    fill_field_info<T>(fi);
    using target_type = decltype(get_nth_type_from<N,T>());
    T *const_pod_ptr = &pod;
    char* data_holder_ptr =
            reinterpret_cast<char*>(const_pod_ptr) + fi[N].offset;
    return *(reinterpret_cast<target_type*>(data_holder_ptr));
}

template<std::size_t N, typename T>
auto get_nth_value_from(T const & pod)
    -> decltype(get_nth_type_from<N,T>())
{
    FI fi[get_type_total_count<T>()];
    fill_field_info<T>(fi);
    using target_type = decltype(get_nth_type_from<N,T>());
    T const *const_pod_ptr = &pod;
    char const* data_holder_ptr =
            reinterpret_cast<const char*>(const_pod_ptr) + fi[N].offset;
    return *(reinterpret_cast<target_type const *>(data_holder_ptr));
}
template<std::size_t N=0, typename PodT>
void print_pod_nth_field(PodT const & pod, bool debug=false)
{
    constexpr auto max = get_type_total_count<PodT>();
    static_assert(N < max, "field index out of range");
    std::cout << "#" << N << ":=" << get_nth_value_from<N>(pod);
    if (debug)
    std::cout << "::"
                << get_flat_type_index<N, PodT>() << ":"
              << typeid(decltype(get_nth_value_from<N>(pod))).name();
    std::cout << std::endl;
}
template<class PodT, size_t ... I>
void dump_struct(PodT const & pod, std::index_sequence<I...>)
{
    int results[]{ (print_pod_nth_field<I>(pod, true), 0)... };
    (void)results;
}
template<class PodT, size_t N=get_type_total_count<PodT>()>
void dump_struct(PodT const & pod)
{
    dump_struct(pod, std::make_index_sequence<N>());
}
template<size_t N1, size_t N2, class PodT>
void test_pod_ops_2(PodT & pod)
{
    std::cout << "v1: ";
    print_pod_nth_field<N1>(pod);
    auto & v1 = get_nth_lvalue_ref_from<N1>(pod);
    std::cout << "v2: ";
    print_pod_nth_field<N2>(pod);
    auto & v2 = get_nth_lvalue_ref_from<N2>(pod);
    v1++;
    std::cout << "v1++ ";
    print_pod_nth_field<N1>(pod);
    std::cout << "v2++ ";
    v2++;
    print_pod_nth_field<N2>(pod);
    std::cout << "v1 += v2 ";
    v1 += v2;
    print_pod_nth_field<N1>(pod);
}

#else
#include "total_count_obsolete.h"
#endif
template<typename T>
constexpr T get_value_from_ptr(void *ptr)
{ return *(*T(ptr)); }
template<typename T>
constexpr T & get_ref_from_ptr(void *ptr)
{ return *(*T(ptr)); }

