#pragma once

#include <type_traits>
#include "mp.h"
#include "mp_utils.h"

struct cypher_base
{
    enum
    {
        fund_type_mask = 0x1F, shift = 4,
        ptr_bit = 5, cnst_bit = 6, volatile_bit = 7,
        MAX_DEPTH = 7,
    };
};
template<typename Type, std::size_t N=0>
struct type_cypher : cypher_base
{
    using type_in = Type;
    static
    constexpr bool pointer = std::is_pointer<Type>::value;
    using type_w_o_ptr = typename std::remove_pointer<Type>::type;
    static
    constexpr bool volatl = std::is_volatile<type_w_o_ptr>::value;
    using type_w_o_vol = typename std::remove_volatile<type_w_o_ptr>::type;
    static
    constexpr bool constant = std::is_const<type_w_o_vol>::value;
    //using type_signed = typename std::make_signed<type_w_o_ptr>::type;
    using type_result = typename std::remove_const<type_w_o_vol>::type;
    using type_typeid = typename meta_prog::index_of_type<type_result>;

    static constexpr std::size_t type_id = type_typeid::value;
    static constexpr std::size_t encode_qual_mask()
    {
        return (pointer << ptr_bit ) |
                (constant << cnst_bit) |
                (volatl << volatile_bit);
    }
    static constexpr std::size_t encode()
    {
        return type_id  | encode_qual_mask();
    }
    static constexpr std::size_t value =
            (type_id == meta_prog::mp_type_not_found_value?
                   type_id : encode());
};
template<std::size_t CYPHER, class T=meta_prog::mp_type_not_found>
struct decode_cypher_base : cypher_base
{
    using Type = typename
        decode_cypher_base<(CYPHER>>cypher_base::shift),T>::result_type;
    using if_const = typename
        std::conditional<((CYPHER & (1 << cnst_bit))>0),
            typename std::add_const< Type >::type,
    Type>::type;
    using if_vol = typename
        std::conditional<((CYPHER & (1 << volatile_bit)) > 0),
            typename std::add_volatile< if_const >::type,
            if_const >::type;
    using if_ptr = typename
        std::conditional< ((CYPHER & (1 << ptr_bit)) > 0),
            typename std::add_pointer< if_vol >::type,
            if_vol >::type;
    using result_type = if_ptr;
};
template<typename Type>
struct decode_cypher_base<0,Type>
{
    using RetT = Type;
    using result_type = RetT;
};
template<std::size_t CYPHER>
struct decode_cypher : cypher_base
{
    using RetT = typename
        meta_prog::type_by_index< (CYPHER & fund_type_mask) >;
    using type = typename decode_cypher_base<CYPHER,RetT>::result_type;
};
template<std::size_t TI>
struct get_type
{
    using type = typename decode_cypher< TI >::type;
};
template<>
struct get_type<meta_prog::mp_type_open_brace_value>
{
    using type = meta_prog::mp_type_open_brace;
};
template<>
struct get_type<meta_prog::mp_type_close_brace_value>
{
    using type = meta_prog::mp_type_close_brace;
};
//#define DEBUG_NO_CONSTEXPR
template<typename type_result, typename type_typeid, std::size_t N = 0>
#ifndef DEBUG_NO_CONSTEXPR
constexpr
#endif
typename std::enable_if<
(std::is_same<type_typeid, meta_prog::mp_type_not_found>::value == false ||
        (std::is_same<type_typeid, meta_prog::mp_type_not_found>::value &&
 N>=cypher_base::MAX_DEPTH)),std::size_t>::type encode_type() noexcept
{
    using type_res = type_cypher<type_result>;
    return type_typeid::value |
            (type_res::encode_qual_mask() <<
                             (cypher_base::shift*N));
}
template<typename type_result, typename type_typeid, std::size_t N = 0>
#ifndef DEBUG_NO_CONSTEXPR
constexpr
#endif
typename std::enable_if<
(std::is_same<type_typeid, meta_prog::mp_type_not_found>::value &&
                                            N<cypher_base::MAX_DEPTH),
std::size_t>::type encode_type() noexcept
{
    using type_res = typename type_cypher<type_result>::type_result;
    using type_res_typeid = typename type_cypher<type_result>::type_typeid;
    return encode_type<type_res, type_res_typeid, N+1>() |
            (type_cypher<type_result>::encode_qual_mask() <<
                                            (cypher_base::shift*N));
}
template<typename Type>
#ifndef DEBUG_NO_CONSTEXPR
constexpr
#endif
std::size_t encode_type() noexcept
{
    using type_result = type_cypher<Type>;
    using type_typeid = typename type_result::type_typeid;
    return encode_type<Type,type_typeid>();
}
