#pragma once
#include <type_traits>

#include "typeid_array.h"
#include "type_cypher.h"

template<typename T>
struct is_pod_struct :
    public std::integral_constant<bool,
    std::is_pod<T>::value &&
    std::is_class<T>::value &&
    !std::is_enum<T>::value &&
    !std::is_empty<T>::value>
{};
//    (std::is_pod<T>::value || std::is_same<T, std::string>::value ) &&
//    (std::is_class<T>::value || std::is_same<T, std::string>::value) &&
template <class T>
constexpr T construct_helper() noexcept
//Original comment: adding const here allows to deal with copyable only types
{
    return{};
}

template<std::size_t Nth, std::size_t FieldCount, bool InsertBraces = false>
struct extract_type_info
{
    typeid_array<FieldCount, InsertBraces> & meta_info_ref;
    template <class Type> constexpr operator Type()
    CPP_11_NO_CONST_WARN_OFF noexcept
    {
        return
            (is_pod_struct<Type>::value?
                when_not_found<Type>() :
#ifdef USE_CPP_11_WAY
                 addInfo(encode_type<Type>(), sizeof(Type), alignof(Type))
#else
                meta_info_ref.addInfo({ Nth, encode_type<Type>(), sizeof(Type), 1, 0 })
#endif
                ),
            construct_helper<Type>();
    }
    constexpr extract_type_info(typeid_array<FieldCount, InsertBraces> & miref) :
        meta_info_ref(miref)
    {}
    template <class Type> constexpr std::size_t when_not_found()
    CPP_11_NO_CONST_WARN_OFF noexcept;
    constexpr
    std::size_t addInfo
    (
        std::size_t type_id,
        std::size_t size = 0,
        std::size_t align_of = 0
    ) const noexcept
    {
        return //meta_info_ref.index = type_id;
#if defined(CPP_11)// || defined(TRY_CPP_14_TESTS)
            const_cast<extract_type_info*>(this)->
                meta_info_ref.type_sizes[meta_info_ref.index] = size,
            const_cast<extract_type_info*>(this)->
                meta_info_ref.type_aligns[meta_info_ref.index] = align_of,
            const_cast<extract_type_info*>(this)->
               meta_info_ref.type_ids[meta_info_ref.index] = type_id,
            const_cast<extract_type_info*>(this)->meta_info_ref.index = meta_info_ref.index + 1;
#else
                meta_info_ref.type_sizes[meta_info_ref.index] = size,
                meta_info_ref.type_aligns[meta_info_ref.index] = align_of,
           meta_info_ref.type_ids[meta_info_ref.index++] = type_id;
#endif
    }
};
template<typename Type, typename TypeidArray,
    std::size_t FieldCount, bool InsertBraces, std::size_t ...I>
constexpr std::size_t get_type_meta_infos(TypeidArray & info) noexcept
{
    return (void)Type {extract_type_info<I, FieldCount, InsertBraces> {info}...},
        info.index;
}
template<typename Type, typename TypeidArray,
    std::size_t FieldCount, bool InsertBraces, std::size_t ...I>
    constexpr typename std::enable_if<is_pod_struct<Type>::value, std::size_t>::type
get_full_type_info(std::index_sequence<I...>, TypeidArray & info) noexcept
{
    return get_type_meta_infos<Type, TypeidArray, FieldCount, InsertBraces, I...>(info);
}
template<class T, typename TypeidArray, std::size_t FieldCount,
         bool InsertBraces, std::size_t ...I>
constexpr
typename std::enable_if<!is_pod_struct<T>::value, std::size_t>::type
get_full_type_info(std::index_sequence<I...>, TypeidArray & info) noexcept
{
    return info.index;
}
template<std::size_t Nth, std::size_t FieldCount, bool InsertBraces>
template <class Type>
constexpr std::size_t extract_type_info<Nth, FieldCount, InsertBraces>::when_not_found()
CPP_11_NO_CONST_WARN_OFF noexcept
{
    using typeid_array_n =
        typename std::remove_reference<decltype(meta_info_ref)>::type;
    return (InsertBraces?
     #ifdef USE_CPP_11_WAY
         addInfo(meta_prog::mp_type_open_brace_value, sizeof(Type), alignof(Type))
     #else
        meta_info_ref.addInfo(meta_info{ Nth, meta_prog::mp_type_open_brace_value,
            sizeof(Type), fields_count<Type>(), FieldCount })
     #endif
       : 0),
    get_full_type_info
        <Type, typeid_array_n, FieldCount, InsertBraces >
        (std::make_index_sequence<fields_count<Type>()>{}, meta_info_ref),
            (InsertBraces)?
            #ifdef USE_CPP_11_WAY
                addInfo(meta_prog::mp_type_close_brace_value, sizeof(Type), alignof(Type))
            #else
                meta_info_ref.addInfo({ Nth, meta_prog::mp_type_close_brace_value,
                sizeof(Type), fields_count<Type>(),FieldCount })
            #endif
              :0;
}
template<class T, std::size_t Fields = fields_count<T>(), bool InsertBraces = true>
constexpr typeid_array<sizeof(T), InsertBraces>
get_pod_meta_infos(typeid_array<sizeof(T), InsertBraces> info = {}) noexcept
{
    return get_full_type_info<T, decltype(info), info.size(), InsertBraces>
                            (std::make_index_sequence<Fields>{}, info), info;
}
