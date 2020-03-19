#pragma once

#if defined(CPP_11) 
    #warning You are in defined(CPP_11)  MODE
#endif
#if defined(TRY_CPP_14_TESTS)
    #warning You are in  defined(TRY_CPP_14_TESTS) MODE
#endif

#ifdef CPP_11
#include "../mp_utils.h"
#include "../typeid_array.h"


template<typename Iterator, typename T>
constexpr  T
sumup(T init, Iterator beg, Iterator end)
{
    return beg != end?
        sumup(beg[0] + init, beg+1, end) : init;
}
template<std::size_t Nth>
struct ubiq_constructor
{
    meta_info mi{ Nth,meta_prog::mp_type_not_found_value,0,1,0 };
    meta_info & meta_info_ref;
    template <class Type> constexpr operator Type()
    CPP_11_NO_CONST_WARN_OFF
    noexcept;

    constexpr ubiq_constructor() :  meta_info_ref(mi)
    {}
    constexpr ubiq_constructor(meta_info & miref) : meta_info_ref(miref)
    {}
};
template<typename Type, std::size_t ...I>
constexpr
typename std::enable_if<!is_pod_struct<Type>::value, std::size_t>::type
get_type_total_count(std::index_sequence<I...>) noexcept
{
    return 1;
}
template<typename Type, std::size_t ...I>
constexpr typeid_array<sizeof...(I)>
get_type_meta_infos(typeid_array<sizeof...(I)> info ={}) noexcept
{
    return (void)Type{ubiq_constructor<I> {info.type_meta_info[I]}...}, info;
}
template<typename Array>
constexpr std::size_t get_internal_count(const Array& ids, std::size_t index) noexcept
{
    return ids.type_meta_info[index].internal_count;
}
template<std::size_t N>
constexpr std::size_t sum_internal_count(const std::size_t(& ids)[N]) noexcept
{
    return sumup(0, ids, ids+N);
}
template<typename Array, std::size_t ...I>
constexpr
std::size_t get_type_total_count(const Array& ids, std::index_sequence<I...>) noexcept
{
    return sum_internal_count
        #ifdef __clang__
            <sizeof...(I)>
        #endif
            ({ get_internal_count(ids, I)... });
}
template<typename Type, std::size_t ...I>
constexpr
typename std::enable_if<is_pod_struct<Type>::value, std::size_t>::type
get_type_total_count(std::index_sequence<I...> sqs) noexcept
{
    return get_type_total_count( get_type_meta_infos<Type, I...>(),sqs);
}
template<typename Type, std::size_t FldCnt = fields_count<Type>()>
constexpr std::size_t get_type_total_count() noexcept
{
    return get_type_total_count<Type>(std::make_index_sequence<FldCnt>{});
}
template<std::size_t Nth>
template <class Type>
constexpr ubiq_constructor<Nth>::operator Type() CPP_11_NO_CONST_WARN_OFF
noexcept
{
    return meta_info_ref.internal_count =
            // == meta_prog::mp_type_not_found_value?
            (is_pod_struct<Type>::value?
                 get_type_total_count<Type>() : 1),
            construct_helper<Type>();
}
#else
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
    return get_flat_count(get_pod_meta_infos<T>());
}

#endif
