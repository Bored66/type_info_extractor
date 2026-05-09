#pragma once
#include <type_traits>

#include "typeid_array.h"
#include "type_cypher.h"

template <typename T>
struct is_pod_struct : public std::integral_constant<bool,
#if __cplusplus >= 201700
    std::is_aggregate_v<T> &&
    //  std::is_standard_layout_v<T> && 
    //  std::is_trivially_constructible_v<T> &&
#else
    std::is_pod<T>::value &&
    std::is_class<T>::value &&
#endif
    !std::is_enum<T>::value &&
    !std::is_empty<T>::value>
{
};

template <std::size_t Nth, std::size_t FieldCount, bool InsertBraces = false>
struct extract_type_info
{
    typeid_array<FieldCount, InsertBraces> &meta_info_ref;
    template <class Type>
    constexpr operator Type() noexcept
    {
        if (is_pod_struct<Type>::value)
             addTypeInfo<Type>();
        else
             addInfo(encode_type<Type>(), sizeof(Type), alignof(Type));
        return {};
    }
    constexpr extract_type_info(typeid_array<FieldCount, InsertBraces> &miref) : meta_info_ref(miref)
    {
    }
    template <class Type>
    constexpr void addTypeInfo() noexcept;
    constexpr void addInfo(
        std::size_t type_id,
        std::size_t size = 0,
        std::size_t align_of = 0) const noexcept
    {
        meta_info_ref.type_sizes[meta_info_ref.index] = size;
        meta_info_ref.type_aligns[meta_info_ref.index] = align_of;
        meta_info_ref.type_ids[meta_info_ref.index++] = type_id;
    }
};
template <typename Type, typename TypeidArray,
          std::size_t FieldCount, bool InsertBraces, std::size_t... I>
constexpr void get_type_meta_infos(TypeidArray &info) noexcept
{
    (void)Type{extract_type_info<I, FieldCount, InsertBraces>{info}...};
    // ^^^if you add here a variable name, it might spill out a lot of warnings with template info
    // we could try to use for debugging? - GXX
    // for Clang we have to add (void) to surpress same kind of warnings
}

template <class Type, typename TypeidArray, std::size_t FieldCount,
          bool InsertBraces, std::size_t... I>
constexpr void get_full_type_info(std::index_sequence<I...>, TypeidArray &info) noexcept
{
    if constexpr(is_pod_struct<Type>::value)
        return get_type_meta_infos<Type, TypeidArray, FieldCount, InsertBraces, I...>(info);
}

template <std::size_t Nth, std::size_t FieldCount, bool InsertBraces>
template <class Type>
constexpr void extract_type_info<Nth, FieldCount, InsertBraces>::addTypeInfo() noexcept
{
    using typeid_array_n =
        typename std::remove_reference<decltype(meta_info_ref)>::type;
    if (InsertBraces)
        addInfo(meta_prog::mp_type_open_brace_value, sizeof(Type), alignof(Type));
    get_full_type_info<Type, typeid_array_n, FieldCount, InsertBraces>(std::make_index_sequence<fields_count<Type>()>{}, meta_info_ref);
    if (InsertBraces)
        addInfo(meta_prog::mp_type_close_brace_value, sizeof(Type), alignof(Type));
    return;
}
template <class T, std::size_t Fields = fields_count<T>(), bool InsertBraces = true>
constexpr typeid_array<sizeof(T), InsertBraces>
get_pod_meta_infos(typeid_array<sizeof(T), InsertBraces> info = {}) noexcept
{
    get_full_type_info<T, decltype(info), info.size(), InsertBraces>(std::make_index_sequence<Fields>{}, info);
    return info;
}
